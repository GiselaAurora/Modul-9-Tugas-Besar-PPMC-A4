/*EL2208 Praktikum Pemecahan Masalah dengan C 2022/2023
*Modul              : 9 - Tugas Besar
*Kelompok           : A4
*Hari dan Tanggal   : 
*Asisten (NIM)      : Eunike Kristianti (18320019)
*Nama File          : tubes.c
*Deskripsi          : Penyelesaian markas militer negara api dengan convex hull algorithm
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define M_PI 3.14
struct definition{
  double lintang;
  double bujur;
  int nomor;
  int masukjalur;
  char nama[100];
};

double totaldistance = 0;


#define R 6371 // Earth's radius in kilometers

double get_angle(struct definition p1, struct definition p2, struct definition p3) {//menentukan sudut antara dua garis yang dihasilkan oleh tiga titik
  // Calculate the vectors
  double v1bujur = p2.bujur - p1.bujur;
  double v1lintang = p2.lintang - p1.lintang;
  double v2bujur = p3.bujur - p2.bujur;
  double v2lintang = p3.lintang - p2.lintang;

  // Calculate the dot product and magnitudes
  double dot_product = v1bujur * v2bujur + v1lintang * v2lintang;
  double magnitude1 = sqrt(v1bujur * v1bujur + v1lintang * v1lintang);
  double magnitude2 = sqrt(v2bujur * v2bujur + v2lintang * v2lintang);

  // Calculate the angle in radians and degrees
  double angle_in_radians = acos(dot_product / (magnitude1 * magnitude2));
  double angle_in_degrees = angle_in_radians * 180.0 / M_PI;

  return angle_in_degrees;
}

double toRadians(double degree) {
    return degree * M_PI / 180;
}

double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);

    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(toRadians(lat1)) * cos(toRadians(lat2)) *
               sin(dLon / 2) * sin(dLon / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    double distance = R * c;

    return distance;
}

float mins = 10000;
int tandapalingbawah;

//float distance(struct definition p,struct definition q){
//	return (sqrt(pow((p.bujur-q.bujur),2)+pow((p.lintang-q.lintang),2)));
//}

float orientation(struct definition p,struct definition q,struct definition r)
{
    float val = (q.lintang - p.lintang) * (r.bujur - q.bujur) -
              (q.bujur - p.bujur) * (r.lintang - q.lintang);
    // if (val == 0) return 0;  // collinear
    // return (val > 0)? 1: 2; // clock or counterclock wise
    return val;
}

void convexHull(struct definition points[100], int n)
{
    struct definition hull[100];
    int banyakhull = 0;
    int p = 99, q=tandapalingbawah,i,r;
    do
    {
//    	printf("masih jalan, ini urutan ke :%d,p sekarang :%s,q sekarang :%s\n",banyakhull,points[p].nama,points[q].nama);
       hull[banyakhull] = points[q];
        banyakhull++;
        float mins = 100000;
        for ( i = 0; i < n; i++)
        {
          
           if ((orientation(points[p], points[q], points[i]) > 0) && (haversine(points[q].lintang,points[q].bujur,points[i].lintang,points[i].bujur)<2500)&&(get_angle(points[p], points[q], points[i]) < mins)){
              mins = get_angle(points[p], points[q], points[i]);
              r = i;
//              printf("r sekarang :%s, dan nilai mins :%f\n",points[r].nama,mins);
//               if(q==14){
//                printf("ini pas davao berubah %s\n",points[r].nama);
//               }
           }
        }
        totaldistance += haversine(points[q].lintang,points[q].bujur,points[r].lintang,points[r].bujur);
        p = q;
        q = r;
  
    } while (r != tandapalingbawah);

    for(i = 0 ; i < n; i++){//proses mencari titik yang diluar convex hull
       int bisa = 1,j;
       for(j=0;j<banyakhull-1;j++){
        if(orientation(hull[j],hull[j+1],points[i]) < 0){// kalau dia counclock berarti diluar hull
          bisa = 0;
        }
       }
      points[i].masukjalur = bisa;
    } 
  
    for (i = 0; i < banyakhull; i++){   
      printf("%s->\n",hull[i].nama);
      points[hull[i].nomor].masukjalur = 1;
    }
    printf("%s\n",hull[0].nama);
}

struct definition points[100];


int main(){
  char file[255];
    FILE* stream = fopen("markas.csv", "r");
	if(stream == NULL){
		printf("\n");
    printf("File tidak ada!");
    return 0;
	}
  char line[255]; char* token; int row = 0;
	
    fgets(line, 255, stream);
    token = strtok(line, "\n");
    int N = atoi(token);
    int matriks[N][5],nourut = 0,i;
    int *ptr; 
    ptr = &matriks[0][0]; //inisialisasi pointer
   // int* matriks2[N][5];

    while(fgets(line, 255, stream)) {//proses pemindahan data ke matriks
        token = strtok(line, ",");  strcpy(points[nourut].nama,token); 
        token = strtok(NULL, ",");  points[nourut].lintang = atof(token);
        if(points[nourut].lintang < mins){
          mins = points[nourut].lintang;
          tandapalingbawah = nourut;
        }
        token = strtok(NULL, "\n"); points[nourut].bujur = atof(token);
        points[nourut].nomor=nourut;
        points[nourut].masukjalur = 0;
        nourut++;
    }
    points[99].lintang = points[tandapalingbawah].lintang;
    points[99].bujur = points[tandapalingbawah].bujur + 1;
    // printf("total peserta %d\n",nourut);
    // printf("ini no paling bawah %d\n",tandapalingbawah);
    // for(i = 0 ; i < nourut;i++){
    //   if(points[i].nomor == tandapalingbawah){
    //     printf("ini paling bawah nama : %s,lintang: %f,bujur : %f\n",points[i].nama,points[i].lintang,points[i].bujur);
    //   }
    //   else{
    //     printf("nama : %s,lintang: %f,bujur : %f\n",points[i].nama,points[i].lintang,points[i].bujur);
    //   }
    // }
    printf("Perbatasan Efektif Negara Api:\n");
	convexHull(points,nourut);
    // printf(" : %lf",haversine(25.03,121.56,7.19,125.45));
    // printf("%s and %s and %s",points[14].nama,points[0].nama,points[6].nama);
    // if(orientation(points[14],points[0],points[6])==1){
    //   printf("ini kena");
    // }
    printf("\n");
    printf("Panjang Perbatasan Efektif Negara Api:%lf km\n\n",totaldistance);
    printf("Markas di Luar Perbatasan Efektif:\n");
    int temp = 1;
//    if(orientation(points[14],points[13],points[3]) == 2){
//      printf("%s diluar hull\n",points[3].nama);
//    }
    for(i = 0;i < nourut;i++){
      if(points[i].masukjalur == 0){
        printf("%d.%s\n",temp,points[i].nama);
        temp++;
      }
    }
}
