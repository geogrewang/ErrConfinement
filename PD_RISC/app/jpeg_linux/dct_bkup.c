#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 16 // 512
#define SCALE 2 // 64
#define NR_FAULT 0
#define CHK_QUAN
//#define CHK_DEQUAN
#define PAR 
//#define ECC

double COS[8][8], C[8];
int pic[N][N];
double dct[N][N], idct[N][N];
int par_dct[N][N]={0};
int ecc_dct[N][N]={0};
int par_quan[N][N]={0};
double avg_dct[N][N]={0};
double avg_quant[N][N]= {0};

double golden_dct[8][8]= {{ -242.538422, -1.565797, 0.646120, 1.169245, -0.020233, 0.940231, 0.027707, 0.733925, },
                          { -0.222834, -0.108308, -0.780093, 0.133814, 0.039231, 0.062401, 0.025521, -0.004177,  },
                          { -0.649843, -0.512487, 0.228183, 0.353566, 0.000773, 0.069678, 0.012114, -0.001641,   },
                          { -0.641493, 0.030510, 0.412436, 0.099906, 0.021144, -0.039063, -0.002198, 0.004568,   },
                          { 0.017609, -0.038111, -0.065816, -0.025756, -0.081390, 0.005183, -0.007455, -0.005406,},  
                          { -0.430538, -0.021713, 0.046092, 0.007092, -0.002983, -0.000642, -0.005067, 0.005527, },
                          { 0.084360, 0.018600, -0.003267, -0.000333, -0.004912, 0.004006, 0.001126, -0.009209,  },
                          { -2.868614, 0.005601, -0.002588, 0.005061, 0.000778, -0.002361, -0.002779, -0.003040  }};

double golden_quan[8][8]= {{-15.157227, -0.146973, 0.068604, 0.073486, -0.002197, 0.003418, 0.000000, 0.000000,},
                           {-0.017090, -0.005859, -0.054688, 0.002197, 0.000488, 0.000244, 0.000000, 0.000000, },
                           {-0.047119, -0.195312, 0.013184, 0.003174, -0.001709, 0.000000, 0.000000, 0.000000, },
                           {-0.035889, -0.000488, 0.018555, 0.002686, 0.000244, 0.000000, 0.000000, 0.000000,  },
                           {-0.002441, -0.002197, -0.000732, -0.000244, 0.000000, 0.000000, 0.000000, 0.000000,},  
                           {-0.002686, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, },
                           {0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,  },
                           {0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000  }};
int gmatrix_32[32][38]={
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1}
};

int hmatrix_32[6][38]={
{1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
{1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0},
{0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0},
{0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0},
{0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
{0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1}
};
void init() {
  int i, j;
  double k;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++)
    {
      COS[i][j] = cos((2 * i + 1) * j * acos(-1) / 16.0);
    }
    if (i) C[i] = 1;
    else C[i] = 1 / sqrt(2);
  }
}

int parity( int value) {
  int i, temp;
  int MASK;
  int pari=0;
  
//  printf("%d \n", value);

  for (i = 0; i < 32; i++) 
  {
      MASK=1<<i;
      temp=value & MASK;
      if (temp) pari^=1;
      else pari^=0;
  }

//  printf("parity %d \n", pari);

  return pari;
}

int encode_32(int data[])
{
    int i,j;
    int *encoded = (int*)malloc(sizeof(int) * 38);
    int *ecc = (int*)malloc(sizeof(int) * 6);
    int ecc_word=0;
    for(i=0;i<38;i++)
    {
        encoded[i]=0;
        for(j=0;j<32;j++)
            encoded[i]+=(data[j]*gmatrix_32[j][i]);
        encoded[i]=encoded[i]%2;
    }
    // for debug
   // for(i=37;i>-1;i--)
   // {
   //     printf("%d", encoded[i]);
   // }
   // printf("\n");

    for(i=32;i<38;i++)
    {
        ecc[i-32]=encoded[i];
    }

    for(i=5;i>-1;i--)
    {
   //     printf("%d", ecc[i]);
        ecc_word|=ecc[i]<<i;
    }
   // printf("\n");
    return ecc_word;
}

