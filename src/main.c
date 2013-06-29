#include <stdio.h>
#include <math.h>
#include "fft.h"



int main(void)
{

	//Some test cases:
	double data16A[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double data16B[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	double data32A[] = {1,0.938,0.324,-0.057,0.293,1.119,1.631,1.214,0,-1.214,-1.631,-1.119,-0.293,0.057,-0.324,-0.938,-1,-0.173,1.09,1.905,1.707,0.729,-0.217,-0.449,0,0.449,0.217,-0.729,-1.707,-1.905,-1.09,0.173};
	double data32B[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int loop = 0;

	fft(data16A,data16B,16);

	printf("\nDoing FFT with an impulse. Output should be a just 1s");
	for (loop=0; loop<16;loop++)
	{
		printf("%d\t%f+i%f\n",loop,data16A[loop],data16B[loop]);
	}

	fft(data32A,data32B,32);
	printf("\nDoing FFT with an a sine and a cosine.Cosine at 5, and sine at 2");
	for (loop=0; loop<32;loop++)
	{
		printf("%d\t%f+i%f\n",loop,data32A[loop],data32B[loop]);
	}

	return 0;
}

