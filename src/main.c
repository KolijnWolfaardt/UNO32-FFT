#include <stdio.h>
#include <math.h>
#include "fft.h"

#define fft_size 32

int main(void)
{
	int data32A[] = {510,-66,55,466,-435,-305,83,59,255,59,83,-305,-435,466,55,-66,510,-490,-416,366,-75,195,278,-224,-255,-224,278,195,-75,366,-416,-490};
	int data32B[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int tfstoreA[fft_size];
	int tfstoreB[fft_size];
	int brLookup[fft_size];

	int loop = 0;

	init(tfstoreA,tfstoreB,brLookup);

	fft(data32A,data32B,tfstoreA,tfstoreB,brLookup);
	
	printf("\nDoing FFT with an a sine and a cosine.Cosine at 5, and sine at 2");
	for (loop=0; loop<fft_size;loop++)
	{
		printf("%d\t%d+i%d\n",loop,data32A[loop],data32B[loop]);
	}

	return 0;
}

