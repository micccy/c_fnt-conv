#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/func.h"

void errex(char*);

FILE *incpi=NULL;
long int insize;

uint8_t *buffer=NULL;
uint16_t cpind;
struct cpiffh_def *fihead;
struct cpifih_def *header;
struct cpiceh_def *curcp;

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
	fihead=(struct cpiffh_def*)buffer;																		//Set pointer to font file header
	header=(struct cpifih_def*)(buffer+fihead->fihoff);														//Set pointer to font info header
	curcp=&(header->fcp);																					//Set current codepage pointer to first
	cpind=0;																								//Set current codepage index to 0
	while(curcp){
		printf("%04lX, %s, \'",(uint8_t*)curcp-buffer,(curcp->device==1)?" screen":"printer");
		for(uint8_t i=0;i<8;i++) putchar((char)curcp->devname[i]);
		printf("\', CP%u\n",curcp->cpid);
		if(curcp->next&&(curcp->next!=~0)&&(++cpind<header->cpnum)) 
			curcp=(struct cpiceh_def*)(buffer+curcp->next);													//Set pointer to next codepage if it exists
		else curcp=NULL;
	};
	return 0;
};

void errex(char *s){
	if(buffer) free(buffer);
	if(incpi) fclose(incpi);
	printf("%s\n", s);
//Add help information
	exit(1);
};