int* serbit(int data, int size)
{
    int *bits = (int*)malloc(sizeof(int) * size);
    int pos=0;
    int i;
    unsigned int n;

    //printf("data: %d\n", data);

    for(i=0;i<size;i++)
    {
        bits[i]=0;
    }

    n = (unsigned int) data;

    while (n) 
    {
        if (n & 1)  bits[pos]=1;
        else        bits[pos]=0;
        n >>= 1;
        pos++;
    }

    // for debug
    //for(i=size-1;i>-1;i--)
    //{
    //    printf("%d", bits[i]);
    //}
    //printf("\n");

    return bits;
}

int parbit(int data[], int size)
{
    int i,j;
    int par;

    for(i=0;i<size;i++)
    {
        par|=data[i]<<i;
    }
   // printf("%d\n", par);
    return par;
}

int* decode_32(int ecc_bits[], int data_bits[])
{
    int i,j;
    int *syn = (int*)malloc(sizeof(int) * 6);
    int *data = (int*)malloc(sizeof(int) * 38);

    for(j=0;j<32;j++) data[j]=data_bits[j];
    for(j=32;j<38;j++) data[j]=ecc_bits[j-32];

   // printf("decode data: ");
   // for(j=37;j>-1;j--) printf("%d", data[j]);
   // printf("\n");

    for(i=0;i<6;i++)
    {
        syn[i]=0;
        for(j=0;j<38;j++)
           syn[i]+=(hmatrix_32[i][j]*data[j]);
           syn[i]=syn[i]%2;
    }
    return syn;
}

int detect(int syn[])
{
    int i,j;
    for(i=0;i<38;i++)
    {
        for(j=0;j<6;j++){
           if (syn[j]==hmatrix_32[j][i]) { if (j==5) return i;}
           else break;
        }
    }
    return -1;
}

void fault_inject() {
    int ii;
    int r ,c ,i, j, pos;
    //int p1, p2;
    int MASK;
    int value;
    for (ii=0; ii<NR_FAULT; ii++)
    {
        r=rand()%SCALE;
        c=rand()%SCALE;
        i=rand()%8;
        j=rand()%8;
        pos=rand()%32;
        MASK=1<<pos;

        printf("fault injection in dct[] array at position %d, %d, %d, %d, bit %d ", r, c, i, j, pos);

        value = ( int)dct[r * 8 + i][c * 8 + j];
        printf("original data %d ", value);
        //p1=parity(value);
        value ^= MASK;
        printf("after FI data %d\n", value);
        //p2=parity(value);
        dct[r * 8 + i][c * 8 + j] = (double)value;
    }
    
#ifdef PAR
    int nr_par=NR_FAULT/32;
    for (ii=0; ii<nr_par; ii++)
    {
        r=rand()%SCALE;
        c=rand()%SCALE;
        i=rand()%8;
        j=rand()%8;
        pos=0;
        MASK=1<<pos;

        printf("fault injection in par[] array at position %d, %d, %d, %d, bit %d ", r, c, i, j, pos);

        value = par_dct[r * 8 + i][c * 8 + j];
        printf("original data %d ", value);
        //p1=parity(value);
        value ^= MASK;
        printf("after FI data %d\n", value);
        //p2=parity(value);
        par_dct[r * 8 + i][c * 8 + j] = value;
    }
#endif
#ifdef ECC 
    int nr_ecc=NR_FAULT*6/32;
    for (ii=0; ii<nr_ecc; ii++)
    {
        r=rand()%SCALE;
        c=rand()%SCALE;
        i=rand()%8;
        j=rand()%8;
        pos=rand()%6;
        MASK=1<<pos;

        printf("fault injection in ecc[] array at position %d, %d, %d, %d, bit %d ", r, c, i, j, pos);

        value = ecc_dct[r * 8 + i][c * 8 + j];
        printf("original data %d ", value);
        //p1=parity(value);
        value ^= MASK;
        printf("after FI data %d\n", value);
        //p2=parity(value);
        ecc_dct[r * 8 + i][c * 8 + j] = value;
    }
#endif

    printf("\n");
}

