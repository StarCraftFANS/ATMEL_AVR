/*********************************************************************
 *	�g�h�c�Q�l��������
 *********************************************************************
 *
 * �R�}���h�w���v�� 'h' �������� '?' �ł�.

�A�h���X�͈͎w��ɂāA
 ADDRESS < ADDRESS2 �𖞂����Ȃ��Ƃ��� ADDRESS2 ���o�C�g���Ƃ݂Ȃ��܂�.
�R�}���h��A�h���X�̋�؂�͋󔒂������� ',' �̂ǂ���ł���.

 ADDRESS �͂P�U�i���A�������̓|�[�g������.

 *********************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "monit.h"
#include "util.h"
#include "hidasp.h"
#include "gr.h"

void cmdPortPrintOne(char *name,int adrs,int val);
int	  kbhit();

#include "portlist.h"

#define DEFAULT_SERIAL	"0001"

int hidmon_mode = 1;

char HELP_USAGE[]=
"* AVR_Monit Ver 0.1\n"
"Usage is\n"
"  monit [Options]\n"
"Options\n"
"  -l<logfile> ...  save log file.\n"
"  -i<file>    ...  input script file to execute.\n"
"  -v          ...  print verbose.\n"

;

char HELP_MESSAGES[]=
"* HID_Monit Ver 0.1\n"
"Command List\n"
" d  <ADDRESS1> <ADDRESS2>    Dump Memory(RAM)\n"

" dr <ADDRESS1> <ADDRESS2>    Dump Memory(EEPROM)\n"
" dp <ADDRESS1> <ADDRESS2>    Dump Memory(PGMEM)\n"

" e  <ADDRESS1> <DATA>        Edit Memory\n"
" f  <ADDRESS1> <ADDRESS2> <DATA> Fill Memory\n"
" p ?                         Print PortName-List\n"
" p .                         Print All Port (column format)\n"
" p *                         Print All Port (dump format)\n"
" p <PortName>                Print PortAddress and data\n"
" p <PortName> <DATA>         Write Data to PortName\n"
" p <PortName>.<bit> <DATA>   Write Data to PortName.bit\n"
" sleep <n>                   sleep <n> mSec\n"
" bench <CNT>                 HID Write Speed Test\n"
" boot                        Start user program\n"
" go <address>                Start user program at <address>\n"
" poll <portName>             Continuous polling port\n"
" graph <portName>            Graphic display\n"
" q                           Quit to DOS\n"

;

//
//	�R�}���h��͗p���[�N.
//
#define	MAX_CMDBUF	4096
static	char cmdbuf[MAX_CMDBUF];
static  char *arg_ptr[128];
static  int  arg_val[128];
static  int  arg_hex[128];
static  int  arg_cnt;
static	char isExit = 0;		//1:�R�}���h���̓��[�v�𔲂���.

static	int	 adrs = 0;
static	int	 adrs2= 0;
static	int	 memcnt=0;
static	int  arena= 0;

static	int  ADDR_pinA = 0;
static	int  ADDR_pinB = 0;
static	int  ADDR_pinC = 0;
static	int  ADDR_pinD = 0;

static	char usb_serial[128];	/* �g�p����HIDaspx�̃V���A���ԍ����i�[ */


//
//	�I�v�V�����������[�N.
//
static	char verbose_mode = 0;	//1:�f�o�C�X�����_���v.
static	FILE *script_fp = NULL;	//�O���t�@�C������R�}���h��ǂݍ���.
static	FILE *log_fp = NULL;	//�[�����O���t�@�C���ɏ�������.

void ADDR_pinname_init()
{
	ADDR_pinA = portAddress("PINA");
	ADDR_pinB = portAddress("PINB");
	ADDR_pinC = portAddress("PINC");
	ADDR_pinD = portAddress("PIND");
}

/**********************************************************************
 *		������ p �� ���� c �ŕ������Aargs[] �z��ɓ����B
 *		������͕��f����邱�Ƃɒ��ӁB
 *		�󔒂��X�v���b�^�[�ɂȂ�悤�ɂ��Ă���̂Œ���
 **********************************************************************
 */
static	int	splitchr=' ';
static	int is_spc(int c)
{
	if(c==splitchr) return 1;
	if(c==' ') 		return 1;
	if(c=='\t') 	return 1;
	return(0);
}

