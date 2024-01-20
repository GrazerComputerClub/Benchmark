// GPIO write speed benchmark
//
// by GC2 - mstroh
// Licence: CC BY 4.0
// Install: sudo apt install libgpiod2 libgpiod-dev 
// Compile: gcc gpio_libgpiod.c -o gpio_gpiod -Wall -lgpiod

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <byteswap.h>


const int Pi5_ModelNo = 23;
const char* revfile = "/proc/device-tree/system/linux,revision";

int detect_pi_modelno() {
	unsigned char revision[4]={0,0,0,0};

	FILE* fp = fopen(revfile,"rb");
	if (!fp) {
		perror(revfile);
		return -1; // revision file not found or no access
	}
	int result = fread(revision,4,1,fp);
	fclose(fp);
	if (result<1) {
		perror(revfile);
		return -2; // read error
	}
	//printf("revision: %02x %02x %02x %02x\n", revision[0], revision[1], revision[2], revision[3]);
	unsigned char byte0 = revision[3] >> 4;
	unsigned char byte1 = revision[2] & 0x0F;
	unsigned char nflag = revision[1] & 0x80;
	if (nflag>0) {
		return byte1*16+byte0; // model found
	}
	return 0; // unknown model
}


uint32_t GetPiRevision() {
	uint32_t revision;
	_Static_assert(sizeof(revision)==4, "should be unsigend integer with 4 byte size");

	FILE* fp = fopen(revfile,"rb");
	if (!fp) {
		perror(revfile);
		return 0; // revision file not found or no access
	}
	int result = fread(&revision,sizeof(revision),1,fp);
	fclose(fp);
	if (result<1) {
		perror(revfile);
		return 0; // read error
	}
	revision = bswap_32(revision);
	//printf("revision: %x\n", revision); 
	return revision;
}


int main(int argc, char **argv) {
	char *chipname    = "gpiochip0";
	char *chipnamePi5 = "gpiochip4";
	unsigned int line_num = 24;	// GPIO Pin #24
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int ret;
	const int  ToggleValue = 5000000;
	struct timeval t1, t2;
	double elapsedTime, fTimePerOperation, fFreq;

	int model = detect_pi_modelno();
	if (model==Pi5_ModelNo) { //Raspberry Pi 5
		printf("found model: %d, Raspberry Pi 5\n", model);
	} else if(model>0) {
		printf("found model: %d\n", model);
	} else if(model==0) {
		printf("unknown model (old style revision code %x)\n", GetPiRevision());
	} else {
		printf("could not detect revision, looks like this is not a Raspberry Pi system\n");
		return EXIT_FAILURE;
	}

	printf("gpiod GPIO speed test program (using line %d)\n", line_num);

	if (model>=Pi5_ModelNo) { //Raspberry Pi 5
		chip = gpiod_chip_open_by_name(chipnamePi5);
	} else {
		chip = gpiod_chip_open_by_name(chipname);
	}
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

	line = gpiod_chip_get_line(chip, line_num);
	if (!line) {
		perror("Get line failed\n");
		goto close_chip;
	}

	ret = gpiod_line_request_output(line, "gpio_bench", 0);
	if (ret < 0) {
		perror("Request line as output failed\n");
		goto release_line;
	}

	printf("toggle % 3d million times ...\n", ToggleValue/1000000);

	gettimeofday(&t1, NULL);
	for (int loop=1; loop<ToggleValue; loop++) {
		gpiod_line_set_value(line, 0);
		gpiod_line_set_value(line, 1);
	}
	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec)/1000000.0;
	fTimePerOperation = elapsedTime*1000000.0/ToggleValue;
	fFreq = ToggleValue/elapsedTime/1000000.0;
	printf("  % 9d toggle took %.3f s, Time per toggle %.3f us, Freq %.3f MHz \n",
	  ToggleValue, elapsedTime, fTimePerOperation, fFreq);

release_line:
	gpiod_line_release(line);
close_chip:
	gpiod_chip_close(chip);
end:
	return EXIT_SUCCESS;
}
