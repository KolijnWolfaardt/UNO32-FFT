/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
#import "fft.h"

 
int data32A[] = {510,-66,55,466,-435,-305,83,59,255,59,83,-305,-435,466,55,-66,510,-490,-416,366,-75,195,278,-224,-255,-224,278,195,-75,366,-416,-490};
int data32B[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setup()
{                
  int loop = 0;
  
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  Serial.begin(9600);
  Serial.println("Starting");
  
  //Do the fft
  int startTime = micros();
  fft(data32A,data32B,32);
  int endTime = micros();
  
  
  Serial.print("FFT started at ");
  Serial.print(startTime);
  Serial.print(", and endedat");
  Serial.print(endTime);
  Serial.print("\nTotal time of ");
  Serial.print(endTime-startTime);
  Serial.print("\n");
  
  for (loop=0; loop<32;loop++)
  {
    Serial.print(loop);
    Serial.print("\t");
    Serial.print(data32A[loop]);
    Serial.print("\t");
    Serial.println(data32B[loop]);
  }
  Serial.println("Done");
  
}

void loop() {
  /*digitalWrite(2, HIGH);   // set the LED on
  fft(data32A,data32B,32);
  digitalWrite(2, LOW); 
  delay(1000);              // wait for a second
*/
}
