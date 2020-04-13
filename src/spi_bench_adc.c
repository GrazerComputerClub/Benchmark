// SPI read speed benchmark via reading voltages from MCP3202 ADC
// by GC2
// Licence: MIT
// Info: IC MCP3202 needs to be connected to SPI0-CS1
// Compile: gcc spi_bench_adc.c -o spi_bench_adc -Wall -lwiringPi

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define TRUE                (1==1)
#define FALSE               (!TRUE)
#define CHAN_CONFIG_SINGLE  8
#define CHAN_CONFIG_DIFF    0

const int  MaxLoops = 500000;
const int  SPIFrequency = 1000000*4;
const float fRefVoltage = 3.3f;
const float fResolution = 4096; //12-Bit
static sig_atomic_t end = 0;

static void sighandler(int signo) {
  end = 1;
}


void spiSetup(int spiChannel, int* hSPI) {
    if ((*hSPI = wiringPiSPISetup (spiChannel, SPIFrequency)) < 0) { //~1 MHz SPI clock max.
        fprintf(stderr, "Can't open the SPI bus: %s\n", strerror(errno));
        exit (EXIT_FAILURE);
    }
}


static int AnalogRead(int spiChannel, int analogChannel) {
    if (analogChannel<0 || analogChannel>1) {
        return -1;
    }

    unsigned char spiData[3];
    unsigned char chanBits;

    if (analogChannel == 0) {
        chanBits = 0b11010000;
    } else {
        chanBits = 0b11110000;
    }
    spiData[0] = chanBits;
    spiData[1] = 0;
    spiData[2] = 0;
    wiringPiSPIDataRW(spiChannel, spiData, 3);
    return ((spiData [0] << 9) | (spiData [1] << 1) | (spiData[2] >> 7)) & 0xFFF;
}


int main(int argc, char *argv []){
    struct sigaction sa;
    struct timeval t1, t2;
    double elapsedTime, fTimePerOperation;

    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sighandler;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    
    const int spiChannel = 1;
    int hSPI;
    int CH0, CH1;

    wiringPiSetup();
    spiSetup(spiChannel, &hSPI);
    CH0 = AnalogRead(spiChannel, 0);
    CH1 = AnalogRead(spiChannel, 1);
    if (CH0>0 || CH1>0) {
      float value0 = CH0 * fRefVoltage / fResolution;
      float value1 = CH1 * fRefVoltage / fResolution;
      printf("MCP3202(CE1): CH 0,1 = %04d, %04d -> %.3f, %.3f V\n",
	    CH0, CH1, value0, value1);
    } else {
      fprintf(stderr, "no answer\n");
      close(hSPI);
      return 1;
    }
    gettimeofday(&t1, NULL);
    for (int loop=0; loop<MaxLoops; loop++) {
        //CH0 = AnalogRead(spiChannel, 0);
        CH1 = AnalogRead(spiChannel, 1);
    }; 
    gettimeofday(&t2, NULL);
    close(hSPI);
    elapsedTime = (t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec)/1000000.0;
    fTimePerOperation = elapsedTime*1000000.0/MaxLoops;
    printf("  %d iterations took %.3f s, Time per operation %.3f us\n",
	  MaxLoops, elapsedTime, fTimePerOperation);

    return 0;
}

