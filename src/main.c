#include <stdio.h>
#include <math.h>
#include "fft.h"



int main(void)
{

	//Some test cases:
	int data16A[] = {255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int data16B[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int data32A[] = {510,-66,55,466,-435,-305,83,59,255,59,83,-305,-435,466,55,-66,510,-490,-416,366,-75,195,278,-224,-255,-224,278,195,-75,366,-416,-490};
	int data32B[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int loop = 0;

	fft(data16A,data16B,16);

	
	printf("\nDoing FFT with an impulse. Output should be a just 1s");
	for (loop=0; loop<16;loop++)
	{
		printf("%d\t%d+i%d\n",loop,data16A[loop],data16B[loop]);
	}

	fft(data32A,data32B,32);
	
	printf("\nDoing FFT with an a sine and a cosine.Cosine at 5, and sine at 2");
	for (loop=0; loop<32;loop++)
	{
		printf("%d\t%d+i%d\n",loop,data32A[loop],data32B[loop]);
	}

	return 0;
}

