/*
This file is release under the MIT licence.

This is an implementation of the Cooley-Turkey FFT algorithm. It is supposed
to be used on a microcontroller, so it will be optimized for low-memory, no
floating point numbers and no math library.
*/
#include <stdio.h>
#include "fft.h"

#define fft_size 64
#define lookupTableSize 64

/*
Note: not using structs or anything fancy, because this needs to go on a microprocessor.
*/
void init(double* tfstoreA,double* tfStoreB,int* brLookup);
void fft(double* data,double* result,double* tfstoreA,double* tfStoreB,int* brLookup);
void fill(int* data,int start,int end,int num,int incNum);
int myPow(int x,int n);
double lookUpSin(int k);
double lookUpCos(int k);

double sinTable[] = {0,0.02454,0.04907,0.07356,0.09802,0.12241,0.14673,0.17096,
0.19509,0.2191,0.24298,0.26671,0.29028,0.31368,0.33689,0.3599,
0.38268,0.40524,0.42756,0.44961,0.4714,0.4929,0.5141,0.535,
0.55557,0.57581,0.5957,0.61523,0.63439,0.65317,0.67156,0.68954,
0.70711,0.72425,0.74095,0.75721,0.77301,0.78835,0.80321,0.81758,
0.83147,0.84485,0.85773,0.87009,0.88192,0.89322,0.90399,0.91421,
0.92388,0.93299,0.94154,0.94953,0.95694,0.96378,0.97003,0.9757,
0.98079,0.98528,0.98918,0.99248,0.99518,0.99729,0.9988,0.9997,1};

void init(double* tfstoreA,double* tfstoreB,int* brLookup)
{
  int loop =0;

  for (loop=0;loop<fft_size/2;loop++)
  {
    //Calculate the cos term
    tfstoreA[loop] = lookUpCos(loop);
    //Calculate the sin term
    tfstoreB[loop] = lookUpSin(loop);
  }

  //The bit-reversed addresses.
  fill(brLookup,0,fft_size,0,1);
}

/*
	This function does the fft. 
	dataA is an array with size N
	dataB is an array with size N
	wA,wB and bitRLocations are arrays created by init()
*/
void fft(double* dataA,double* dataB,double* wA,double* wB,int* bitRLocations)
{
	//Loop variables
	int loop = 0;
	int innerLoop = 0;

	//Size and position variables
	int butterflySize = 0;
	int butterflySizeHalf=0;
	int currButterfly = 0;
	int pos1,pos2;
	int tfLocation=0;
	double swapA;
	double swapB;

	//Reverse the data points
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
	for (loop=1; myPow(2,loop)<=fft_size; loop+=1)
	{
		butterflySize = myPow(2,loop);
		//Internal Loop
		for (innerLoop=0; innerLoop<fft_size; innerLoop+=butterflySize)
		{
			butterflySizeHalf = butterflySize>>1;
			//Figure out what butterflies to do
			for (currButterfly=0;currButterfly<butterflySizeHalf;currButterfly+=1)
			{
				//Get the data positions
				pos1 = innerLoop+currButterfly;
				pos2 = pos1+butterflySizeHalf;
				//tfLocation = currButterfly*fft_size/(butterflySize);
				tfLocation = currButterfly*fft_size>>loop;

				//Multiply all the second terms with the twiddle factors
				swapA = dataA[pos2]*wA[tfLocation] - dataB[pos2]*wB[tfLocation];
				swapB = dataA[pos2]*wB[tfLocation] + dataB[pos2]*wA[tfLocation];

				/*
				This part was modified heavily from the original. The 2-point fft is done here,
				but it uses the fact that two of the values are already stored in swapA and swapB,
				and does the butterfly is a different order.
				*/
				//Second part of the butterfly
				dataA[pos2] = dataA[pos1]-swapA;
				dataB[pos2] = dataB[pos1]-swapB;

				//First part of the butterfly
				dataA[pos1] = dataA[pos1]+swapA;
				dataB[pos1] = dataB[pos1]+swapB;
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
Performs x^n, to remove math.h dependancy
*/
int myPow(int x,int n)
{
	int i;
	int ans = 1;
	for(i=0;i<n;i++)
		ans=ans*x;

	return ans;
}

double lookUpSin(int k)
{
	//There are 64 points in the lookup table
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
	return -sinTable[lookupPos];
}

double lookUpCos(int k)
{
	
	int M = 64;
	int lookupPos = 0;

	
	if (k<fft_size/4)
	{
		lookupPos = M-4*M*k/fft_size;
		return sinTable[lookupPos];
	}
	else
	{
		lookupPos = 4*M*k/fft_size-M;
		return -sinTable[lookupPos];
	}
}