void DCT() {
  FILE *ofp, *ofppar, *ofpavg;
  ofp=fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_work/dct.txt", "w");
  ofppar=fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_work/par_dct.txt", "w");
  ofpavg=fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_work/avg_dct.txt", "w");
  int r, c, i, j, x, y;
  for (r = 0; r < SCALE; r++)
    for (c = 0; c < SCALE; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
          double sum = 0;
          for (x = 0; x < 8; x++)
            for (y = 0; y < 8; y++)
              sum += (pic[r * 8 + x][c * 8 + y] - 128) * COS[x][i] * COS[y][j];
          sum *= C[i] * C[j] * 0.25;
          dct[r * 8 + i][c * 8 + j] = sum;
          printf("DCT done at position %d, %d, %d, %d\n", r, c, i, j);
#ifdef PAR
          // encode parity
          par_dct[r * 8 + i][c * 8 + j] = parity((int)dct[r * 8 + i][c * 8 + j]);
#endif
#ifdef ECC 
          // encode ecc
          int *data_bits = serbit(( int)dct[r * 8 + i][c * 8 + j], 32);
          ecc_dct[r * 8 + i][c * 8 + j] = encode_32(data_bits);
      //    printf("encode ecc : %d\n", ecc_dct[r * 8 + i][c * 8 + j]);
      //    printf("encode data: %d\n", (int)dct[r * 8 + i][c * 8 + j]);
#endif
          avg_dct[i][j]+=sum;
      }
  for (r = 0; r < N; r++)
  {
    for (c = 0; c < N; c++)
    {
      fprintf(ofp, "%f ", dct[r][c]);
      fprintf(ofppar, "%d ", par_dct[r][c]);
    }
    fprintf(ofp, "\n");
    fprintf(ofppar, "\n");
  }

  double avalue;
  for (i = 0; i < 8; i++)
  {
    for (j = 0; j < 8; j++)
    {
      avalue=avg_dct[i][j]/4096;
      fprintf(ofpavg, "%f ", avalue);
    }
    fprintf(ofpavg, "\n");
  }
}

void IDCT() {
    FILE *ofp;
  ofp=fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_work/idct.txt", "w");
  int r, c, i, j, x, y;
  for (r = 0; r < SCALE; r++)
    for (c = 0; c < SCALE; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
          double sum = 0;
          for (x = 0; x < 8; x++)
            for (y = 0; y < 8; y++)
            {
              sum += C[x] * C[y] * dct[r * 8 + x][c * 8 + y] * COS[i][x] * COS[j][y];
            }
          sum *= 0.25;
          sum += 128;
          idct[r * 8 + i][c * 8 + j] = sum;
          printf("IDCT done at position %d, %d, %d, %d\n", r, c, i, j);
      }
  for (r = 0; r < N; r++)
  {
    for (c = 0; c < N; c++)
    {
      fprintf(ofp, "%d ", (int)idct[r][c]);
    }
    fprintf(ofp, "\n");
  }
}

void quantization() {
  FILE *ofpavg;
  ofpavg=fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_work/avg_quant.txt", "w");
  int table[8][8] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 82,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
  };
  int r, c, i, j;
  for (r = 0; r < SCALE; r++)
    for (c = 0; c < SCALE; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            // check parity
#ifdef CHK_QUAN
#ifdef PAR
              int par_chk;
              par_chk = parity(( int)dct[r * 8 + i][c * 8 + j]);

              if (par_chk!=par_dct[r * 8 + i][c * 8 + j])
              {
                 printf("missmatch! location %d, %d, %d, %d, Substitute with golden DCT matrix.\n", r, c, i, j);
                 dct[r * 8 + i][c * 8 + j]=golden_dct[i][j];
              }
#endif
#ifdef ECC
            // ecc check
  //            printf("decode ecc : %d\n", ecc_dct[r * 8 + i][c * 8 + j]); 
  //            printf("decode data: %d\n", (int)dct[r * 8 + i][c * 8 + j]); 

              int *ecc = serbit(ecc_dct[r * 8 + i][c * 8 + j], 6);
              int *data = serbit((int)dct[r * 8 + i][c * 8 + j], 32);
              int *syndrome = decode_32(ecc, data);
              int err_pos = detect(syndrome);
              if (err_pos != -1)
              {
                  printf("bit position: %d is wrong\n", err_pos); 
                  if (err_pos < 32)
                  {
                      int wrong = parbit(data, 32);
                      printf("wrong data %d ", wrong);
                      data[err_pos]= data[err_pos]?0:1;
                      int update = parbit(data, 32);
                      dct[r * 8 + i][c * 8 + j]= update;
                      printf("after correction: %d\n", update);
                  }
              }
#endif
#endif
              dct[r * 8 + i][c * 8 + j] = round(dct[r * 8 + i][c * 8 + j] / table[i][j]);

          // encode parity
#ifdef PAR
              par_quan[r * 8 + i][c * 8 + j] = parity(( int)dct[r * 8 + i][c * 8 + j]);
#endif
#ifdef ECC 
          // encode ecc
              int *data_bits = serbit(( int)dct[r * 8 + i][c * 8 + j], 32);
              ecc_dct[r * 8 + i][c * 8 + j] = encode_32(data_bits);
          //    printf("encode ecc : %d\n", ecc_dct[r * 8 + i][c * 8 + j]);
          //    printf("encode data: %d\n", (int)dct[r * 8 + i][c * 8 + j]);
#endif
              avg_quant[i][j]+=dct[r * 8 + i][c * 8 + j];
        }

  double avalue;
  for (i = 0; i < 8; i++)
  {
    for (j = 0; j < 8; j++)
    {
      avalue=avg_quant[i][j]/4096;
      fprintf(ofpavg, "%f ", avalue);
    }
    fprintf(ofpavg, "\n");
  }
}

