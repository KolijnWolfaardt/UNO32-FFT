/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
#import "fft.h"

#define fft_size 64

void setup()
{                
  
  double data64A[] = {774,784,780,776,787,774,786,777,779,786,774,788,775,781,783,775,788,775,784,781,776,788,773,787,779,779,786,773,787,777,782,784,774,788,775,784,782,776,788,774,786,779,778,787,774,788,777,780,785,775,788,775,783,782,776,788,774,786,780,778,787,774,787,777};
  double data64B[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  double tfstoreA[fft_size/2];
  double tfstoreB[fft_size/2];
  int brLookup[fft_size];
  int loop=0;

  
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  Serial.begin(9600);
  Serial.println("Starting");
  
  //Do the initialization
  int startTime = micros();
  init(tfstoreA,tfstoreB,brLookup);
  int endTime = micros();
  
  Serial.print("\nInitializing took ");
  Serial.print(endTime-startTime);
  Serial.print("\n");
  
  //Do the FFT
  startTime = micros();
  fft(data64A,data64B,tfstoreA,tfstoreB,brLookup);
  endTime = micros();
  Serial.print("\n64 Point FFT took ");
  Serial.print(endTime-startTime);
  Serial.print("\n");
  
  
  for (loop=0; loop<fft_size;loop++)
  {
    Serial.print(loop);
    Serial.print("\t");
    Serial.print(data64A[loop]);
    Serial.print("\t");
    Serial.println(data64B[loop]);
  }
  Serial.println("Done");
  
}

void loop()
{
  /*digitalWrite(2, HIGH);   // set the LED on
  fft(data32A,data32B,32);
  digitalWrite(2, LOW); 
  delay(1000);              // wait for a second
*/
}
