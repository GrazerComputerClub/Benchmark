// SPI/GPIO write speed benchmark, example for vector gaming via MCP4922 ADC
// Video: https://www.youtube.com/watch?v=hxSL9cxeJXo
// by Martin Strohmayer
// Licence: CC BY 4.0
// Info: It's not needed to connect hardware - it's a write benchmark
// Compile: gcc spi_bench_dac-vectorgaming.c -o spi_bench_dac-vectorgaming -Wall -lwiringPi -lm

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define WORD unsigned short
const int GPIO25 = 25;


void SPIWrite(int fd, unsigned char Channel, int Gain, unsigned short wValue) {
	unsigned char SPIData[2];

	if (wValue>0x0FFF) {
		wValue=0x0FFF;
	}
	SPIData[1] = wValue & 0xFF;
	SPIData[0] = ((wValue >> 8) & 0x0F) | 0x10;
	if (1==Channel) {
		SPIData[0] |= 0x80;
	}
	if(Gain <= 1 ){ // max Vdd
		SPIData[0] |= 0x20;
	}
	if (write(fd, &SPIData, sizeof(SPIData)) != sizeof(SPIData)) {
		fprintf(stderr, "Failed to write to spi bus (%s)\n", strerror(errno));
		return;
	}
	//wiringPiSPIDataRW (0, SPIData, 2) ; //10% slower than write on RaspPi!
}


void DualDACWrite(int fd, WORD wValue0, WORD wValue1) {
	digitalWrite(GPIO25, HIGH);
	SPIWrite(fd, 0, 1, wValue0);
	SPIWrite(fd, 1, 1, wValue1);
	digitalWrite(GPIO25, LOW);
}


unsigned short  DiscObjectX[1024], DiscObjectY[1024];
unsigned short  Paddle1ObjectX[1024], Paddle1ObjectY[1024];
unsigned short  Paddle1Object2X[1024], Paddle1Object2Y[1024];
int XDir = 1;
int YDir = -1;
unsigned short woffset = 4096/2;
int area;
int nDotPerObject = 50;
int XPos;
int YPos;


int PongSample(int fd) {
	int Pos;
	int loopDot;
	int nDots = 0;
	int Poti1 = 0; //poti (ADC) removed 
	int Poti2 = 0; //poti (ADC) removed
	int step = 20;

	//Disc
	for (loopDot = 0; loopDot < nDotPerObject; loopDot++) {
		DualDACWrite(fd, XPos+DiscObjectX[loopDot], YPos+DiscObjectY[loopDot]);
	}
	nDots += nDotPerObject;

	//paddle right
	for (loopDot = 0; loopDot < nDotPerObject; loopDot++) {
		DualDACWrite(fd, 2000+Paddle1ObjectX[loopDot], Poti2*-2+Paddle1ObjectY[loopDot]);
	}
	nDots += nDotPerObject;

	//paddle left
	for (loopDot = 0; loopDot < nDotPerObject; loopDot++) {
		DualDACWrite(fd, Paddle1ObjectX[loopDot]-2000, Poti1*-2+Paddle1ObjectY[loopDot]);
	}
	nDots += nDotPerObject;

	Pos = XPos+(XDir*step);
	if (Pos < (area) && Pos>(area*-1)) {
		XPos = Pos;
	} else {
		XDir = XDir*-1;
	}
	Pos = YPos+(YDir*step);
	if(Pos < area && Pos>(area*-1)) {
		YPos=Pos;
	} else {
		YDir = YDir*-1;
	}

	return(nDots);
}


int SpaceInv(int fd){
	int Pos;
	int loopDot, loopObject, loopLine;
	int area2 = area/2;
	int nDots = 0;
	int Poti = 0; //poti (ADC) removed
	int step = 8;

	//Disc
	for (loopLine = 0; loopLine < 2; loopLine++) {
		for (loopObject = 0; loopObject < 6; loopObject++) {
			for (loopDot = 0; loopDot < nDotPerObject; loopDot++) {
				DualDACWrite(fd,
				 (loopLine%2*350)+-1800+loopObject*550+DiscObjectX[loopDot],
				 loopLine*350+YPos+DiscObjectY[loopDot]);
			}
		}
	}
	nDots += (nDotPerObject*2*6);

	//paddle bottom
	for (loopDot = 0; loopDot < nDotPerObject; loopDot++) {
		DualDACWrite(fd, -2096+4*Poti + Paddle1Object2X[loopDot],
		 -2000+Paddle1Object2Y[loopDot]);
	}
	nDots += nDotPerObject;


	Pos = XPos+(XDir*step/10);
	if (Pos < (area) && Pos>(area*-1)) {
		XPos=Pos;
	} else {
		XDir = XDir*-1;
	}
	Pos = YPos+(YDir*step );
	if(Pos < area2 && Pos>(area2*-1)) {
		YPos=Pos;
	} else {
		YDir = YDir*-1;
	}

	return(nDots);
}


