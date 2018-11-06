/*************************************************************************
    > File Name: 2.c
    > Author: Yunruowu
    > Mail: xuyunyang12345@163.com
    > Created Time: 2018年09月12日 星期三 15时05分00秒
 ************************************************************************/
#ifndef ISA
#define ISA 
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
/**********新修改的部分*************/
//cache的结构
#define DC_BLOCK_SIZE 20
#define DC_NUM_SETS 10
#define DC_SET_SIZE 20
#define DC_WR_BUFF_SIZE 10
#define DC_INVALID 2
struct cacheBlk{
    int tag;
    int status;
    int trdy;
}dCache[DC_NUM_SETS][DC_SET_SIZE];

//写缓存的结构
struct writeBuffer{
  int tag;
  int trdy;
}dcWrBuff[DC_WR_BUFF_SIZE];


int MemRdLatency = 5;
int slot;
int lurSlot,lurTime;
//求解标志和地址
int blkOffsetBits,IndexMask,tagMask,blk,Index,tag;
void setword(int addr){
  
    blkOffsetBits = log(DC_BLOCK_SIZE)/log(2);
    IndexMask = (unsigned)(DC_NUM_SETS-1);
    tagMask = ~IndexMask;

    blk = ((unsigned)addr)>>blkOffsetBits;
    Index = (int)(blk & IndexMask);
    tag = (int)(blk & tagMask);
    return ;
}
//判断是命中还是缺失
int HorM(int tag){
  for(int i=0;i<DC_SET_SIZE;i++){
      if((dCache[Index][i].tag == tag)&&(dCache[Index][i].status != DC_INVALID)){
        slot = i;
        return 1;
      }else{
        if(dCache[Index][i].trdy < lurTime){
          lurTime = dCache[Index][i].trdy;
          lurSlot = i;
        }
      }
  }
  return 0;
}

int wrBack(int tag,int time){
    for(int i=0; i<DC_WR_BUFF_SIZE;i++){
        if(dcWrBuff[i].tag == tag)
          return 0;
        else
          return 5;
    }
}
//访问数据cache
int accessDCache(int opcode, int addr, int time)
{
    setword(addr);
    int HoM = HorM(tag);
    printf("命中或缺失：%d   ",HoM);
    printf("Index：%d  tag: %d  status:%d\n",Index,tag,dCache[Index][slot].status);
    if(HoM==1)//hit
    {
        if(opcode==6)//read
        {
            dCache[Index][slot].trdy = time;
        }else{//write
            dCache[Index][slot].trdy = time;
            dCache[Index][slot].status = 2;
        }
        return 0;
    }else{//miss
        if(opcode==6)//read
        {
            int Ttrdy = MemRdLatency;
            if(dCache[Index][lurSlot].status==2){
                Ttrdy += wrBack(tag,time);
            }
            dCache[Index][lurSlot].tag = tag;
            dCache[Index][lurSlot].trdy = time + Ttrdy;
            dCache[Index][lurSlot].status = 1;
            return Ttrdy;
        }else{
            int Ttrdy = 0;
            if(dCache[Index][lurSlot].status==2){
                Ttrdy = wrBack(tag,time);
            }else{
                dCache[Index][lurSlot].status = 2;
            }
            Ttrdy += MemRdLatency;
            dCache[Index][lurSlot].tag = tag;
            dCache[Index][lurSlot].trdy = time + Ttrdy;
            return Ttrdy;
        }
    }

}

enum opcodes{
	//	R-Type
	//	OP_JR = 0,
		OP_SPECIAL = 0,
//I-Type
	//	I-Type ALU Insns
		OP_ADDI,OP_ADDUI,
	//	I-Type Branch and Jump Insns
		OP_BNEZ,OP_BRE,
	//	I-Type Load and Store Insns
		OP_STORED,OP_LOADI,OP_LOADIMM,OP_LOADD,
	//	J-Type Insns
		OP_NOP,OP_JR,
	//	MOV
		OP_MOVI,OP_MOVD,
  //  val 
    OP_VLOAD,OP_VSTORE,OP_VADD,
		NUM_OPCODES
};
enum functions{
	OP_ADD = 0,OP_ADDU,OP_ADDDOU,

	NUM_FUNCS
};







int time;
char* mem;
long mem_size = 0x080000;
int ireg_size = 0x1000;
int *r;
int dreg_size = 0x8;
double *rrd;
char* val;
int val_size = 128;



unsigned char read_mem_ubyte(long addr)
{
	return *((unsigned char *)(mem + addr));
}

void write_mem_ubyte(long addr, unsigned char val)
{
	*((unsigned char *)(mem + addr)) = val;
}


