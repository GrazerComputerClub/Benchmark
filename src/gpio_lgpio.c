// GPIO write speed benchmark
//
// by GC2 - mstroh
// Licence: CC BY 4.0
// Install: sudo apt install liblgpio1 liblgpio-dev
// Compile: gcc gpio_lgpio.c -o gpio_lgpio -llgpio -Wall
// Execute: sudo ./gpio_lgpio


#include <lgpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "pimodel.h"


const int GPIO = 24;
const int GPIOChip = 0;


int main (void) {
	const int  ToggleValue = 6050000;
	struct timeval t1, t2;
	double elapsedTime, fTimePerOperation, fFreq;

	printf("lgpio GPIO speed test program (using GPIO %d)\n", GPIO);

	int model = GetRPiModel();
	if (model==Pi5_ModelNo) { //Raspberry Pi 5
			printf("found model: %d, Raspberry Pi 5\n", model);
	} else if(model>0) {
			printf("found model: %d\n", model);
	} else if(model==0) {
			printf("unknown model (old style revision code %x)\n", GetRPiRevision());
	} else {
			printf("could not detect revision, looks like this is not a Raspberry Pi system\n");
			return EXIT_FAILURE;
	}

	int chip = -1;
	if (model>=Pi5_ModelNo) { //Raspberry Pi 5
			chip = lgGpiochipOpen(4);
	} else {
			chip = lgGpiochipOpen(0);
	}

	if (chip<0) {
		printf("ERROR: Failed to get GPIO chip %d.\n", GPIOChip);
		return 1;
	}
	int lFlags = 0;
	int initLevel = 0;
	if (lgGpioClaimOutput(chip, lFlags, GPIO, initLevel) != LG_OKAY) {
		printf("ERROR: Failed to get GPIO %d for output.\n", GPIO);
		lgGpiochipClose(chip);
		return 1;
	}

	printf("toggle % 3d million times ...\n", ToggleValue/1000000);
	gettimeofday(&t1, NULL);
	for (int loop=1; loop<ToggleValue; loop++) {
		lgGpioWrite(chip, GPIO, 1);
		lgGpioWrite(chip, GPIO, 0);
	}
	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec)/1000000.0;
	fTimePerOperation = elapsedTime*1000000.0/ToggleValue;
	fFreq = ToggleValue/elapsedTime/1000000.0;
	printf("  % 9d toggle took %.3f s, Time per toggle %.3f us, Freq %.3f MHz \n",
	  ToggleValue, elapsedTime, fTimePerOperation, fFreq);

	lgGpioClaimInput(chip, lFlags, GPIO);
	lgGpiochipClose(chip);

	return(EXIT_SUCCESS);
}
