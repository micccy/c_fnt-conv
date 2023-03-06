#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/func.h"

void errex(char*);

FILE *incpi=NULL;
long int insize;

uint8_t *buffer=NULL;

int main(int argc,char *argv[]){
	if(argc!=2) errex("Invalid number of arguments");

	if(!(incpi=fopen(argv[1],"rb"))) errex("Can\'t open file");												//Open input file
	if(fseek(incpi,0,SEEK_END)) errex("Can\'t get file\'s end");											//Seek to end
	if((insize=ftell(incpi))<=0) errex("Can\'t get file size");												//Get current position
	if(fseek(incpi,0,SEEK_SET)) errex("Can\'t rewind file");												//Rewind
	if(!(buffer=malloc(insize))) errex("Can\'t reserve enough memory");										//Allocate buffer to read file
	if(insize!=fread(buffer,1,insize,incpi)) errex("Can\'t read file");										//Read whole file in buffer
	fclose(incpi); incpi=NULL;																				//Close file

	if((*((uint32_t*)buffer)!=0x4E4F46FF)||(*((uint32_t*)buffer+1)!=0x20202054)) errex("Not a cpi file");	//Check ID

	return 0;
};

void errex(char *s){
	if(buffer) free(buffer);
	if(incpi) fclose(incpi);
	printf("%s\n", s);
//Add help information
	exit(1);
};
