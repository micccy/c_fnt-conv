#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/func.h"

void errex(char*);

FILE *incpi=NULL;
long int insize;

uint8_t *buffer=NULL;
uint16_t cpind,find;
struct cpiffh_def *fihead;
struct cpifih_def *header;
struct cpiceh_def *curcp;
struct cpicih_def *curci;
struct cpisfh_def *cursf;
struct cpipfh_def *curpf;

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
	while(curcp){																							//While the pointed codepage exists
		printf("%s, \'",(curcp->device==1)?" screen":"printer");
		for(uint8_t i=0;i<8;i++) putchar((char)curcp->devname[i]);
		printf("\', CP%u, ",curcp->cpid);																	//Print information about codepage

		curci=(struct cpicih_def*)(buffer+(curcp->size<28?curcp->cih&0xFFFF:curcp->cih));					//Set pointer to current codepage information
		find=0;																								//Set font index to 0;
		if(curcp->device==1) cursf=(struct cpisfh_def*)((uint8_t*)curci+sizeof(struct cpicih_def));			//Set pointer to current screen font data header
		else curpf=(struct cpipfh_def*)((uint8_t*)curci+sizeof(struct cpicih_def));							//Set pointer to current printer data header
		while(find<curci->fnum){																			//While the current font exists
			if(curcp->device==1){																			//If it's a screen font
				printf("%04lX: %3u-%ux%2u",(uint8_t*)cursf-buffer,cursf->charnum,cursf->width,cursf->height);	//Dump data about it
				cursf=(struct cpisfh_def*)((uint8_t*)cursf+sizeof(struct cpisfh_def)+
				(cursf->height*cursf->charnum));															//Point to next font
			}else{																							//If it's a printer font
				printf("%04lX: %4u-%s, %u",(uint8_t*)curpf-buffer,curci->fsize,
				((curpf->printype==1)?"4201/1050/EPS":"5202/4208/PPDS"),curpf->slength);					//Dump data about it
				curci->fnum=1;																				//Only one printer font can exist but sometimes is reported as two
			};
			if(++find<curci->fnum) printf("\n                            ");
		};
		printf("\n");

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
