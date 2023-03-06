#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/func.h"

void phelp(char*);

FILE *bsf;
long filesize;

uint8_t *b,*ct,*ut;
struct psf2h_def *h;

int main(int argc, char *argv[]){
	if(argc==2){
		if((bsf=fopen(argv[1],"rb"))){
			fseek(bsf,0,SEEK_END);																			//Get filesize
			filesize=ftell(bsf);
			fseek(bsf,0,SEEK_SET);
			if((b=malloc(filesize))){
				if(fread(b,1,filesize,bsf)==filesize){
					fclose(bsf);																			//All the file is in b[]
					if((*((uint16_t*)b)==0x0436)||(*((uint32_t*)b)==0x864AB572)){							//Check magic bytes
						if(*((uint16_t*)b)==0x0436){														//If this is a psf1 file build a fake psf2 header to hold info
							h=malloc(sizeof(struct psf2h_def));
							h->magicid=0x864AB572;															//Magic id
							h->version=~0;																	//Correct would be 0, do this to report psf1
							h->hsize=sizeof(struct psf2h_def);												//Header size
							h->flags=(((struct psf1h_def *)b)->flags&0x06)?1:0;								//Set unicode table flag
							h->length=(((struct psf1h_def *)b)->flags&0x01)?512:256;						//Set size
							h->csize=((struct psf1h_def *)b)->size;											//Set character size
							h->height=h->csize;																//Height equals size since
							h->width=8;																		//psf1 characters are always 8 bits wide
							ct=b+sizeof(struct psf1h_def);													//Set pointer to character font table
						}else{
							h=(struct psf2h_def*)b;															//If this is a psf2 file point to header
							ct=b+h->hsize;																	//Set pointer to character font table
						};
						ut=ct+(h->csize*h->length);															//Set pointer to unicode conversion table
						printf("\'%s\', psf%c, %u glyphs, %ux%u, %s mapping\n",argv[1],(h->version==~0)?'1':
						'2',h->length,h->width,h->height,h->flags?"unicode":"direct");						//Dump info

						uint16_t *table=(uint16_t *)ut;														//Dunmp psf1 table
						for(uint32_t i=0;i<h->length;i++){
							printf("%3u (0x%03X) = U+%04X ('%s')\n",i,i,*table,utf8(*table));
							table++;
							while(*table<0xfffe){
								printf("              U+%04X ('%s')\n",*table,utf8(*table));
								table++;
							};
							while(*table==0xfffe){
								table++;
								printf("             U+%04X ('%s')",*table,utf8(*table));
								table++;
								while(*table<0xfffe){
									printf(",U+%04X ('%s')",*table,utf8(*table));
									table++;
								};
								printf("\n");
							};
							table++;
						};
						printf("\n");

						free(b);
						return 0;
					}else printf("Not a psf file!\n");
				}else printf("Could not read \'%s\'\n", argv[1]);
				free(b);
			}else printf("Could not allocate memory\n");
			fclose(bsf);
		}else printf("Could not open \'%s\'\n", argv[1]);
	} else printf("Invalid number of arguments\n");
	phelp(argv[0]);
	return 1;
};

void phelp(char* n){
	printf("\n\
This utility reads a PC Screen Font format file\'s unicode table\n\
and displays for each glyph the corresponding character(s)\n\n\
\tusage: %s input_file\n\n\
The input file must be in psf format, if no unicode table is present\n\
each glyph\'s number is also its corresponding unicode character\n\
\n", n);
};
