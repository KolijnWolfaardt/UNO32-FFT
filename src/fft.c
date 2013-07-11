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
void fft2point(double* dataA,double* dataB,int pos1,int pos2);
void fft(double* data,double* result);
void fill(int* data,int start,int end,int num,int incNum);
double complexMuxA(double a1,double b1,double a2,double b2);
double complexMuxB(double a1,double b1,double a2,double b2);

/*
This function does a simply butterfly. It should not be used, rather use fft()
to do a 2 point fft if you need it.
*/
void fft2point(double* dataA,double* dataB,int pos1,int pos2)
{
	double temp1 = dataA[pos1];
	double temp2 = dataB[pos1];

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
void fft(double* dataA,double* dataB)
{
	//First, calculate the twiddle factors. Store as a double for percision.
	double wA[fft_size/2];		    //Twiddle Factors
	double wB[fft_size/2];
	int bitRLocations[fft_size];   //Bit reversed Locations

	//Loop variables
	int loop = 0;
	int innerLoop = 0;

	//Size and position variables
	int butterflySize = 0;
	int currButterfly = 0;
	int pos1,pos2;
	double swapA;
	double swapB;


	for (loop=0;loop<fft_size/2;loop++)
	{
		//Calculate the cos term
		wA[loop] = cos(2*PI*(double)loop/(double)fft_size);
		//Calculate the sin term
		wB[loop] = sin(-2*PI*(double)loop/(double)fft_size);
	}

	//The bit-reversed addresses.
	fill(bitRLocations,0,fft_size,0,1);
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

				///Pre-multiply all the second terms with the twiddle factors
				swapA = complexMuxA(dataA[pos2],dataB[pos2],wA[currButterfly*fft_size/(butterflySize)],wB[currButterfly*fft_size/(butterflySize)]);
				swapB = complexMuxB(dataA[pos2],dataB[pos2],wA[currButterfly*fft_size/(butterflySize)],wB[currButterfly*fft_size/(butterflySize)]);
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
double complexMuxA(double a1,double b1,double a2,double b2)
{
	return a1*a2-b1*b2;
}

double complexMuxB(double a1,double b1,double a2,double b2)
{
	return a1*b2+b1*a2;
}