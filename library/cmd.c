#include "./func.h"
#include "./graph.h"
#include "./int.h"
#include "./segment.h"
#include <string.h>
struct ComdInfo{
  short cnt;
  char* prts[256];
};
static void getStr(char*str, ScrnInfo *scnini, bool out, u16 nowx, u16 nowy);
u16 run (ScrnInfo *scnini, char*commandline,u16 nowx,u16 nowy);

void CMD_main (){
  static char sym[512]=">",buf[512]="Welcome to Symbol OS!";
  u16 nowx,nowy,ret;
  ScrnInfo *scnini = (ScrnInfo *)0x0ff0;

  nowx=16,nowy=scnini->scrny-32;
  box_fill(scnini,RGB_000000,0,nowy,scnini->scrnx,scnini->scrny);
  put_fonts8_asc(scnini, 16, nowy, RGB_FFFFFF, buf);
  read_Floppy(0,img);
  swim_line(scnini);
  put_fonts8_asc(scnini, 16, nowy, RGB_FFFFFF, (char*)(img+54));
  swim_line(scnini);

  while(1){
    put_fonts8_asc(scnini, 0, nowy, RGB_00FF00, sym);
    buf[0]=0;
    getStr(buf,scnini,1,nowx,nowy);
    swim_line(scnini);
    ret=run(scnini,buf,nowx,nowy);
    sprintf(buf,"RET: %X\n\r",ret);
    putstr(scnini,0,nowy,RGB_00FF00,buf);
    swim_line(scnini);
  }
  return;
}

static void getStr(char*str, ScrnInfo *scnini, bool out, u16 nowx, u16 nowy){
  short prt=0;char tmp;

  while(1){
    asm_hlt();
    if(hasC()){
      tmp=getC();
      if(tmp=='\n'||tmp=='\r')
        break;
      else if(tmp=='\b')
        str[prt=(prt>0?prt-1:0)]=0,box_fill(scnini,RGB_000000,nowx+prt*8,nowy,nowx+prt*8+7,nowy+15);
      else
        str[prt]=tmp,str[prt+1]=0,put_font8(scnini,nowx+prt*8,nowy,RGB_FFFFFF,font+tmp*16),prt++;
      prt=(prt<0?0:prt),prt=(prt<120?prt:119);
    }
  }
  return;
}

u16 run (ScrnInfo *scnini, char*commandline,u16 nowx,u16 nowy){
  u16 ret=0;
  struct ComdInfo cmd;
  int prt=0;

  cmd.cnt=0;

  cmd.prts[cmd.cnt++]=commandline;
  while(!isspace(commandline[prt])&&commandline[prt])prt++;
  while(commandline[prt]){
    commandline[prt++]=0;
    while(isspace(commandline[prt])&&commandline[prt])prt++;
    cmd.prts[cmd.cnt++]=commandline+prt;
    while(!isspace(commandline[prt])&&commandline[prt])prt++;
  }
  strlwr(cmd.prts[0]);
  if(strcmp(cmd.prts[0],"cls")){
    if(strcmp(cmd.prts[1],"-s"))
      init_screen(scnini),box_fill(scnini,RGB_000000,0,scnini->scrny-16,scnini->scrnx,scnini->scrny);
    else
      cls(scnini);
  }else if(strcmp(cmd.prts[0],"echo")){
    for(prt=1;prt<cmd.cnt;prt++)
      nowx=putstr(scnini,nowx,nowy,RGB_FFFFFF,cmd.prts[prt]),nowx+=8;
    swim_line(scnini);
  }else if(strcmp(cmd.prts[0],"dir")){
    sprintf(cmd.prts[0],"%d-%d-%d\n\r\n\r",mousebuf.x,mousebuf.y,mousebuf.flag);
    putstr(scnini,nowx,nowy,RGB_FFFFFF,cmd.prts[0]);
  }else if(strcmp(cmd.prts[0],"time")){
    get_time(&TIME),time_forms(&TIME,cmd.prts[0]);
    putstr(scnini,nowx,nowy,RGB_FFFFFF,cmd.prts[0]);
    putstr(scnini,nowx,nowy,RGB_FFFFFF,"\n\r\n\r");
  }
  else if(cmd.prts[0][0]==0)return 0;
  else{
    nowx=putstr(scnini,0,nowy,RGB_FF0000,cmd.prts[0]);
    putstr(scnini,nowx,nowy,RGB_FFFF00," < Warning Command! Command \"help\" to get helps.\n\r");
  }
  return ret;
}