char read_mem_byte(long addr)
{
	return *((char*)(mem + addr));
}
void write_mem_byte(long addr, char val)
{
	*((char *)(mem + addr)) = val;
}



//unsigned short
unsigned short read_mem_uhalfword(long addr)
{
	return *((unsigned short*)(mem + addr));
}
void write_mem_uhalfword(long addr, unsigned short val)
{
	*((unsigned short*)(mem + addr)) = val;
}


//short
short read_mem_halfword(long addr)
{
	return *((short*)(mem + addr));
}
void write_mem_halfword(long addr, short val)
{
	*((short*)(mem + addr)) = val;
}

//unsigned int
unsigned int read_mem_uword(long addr)
{
	return *((unsigned int*)(mem + addr));
}
void write_mem_uword(long addr,unsigned int val)
{
	*((unsigned int*)(mem + addr)) = val;
}

//int
int read_mem_word(long addr)
{
	return *((int*)(mem + addr));
}


void write_mem_word(long addr, int val)
{
	*((int*)(mem + addr)) = val;
}



float read_mem_float(long addr)
{
	return *((float*)(mem + addr));
}
void write_mem_float(long addr, float val)
{
	*((float*)(mem + addr)) = val;
}



double read_mem_double(long addr)
{
	return *((double*)(mem + addr));
}
void write_mem_double(long addr, double val)
{
	*((double*)(mem + addr)) = val;
}


//functions for register file
int get_int(long addr)
{
	return *((int*)(r+addr));
}

void put_int(long addr,int val)
{
	*((int*)(r+addr))= val;
}

int get_uint(long addr)
{
	return *((unsigned int*)(r+addr));
}

void put_uint(long addr,int val)
{
	*((unsigned int *)(r+addr)) = val;
}

float get_flo(long addr)
{
	return *((float*)(r+addr));
}

void put_flo(long addr,float val)
{
	*((float *)(r+addr))=val;
}

short get_shor(long addr)
{
	return *((short*)(r+addr));
}

void put_shor(long addr, short val)
{
	*((short*)(r+addr))=val;
}

unsigned short get_ushor(long addr)
{
	return *((unsigned short*)(r+addr));
}

void put_ushor(long addr,unsigned short val)
{
	*((unsigned short*)(r+addr))=val;
}

char get_char(long addr){
	return *((char*)(r+addr));
}

void put_char(long addr,char val){
	*((char*)(r+addr)) = val;
}

//double register
double get_double(long addr){
	return *(double*)(rrd + addr);
}

void put_double(long addr,double val){
	*((double*)(rrd+addr)) = val;
}



float get_V_float(long addr){
  return *(float*)(val+addr);
}
void put_V_float(long addr,float v){
  *((float*)(val+addr)) = v;
}









long INSA_ADD(long pc){
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int RS1	= (insn>>21) & 0x1f;
	int RS2 = (insn>>16) & 0x1f;
	int RD = (insn>>11) & 0x1f;
	int *rd = r + RD;
	int rs1 = get_int(RS1);
	int rs2 = get_int(RS2);
	*rd = rs1 + rs2;
	return pc + 4;
}

long INSA_ADDU(long pc){
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int RS1	= (insn>>21) & 0x1f;
	int RS2 = (insn>>16) & 0x1f;
	int RD = (insn>>11) & 0x1f;
	unsigned int rs1 = get_uint(RS1);
	unsigned int rs2 = get_uint(RS2);
	return pc + 4;
}


long INSN_ADDI(long pc){
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int RS1	= (insn>>21) & 0x1f;
	int RS2 = (insn>>16) & 0x1f;
	int IMM = (int)(short)(insn & 0x1fffff);//符号扩展
	put_int(RS1,get_int(RS1)+IMM);
	return pc + 4;

}
/*long INSN_ADDUI(long pc){
	unsigned int *rd = r + RS2;
	unsigned int rs = get_int(RS1);
	unsigned int UIMM= (unsigned int)(insn & 0xffff);
	*rd = rd + UIMM;
	return pc + 4;
}*/

long INSA_ADDUI(long pc){
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int RS1	= (insn>>21) & 0x1f;
	int RS2 = (insn>>16) & 0x1f;
	int IMM = (int)(short)(insn & 0xffff);//符号扩展
	int *rd = r + RS2;
	int rs = get_int(RS1);
	*rd = rs + IMM;
	return pc + 4;
}

long INSN_ADDDOU(long pc){//double的寄存器加
	unsigned int insn = read_mem_uword(pc);//读第一条指令
		//指令译码
	int OPCODE = (insn>>26) & 0x3f;
	int RS1	= (insn>>21) & 0x1f;
	int RS2 = (insn>>16) & 0x1f;
	int RD = (insn>>11) & 0x1f;
	
	put_double(RD,get_double(RD) + read_mem_double(get_int(RS1)+get_int(RS2)));
	return pc + 4;
}

