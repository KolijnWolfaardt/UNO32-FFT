#include <stdio.h>
#include "fft.h"

#define fft_size 64

int main(void)
{
	
	double data64A[] = {774,784,780,776,787,774,786,777,779,786,774,788,775,781,783,775,788,775,784,781,776,788,773,787,779,779,786,773,787,777,782,784,774,788,775,784,782,776,788,774,786,779,778,787,774,788,777,780,785,775,788,775,783,782,776,788,774,786,780,778,787,774,787,777};
	double data64B[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	double tfstoreA[fft_size/2];
	double tfstoreB[fft_size/2];
	int brLookup[fft_size];

	/*
	0:	49963
	1:	-8.8961 + 5.7966i
	2:	-6.0235 + 3.2627i
	3:	-4.1953 + 2.531i
	4:	-6.8501 + 1.3521i
	5:	-7.9719 + 2.415i
	6:	-6.3409 - 0.36504i
	7:	-4.4714 - 0.18974i
	8:	-3.5147 - 1.4853i
	9:	-6.1111 - 6.3124i
	10:	-9.9083 + 5.2851i
	11:	-5.3762 + 2.6742i
	*/
	int loop = 0;
	float a = 2.0;
	double num =0;

	
	init(tfstoreA,tfstoreB,brLookup);

	fft(data64A,data64B,tfstoreA,tfstoreB,brLookup);

	printf("\n");
	for (loop=0; loop<fft_size; loop++)
	{
		printf("%d\t%f + i%f\n",loop,data64A[loop],data64B[loop]);
	}

	return 0;
}
