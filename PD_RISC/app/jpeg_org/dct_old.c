#include <stdio.h>
//#include<conio.h>
#include<math.h>
//#include<iostream>
//using namespace std;

#define ROW_SIZE 8 
#define COL_SIZE 8
#define SIZE 8


    int imageEx[SIZE][SIZE];
    int imageData[ROW_SIZE][COL_SIZE];
    int imageExtended[ROW_SIZE][COL_SIZE];
    long imageAverage[ROW_SIZE][COL_SIZE];
    int lut_imageEx[8][8]={{780, -1, 0, 1, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           { -2,  0, 0, 0, 0, 0, 0, 0 }};

// Define range of data for protection and statistic LUT
asm volatile set_data( int start, int size, int lut_start )
{
    nop;
    nop;
    set_data @{start} @{size} @{lut_start};
    nop;
}

// Encoding parity start, should be used before write of protected data
asm volatile enable_parity()
{
      en_parity;
}

// Encoding parity end, should be used after data write of protected data
asm volatile disable_parity()
{
      dis_parity;
}

// Check values of protected data, use values from LUT to replace in case of error detection. Used after reading of protected data.

asm volatile check_value( int dst, int src, int lut_index )
{
      chk_ld @{dst} @{src} @{lut_index};
}

//function to perform the discrete cosins transform
int DiscreteCosine(int imageData[SIZE][SIZE], int imageEx[SIZE][SIZE], long imageAverage[SIZE][SIZE])
{
        int X, Y,num, X1, Y1;
        int i, j, m, n;
        float sum, pi, k0, k1, ktx, kty, A;
        pi = 3.141592;
        num = 8;
        k0 = 0.125;
        k1 = 0.250;
        int k=0;
        int k11, i1, j1;

        printf("Start DCT.\n");


        do
        {
        printf("Block %d.\n", k);
            k11=0;
             do
             {
             for(i1=k, i=0; i1<8+k; i++,i1++)  //m=i
             {
              for(j1=k11, j=0; j1<8+k11; j++, j1++)  //n=j
              {
                 sum =0.0;
                 for(X=0, X1=k; X1<8+k; X++, X1++)
                 {
                    A=cos((float)((2*X+1)*i*pi/16));
                    for(Y=0, Y1=k11; Y1<8+k11; Y++, Y1++)
                    
                     { 
                             cout << "A: "<<A <<"  B: "<< cos((float)((2*Y+1)*j*pi/16))<<endl;
                             sum= sum + (imageData[X1][Y1])*(cos((float)((2*Y+1)*j*pi/16))*A);
                     } 
                  }
              if(j==0) 
              sum= sum*k0;
              else      
              sum= sum*k1;
              if(i==0)  
              sum=sum*k0;
              else      
              sum= sum*k1;
         
              // turn on load encoding
              enable_parity();
              imageEx[i1][j1]= (int)sum;
              disable_parity();
              // turn off load encoding
           imageAverage[i][j] += imageEx[i1][j1];
  } }
 k11=k11+8;
 }while(k11<SIZE-7); 
k=k+8;
}while(k<SIZE-7);

            for(i=0; i<8; i++) 
            {
                     for(j=0; j<8; j++)
                     {
               //      imageAverage[i][j] = imageAverage[i][j]/4096;
                     imageAverage[i][j] = imageAverage[i][j];
                     }
            }
        printf("End DCT.\n");
return 0;
}

//function to perform the quantisation function
int quant(int imageEx[SIZE][SIZE], int imageExtended[SIZE][SIZE], long imageAverage[SIZE][SIZE])
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
            for(i=0, i1=k; i1<8+k; i++, i1++)  //m=i
            {
                     for(j=0, j1=k11; j1<8+k11; j++, j1++)  //n=j
                     {
                     //To quantise, original matrix is divided by quantisation matrix
                     int src = imageEx[i1][j1];
                     int dst;
                     
                     // After reading protected data, check its accuraccy and apply correction if there is error.
                     check_value(dst, src, (8*i+j) );
                     imageExtended[i1][j1]= (dst/quant[i][j]);
                     //imageAverage[i][j] += imageExtended[i1][j1];
                     }
            }
          k11=k11+8;
      }while(k11<SIZE-7);
 k=k+8;
 }while(k<SIZE-7);

            for(i=0; i<8; i++)  //m=i
            {
                     for(j=0; j<8; j++)  //n=j
                     {
                     //To quantise, original matrix is divided by quantisation matrix
                     //imageAverage[i][j] = imageAverage[i][j]/4096;
                     imageAverage[i][j] = imageAverage[i][j];
                     }
            }

return 0;
}

//Reading from the text file
int Fileinput(int imageData[SIZE][SIZE], long imageAverage[ROW_SIZE][COL_SIZE])
{
   FILE *ifp;
   int i,j;
   //myFile_512 is the text file in which the input file is defined
//    ifp = fopen("lena512.txt", "r");
    ifp = fopen("/net/home/wangz/wangz/zheng-work/CI_QoS/Arch/PD_RISC_new/app/jpeg/lena512.txt", "r");
    if (ifp == NULL) 
    {
        printf("Can't open input file lena512.txt!\n");
        return 0;
    }
    for (i=0; i<ROW_SIZE; i++) 
    {
       // printf("Row %d.\n", i);

        for (j=0; j<COL_SIZE; j++) 
        {
            fscanf(ifp, "%d", &imageData[i][j]);
            imageAverage[i][j]=0;
        }
    }
    fclose(ifp);
    return 0;
}

//Writing the output in the text file
int Fileoutput(int imageExtended[SIZE][SIZE], long imageAverage[SIZE][SIZE])
{
    FILE *ofp, *ofp_avg;
    int i,j;
    //outputData_dct_512_func is the text file where the output is stored
    ofp = fopen("outputData_dct_512_func.txt", "w");
    ofp_avg = fopen("average_dct_512_func.txt", "w");
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
    
    for (i=0; i<8; i++) 
    {
        for (j=0; j<8; j++)
         {
            fprintf(ofp_avg, "%d ", imageAverage[i][j]);
         }
        fprintf(ofp_avg, "\n");
    }

    fclose(ofp);
    fclose(ofp_avg);
    
    return 0;
}

//Main Function        
int main() 
{
    // first register data range of protection
    set_data(imageEx, 16384, lut_imageEx);

    Fileinput(imageData, imageAverage);
    DiscreteCosine(imageData, imageEx, imageAverage);
    quant(imageEx, imageExtended, imageAverage);
    Fileoutput(imageExtended, imageAverage);   
    printf("Over\n");   
    //getch();
    return 0;        
}
