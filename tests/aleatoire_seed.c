
#include <stdio.h>

#define W 32
#define R 16
#define P 0
#define M1 13
#define M2 9
#define M3 5

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define MAT3NEG(t,v) (v<<(-(t)))
#define MAT4NEG(t,b,v) (v ^ ((v<<(-(t))) & b))

#define V0            STATE[state_i                   ]
#define VM1           STATE[(state_i+M1) & 0x0000000fU]
#define VM2           STATE[(state_i+M2) & 0x0000000fU]
#define VM3           STATE[(state_i+M3) & 0x0000000fU]
#define VRm1          STATE[(state_i+15) & 0x0000000fU]
#define VRm2          STATE[(state_i+14) & 0x0000000fU]
#define newV0         STATE[(state_i+15) & 0x0000000fU]
#define newV1         STATE[state_i                 ]
#define newVRm1       STATE[(state_i+14) & 0x0000000fU]

#define FACT 2.32830643653869628906e-10

static unsigned int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;

void InitWELLRNG512a (unsigned int *init)
{
   int j;
   state_i = 0;
   for (j = 0; j < R; j++)
      STATE[j] = init[j];
}

double WELLRNG512a (void)
{
   z0 = VRm1;
   z1 = MAT0NEG(-16, V0) ^ MAT0NEG(-15, VM1);
   z2 = MAT0POS(11, VM2);
   newV1 = z1 ^ z2;
   newV0 = MAT0NEG(-2, z0) ^ MAT0NEG(-18, z1) ^ MAT3NEG(-28, z2) ^ MAT4NEG(-5, 0xda442d24U, newV1);
   state_i = (state_i + 15) & 0x0000000fU;
   return ((double) STATE[state_i]) * FACT;
}


#define MASK32  0xffffffffU
#define JMAX 16
static unsigned int B[JMAX];


static void init (unsigned int *A)
{
   int i;
   A[0] = 123456789;
   for (i = 1; i < JMAX; i++)
      A[i] = (663608941 * A[i - 1]) & MASK32;
}

void aff_state()
{
   int i;
   for(i=0;i<R;i++)
   {
      printf("%d ",STATE[i]);
   }
   printf("\n");
}


int main (void)
{
   int i;
   double u, som = 0;
   int n = 10;
   int states[10];
   init (B);
   InitWELLRNG512a (B);
   for (i = 0; i < n; i++) {
      states[i]=state_i;
      aff_state();
      printf("%f %d\n",u = WELLRNG512a(),state_i);
   }
   printf("\n");
   for (i = 9; i >=0; i--) {
      state_i= states[i];
      aff_state();
      printf("%f %d\n",u = WELLRNG512a(),state_i);
   }

   return 0;
}