/*long INSN_ADDDOUI(pc){//double的立即数加（感觉没有用）
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int RS1	= (insn>>21) & 0x1f;
	int RS2 = (insn>>16) & 0x1f;
	double *rd = rrd + RS2;
	double rs = get_double(RS1);
	*rd = rs + IMM;
	return pc + 4;
}*/

long INSN_MOVI(long pc){
	unsigned int insn = read_mem_uword(pc);

	int RS1 = (insn>>21) & 0x1f;//寄存器号
	int RS2 = (insn>>16) & 0x1f;
	put_int(RS2,get_int(RS1));
	return pc + 4;
}

long INSN_MOVD(long pc){
	unsigned int insn = read_mem_uword(pc);
	int RS1 = (insn>>21) & 0x1f;//寄存器号
	int RS2 = (insn>>16) & 0x1f;
	put_double(RS2,get_double(RS1));
	return pc + 4;
}


long INSN_LOADI(long pc){
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int OPCODE = (insn>>26) & 0x3f;
  time += accessDCache(6,pc,time);
	int RS1 = (insn>>21) & 0x1f;//寄存器号
	int RS2 = (insn>>16) & 0x1f;
	int IMM = (int)(short)(insn & 0xffff);
	int val=read_mem_word ( (long) ( IMM + get_int ( RS1 ) ) );
	put_int(RS2,val);
	return pc+4;

}

long INSN_LOADIMM(long pc){
	unsigned int insn = read_mem_uword(pc);
	int OPCODE = (insn>>26) & 0x3f;
  time += accessDCache(6,pc,time);
	int RS1 = (insn>>21) & 0x1f;
	int IMM = (int)(short)(insn & 0xfffff);
	put_int(RS1,IMM);
	return pc+4;
}
long INSN_LOADD(long pc){
	unsigned int insn = read_mem_uword(pc);
	int OPCODE = (insn>>26) & 0x3f;
  time += accessDCache(6,pc,time);
	int RS1 = (insn>>21) & 0x1f;
	int RS2 = (insn>>16) & 0x1f;
	int IMM = (int)(short)(insn & 0xffff);
	put_double(RS2,read_mem_double((long)(IMM + get_int(RS1))));
	return pc + 4;
}

long INSN_STORED(long pc){
	unsigned int insn = read_mem_uword(pc);
	int RS1 = (insn>>21) & 0x1f;
	int RS2 = (insn>>16) & 0x1f;
	int IMM = (int)(short)(insn & 0xffff);
	double rt = get_int(RS2);
	double rs = get_double(RS1);
	write_mem_double(rt+IMM,rs);
	return pc + 4;

}


long INSN_BRE(long pc){//分支指令
	unsigned int insn = read_mem_uword(pc);
	int RS1 = (insn>>21) & 0x1f;//存放i
	int RS2 = (insn>>16) & 0x1f;//存放N
	int IMM = (int)(short)(insn & 0xffff);
	int rt = get_int(RS2);
	int rs = get_int(RS1)/8;
 	if(rs < rt){
		return pc - 8;
	}
	else{
		return pc + 4;
	}


}

long INSN_NOP(long pc){
	return pc + 4;
}

long INSN_JR(long pc)
{
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int RS1	= (insn>>21) & 0x1f;
	return 0;
}

//向量指令
long INSN_VLOAD(long pc){
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int RS1	= (insn>>21) & 0x1f;
	int RD = (insn>>16) & 0x1f;
  int type = (insn>>13) & 0x7;//type of data 
  int num = (insn) & 0xf;//num of data
  int addr = get_int(RS1);
  int addrv = get_int (RD);
  for(int i = 0;i <num;i++){
      float a = read_mem_float(addr+i*num);
      put_V_float(RD*16+i*num,a);
  }
  
  return pc+4;
}

long INSN_VSTORE(long pc){
	unsigned int insn = read_mem_uword(pc);//读第一条指令
	int RS1	= (insn>>21) & 0x1f;
	int RD = (insn>>16) & 0x1f;
  int type = (insn>>13) & 0x7;//type of data 
  int num = (insn) & 0xf;//num of data
  int addr1 = get_int(RS1);
  int addrv = get_int (RD);
  for(int i = 0;i < num;i++){
      write_mem_float((addrv+i*num),get_V_float(addr1*16+i*num)); 
  }
  return pc + 4;
}

