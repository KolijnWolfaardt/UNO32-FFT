#include <stdio.h>
#include <math.h>
#include "fft.h"



int main(void)
{

	double data8A[] = {1,2,3,2,1,0,-1,-2};
	double data8B[] = {0,0,0,0,0,0,0,0};
	int N = 8;
	int loop = 0;


	fft(data8A,data8B,N);

	printf("\n");
	for (loop=0; loop<N;loop++)
	{
		printf("%d\t%f+i%f\n",loop,data8A[loop],data8B[loop]);
	}

	return 0;
}