int main (void) {
	int fd;
	int loopDot, loopFrame;
	struct timeval t1, t2;
	double elapsedTime, fTimePerOperation, fFPS;
	const int SPIFreqMHz = 6;
	const int SPIDeviceNo = 0;
	const char SPIDevice[] = "/dev/spidev0.0";

	printf("WiringPi SPI speed (vector gaming) test program\n");
	
	if (wiringPiSPISetup(SPIDeviceNo, SPIFreqMHz * 1000000) == -1) {
		printf("wiringPiSPISetup failed\n\n");
		exit(EXIT_FAILURE);
	}
	if ( wiringPiSetupGpio () == -1) {
		printf("wiringPiSetup failed \n\n");
		exit(EXIT_FAILURE) ;
	}
	pinMode(GPIO25, OUTPUT);
	digitalWrite(GPIO25, LOW);

	printf("open device '%s' (%d MHz) ...\n", SPIDevice, SPIFreqMHz);
	if ((fd = open(SPIDevice, O_RDWR)) < 0) {
		fprintf(stderr,"Failed to open spi device '%s'\n", SPIDevice);
		exit(EXIT_FAILURE);
    }

	const double pi = 3.1415926535897932384626433832795;
	double sinus;
	double cosinus;

	//Create Opjects
	int nSize = 150;
	for (loopDot = 0; loopDot < nDotPerObject; loopDot++) {
		sinus = sin(2*pi*loopDot/nDotPerObject);
		DiscObjectX[loopDot] = (unsigned short)woffset+sinus*nSize;
		cosinus = cos(2*pi*loopDot/nDotPerObject);
		DiscObjectY[loopDot] = (unsigned short)woffset+cosinus*nSize;

		Paddle1ObjectX[loopDot] = woffset+nDotPerObject%10;
		Paddle1ObjectY[loopDot] = (unsigned short)woffset+nSize*2*loopDot/nDotPerObject;

		Paddle1Object2X[loopDot] = (unsigned short)woffset+nSize*2*loopDot/nDotPerObject;
		Paddle1Object2Y[loopDot] =  woffset+nDotPerObject%10;
	}

	area = woffset-300;
	XPos = area/2;
	YPos = area/2;
	int nDots = 0;
	int nDotsPerFrame = 0;
	int Frames = 1000;

	printf("Pong test:\n");
	gettimeofday(&t1, NULL);
	for (loopFrame = 1; loopFrame < Frames; loopFrame++) {
		nDotsPerFrame = PongSample(fd);
		nDots += nDotsPerFrame;
	}
	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec)/1000000.0;
	fTimePerOperation = elapsedTime*1000*1000/nDots;
	fFPS = Frames / elapsedTime ;
	printf("  %d frames took %.3f s, dots per frame %d, Time per dot %.0f us, fps %.0f \n",
	 Frames, elapsedTime, nDotsPerFrame, fTimePerOperation, fFPS);

	area = woffset-300;
	XPos = area/2;
	YPos = area/2;
	nDots = 0;
	Frames = 300;

	printf("Space Invaders test:\n");
	gettimeofday(&t1, NULL);
	for (loopFrame = 1; loopFrame < Frames; loopFrame++) {
		nDotsPerFrame = SpaceInv(fd);
		nDots += nDotsPerFrame;
	}
	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec)/1000000.0;
	fTimePerOperation = elapsedTime*1000*1000 / nDots;
	fFPS = Frames / elapsedTime ;
	printf("  %d frames took %.3f s, dots per frame %d, Time per dot %.0f us, fps %.0f \n",
	 Frames, elapsedTime, nDotsPerFrame, fTimePerOperation, fFPS);

	close(fd);
	exit(EXIT_SUCCESS);
}
