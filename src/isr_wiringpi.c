// GPIO ISR speed benchmark
//
// by GC2 - mstroh
// Licence: CC BY 4.0
// Install: see https://github.com/WiringPi/WiringPi
// Compile: gcc isr_wiringpi.c -o isr_wiringpi -Wall -lwiringPi

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

static volatile int globalCounter;
volatile long long gStartTime, gEndTime;

static void wfi (void) { 
  struct timeval now;

  gettimeofday(&now, 0);
  if (0==gStartTime) {
    gStartTime = now.tv_sec*1000000LL + now.tv_usec;
  } else {
    gEndTime = now.tv_sec*1000000LL + now.tv_usec;
  }
  globalCounter++;
}


long long DurationTime(int Enge, int OUTpin, int IRQpin) {
  struct timeval now;
  long long fTime = 0;

  gStartTime = 0;
  gEndTime = 0;
  globalCounter = 0;
  printf("."); fflush(stdout);

  if (INT_EDGE_RISING == Enge) {
    digitalWrite(OUTpin, LOW); 
    delay(200);
    gStartTime = 0;
    gEndTime = 0;
    globalCounter = 0;
    gettimeofday(&now, 0);
    gStartTime = now.tv_sec*1000000LL + now.tv_usec;
    digitalWrite(OUTpin, HIGH);  
    delay(20);
    digitalWrite(OUTpin, LOW);  
   } else if (INT_EDGE_FALLING == Enge) {
    digitalWrite(OUTpin, HIGH); 
    delay(200);
    gStartTime = 0;
    gEndTime = 0;
    globalCounter = 0;
    gettimeofday(&now, 0);
    gStartTime = now.tv_sec*1000000LL + now.tv_usec;    
    digitalWrite(OUTpin, LOW);  
  }

  delay(200);
  fTime = (gEndTime - gStartTime);
  printf("."); fflush(stdout);

  return fTime;
}

int main (int argc, char *argv[])
{
  const int IRQpin = 22 ;
  const int OUTpin = 25 ;
  int Enge = INT_EDGE_RISING;
  const int IterationCalibration = 10000000;
  int IterationIRQ = 20;
  long long* lltime;
  long long lltime_sum, duration, duration_per_call;
  struct timeval now;
  int major, minor;

  wiringPiVersion(&major, &minor);

  wiringPiSetupGpio() ;

  pinMode(IRQpin, INPUT);
  pinMode(OUTpin, OUTPUT);
  

  if (argc >= 2 && 0==strcmp(argv[1], "falling")) {
    Enge = INT_EDGE_FALLING;
  }
  if (argc >= 3) {
    IterationIRQ = atoi(argv[2]);
  }
  lltime = (long long*)calloc(IterationIRQ, sizeof(long long));

  printf("\nMeasuring digitalWrite time (WiringPi %d.%d)\n", major, minor); 
  gettimeofday(&now, 0);
  gStartTime = now.tv_sec*1000000LL + now.tv_usec;
  for (int iteration=0; iteration<IterationCalibration; iteration++) { 
    digitalWrite (OUTpin, LOW) ;
  }
  gettimeofday(&now, 0);
  gEndTime = now.tv_sec*1000000LL + now.tv_usec;
  duration = (gEndTime - gStartTime);
  duration_per_call =  duration / IterationCalibration;
  printf("Duration digitalWrite: %lld usec (%d), per call %lld usec\n\n", duration, IterationCalibration, duration_per_call);

  printf("Measuring duration IRQ @ GPIO%d with trigger @ GPIO%d %s\n", IRQpin, OUTpin, INT_EDGE_RISING==Enge ? "rising" : "falling");
  wiringPiISR (IRQpin, Enge, &wfi) ; 
  for (int iteration=0; iteration<IterationIRQ; iteration++) { 
    lltime[iteration] = DurationTime(Enge, OUTpin, IRQpin);
  }
  //wiringPiISRStop (IRQpin) ;  // only 3.2 not active to support old 2.x too


  printf("\nResult:\n");
  lltime_sum = 0 ;
  for (int iteration=0; iteration<IterationIRQ; iteration++) { 
    printf("iteration %3d: %4llu usec\n", iteration+1, lltime[iteration]-duration_per_call);
    lltime_sum += lltime[iteration]-duration_per_call;
  }
  printf("\nIRQ detect time avg. %llu usec\n\n", lltime_sum/IterationIRQ);


  pinMode(OUTpin, INPUT);
  free(lltime);
}
