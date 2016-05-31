/*
 * @(#)sieve.c	2205.1 02/02/08
 * All rights reserved.  Subject to conditions in RESTRICTIONS file.
 */

/*
 *		UNIX Operating System Implementation Test #4
 *
 * No benchmark suit would be complete without the ever-popular
 * sieve benchmark. It is a good test of compiler efficiency and
 * CPU throughput. Below is a sieve benchmark as presented in the
 * januari 1983 issue of BYTE, with some minor changes: Register 
 * declarations have been added, and some unnecessary (from our
 * standpoint) printf() statement removed.
 *
 */

/* Eratostenes Sieve Prime Number Program in C */

#define TRUE 1
#define FALSE 0
#define SIZE	256
#define ITERATIONS	1

char flags[SIZE+1];

int main()
{
    register int i, prime, k, count, iter;


    for (iter=1; iter <= ITERATIONS; iter++) {
        count=0;
        for (i=0; i <= SIZE; i++)
            flags[i]=TRUE;
        for (i=0; i <= SIZE; i++) {
            if (flags[i]) {
                prime = i + i + 3;
                for (k=i+prime; k <= SIZE; k +=prime)
                    flags[k]=FALSE;
                count++;
            }
        }
    }

    return count;
}
