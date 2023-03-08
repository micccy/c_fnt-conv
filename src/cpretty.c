#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/func.h"

void errex(char*);
uint16_t printline(FILE*);

FILE *cptxtin;
uint8_t c;
uint16_t line,inlp;
uint32_t val;

int main(int argc,char *argv[]){
	if(argc!=2) errex("Invalid number of arguments");
	if(!(cptxtin=fopen(argv[1],"rb"))) errex("Can't open input file");
	line=0;
	inlp=0;
	val=0;
	inlp=printline(cptxtin);
	while(inlp<49){printf(" ");inlp++;};
	printf(" 0x%02X (%3u) : ",line,line);
	while((c<'0'||c>'9')&&(c<'A'||c>'F')&&(c<'a'||c>'f')&&(!feof(cptxtin))) c=(uint8_t)getc(cptxtin);
	while(!feof(cptxtin)){
		if((c>='0')&&(c<='9')){val<<=4;val+=(c-'0');c=(uint8_t)getc(cptxtin);
		}else if((c>='A')&&(c<='F')){val<<=4;val+=(c-'A'+0x0A);c=(uint8_t)getc(cptxtin);
		}else if((c>='a')&&(c<='f')){val<<=4;val+=(c-'a'+0x0A);c=(uint8_t)getc(cptxtin);
		}else{
			printf("\'%s\'",utf8(val));
			if(c==','||c=='+') inlp+=printf("%c",c);
			else{
				while(c!='\n'&&(!feof(cptxtin))) c=(uint8_t)getc(cptxtin);
				printf("\n");
				inlp=printline(cptxtin);
				if(inlp){
					while(inlp<49){printf(" ");inlp++;};
					line++;
					printf(" 0x%02X (%3u) : ",line,line);
				};
				while((c<'0'||c>'9')&&(c<'A'||c>'F')&&(c<'a'||c>'f')&&(!feof(cptxtin))) c=(uint8_t)getc(cptxtin);
			};
			val=0;
			while((c<'0'||c>'9')&&(c<'A'||c>'F')&&(c<'a'||c>'f')&&(!feof(cptxtin))) c=(uint8_t)getc(cptxtin);
		};
	}
	fclose(cptxtin);
	return 0;
};

uint16_t printline(FILE *f){
	long int cposition=ftell(f);
	uint16_t rdc=0;
	char c=getc(f);
	while((!((c<'0'||c>'9')&&(c<'A'||c>'F')&&(c<'a'||c>'f')&&(c!=',')&&(c!='+')))&&(!feof(f))){
		printf("%c",(c<'a'||c>'f')?c:c-('a'-'A'));
		rdc++;
		c=(uint8_t)getc(cptxtin);
	};
	fseek(f,cposition,SEEK_SET);
	return rdc;
}

void errex(char *s){
	printf("%s\n", s);
	//Print help
	exit(1);
};
