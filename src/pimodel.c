#include "pimodel.h"
#include <stdio.h>
#include <stdint.h>
#include <byteswap.h>
#include <stdio.h>


const char* revfile = "/proc/device-tree/system/linux,revision";

int GetRPiModel() {
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


uint32_t GetRPiRevision() {
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

