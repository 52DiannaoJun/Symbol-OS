#ifndef _SYM_OS_FS_H_
#define _SYM_OS_FS_H_

#include "./func.h"
typedef struct{
	code	JMPcode[3];		//JMP start NOP
	u16		BytePerSec;		//每个扇区 (sector) 的字节数
	u8		SecPerClus;		//每个簇 (cluster) 的扇区数 
	u16		SecCNT_;		//磁盘的扇区总数,若为0则使用下一行记录
	u32		SecCNT;			//大容量扇区总数,若上一行记录的值为0则使用本行记录扇区数
	u8		RevSec;			//分区数量
	u8		DiskType;		//磁盘的种类
	u32		uid;			//磁盘序号
	char 	TypeFlag[8];	//系统引导扇区格式
} FS_SBR; 



#ifndef _FS_SFS_
	#define _FS_SFS_
	typedef struct{
		char	TypeFlag[8];	//文件系统类型 (8字节)
		u8		SecPerBlock;	//每个块 (block) 的扇区数
		u16		StartSec_;		//开始扇区,若为0则使用下一行记录
		u32		StartSec;		//开始扇区,若上一行记录的值为0则使用本行记录
		u16		SecCNT_;		//分区的扇区总数,若为0则使用下一行记录
		u32		SecCNT;			//大分区扇区总数,若上一行记录的值为0则使用本行记录扇区数
		u16		SecBeforeFAT;	//FAT前的预留扇区数 (包含boot扇区)
		u8		FAT_CNT;		//FAT表的数量
		u16		RootFileCNT;	//根目录文件的最大值
		u16		BR_Backup;		//备份引导扇区
		char	title[11]; 		//卷标 (11字节)
	} FS_SFS;
	typedef struct{
		char name[8],ext[3];	//名称
		code Date[5];			//时间日期
		u32 size,Sec;			//大小、起始扇区
		code info;				//属性
		u8 drvuid;				//设备编码
		u32 mov;				//偏移扇区
		code rev[2];			//保留
		//日期时间 所占位
		//	年=14 月=4 日=5 时=5 分=6 秒=6
		//属性
		//	目录	隐藏	系统	只读
		//	08	04	02	01
	} FS_SFS_F;
	extern FS_SFS_F* SFS_ROT;
  	extern FS_SFS_F* SFS_NOW;
  	static void SFS_list (ScrnInfo*scnini,FS_SFS_F*path){
  		static u8 buf[512],fat[512];
  		static FS_SFS_F*temp;
  		u32 i=path->Sec+path->mov,j=0,cnt=0;

  		while(cnt*32<path->size){
  			if(path->drvuid<2){
	  			read_Floppy(i,buf);
  			}else{
  				;
  			}
  			for(j=0;j<16;j++,cnt++){
  				temp=(FS_SFS_F*)(buf+j*32);
  				//if(temp->name[0]==0)
  				//	goto outside0;
  				sprintf((char*)fat,"%8s. %3s\n\r",temp->name,temp->ext);
  				putstr(scnini,0,scnini->scrny-16,RGB_FFFFFF,(char*)fat);
  			}
  			if(path->drvuid<2){
  				read_Floppy(i/128+1+path->mov,fat);
  			}else{
  				;
  			}
  			i=((u32*)fat)[i%128];
  			while(1){
  				asm_hlt();
  				if(hasKey())
  					break;
  			}
  		}
  		outside0:
  		return;
  	}
  	void SFS_find (char*fn,FS_SFS_F*path,FS_SFS_F*val){
;
  	}
#endif

#endif