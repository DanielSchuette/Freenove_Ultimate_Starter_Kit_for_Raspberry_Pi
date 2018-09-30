#include <wiringPi.h>
#include <stdio.h>
#include <sys/time.h>

#define trigPin 4       
#define echoPin 5
#define ledPin 0		// look pin naming table up: $gpio readall
#define MAX_DISTANCE 220        // define the maximum measured distance
#define timeOut MAX_DISTANCE*60 // calculate timeout according to the maximum measured distance
#define THRESHOLD 7		// threshold of 7cm for a warning signal via `ledPin' LED

#ifndef TRUE
#  define TRUE  (1 == 1)
#  define FALSE (1 == 2)
#endif

int pulseIn(int pin, int level, int timeout);
float getSonar();

int main(){
    printf("Program is starting ... \n");
    /* when initialize wiring failed,print messageto screen */
    if(wiringPiSetup() == -1){ 
        printf("setup wiringPi failed !");
        return 1; 
    }
    float distance = 0;
    pinMode(ledPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    while(TRUE){
        distance = getSonar();
        printf("distance : %7.2f cm\n", distance);
	if (distance < THRESHOLD) {
		digitalWrite(ledPin, HIGH);
	} else {
		digitalWrite(ledPin, LOW);
	}
        delay(1000);
    }   
    return 1;
}

/* get the measurement results of ultrasonic module in cm */
float getSonar(){   
    long pingTime;
    float distance;
    digitalWrite(trigPin, HIGH); // trigPin gets a 10us high signal
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    pingTime = pulseIn(echoPin, HIGH, timeOut);   // read `plus time' of echoPin
    distance = (float)pingTime * 340.0 / 2.0 / 10000.0; // the speed of sound is 340m/s
    return distance;
}

int pulseIn(int pin, int level, int timeout)
{
   struct timeval tn, t0, t1;
   long micros;
   gettimeofday(&t0, NULL);
   micros = 0;
   while (digitalRead(pin) != level)
   {
      gettimeofday(&tn, NULL);
      if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
      micros += (tn.tv_usec - t0.tv_usec);
      if (micros > timeout) return 0;
   }
   gettimeofday(&t1, NULL);
   while (digitalRead(pin) == level)
   {
      gettimeofday(&tn, NULL);
      if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
      micros += (tn.tv_usec - t0.tv_usec);
      if (micros > timeout) return 0;
   }
   if (tn.tv_sec > t1.tv_sec) micros = 1000000L; else micros = 0;
   micros += (tn.tv_usec - t1.tv_usec);
   return micros;
}
