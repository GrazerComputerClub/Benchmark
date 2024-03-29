// GPIO write speed benchmark
//
// by GC2 - mstroh
// Licence: CC BY 4.0
// Install: see https://github.com/GrazerComputerClub/WiringPi
// Compile: gcc gpio_wiringpi_sysfs.c -o gpio_wiringpi_sysfs -Wall -lwiringPi

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


const int GPIO = 24;
const int  ToggleValue = 6700000;

void execute(const char* cmd) {
	printf("execute: %s\n", cmd);
	system(cmd);
}

int main (void) {
	struct timeval t1, t2;
	double elapsedTime, fTimePerOperation, fFreq;
	char cmd[64];

	printf("WiringPi GPIO speed test program (using GPIO %d via sysfs)\n", GPIO);
	sprintf(cmd, "gpio export %d out", GPIO);
	execute(cmd);

	if (wiringPiSetupSys()  == -1) {
		printf("wiringPiSetupSys failed\n\n");
		exit(EXIT_FAILURE);
	}

	printf("toggle % 3d million times ...\n", ToggleValue/1000000);
	gettimeofday(&t1, NULL);
	for (int loop=1; loop<ToggleValue; loop++) {
		digitalWrite(GPIO, LOW);
		digitalWrite(GPIO, HIGH);
	}
	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec)/1000000.0;
	fTimePerOperation = elapsedTime*1000000.0/ToggleValue;
	fFreq = ToggleValue/elapsedTime/1000000.0;
	printf("  % 9d toggle took %.3f s, Time per toggle %.3f us, Freq %.3f MHz \n",
	  ToggleValue, elapsedTime, fTimePerOperation, fFreq);
	digitalWrite(GPIO, LOW);
	sprintf(cmd, "gpio export %d in", GPIO);
	execute(cmd);
	sprintf(cmd, "gpio unexport %d", GPIO);
	execute(cmd);

	return(EXIT_SUCCESS);
}