/**********************************************************************
 *		������ *p ���f���~�^���� c ���邢�͋󔒂ŕ�����
 *             *args[] �|�C���^�ɓ����
 *		������ n ��Ԃ�.
 **********************************************************************
 */
static	int split_str(char *p,int c,char **args)
{
	int argc=0;
	int qq=0;
	splitchr=c;

	while(1) {
		while( is_spc(*p) ) p++;
		if(*p==0) break;

		if(*p != 0x22) {
			args[argc++]=p;		/* ���ڂ̐擪���� */

			while( *p ) {		/* ��؂蕶���܂œǂݐi�߂� */
				if(is_spc(*p))break;
				p++;
			}

			if(is_spc(*p)) {	/* ��؂蕶���ł���� */
				*p=0;p++;		/* NULL�Ő؂� */
			}
		}else{
			qq=*p++;
			args[argc++]=p;		/* ���ڂ̐擪���� */

			while( *p ) {		/* ��؂蕶���܂œǂݐi�߂� */
				if(*p==qq)break;
				p++;
			}
			if(*p==qq) {	/* ��؂蕶���ł���� */
				*p=0;p++;		/* NULL�Ő؂� */
			}
		}
	}
	return argc;
}


/*********************************************************************
 *	���� c ���󔒕����Ȃ�P��Ԃ�.
 *********************************************************************
 */
int is_space(int c)
{
	if((c==' ') || (c=='\t')) return 1;
	return 0;
}
/*********************************************************************
 *	������ *buf �̐擪�]����ǂݔ�΂�.
 *********************************************************************
 */
char *sp_skip(char *buf)
{
	while(*buf) {
		if(is_space(*buf)==0) return buf;
		buf++;
	}
	return buf;
}
/*********************************************************************
 *	������ *buf ���Q�i�l '0101_1111' �ƌ��Ȃ��邩�ǂ����`�F�b�N����.
 *********************************************************************
 */
int radix2scanf(char *s,int *p)
{
	int rc = 0;
	int c;

	// �Q�i�l�������ꍇ�̏��� �� �P�O�����ȉ��A���A _ ���܂�ł��邱��.
	// 0b �ł͂��߂���@�����邪�A�P�U�i�l�Ƌ�ʂ����Ȃ��̂�,�o�����_������.

	if( (strlen(s)<=10) && (strchr(s,'_')) ) {
		while(1) {
			c = *s++;
			if( c==0 ) break;		//������I�[.
			if((c=='0')||(c=='1')) {
				rc = rc << 1;		// �Q�{.
				rc |= (c-'0');		// '1' �Ȃ�LSB��On.
			}else
			if(c != '_') {
				return (-1);	//���e�ł��Ȃ������������̂Ŏ��s.
			}
		}
		*p = rc;	//�Q�i�l��Ԃ�.
		return 0;	// OK.
	}
	return (-1);
}

/*********************************************************************
 *	arg_ptr[] �̕������HEX���l�Ƃ݂Ȃ��Ēl�ɕϊ��� arg_hex[]�ɓ����
 *********************************************************************
 */
void scan_args(int arg_cnt)
{
	int i,v;
	for(i=0;i<arg_cnt;i++) {
		v = portAddress(arg_ptr[i]);	//�V���{�����ɂ�����.
		if(v) {
			arg_hex[i] = v;
			arg_val[i] = v;
		}else{
			v=(-1);
			sscanf(arg_ptr[i],"%x",&v);
			arg_hex[i] = v;

			radix2scanf(arg_ptr[i],&arg_hex[i]);

			v=(-1);
			sscanf(arg_ptr[i],"%d",&v);
			arg_val[i] = v;
		}
	}
}
/*********************************************************************
 *	������ *buf ����A�p�����[�^��ǂݎ��A( adrs , memcnt ) �����߂�
 *********************************************************************
 */
