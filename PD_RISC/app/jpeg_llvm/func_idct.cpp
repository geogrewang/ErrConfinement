#include <stdio.h>
#include<math.h>
#include<iostream>
using namespace std;

#define ROW_SIZE 512
#define COL_SIZE 512
#define SIZE 512

 int image[SIZE][SIZE];
 int imageExtend[SIZE][SIZE];
 int imageData[SIZE][SIZE];

int InverseDCT(int image[SIZE][SIZE], int imageExtend[SIZE][SIZE]) 
{
  int X, Y,num, i, j;
  float sum, pi, k0, k1, ktx, kty, A;
  pi= 3.141592;
  num = 8;
  k0=sqrt((double)1.0/(double)num);
  k1=sqrt((double)2.0/(double)num);
  int i1, j1, X1, Y1; 
  int k=0; 
  int k11;

 k=0;
 
do
{  
      k11=0;
      do
      {
  for(i=0, i1=k; i1<8+k; i++, i1++)  //m=i
     {
        for(j=0, j1=k11; j1<8+k11; j++, j1++)  //n=j
           {
            sum =0.0;
         
                   
            for(X=0, X1=k; X1<8+k; X++, X1++)
            {
                    A=cos((double)((2.0*(float)j+1)*X*pi/2.0/num));
                    for(Y=0, Y1=k11; Y1<8+k11; Y++, Y1++)
                    {
                     {
                             if(X==0)
                                     ktx=k0;
                             else
                                     ktx=k1;
                             if(Y==0)
                                     kty=k0;
                             else
                                     kty=k1;
                             
                              sum= sum + (image[X1][Y1])*(cos((double)((2.0*(float)i+1)*Y*pi/2.0/(float)num))*A*ktx*kty);
                     } 
                    }
            }
           imageExtend[i1][j1]= int(sum);
  } }
k11=k11+8;
}while(k11<SIZE-7);
 k=k+8;
 }while(k<SIZE-7);
 
 return 0;
}

int invquant(int imageData[SIZE][SIZE], int image[SIZE][SIZE])
{
    int i1, j1, X1, Y1,i,j; 
    int k=0; 
    int k11;
    int quant[8][8]= { {6, 4, 4, 6, 10, 16, 20, 24},
                       {5, 5, 6, 8, 10, 23, 24, 22},
                       {6, 5, 6, 10, 16, 23, 28, 22},
                       {6, 7, 9, 12, 20, 35, 32, 25},
                       {7, 9, 15, 22, 27, 44, 41, 31},
                       {10, 14, 22, 26, 32, 42, 45, 37},
                       {20, 26, 31, 35, 41, 48, 48, 40},
                       {29, 37, 38, 39, 45, 40, 41, 40}};
                       
    do
{  
      k11=0;
      do
      {
            for(i=0, i1=k; i1<8+k; i++, i1++)  //m=i
            {
                     for(j=0, j1=k11; j1<8+k11; j++, j1++)  //n=j
                     {
                              image[i1][j1]= imageData[i1][j1]*quant[i][j];
                     }
            }
          k11=k11+8;
      }while(k11<SIZE-7);
 k=k+8;
 }while(k<SIZE-7);
 
 return 0;
}

int Fileinput(int imageData[SIZE][SIZE])
{
    int i,j;
    FILE *ifp;
    ifp = fopen("outputData_dct_512_func.txt", "r");
    if (ifp == NULL) 
    {
        printf("Can't open input file inputData.txt!\n");
        return 0;
    } 
    for (i=0; i<SIZE; i++) 
    {
        for (j=0; j<SIZE; j++) 
        {
            fscanf(ifp, "%d", &imageData[i][j]);
        }
    }
    fclose(ifp);
    
    return 0;
}

int Fileoutput(int imageExtend[SIZE][SIZE])
{
    FILE *ofp;
    int i,j;
    ofp = fopen("outputData_idct_512_func.txt", "w");
        if (ofp == NULL)
         {
           fprintf(stderr, "Can't open output file outputData.txt!\n");
           return 0;
         }
         
    for (i=0; i<ROW_SIZE; i++) 
    {
        for (j=0; j<COL_SIZE; j++)
         {
            fprintf(ofp, "%d ", imageExtend[i][j]);
         }
        fprintf(ofp, "\n");
    }
     fclose(ofp);
    return 0;
}
    
//Main Function
int main() 
{
    int i, j;
   
   Fileinput(imageData);
   invquant(imageData, image);    
   InverseDCT(image, imageExtend);
   Fileoutput(imageExtend);
   printf("bye");
   return 0;        
}
