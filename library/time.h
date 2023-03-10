#ifndef _SYM_OS_TIME_H_
#define _SYM_OS_TIME_H_

#define PORT_CMOS_CMD 0x70
#define PORT_CMOS_DAT 0x71
#define PORT_CMOS_CMD_ 0x72
#define PORT_CMOS_DAT_ 0x73

struct _timerBuf_{
	unsigned long cnt;
};
struct _time_now_{
	u8 year_,year;
	u8 mnth,days,hour,mint,secd,week;
};
extern struct _timerBuf_ timer;
extern struct _time_now_ TIME;
extern char week_strs_s[07][5];
extern char mnth_strs_s[13][5];

unsigned long getMs(void){
  return timer.cnt;
}
void get_time (struct _time_now_ *TIME){
	out8(PORT_CMOS_CMD,0x0);
	TIME->secd=in8(PORT_CMOS_DAT);
	out8(PORT_CMOS_CMD,0x2);
	TIME->mint=in8(PORT_CMOS_DAT);
	out8(PORT_CMOS_CMD,0x4);
	TIME->hour=in8(PORT_CMOS_DAT);
	out8(PORT_CMOS_CMD,0x6);
	TIME->week=in8(PORT_CMOS_DAT);
	out8(PORT_CMOS_CMD,0x7);
	TIME->days=in8(PORT_CMOS_DAT);
	out8(PORT_CMOS_CMD,0x8);
	TIME->mnth=in8(PORT_CMOS_DAT);
	out8(PORT_CMOS_CMD,0x9);
	TIME->year=in8(PORT_CMOS_DAT);
	out8(PORT_CMOS_CMD,0x32);
	TIME->year_=in8(PORT_CMOS_DAT);
	return;
}
void time_forms (struct _time_now_ *TIME,char*str){
	sprintf(str,"%X%X/%X/%X %X:%X:%X %s",TIME->year_,
		TIME->year,TIME->mnth,TIME->days,TIME->hour,
		TIME->mint,TIME->secd,week_strs_s[TIME->week]);
	return;
}
void time_form (char*form,struct _time_now_ *TIME,char*str){
	while(*form){
		if(*form=='%'){
			char ch=*(++form);
			if(ch=='y'){
				*str=(TIME->year>>4)+'0',str++;
				*str=(TIME->year>>0)+'0',str++;
			}else if(ch=='Y'){
				*str=(TIME->year_>>4)+'0',str++;
				*str=(TIME->year_>>0)+'0',str++;
				*str=(TIME->year>>4)+'0',str++;
				*str=(TIME->year>>0)+'0',str++;
			}else if(ch=='M'){
				*str=(TIME->mnth>>4)+'0',str++;
				*str=(TIME->mnth>>0)+'0',str++;
			}else if(ch=='n'){
				*str=mnth_strs_s[TIME->mnth][0],str++;
				*str=mnth_strs_s[TIME->mnth][1],str++;
				*str=mnth_strs_s[TIME->mnth][2],str++;
				*str=mnth_strs_s[TIME->mnth][3],str++;
			}else if(ch=='d'){
				*str=(TIME->days>>4)+'0',str++;
				*str=(TIME->days>>0)+'0',str++;
			}else if(ch=='h'){
				*str=(TIME->hour>>4)+'0',str++;
				*str=(TIME->hour>>0)+'0',str++;
			}else if(ch=='m'){
				*str=(TIME->mint>>4)+'0',str++;
				*str=(TIME->mint>>0)+'0',str++;
			}else if(ch=='s'){
				*str=(TIME->secd>>4)+'0',str++;
				*str=(TIME->secd>>0)+'0',str++;
			}else if(ch=='w'){
				*str='w',str++;
				*str=(TIME->week>>0)+'0',str++;
			}else if(ch=='W'){
				*str=week_strs_s[TIME->week][0],str++;
				*str=week_strs_s[TIME->week][1],str++;
				*str=week_strs_s[TIME->week][2],str++;
				*str=week_strs_s[TIME->week][3],str++;
			}else
				*str=*form,str++;
		}else
			*str=*form,str++;
		form++,*str=0;
	}
	return;
}
#endif
