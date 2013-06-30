/*
This file is release under the MIT licence.

This is an implementation of the Cooley-Turkey FFT algorithm. It is supposed
to be used on a microcontroller, so it will be optimized for low-memory, no
floating point numbers and no math library.
*/

#include "fft.h"
#include <stdio.h>

#define fft_size 32
/*
Note: not using structs or anything fancy, because this needs to go on a microprocessor.
*/
void fft2point(int* dataA,int* dataB,int pos1,int pos2);
void init(int* tfstoreA,int* tfstoreB,int* brLookup);
void fft(int* dataA, int* dataB,int* wA,int* wB,int* bitRLocations);
void fill(int* data,int start,int end,int num,int incNum);
double complexMuxA(int a1,int b1,int a2,int b2);
double complexMuxB(int a1,int b1,int a2,int b2);
int lookUpSin(int k);
int lookUpCos(int k);
int doPow(int x,int y);

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
This function does the initialization for the FFT. This is to save execution
time at the expense of memory. The function calculates and stores the twiddle
factors, and the bit-reversed lookup. The computed values should be passed
to the main fft function.
*/
void init(int* tfstoreA,int* tfstoreB,int* brLookup)
{
	int loop = 0;

	//Calculate and store the twiddle factors using the sin lookup table
	for (loop=0;loop<fft_size/2;loop++)
	{
		tfstoreA[loop] = lookUpCos(loop);
		tfstoreB[loop] = lookUpSin(loop);
	}

	//Calculate he bit-reversed addresses.
	fill(brLookup,0,fft_size,0,1);
}

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
void fft(int* dataA, int* dataB,int* wA,int* wB,int* bitRLocations)
{
	//Loop variables
	int loop = 0;
	int innerLoop = 0;

	//Size and position variables
	int butterflySize = 0;
	int butterflySizeHalf = 0;
	int currButterfly = 0;
	int pos1,pos2;
	int swapA;
	int swapB;
	int tf_pos=0;

	//Reverse the data points. 
	/*
	Note: The data is placed at the bit-reversed locations here. This
	operation executes in O(N) time, so I don't regard it as too much
	overhead. Doing it this way allows the function to do the fft in-place.
	If the data is not moved around a new buffer needs to be created for the
	result of the fft.
	*/
	for (loop=0;loop<fft_size;loop++)
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
	for (loop=1; doPow(2,loop)<=fft_size; loop+=1)
	{
		butterflySize = doPow(2,loop);
		butterflySizeHalf = butterflySize>>1; //Optimization, so we don't need to do this everytime we need it. It is basically divide by 2
		//Internal Loop
		for (innerLoop=0; innerLoop<fft_size; innerLoop+=butterflySize)
		{
			//Figure out what butterflies to do
			for (currButterfly=0;currButterfly<butterflySizeHalf;currButterfly+=1)
			{
				//Get the data positions
				pos1 = innerLoop+currButterfly;
				pos2 = innerLoop+currButterfly+butterflySizeHalf;

				tf_pos = currButterfly*fft_size>>loop; //This does the eqivalent of dividing by butteflySize

				///Pre-multiply all the second terms with the twiddle factors
				swapA = complexMuxA(dataA[pos2], dataB[pos2], wA[tf_pos], wB[tf_pos]);
				swapB = complexMuxB(dataA[pos2], dataB[pos2], wA[tf_pos], wB[tf_pos]);

				//Some hacking going to happen here:
				/*
				dataA[pos2] = swapA;
				dataB[pos2] = swapB;
			
				//Do the butterfly. This used to be in it's own function, but this is much faster
				swapA = dataA[pos1];
				swapB = dataB[pos1];

				//First part of the butterfly
				dataA[pos1] = swapA+dataA[pos2];
				dataB[pos1] = swapB+dataB[pos2];

				//Second part of the butterfly
				dataA[pos2] = swapA-dataA[pos2];
				dataB[pos2] = swapB-dataB[pos2];*/

				//Mental note: swapA and swapB already contain dataA[pos] and dataB[pos2] assign them first
				//The data was placed into swapA and swapB by the multiply functions above
				//Do pos2 data first, because pos2's values are place in swap
				dataA[pos2] = dataA[pos1]-swapA;
				dataB[pos2] = dataB[pos1]-swapB;
				
				dataA[pos1] = dataA[pos1]+swapA;
				dataB[pos1] = dataB[pos1]+swapB;

			}
		}
	}
	//done :-)
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

int lookUpSin(int k)
{
	//There are 64 points in the loopup table
	int M = 64;
	int lookupPos = 0;

	if (k<fft_size/4)
	{
		//First part of the sine wave
		lookupPos = 4*M*k/fft_size;
	}
	else
	{
		//Second part of the sine wave
		lookupPos = 2*M-4*M*k/fft_size;
	}
	return -sinLookUp[lookupPos];
}

int lookUpCos(int k)
{
	int M = 64;
	int lookupPos = 0;

	if (k<fft_size/4)
	{
		lookupPos = M-4*M*k/fft_size;
		return sinLookUp[lookupPos];
	}
	else
	{
		lookupPos = 4*M*k/fft_size-M;
		return -sinLookUp[lookupPos];
	}
}

/*
Calculates x^y, so that I don't need <math.h>
*/
int doPow(int x,int y)
{
	int ans =x;
	int l = 0;
	for (l=0;l<y-1;l++)
	{
		ans=ans*x;
	}
	return ans;
}