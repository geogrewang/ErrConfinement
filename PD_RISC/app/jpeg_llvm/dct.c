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
    int lut_imageEx[8][8]={{780, -1, 0, 1, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           {  0,  0, 0, 0, 0, 0, 0, 0 },
                           { -2,  0, 0, 0, 0, 0, 0, 0 }};

// Cosy C compiler version following (LLVM based implementation in the code):
//// Define range of data for protection and statistic LUT
//asm set_data( int start, int size, int lut_start, int lut_size )
//{
//    nop;
//    nop;
//    nop;
//    set_data @{start} @{size} @{lut_start} @{lut_size};
//    nop;
//}
//
//// Encoding parity start, should be used before write of protected data
//asm enable_parity()
//{
//      en_parity;
//}
//
//// Encoding parity end, should be used after data write of protected data
//asm disable_parity()
//{
//      dis_parity;
//}
//
//// Check values of protected data, use values from LUT to replace in case of error detection. Used after reading of protected data.
//
//inline asm int check_value( int src )
//{
//    @[
//    ]
//    nop;
//    nop;
//    chk_ld @{} @{src};
//    nop;
//    nop;
//    nop;
//}
    
  //     .target dest 
    //mov @{}, @{dest};

//function to perform the discrete cosins transform
int DiscreteCosine(int imageData[SIZE][SIZE], int imageEx[SIZE][SIZE])
{
        int X, Y,num, X1, Y1;
        int i, j, m, n;
        float sum, pi, k0, k1, ktx, kty, A, B;
        pi= 3.141592;
        num = 8;
        //k0=sqrt((double)1.0/(double)num);
        //k1=sqrt((double)2.0/(double)num);
        k0=0.125;
        k1=0.250;
        int k=0;
        int k11, i1, j1;
        float COS[8][8]={{1.000000,  1.000000,  1.000000,  1.000000,  1.000000,  1.000000,  1.000000,  1.000000, },
                         {0.980785,  0.831470,  0.555570,  0.195091, -0.195090, -0.555570, -0.831469, -0.980785, },
                         {0.923880,  0.382684, -0.382683, -0.923879, -0.923880, -0.382684,  0.382683,  0.923879, },
                         {0.831470, -0.195090, -0.980785, -0.555571,  0.555569,  0.980785,  0.195092, -0.831469, },
                         {0.707107, -0.707106, -0.707107,  0.707106,  0.707108, -0.707105, -0.707108,  0.707105, },
                         {0.555570, -0.980785,  0.195089,  0.831470, -0.831469, -0.195092,  0.980786, -0.555568, },
                         {0.382684, -0.923880,  0.923879, -0.382682, -0.382686,  0.923880, -0.923878,  0.382680, },
                         {0.195091, -0.555571,  0.831470, -0.980786,  0.980785, -0.831468,  0.555568, -0.195086  }};

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
                     A=COS[i][X];
                    for(Y=0, Y1=k11; Y1<8+k11; Y++, Y1++)
                     {
                         B=COS[j][Y];
                         //printf("A_org: %f, A_pro: %f\n", deg_A, A);
                         //printf("B_org: %f, B_pro: %f\n", deg_B, B);
                         sum= sum + (imageData[X1][Y1])*(B*A);
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
                  //enable_parity();
                  
                  asm("en_parity");
                  imageEx[i1][j1]= (int)sum;
                  asm("dis_parity");

                  printf("Element %d, %d\n", i1, j1);
                  //disable_parity();
                  // turn off load encoding
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
  
                         printf("imageEx[6][7]: %d\n", imageEx[6][7]);
 imageEx[6][7] = 11111;

                         printf("imageEx[6][7]: %d\n", imageEx[6][7]);
 do
 {  
      k11=0;
      do
      {
            printf("Block %d, %d\n", k, k11);
            for(i=0, i1=k; i1<8+k; i++, i1++)  //m=i
            {
                int src, dst;
                for(j=0, j1=k11; j1<8+k11; j++, j1++)  //n=j
                {
                    //To quantise, original matrix is divided by quantisation matrix
                    src = imageEx[i1][j1];
                    // After reading protected data, check its accuraccy and apply correction if there is error.
                    // int dst = check_value(src);
                    asm (   "nop\n"
                            "nop\n"
                            "chk_ld %[one] %[two]\n"
                            "nop\n"
                            "nop\n"
                            "nop"
                            : [one]"=r" (dst): [two]"r" (src):
                        );

                    printf("src: %d, dst: %d\n", src, dst);
                    imageExtended[i1][j1]= (dst/quant[i][j]);
                    printf("Element %d, %d\n", i1, j1);
                }
            }
          k11=k11+8;
      }while(k11<SIZE-7);
 k=k+8;
 }while(k<SIZE-7);

//                         printf("imageEx[6][7]: %d\n", imageEx[6][7]);

return 0;
}

//Reading from the text file
int Fileinput(int imageData[SIZE][SIZE])
{
    FILE *ifp;
   int i,j;
   //myFile_512 is the text file in which the input file is defined
    ifp = fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_usha/lena512.txt", "r");
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
    ofp = fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_usha/outputData_dct.txt", "w");
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
    // first register data range of protection
    
    printf("LUT start address: %d\n", lut_imageEx);
    printf("imageEX start address: %d\n", imageEx);

  //  set_data(imageEx, 64, lut_imageEx, 64); // protected array, protected size, lut array, lut size
    int size=64;
    asm (   "nop\n"
            "nop\n"
            "nop\n"
            "set_data %[one] %[two] %[three] %[four]\n"
            "nop"
            :: [one]"r" (imageEx), [two]"r" (size), [three]"r" (lut_imageEx), [four]"r" (size):
        );
   
    Fileinput(imageData);
    DiscreteCosine(imageData, imageEx);
    quant(imageEx, imageExtended);
    Fileoutput(imageExtended);   
    printf("Over");   
    //getch();
    return 0;        
}
