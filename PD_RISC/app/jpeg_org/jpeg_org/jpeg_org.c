#include <stdio.h>
//#include<conio.h>
#include<math.h>
//#include<iostream>
//using namespace std;

#define ROW_SIZE 512
#define COL_SIZE 512
#define SIZE 512

 
    int imageEx[SIZE][SIZE];
    int imageData[ROW_SIZE][COL_SIZE];
    int imageExtend[ROW_SIZE][COL_SIZE];
    int imageAfter[ROW_SIZE][COL_SIZE];
    int imageOut[ROW_SIZE][COL_SIZE];

//        float COS[8][8]={{1.000000,  1.000000,  1.000000,  1.000000,  1.000000,  1.000000,  1.000000,  1.000000, },
//                         {0.980785,  0.831470,  0.555570,  0.195091, -0.195090, -0.555570, -0.831469, -0.980785, },
//                         {0.923880,  0.382684, -0.382683, -0.923879, -0.923880, -0.382684,  0.382683,  0.923879, },
//                         {0.831470, -0.195090, -0.980785, -0.555571,  0.555569,  0.980785,  0.195092, -0.831469, },
//                         {0.707107, -0.707106, -0.707107,  0.707106,  0.707108, -0.707105, -0.707108,  0.707105, },
//                         {0.555570, -0.980785,  0.195089,  0.831470, -0.831469, -0.195092,  0.980786, -0.555568, },
//                         {0.382684, -0.923880,  0.923879, -0.382682, -0.382686,  0.923880, -0.923878,  0.382680, },
//                         {0.195091, -0.555571,  0.831470, -0.980786,  0.980785, -0.831468,  0.555568, -0.195086  }};
    

//function to perform the discrete cosins transform
int DiscreteCosine(int imageData[SIZE][SIZE], int imageEx[SIZE][SIZE])
{
        int X, Y,num, X1, Y1;
        int i, j, m, n;
        float sum, pi, k0, k1, ktx, kty, A, B;
        pi= 3.141592;
        num = 8;
        k0=0.125;
        k1=0.250;
       // k0=sqrt((double)1.0/(double)num);
       // k1=sqrt((double)2.0/(double)num);
        int k=0;
        int k11, i1, j1;

        printf("Start DCT.\n");

        do
        {   
            k11=0;
            do
            {
            printf("Block %d, %d\n", k, k11);

             for(i1=k, i=0; i1<8+k; i++,i1++)  //m=i
             {
              for(j1=k11, j=0; j1<8+k11; j++, j1++)  //n=j
              {
                 sum =0.0;
                 for(X=0, X1=k; X1<8+k; X++, X1++)
                 {
                    A=cos((double)((2.0*(float)X+1)*i*pi/2.0/num));
                    for(Y=0, Y1=k11; Y1<8+k11; Y++, Y1++)
                    
                     {      
                             sum= sum + (imageData[X1][Y1])*(cos((double)((2.0*(float)Y+1)*j*pi/2.0/(float)num))*A);
                     } 
                  //   A=COS[i][X];
                  //   for(Y=0, Y1=k11; Y1<8+k11; Y++, Y1++)
                  //   {
                  //       B=COS[j][Y];
                  //       //printf("A_org: %f, A_pro: %f\n", deg_A, A);
                  //       //printf("B_org: %f, B_pro: %f\n", deg_B, B);
                  //       sum= sum + (imageData[X1][Y1])*(B*A);
                  //   } 
                  
                  }
              if(j==0)  
              sum= sum*k0;
              else      
              sum= sum*k1;
              if(i==0)  
              sum=sum*k0;
              else      
              sum= sum*k1;
          
              imageEx[i1][j1]= (int)sum;
              }
             }
             k11=k11+8;
          }while(k11<SIZE-7); 

        k=k+8;

        }while(k<SIZE-7);

        printf("End DCT.\n");

        return 0;
}

//function to perform the quantisation function
int quant(int imageEx[SIZE][SIZE], int imageExtended[SIZE][SIZE])
{
     int i, j, m, n, k11, i1, j1, k=0;
     //Declaration of Standard Quantisation Matrix
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
            printf("Block %d, %d\n", k, k11);
            for(i=0, i1=k; i1<8+k; i++, i1++)  //m=i
            {
                     for(j=0, j1=k11; j1<8+k11; j++, j1++)  //n=j
                     {
                     //To quantise, original matrix is divided by quantisation matrix
                     imageExtended[i1][j1]= (imageEx[i1][j1]/quant[i][j]);
                     }
            }
          k11=k11+8;
      }while(k11<SIZE-7);
 k=k+8;
 }while(k<SIZE-7);
return 0;
}

int InverseDCT(int image[SIZE][SIZE], int imageExtend[SIZE][SIZE]) 
{
  int X, Y,num, i, j;
  float sum, pi, k0, k1, ktx, kty, A, B;
  pi= 3.141592;
  num = 8;
  k0=0.125;
  k1=0.250;
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
                    //A=COS[X][j];
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
                             
                             //B=COS[Y][i];
                             sum= sum + (image[X1][Y1])*(cos((double)((2.0*(float)i+1)*Y*pi/2.0/(float)num))*A*ktx*kty);
                             //sum= sum + (image[X1][Y1])*(B*A*ktx*kty);
                     } 
                    }
            }
           imageExtend[i1][j1]= (int)sum;
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
//Reading from the text file
int Fileinput(int imageData[SIZE][SIZE])
{
    FILE *ifp;
   int i,j;
   //myFile_512 is the text file in which the input file is defined
    ifp = fopen("/net/home/wangz/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_org/jpeg_org/lena512.txt", "r");
    if (ifp == NULL) 
    {
        printf("Can't open input file inputData.txt!\n");
        return 0;
    } 
    for (i=0; i<ROW_SIZE; i++) 
    {
        printf("read ROW %d\n", i);
        for (j=0; j<COL_SIZE; j++) 
        {
            fscanf(ifp, "%d", &imageData[i][j]);
        }
    }
    fclose(ifp);
    return 0;
}

//Writing the output in the text file
int Fileoutput(int imageExtended[SIZE][SIZE])
{
    FILE *ofp;
    int i,j;
    //outputData_dct_512_func is the text file where the output is stored
    ofp = fopen("/net/home/wangz/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_org/jpeg_org/outputData_dct.txt", "w");
        if (ofp == NULL)
         {
           fprintf(stderr, "Can't open output file outputData.txt!\n");
           return 0;
         }
         
    for (i=0; i<SIZE; i++) 
    {
        for (j=0; j<SIZE; j++)
         {
            fprintf(ofp, "%d ", imageExtended[i][j]);
         }
        fprintf(ofp, "\n");
    }
    
    fclose(ofp);
    
    return 0;
}

//Main Function        
int main() 
{
   
    Fileinput(imageData);
    DiscreteCosine(imageData, imageEx);
    //quant(imageEx, imageExtend);
    //invquant(imageExtend, imageAfter);    
    InverseDCT(imageEx, imageOut);
    Fileoutput(imageOut);   
    printf("Over");   
    //getch();
    return 0;        
}
