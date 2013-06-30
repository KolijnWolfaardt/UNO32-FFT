/*
This file is release under the MIT licence.

This is an implementation of the Cooley-Turkey FFT algorithm. It is supposed
to be used on a microcontroller, so it will be optimized for low-memory, no
floating point numbers and no math library.
*/

#include "fft.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265
/*
Note: not using structs or anything fancy, because this needs to go on a microprocessor.
*/
void fft2point(int* dataA,int* dataB,int pos1,int pos2);
void fft(int* data,int* result,int N);
void fill(int* data,int start,int end,int num,int incNum);
double complexMuxA(int a1,int b1,int a2,int b2);
double complexMuxB(int a1,int b1,int a2,int b2);
int lookUpSin(int k,int N);
int lookUpCos(int k,int N);

int sinLookUp[] = {
0,6,13,19,25,31,38,44,
50,56,62,68,74,80,86,92,
98,104,109,115,121,126,132,137,
142,147,152,157,162,167,172,177,
181,185,190,194,198,202,206,209,
213,216,220,223,226,229,231,234,
237,239,241,243,245,247,248,250,
251,252,253,254,255,255,256,256,
256
};

/*
This function does a simply butterfly. It should not be used, rather use fft()
to do a 2 point fft if you need it.
*/
void fft2point(int* dataA,int* dataB,int pos1,int pos2)
{
	int temp1 = dataA[pos1];
	int temp2 = dataB[pos1];

	//First part of the butterfly
	dataA[pos1] = temp1+dataA[pos2];
	dataB[pos1] = temp2+dataB[pos2];

	//Second part of the butterfly
	dataA[pos2] = temp1-dataA[pos2];
	dataB[pos2] = temp2-dataB[pos2];
}

/*
This function does the fft. 
   dataA is an array with size N
   dataB is an array with size N
   N is the size
*/
void fft(int* dataA, int* dataB,int N)
{
	//First, calculate the twiddle factors. Store as a double for percision.
	int wA[N/2];		    //Twiddle Factors
	int wB[N/2];
	int bitRLocations[N];   //Bit reversed Locations

	//Loop variables
	int loop = 0;
	int innerLoop = 0;

	//Size and position variables
	int butterflySize = 0;
	int currButterfly = 0;
	int pos1,pos2;
	int swapA;
	int swapB;


	for (loop=0;loop<N/2;loop++)
	{
		wA[loop] = lookUpCos(loop,N);
		wB[loop] = lookUpSin(loop,N);

		//printf("%.2f\t%.2f\t%.0f\t%.0f\n",wA[loop],wB[loop],cos(-2*PI*(double)loop/(double)N),sin(-2*PI*(double)loop/(double)N));
	}

	//The bit-reversed addresses.
	fill(bitRLocations,0,N,0,1);
	//Reverse the data points
	for (loop=0;loop<N;loop++)
	{
		if (bitRLocations[loop]>loop)
		{
			//Swap them
			swapA = dataA[loop];
			swapB = dataB[loop];

			dataA[loop] = dataA[bitRLocations[loop]];
			dataB[loop] = dataB[bitRLocations[loop]];

			dataA[bitRLocations[loop]] = swapA;
			dataB[bitRLocations[loop]] = swapB;
		}
	}

	//Do the FFT loop.
	//The outer loop is over the different sized butterflies
	//The inner loop is over the different sets of the same sized butterflies
	//The currButterfly loop is over the butterflies in the same set
	for (loop=1; pow(2,loop)<=N; loop+=1)
	{
		butterflySize = pow(2,loop);
		//Internal Loop
		for (innerLoop=0; innerLoop<N; innerLoop+=butterflySize)
		{
			//Figure out what butterflies to do
			for (currButterfly=0;currButterfly<butterflySize/2;currButterfly+=1)
			{
				//Get the data positions
				pos1 = innerLoop+currButterfly;
				pos2 = innerLoop+currButterfly+butterflySize/2;

				///Pre-multiply all the second terms with the twiddle factors
				swapA = complexMuxA(dataA[pos2],dataB[pos2],wA[currButterfly*N/(butterflySize)],wB[currButterfly*N/(butterflySize)]);
				swapB = complexMuxB(dataA[pos2],dataB[pos2],wA[currButterfly*N/(butterflySize)],wB[currButterfly*N/(butterflySize)]);
				dataA[pos2] = swapA;
				dataB[pos2] = swapB;
			
				//Do the butterfly
				fft2point(dataA,dataB,pos1,pos2);
			}
		}
	}
	//done
}

/*
Generates a bit-reversed pattern, to figure out where the numbers are
*/
void fill(int* data,int start,int end,int num,int incNum)
{
	if (start==end-1)
	{
		data[start] = num;
		return;
	}
	//Split into two sections
	fill(data,start,start+(end-start)/2,num,2*incNum);
	fill(data,start+(end-start)/2,end,num+incNum,2*incNum);
}


/*
Two functions for doing the complex multiplications
*/
double complexMuxA(int a1,int b1,int a2,int b2)
{
	return (a1*a2-b1*b2)>>8;
}

double complexMuxB(int a1,int b1,int a2,int b2)
{
	return (a1*b2+b1*a2)>>8;
}

int lookUpSin(int k,int N)
{
	//There are 64 points in the loopup table
	int M = 64;
	int lookupPos = 0;

	if (k<N/4)
	{
		//First part of the sine wave
		lookupPos = 4*M*k/N;
	}
	else
	{
		//Second part of the sine wave
		lookupPos = 2*M-4*M*k/N;
	}
	return -sinLookUp[lookupPos];
}

int lookUpCos(int k,int N)
{
	int M = 64;
	int lookupPos = 0;

	if (k<N/4)
	{
		lookupPos = M-4*M*k/N;
		return sinLookUp[lookupPos];
	}
	else
	{
		lookupPos = 4*M*k/N-M;
		return -sinLookUp[lookupPos];
	}
}