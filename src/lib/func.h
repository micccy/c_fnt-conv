#include "def.h"

char ustr[5]={'\0','\0','\0','\0','\0'};

char *utf8(uint32_t u){
	uint32_t val=0,*pustr=(uint32_t*)ustr;
	uint8_t f=0x80;
	if(u<0x80) val=u;
	else while(u>>6){
		val<<=8;
		val|=0x80|(u&0x3F);
		u>>=6;
		f>>=1;
		f|=0x80;
	};
	if(f!=0x80){
		val<<=8;
		val|=(f|u);
	}
	*pustr=val;
	return ustr;
};
