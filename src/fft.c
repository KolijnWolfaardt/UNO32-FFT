/*
This file is release under the MIT licence.

This is an implementation of the Cooley-Turkey FFT algorithm. It is supposed
to be used on a microcontroller, so it will be optimized for low-memory, no
floating point numbers and no math library.
*/

#include "fft.h"
#include <stdio.h>
#include <math.h>

#define fft_size 64

#define PI 3.14159265
/*
Note: not using structs or anything fancy, because this needs to go on a microprocessor.
*/
void init(double* tfstoreA,double* tfStoreB,int* brLookup);
void fft(double* data,double* result,double* tfstoreA,double* tfStoreB,int* brLookup);
void fill(int* data,int start,int end,int num,int incNum);

void init(double* tfstoreA,double* tfstoreB,int* brLookup)
{
	int loop =0;

	for (loop=0;loop<fft_size/2;loop++)
	{
		//Calculate the cos term
		tfstoreA[loop] = cos(2*PI*(double)loop/(double)fft_size);
		//Calculate the sin term
		tfstoreB[loop] = sin(-2*PI*(double)loop/(double)fft_size);
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
	for (loop=1; pow(2,loop)<=fft_size; loop+=1)
	{
		butterflySize = pow(2,loop);
		//Internal Loop
		for (innerLoop=0; innerLoop<fft_size; innerLoop+=butterflySize)
		{
			//Figure out what butterflies to do
			for (currButterfly=0;currButterfly<butterflySize/2;currButterfly+=1)
			{
				//Get the data positions
				pos1 = innerLoop+currButterfly;
				pos2 = innerLoop+currButterfly+butterflySize/2;
				tfLocation = currButterfly*fft_size/(butterflySize);

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