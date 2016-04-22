#ifndef	_monit_h_
#define	_monit_h_

#include "firmware/hidcmd.h"

typedef	unsigned short ushort;
typedef	unsigned char uchar;

typedef	struct {
	uchar report_id;
	uchar cmd;
	uchar size;		// bit7:6 ��arena�Ɏg�p����\��.
	uchar adr[2];
	uchar data[48];
} cmdBuf;

//  BitBang�f�[�^�r�b�g.

#define	bitTCK	0x80	// TCK
#define	bitTDI	0x40	// TDI MOSI
#define	bitTDO	0x20	// TDO MISO
#define	bitTMS	0x10	// TMS 

//	�������W�X�^ (1=�o��)

#define	dirTCK	0x80
#define	dirTDI	0x40
#define	dirTDO	0x20	// ����PIN�̂ݓ���(==0)
#define	dirTMS	0x10





#if	1
#define	JCMD_bitTMS	bitTMS
#define	JCMD_bitTDI	bitTDI
#define	JCMD_bitTDO	bitTDO
#define	JCMD_bitTCK	bitTCK
#else
#define	JCMD_bitTMS	0x80	// TMS 
#define	JCMD_bitTDI	0x40	// TDI MOSI
#define	JCMD_bitTDO	0x20	// TDO MISO
#define	JCMD_bitTCK	0x10	// TCK
#endif

//
//	area.
//
enum {
	AREA_RAM    = 0   ,
	AREA_EEPROM = 0x40,
	AREA_PGMEM  = 0x80,
//	reserved      0xc0
	AREA_MASK	= 0xc0,
	SIZE_MASK	= 0x3f,
};

/*
 *	�I�v�V����������`�F�b�N
 */
#ifdef MAIN_DEF
char  *opt[128];	/* �I�v�V�����������w�肳��Ă�����A
						���̕����ɑ�����������i�[�A
						�w�肪�Ȃ����NULL�|�C���^���i�[	*/
#else
extern char  *opt[128];
#endif

/*
 *	�I�v�V����������`�F�b�N
 *		optstring �Ɋ܂܂��I�v�V���������́A
 *				  �㑱�p�����[�^�K�{�Ƃ݂Ȃ��B
 */
#define Getopt(argc,argv,optstring)           		\
 {int i;int c;for(i=0;i<128;i++) opt[i]=NULL; 		\
   while( ( argc>1 )&&( *argv[1]=='-') ) {    		\
	 c = argv[1][1] & 0x7f;   						\
       opt[c] = &argv[1][2] ; 						\
       if(( *opt[c] ==0 )&&(strchr(optstring,c))) {	\
         opt[c] = argv[2] ;argc--;argv++;          	\
       }                        					\
     argc--;argv++;           						\
 } }

#define IsOpt(c) ((opt[ c & 0x7f ])!=NULL)
#define   Opt(c)   opt[ c & 0x7f ]

#define Ropen(name) {ifp=fopen(name,"rb");if(ifp==NULL) \
{ printf("Fatal: can't open file:%s\n",name);exit(1);}  \
}

#define Wopen(name) {if(strcmp(name, "con")==0) ofp=stdout; else {ofp=fopen(name,"wb");if(ofp==NULL) \
{ printf("Fatal: can't create file:%s\n",name);exit(1);}}  \
}

#if 0
#define Read(buf,siz)   fread (buf,1,siz,ifp)
#define Write(buf,siz)  fwrite(buf,1,siz,ofp)
#endif
#define Rclose()  fclose(ifp)
#define Wclose()  fclose(ofp)


#define	ZZ	printf("%s:%d: ZZ\n",__FILE__,__LINE__);

#ifdef	_LINUX_
//	MSDOS�ɑ��݂���֐��̃_�~�[�ł��v���g�^�C�v�錾����:
void Sleep(int mSec);			// wait mSec.
void strupr(char *s);			// �������S���啶����.
int	stricmp(char *s1,char *s2);	// �啶���������̋�ʂ����Ȃ��������r.
int getch(void);
int kbhit(void);

#endif


#endif	//_include_monit_h_