void dequantization() {
  int table[8][8] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 82,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
  };
  int r, c, i, j;
  for (r = 0; r < SCALE; r++)
    for (c = 0; c < SCALE; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
            // check parity
#ifdef CHK_DEQUAN
#ifdef PAR
              int par_chk;
              par_chk = parity(( int)dct[r * 8 + i][c * 8 + j]);

              if (par_chk!=par_quan[r * 8 + i][c * 8 + j])
              {
                 printf("missmatch! location %d, %d, %d, %d, Substitute with golden DCT matrix.\n", r, c, i, j);
                 dct[r * 8 + i][c * 8 + j]=golden_quan[i][j];
              }
#endif
#ifdef ECC
            // ecc check
  //            printf("decode ecc : %d\n", ecc_dct[r * 8 + i][c * 8 + j]); 
  //            printf("decode data: %d\n", (int)dct[r * 8 + i][c * 8 + j]); 

              int *ecc = serbit(ecc_dct[r * 8 + i][c * 8 + j], 6);
              int *data = serbit((int)dct[r * 8 + i][c * 8 + j], 32);
              int *syndrome = decode_32(ecc, data);
              int err_pos = detect(syndrome);
              if (err_pos != -1)
              {
                  printf("bit position: %d is wrong\n", err_pos); 
                  if (err_pos < 32)
                  {
                      int wrong = parbit(data, 32);
                      printf("wrong data %d ", wrong);
                      data[err_pos]= data[err_pos]?0:1;
                      int update = parbit(data, 32);
                      dct[r * 8 + i][c * 8 + j]= update;
                      printf("after correction: %d\n", update);
                  }
              }
#endif
#endif
              dct[r * 8 + i][c * 8 + j] = dct[r * 8 + i][c * 8 + j] * table[i][j];
        }
}

void MSE() {
  FILE *ofp, *ofp2;
  ofp=fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_work/sub.txt", "w");
  double MSE = 0;
  int r, c;
  for (r = 0; r < N; r++)
  {
    for (c = 0; c < N; c++) {
      MSE += ((double)pic[r][c] - idct[r][c]) * ((double)pic[r][c] - idct[r][c]);
      fprintf(ofp, "%d ", abs(pic[r][c] - (int)idct[r][c]));
    }
    fprintf(ofp, "\n");
  }
  MSE /= (512 * 512);
  double PSNR = 10 * log10(255 * 255 / MSE);
  ofp2=fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_work/PSNR.txt", "w");
  fprintf(ofp2, "%.2lf\n", PSNR);
}

int main() {
  FILE *ifp;
  ifp = fopen("/net/heap/wangz/zheng-work/CI_QoS/Arch/PD_RISC_LLVM/app/jpeg_work/Lena.txt", "r");
  int r, c;
  for (r = 0; r < N; r++)
  {
    for (c = 0; c < N; c++)
    {
      fscanf(ifp, "%d", &pic[r][c]);
      printf("load at %d, %d, value %d\n", r, c, pic[r][c]);
    }
  }
  init();
  DCT();
  fault_inject();
  quantization();
  fault_inject();
  dequantization();
  IDCT();
  MSE();
  return 0;
}
