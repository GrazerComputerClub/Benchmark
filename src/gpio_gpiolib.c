// GPIO write speed benchmark
//
// by rafael2k
// Licence: CC BY 4.0
// Compile: gcc gpio_gpiolib.c gpiolib/gpiolib.c gpiolib/gpiochip_bcm2712.c gpiolib/gpiochip_bcm2835.c gpiolib/gpiochip_rp1.c gpiolib/util.c -o gpio_gpiolib

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "pimodel.h"
#include "gpiolib/gpiolib.h"

unsigned int line_num = 8;	// Pin #24, GPIO #8
const int  ToggleValue = 240000000;

int main(int argc, char **argv) {
	int ret;

	struct timeval t1, t2;
	double elapsedTime, fTimePerOperation, fFreq;

	printf("GPIOLIB speed test program (using line %d)\n", line_num);

    ret = gpiolib_init();
    if (ret < 0)
    {
        printf("Failed to initialise gpiolib - %d\n", ret);
        return EXIT_FAILURE;
    }

    ret = gpiolib_mmap();
    if (ret)
    {
        if (ret == EACCES && geteuid())
            printf("Must be root\n");
        else
            printf("Failed to mmap gpiolib - %s\n", strerror(ret));
        return EXIT_FAILURE;
    }


    gpio_set_drive(line_num, DRIVE_LOW);
    gpio_set_fsel(line_num, GPIO_FSEL_OUTPUT);

	printf("toggle % 3d million times ...\n", ToggleValue/1000000);

	gettimeofday(&t1, NULL);
	for (int loop=1; loop<ToggleValue; loop++)
    {
        gpio_set_drive(line_num, DRIVE_LOW);
        gpio_set_drive(line_num, DRIVE_HIGH);
	}
	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec)/1000000.0;
	fTimePerOperation = elapsedTime*1000000.0/ToggleValue;
	fFreq = ToggleValue/elapsedTime/1000000.0;
	printf("  % 9d toggle took %.3f s, Time per toggle %.3f us, Freq %.3f MHz \n",
	  ToggleValue, elapsedTime, fTimePerOperation, fFreq);

    gpio_set_drive(line_num, DRIVE_LOW);
    gpio_set_fsel(line_num, GPIO_FSEL_INPUT);

	return EXIT_SUCCESS;
}