int	get_arg(char *buf)
{
	arena = AREA_RAM;
	if(*buf == 'p') {
		buf++; arena = AREA_PGMEM;
	}else
	if(*buf == 'r') {
		buf++; arena = AREA_EEPROM;
	}
	memcnt = 64;
	adrs2 = (-1);

	buf = sp_skip(buf);
	if(*buf==0) return 0;

	arg_cnt = split_str(buf,',',arg_ptr);
	scan_args(arg_cnt);

	if(arg_cnt>=1) {
		adrs = arg_hex[0];
	}
	if(arg_cnt>=2) {
		adrs2 = arg_hex[1];
		if(adrs2 != (-1) ) {
			memcnt = adrs2 - adrs + 1;
		}
		if(	memcnt < 0) {
			memcnt = adrs2;
		}
	}
	return arg_cnt;
}
/*********************************************************************
 *	������ *buf ���R�}���h�� *cmd ���܂�ł��邩�ǂ������ׂ�.
 *********************************************************************
 */
int	str_comp(char *buf,char *cmd)
{
	while(*cmd) {
		if(*buf != *cmd) {return 1;}	// �s��v.
		cmd++;
		buf++;
	}
	return 0;
}
/*********************************************************************
 *
 *********************************************************************
 */
void usage(void)
{
	printf( HELP_USAGE );
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdHelp(char *buf)
{
	printf( HELP_MESSAGES );
}


/*********************************************************************
 *	1bit����On�̃}�X�N�p�^������Abit�ԍ�(0..7)�ɕϊ�.
 *********************************************************************
 */
static	int get_bpos(int mask)
{
	int i;
	for(i=0;i<8;i++) {
		if(mask & 1) return i;
		mask >>= 1;
	}
	return (-1);
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdQuestion(char *buf)
{
	int v;
	if( get_arg(buf) < 1) {
		cmdHelp("");
	}else{
		v = portAddress(arg_ptr[0]);
		if(v) printf("%s=0x%x\n",arg_ptr[0],v);
	}
}

void cmdPortPrintOne(char *name,int adrs,int val)
{
	char tmp[32];
	printf("%8s(%02x) %02x %s\n",name,adrs,val,radix2str(tmp,val));
}

void cmdPortPrintOneCr(char *name,int adrs,int val)
{
	char tmp[32];
	printf("%8s(%02x) %02x %s\r",name,adrs,val,radix2str(tmp,val));
}

void cmdPortPrintAllCr(unsigned char *pinbuf)
{
	char tmp1[32];
	char tmp2[32];
	char tmp3[32];
	int pina = pinbuf[9];
	int pinb = pinbuf[6];
	int pind = pinbuf[0];

	printf("PINA %02x %s | PINB %02x %s | PIND %02x %s \r"
		,pina,radix2str(tmp1,pina)
		,pinb,radix2str(tmp2,pinb)
		,pind,radix2str(tmp3,pind)
	);
}

void cmdPortPrintOne_b(char *name,int adrs,int val,int mask)
{
	int bpos = 0;
	char bitlist[16] = "____ ____";
	if( mask==0) {
		cmdPortPrintOne(name,adrs,val);
	}else{
		bpos = get_bpos(mask);
		if(bpos<0) {
			cmdPortPrintOne(name,adrs,val);
		}else{
			bpos = 7 - bpos;
			if(bpos >= 4) bpos ++;

			if( val & mask ) bitlist[bpos]='1';
			else			 bitlist[bpos]='0';
			printf("%8s(%02x) %02x %s\n",name,adrs, val , bitlist);
		}
	}
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdPort(char *buf)
{
	int v,n,mask=0;
	n = get_arg(buf);	//�����̐�.
	if(n != 0) {
		v = portAddress_b(arg_ptr[0],&mask);	//�|�[�g���̂���A�h���X���Z�o.
		if(v) {		// �A�h���X�����݂���.
			if(n < 2) {					// �P�̃|�[�g��ǂݎ��.
				int val = UsbPeek(v,0);
				cmdPortPrintOne_b(arg_ptr[0],v,val,mask);
			}else{						// p <adrs> <data>
				UsbPoke_b(v,0,arg_hex[1],mask);				//��������.
				int val = UsbPeek(v,0);
				cmdPortPrintOne_b(arg_ptr[0],v,val,mask);	//�ēǂݍ���.
			}
			return;
		}else
		if(	strcmp(arg_ptr[0],"*")==0) {//�_���v�`���Ń|�[�g�\������.
			PrintPortAll(0);
			return;
		}else
		if(	strcmp(arg_ptr[0],".")==0) {//�i�g�`���Ń|�[�g�\������.
			PrintPortColumn();
			return;
		}else
		if(	strcmp(arg_ptr[0],"?")==0) {//�|�[�g���̂̈ꗗ���o��.
			PrintPortNameList();
			return;
		}
	}
	PrintPortAll(1);	//�悭�Q�Ƃ���|�[�g�����\������.
}
/*********************************************************************
 *	�|�[�g���e���|�[�����O����.
 *********************************************************************
 */
void cmdPoll(char *buf)
{
	int v,n,count,i;
	n = get_arg(buf);	//�����̐�.
	if(n != 0) {
		if(n < 2) {					// �P�̃|�[�g��ǂݎ��.
			count = 1000;
		}else{
			count = arg_hex[1];
		}

		v = portAddress(arg_ptr[0]);	//�|�[�g���̂���A�h���X���Z�o.
		if(v) {		// �A�h���X�����݂���.
			UsbSetPoll(v,0);
			for(i=0;i<count;i++) {
				int val = UsbPoll();
				cmdPortPrintOneCr(arg_ptr[0],v,val);
				if(kbhit()) break;
			}
		}else{
			//	A,B,D�S�|�[�g�ǂ�.
			unsigned char portbuf[256];
			int adr = 0x30;	// PORTD
			int size = 10;	// 0x30�`0x39

			for(i=0;i<count;i++) {
				int rc = UsbRead(adr,arena,portbuf,size);
				if( rc != size) {
					printf("poll error %d.\n",rc);
					return;
				}
				cmdPortPrintAllCr(portbuf);
				if(kbhit()) break;
			}
		}
	}
	printf("\n\n");
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdBoot(char *buf)
{
	UsbBootTarget(0,1);
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdGo(char *buf)
{
	int n = get_arg(buf);	//�����̐�.
	if(n != 0) {
		UsbBootTarget(arg_hex[0]/2,0);
	}
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdErase(char *buf)
{
	int n = get_arg(buf);	//�����̐�.
	if(n != 0) {
		UsbEraseTargetROM(adrs,memcnt);
	}
}
/*********************************************************************
 *	�|�[�g���e���|�[�����O����.
 *********************************************************************
 */
#define	SCREEN_W	768
#define	SCREEN_X0	32
#define	SCREEN_H	480

#define	SIG_COLOR	0x00f000
#define	SIG_COLOR2	0x004000
#define	BACK_COLOR	0x000000
#define	FRAME_COLOR	0x504000

int	calc_ypos(int i)
{
	int off;
	if(i>=4) off = 24;
	else off = 0;

	int y1 = 80 + i*48 + off;
	return y1;
}
/*********************************************************************
 *	�P�T���v���̂P�r�b�g����`��
 *********************************************************************
 *	v    : 0 �� ��0   �M�����x��.
 *  diff : 0 �� ��0   �M�����]���N����.
 */
void plot_signal(int x,int y,int v,int diff)
{
	int col0,col1,col2;

	if(v) {	col0 = FRAME_COLOR;	col1 = SIG_COLOR ; }
	else  {	col0 = SIG_COLOR;	col1 = BACK_COLOR ; }

	if(diff) {	col2 = SIG_COLOR2;}
	else	 {	col2 = BACK_COLOR;}

	if(diff) gr_vline(x,y-1,x,y-32,col2);	//�c�̐�.
	if(v) {
		gr_pset(x,y-32,col1);		// H�̐F.
	}else{
		gr_pset(x,y-1 ,col0);		// L�̐F.
	}
	gr_pset(x,y  ,FRAME_COLOR);		// �x�[�X���C����.
}

/*********************************************************************
 *	�P�T���v����`��.
 *********************************************************************
 */
void draw_sample(int x,int y,int val)
{
	static int old_val;
	int i,y1;
	int m=1;
	int diff = val ^ old_val;	//�O��ƒl���ς����?

	for(i=0;i<8;i++,m<<=1) {
		y1 = calc_ypos(i);
		plot_signal(x,y+y1,val & m,diff & m);
	}
	old_val = val;
}

/*********************************************************************
 *	�O���t�\�����̕���������.
 *********************************************************************
 */
void draw_PortName(int adr)
{
	int i,y1;
	char string[128];
	int color = 0x80ff00;
	char *port="-";

	if(adr == ADDR_pinA) port = "A";
	if(adr == ADDR_pinB) port = "B";
	if(adr == ADDR_pinC) port = "C";
	if(adr == ADDR_pinD) port = "D";

	for(i=0;i<8;i++) {
		sprintf(string,"P%s%d",port,i);
		y1 = calc_ypos(i);
		gr_puts(4,y1-20,string,color);
	}
}
/*********************************************************************
 *	�O���t�\��.(��)
 *********************************************************************
 */
void cmdGraph(char *buf)
{
	int x=SCREEN_X0;
	int adr = portAddress("PINB");	// default PINB
	int n = get_arg(buf);	//�����̐�.
	if( n ) {
		adr = portAddress(arg_ptr[0]);	//�|�[�g���̂���A�h���X���Z�o.
	}

	gr_init(SCREEN_W,SCREEN_H,32,0);
	draw_PortName(adr);

	UsbSetPoll(adr,0);
	do {
		int val = UsbPoll();
		gr_vline(x,0,x,SCREEN_H-1,0);			//�c�̐�.
		draw_sample(x,0,val);
		x++;
		if(x>=SCREEN_W) x=SCREEN_X0;
		gr_vline(x,0,x,SCREEN_H-1,0xffff00);	//�c�̐�.
	}while(gr_break()==0);
	gr_close();
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdQuit(char *buf)
{
	printf("Bye.\n");
	isExit = 1;
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdDump(char *buf)
{
	get_arg(buf);
	UsbDump(adrs,arena,(memcnt+7)&(-8));
	adrs += memcnt;
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdEdit(char *buf)
{
	int mask = 0;
	int argc = get_arg(buf);

	if( argc >= 2) {
		UsbPoke(adrs,arena,adrs2,mask);
	}else
	if( argc >= 1) {
		int val = UsbPeek(adrs,arena);
		printf("%06x %02x\n",adrs,val);
	}
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdFill(char *buf)
{
	int i;
	if( get_arg(buf) >= 3) {
		for(i=adrs;i<=adrs2;i++) {
			UsbPoke(i,arena,arg_hex[2],0);
		}
	}
}
/*********************************************************************
 *
 *********************************************************************
 */
void cmdBench(char *buf)
{
	int cnt = 1000;
	int packet = 8;
	int argc = get_arg(buf);

	if(argc >= 1) {
		cnt = arg_val[0];
	}
	if(argc >= 2) {
		packet = arg_val[1];
	}
//	printf("%d,%d\n",cnt,packet);
	UsbBench(cnt,packet);
}
/*********************************************************************
 *
 *********************************************************************
// ATtiny USART Baud Rate Register High UBBRH[11:8]
#define UBRRH   _SFR_IO8(0x02)

// ATtiny USART Control and Status Register C UCSRC
#define UCSRC   _SFR_IO8(0x03)

#define UMSEL   6
#define UPM1    5
#define UPM0    4
#define USBS    3
#define UCSZ1   2
#define UCSZ0   1
#define UCPOL   0

	UCSRC = (1<<USBS)|(3<<UCSZ0);     �ڰь`���ݒ�(8�ޯ�,2�į�� �ޯ�)

#define BAUD 9600             	// �ړIUSART�{�[���[�g���x
#define MYUBRR FOSC/16/BAUD-1 	// �ړIUBRR�l

 */

#ifdef F_CPU
#define FOSC F_CPU	         	// MCU�N���b�N���g��
#else
#define FOSC 12000000         	// MCU�N���b�N���g��
#endif

/*********************************************************************
 *
 *********************************************************************
 */
void cmdSleep(char *buf)
{
	int cnt = 1000;
	int argc = get_arg(buf);
	if(argc >= 1) {
		cnt = arg_val[0];
	}

	_sleep(cnt);
}
/*********************************************************************
 *
 *********************************************************************
 */


CmdList cmdlist[]={
	{	"h"	,	cmdHelp },
	{	"?"	,	cmdQuestion},
	{	"bench",cmdBench},
	{	"sleep",cmdSleep},
	{	"poll", cmdPoll },
	{	"graph",cmdGraph },
	{	"boot",	cmdBoot },
	{	"go",	cmdGo   },
	{	"erase",cmdErase},
	{	"exit",	cmdQuit },

	{	"q"	,	cmdQuit },
	{	"d"	,	cmdDump },
	{	"m"	,	cmdEdit },
	{	"e"	,	cmdEdit },
	{	"f"	,	cmdFill },
	{	"p"	,	cmdPort },
	{	NULL,	cmdQuit },
};



/*********************************************************************
 *	���j�^�R�}���h��͂Ǝ��s.
 *********************************************************************
 */
void cmdMonit(char *buf)
{
	CmdList *t;
	for(t = cmdlist; t->name != NULL ; t++) {
		if( str_comp(buf,t->name)==0 ) {
			buf += strlen(t->name);
			t->func(buf);
			return;
		}
	}
	/*error*/
}
/*********************************************************************
 *	�s����CR/LF ���폜����.
 *********************************************************************
 */
void chop_crlf(char *p)
{
	while(*p) {
		if((*p==0x0d)||(*p==0x0a)) { *p = 0; return; }
		p++;
	}
}

/*********************************************************************
 *	�P�s����.�@�t�@�C���������̓R���\�[���̗��������e.
 *********************************************************************
 */
static void get_line(char *buf)
{
	char *rc;
	if( script_fp != NULL) {
		rc = fgets(buf,MAX_CMDBUF,script_fp);		//�t�@�C���������.
		if(rc == NULL) {		//�t�@�C�����������̓G���[.
			buf[0]=0;			//��s��Ԃ�.
			fclose(script_fp);
			script_fp = NULL;
		}else{
			chop_crlf(buf);		//�s����CR/LF ���폜����.
			printf("%s\n",buf);	//�R�}���h�G�R�[����.
		}
	}else{
		fgets(buf,MAX_CMDBUF,stdin);		//�R���\�[���������.
		chop_crlf(buf);		//�s����CR/LF ���폜����.
	}
}
/*********************************************************************
 *	�P�s����.�@�t�@�C���������̓R���\�[���̗��������e.
 *********************************************************************
 */
void Term_Log(int c)
{
	if( log_fp != NULL) {
		putc(c,log_fp);
	}
}
/*********************************************************************
 *	���C��
 *********************************************************************
 */
int main(int argc,char **argv)
{
	//�I�v�V�������.
	Getopt(argc,argv,"il");
	if(IsOpt('h') || IsOpt('?') || IsOpt('/')) {
		usage();exit(0);
	}
	if(IsOpt('i')) {
		script_fp = fopen( Opt('i') , "rb" );
	}
	if(IsOpt('l')) {
		log_fp = fopen( Opt('l') , "wb" );
	}
	if(IsOpt('v')) {	//�f�o�C�X�����_���v����.
		verbose_mode = 1;
	}
	strcpy(usb_serial, DEFAULT_SERIAL);
	if(IsOpt('p')) {
		char *s;
		s = Opt('p');
		if (s) {
			if (*s == ':') s++;
			if (*s == '?') {
				hidasp_list("hidmon");
				exit(0);
			} else if (isdigit(*s)) {
				if (s) {
					int n, l;
					l = strlen(s);
					if (l < 4 && isdigit(*s)) {
						n = atoi(s);
						if ((0 <= n) && (n <= 999)) {
							sprintf(usb_serial, "%04d", n);
						} else {
							if (l == 1) {
								usb_serial[3] = s[0];
							} else if  (l == 2) {
								usb_serial[2] = s[0];
								usb_serial[3] = s[1];
							} else if  (l == 3) {
								usb_serial[1] = s[0];
								usb_serial[2] = s[1];
								usb_serial[3] = s[2];
							}
						}
					} else {
						strncpy(usb_serial, s, 4);
					}
				}
			} else if (*s == '\0'){
				strcpy(usb_serial, DEFAULT_SERIAL);		// -p�݂̂̎��A
			} else {
				strncpy(usb_serial, s, 4);
			}
			strupr(usb_serial);
		}
	}

	//������.
	if( UsbInit(verbose_mode, 0, usb_serial) == 0) exit(1);
	ADDR_pinname_init();

	do {					//���j�^�[���[�h.
		printf("\rAVR> ");
		get_line(cmdbuf);
		cmdMonit(cmdbuf);
	} while(isExit == 0);

	UsbExit();

	if(script_fp!=NULL) fclose(script_fp);
	if(log_fp!=NULL) fclose(log_fp);
	return 0;
}
/*********************************************************************
 *
 *********************************************************************
 */