long INSN_VADD(long pc){
  unsigned int insn = read_mem_uword(pc);
  int RS1 = (insn>>21) & 0x1f;
  int RS2 = (insn>>16) & 0x1f;
  int RD = (insn>>11) & 0x1f;
  int type = (insn>>8) & 0x7;
  int num = (insn) & 0xf;
  int addr1 = get_int(RS1);
  int addr2 = get_int(RS2);
  int addrd = get_int (RD);
  for(int i = 0; i<num; i++){
    put_V_float((RD*16+num*i),get_V_float(RS1*16+num*i)+get_V_float(RS2*16+num*i));
  }
  
  return pc + 4; 
}
  


void Execution(void)
{
	long pc;
	pc = 0x1000;
	for(;pc;)
	{	
    printf("time:%d\n",time);
		unsigned int insn = read_mem_uword(pc);//读第一条指令
		//指令译码
		int OPCODE = (insn>>26) & 0x3f;
		int RS1	= (insn>>21) & 0x1f;
		int RS2 = (insn>>16) & 0x1f;
		int RD = (insn>>11) & 0x1f;
		int IMM = (int)(short)(insn & 0xffff);//符号扩展
		unsigned int UIMM = (unsigned int)(short)(insn & 0xffff);//0扩展
		int FUNC = insn & 0x7ff;
		int OFFSET = insn & 0x3fffff;
    time++;
		if(OPCODE)
		{
			switch(OPCODE){
				case OP_ADDI:	pc = INSN_ADDI(pc);
								break;
		//		case OP_ADDUI:	pc = INSN_ADDUI(pc);
		//						break;
				case OP_NOP:	pc = INSN_NOP(pc);
								break;
				case OP_LOADI:  pc = INSN_LOADI(pc);
								break;
				case OP_LOADIMM:pc = INSN_LOADIMM(pc);
								break;
				case OP_LOADD:	pc = INSN_LOADD(pc);
								break;
				case OP_STORED:	pc = INSN_STORED(pc);
								break;
				case OP_JR:		pc = INSN_JR(pc);
								break;
				case OP_BRE:	pc = INSN_BRE(pc);
								break;
				case OP_MOVI:	pc = INSN_MOVI(pc);
								break;
				case OP_MOVD:	pc = INSN_MOVD(pc);
                break;
        case OP_VLOAD: pc = INSN_VLOAD(pc);
                break;
        case OP_VSTORE: pc = INSN_VSTORE(pc);
								break;
        case OP_VADD: pc = INSN_VADD(pc);
                break;
				default:		printf("error1:unimplemented instruction\n");
								exit(-1);
			}//end switch(OPCODE)
		}//end if(OPCODE)
		else
		{
			switch(FUNC){
			/*	case OP_ADD:	pc = INSN_ADD(pc);
								break;
				case OP_ADDU:	pc = INSN_ADDU(pc);
								break;*/
				case OP_ADDDOU: pc = INSN_ADDDOU(pc);
								break;
				default:		printf("error3:unimplemented instruction\n");
								exit(-1);
			}//end switch(FUNC)
		}//end else
	}//end for
}//end Execution()







int main()
{
	unsigned char uch1,uch2;
	char cha1,cha2;
	unsigned short uns1,uns2;
	short sho1,sho2;
	int in1,in2;
	unsigned int uni1,uni2;
	float fl1,fl2;
	double dou1,dou2;
	unsigned long addr = 256;
	mem = (char*)malloc(sizeof(char)* mem_size);
	r = (int*)malloc(sizeof(int)*ireg_size);
	rrd = (double*)malloc(sizeof(double)*8);
  val = (char*)malloc(sizeof(char)* val_size);


	if(mem==NULL)
	{
		printf("error:main memory allocation\n");
		exit(-1);
	}
	if(r==NULL)
	{
		 printf("error:main Register file allocation\n");
		exit(-1);
	}
	if(val==NULL)
	{
		printf("error: val allocation\n");
		exit(-1);
  }
	put_int(0,0);
	put_double(0,0.0);
	put_int(1,1);
  

  int time = 1 ;
  write_mem_float(10,3.0);//3.0->mem[10]
  write_mem_float(14,2.0);//2.0->mem[14]
  write_mem_float(18,1.0);//1.0->mem[18]
  write_mem_float(22,-1.0);//-1.0->mem[22]
  
  write_mem_uword(0x1000,0x1c40000a);//LOADII R2 = 10;
  int x=0;
  int y =0;
  for(int i = 0x1004;x<50;i=i+0x4)
  {
     write_mem_uword(i,0x1c40000a);//LOADII R2 = 10;
     x++;
     y = i;
  }
  printf("%d",y);
  write_mem_uword(4300,0x28000000);
  Execution();

	

	free (mem);
	free (r);
  free (val);
return 0;
}

#endif
