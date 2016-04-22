/*********************************************************************
 *	�t�r�a�o�R�̃^�[�Q�b�g�������[�A�N�Z�X
 *********************************************************************
 *�`�o�h:
int		UsbInit(int verbose,int enable_bulk);			������.
int		UsbExit(void);									�I��.
void 	UsbBench(int cnt,int psize);					�x���`�}�[�N

void 	UsbPoke(int adr,int arena,int data,int mask);	��������
int 	UsbPeek(int adr,int arena);						�P�o�C�g�ǂݏo��
int 	UsbRead(int adr,int arena,uchar *buf,int size);	�A���ǂݏo��
void	UsbDump(int adr,int arena,int size);			�������[�_���v
 *
 *�����֐�:
void	dumpmem(int adr,int cnt);
void	pokemem(int adr,int data0,int data1);
void	memdump_print(void *ptr,int len,int off);
 */

#include <windows.h>
#include <stdio.h>
#include <time.h>

#include "hidasp.h"
#include "monit.h"
#include "util.h"
#include "../firmware/hidcmd.h"

int portAddress(char *s);


#define	VERBOSE	0

#define	if_V	if(VERBOSE)

static	char verbose_mode = 0;
static  int  HidLength1 = 7;
static  int  HidLength  = 131;	//31;

// ReportID:4  POLLING PORT�������ςł���.
static	int	pollcmd_implemented;

#define	ID4		4
#define	LENGTH4	5

/****************************************************************************
 *	�������[���e���_���v.
 ****************************************************************************
 *	void *ptr : �_���v�������f�[�^.
 *	int   len : �_���v�������o�C�g��.
 *	int   off : �^�[�Q�b�g���̔Ԓn�\��.
 */
void memdump_print(void *ptr,int len,int off)
{
	unsigned char *p = (unsigned char *)ptr;
	int i,j,c;

	for(i=0;i<len;i++) {
		if( (i & 15) == 0 ) printf("%06x",(int)p - (int)ptr + off);
		printf(" %02x",*p);p++;
		if( (i & 15) == 15 )
		{
#if	1	// ASCII DUMP
			printf("  ");
			for(j=0;j<16;j++) {
				c=p[j-16];
				if(c<' ') c='.';
				if(c>=0x7f) c='.';
				printf("%c",c);
			}
#endif
			printf("\n");
		}
	}
	printf("\n");
}



/*********************************************************************
 *	�`�u�q�f�o�C�X�ɏ����R�}���h�𑗂��āA�K�v�Ȃ猋�ʂ��󂯎��.
 *********************************************************************
 *	cmdBuf   *cmd : �����R�}���h
 *	uchar    *buf : ���ʂ��󂯎��̈�.
 *  int reply_len : ���ʂ̕K�v�T�C�Y. (0�̏ꍇ�͌��ʂ��󂯎��Ȃ�)
 *�߂�l
 *		0	: ���s
 *	   !0   : ����
 */
int QueryAVR(cmdBuf *cmd,uchar *buf,int reply_len)
{
	int rc = 0;
	int size = cmd->size;
	char *s;
	rc = hidWriteBuffer((char*) cmd , HidLength1 );
	if(rc == 0) {
		printf("hidWrite error\n");
		exit(1);
	}
	if(reply_len) {
		rc = hidReadBuffer((char *)cmd , reply_len );
		if(rc == 0) {
			printf("hidRead error\n");
			exit(1);
		}
		s = (char*) cmd;
		memcpy(buf, s+1 ,size);
	}
    return rc;
}
/*********************************************************************
 *	�^�[�Q�b�g���̃������[���e���擾����
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *	int           size :�ǂݎ��T�C�Y.
 *	unsigned char *buf :�󂯎��o�b�t�@.
 *	���ӁF HID Report Length - 1 ��蒷���T�C�Y�͗v�����Ă͂Ȃ�Ȃ�.
 */
int dumpmem(int adr,int arena,int size,unsigned char *buf)
{
	cmdBuf cmd;

	memset(&cmd,0,sizeof(cmdBuf));
	cmd.cmd   = CMD_PEEK ;	//| arena;
	cmd.size  = size | arena;
	cmd.adr[0]= adr;
	cmd.adr[1]= adr>>8;

	if( QueryAVR(&cmd,buf,1+size) == 0) return 0;	//���s.
	return size;
}
/*********************************************************************
 *
 *********************************************************************
 */
int UsbBootTarget(int adr,int boot)
{
	cmdBuf cmd;

	memset(&cmd,0,sizeof(cmdBuf));
	if(boot) {
		cmd.cmd   = HIDASP_BOOT_EXIT;
	}else{
		cmd.cmd   = HIDASP_JMP;
		cmd.adr[0]= adr;
		cmd.adr[1]= adr>>8;
	}

	if( QueryAVR(&cmd,0,0) == 0) return 0;	//���s.
	return 1;
}

#define	ERASE_SIZE 128
/*********************************************************************
 *
 *********************************************************************
 */
int 	UsbEraseTargetROM(int adr,int size)
{
	int rc = 0;
	char buf[256];

	printf("adrs = %x size=%x\n",adr,size);

	while(size>0) {
//		memset(buf+4,0xff,ERASE_SIZE);
		memset(buf+4,0xaa,ERASE_SIZE);
		buf[0] = 2;
		buf[1] = adr;
		buf[2] = adr>>8;
		buf[3] = adr>>16;
		rc = hidWriteBuffer(buf , HidLength );	// 132byte
		if(rc==0) return 0;
		adr  += ERASE_SIZE;
		size -= ERASE_SIZE;
	}
	return 1;
}
/*********************************************************************
 *	�^�[�Q�b�g���̃������[���e�i�P�o�C�g�j������������.
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *	int          data0 :�������݃f�[�^.      (OR)
 *	int          data1 :�������݃r�b�g�}�X�N.(AND)
 *����:
 *	�t�@�[�����̏������݃A���S���Y���͈ȉ��̂悤�ɂȂ��Ă���̂Œ���.

	if(data1) {	//�}�X�N�t�̏�������.
		*adr = (*adr & data1) | data0;
	}else{			//�ׂ���������.
		*adr = data0;
	}

 */
void pokemem(int adr,int arena,int data0,int data1)
{
    cmdBuf cmd;
	char buf[256];	// dummy

	memset(&cmd,0,sizeof(cmdBuf));
	cmd.cmd   = CMD_POKE ;	//| arena;
	cmd.size  = 1 | arena;
	cmd.adr[0]= adr;
	cmd.adr[1]= adr>>8;
	cmd.data[0] = data0;
	cmd.data[1] = data1;
	QueryAVR(&cmd,buf,0);
}

/*********************************************************************
 *	�^�[�Q�b�g�Ƃ̐ڑ��`�F�b�N�i�x���`�}�[�N�����˂�j
 *********************************************************************
 *	int i : 0�`255 �̐��l.
 *�߂�l
 *		�G�R�[�o�b�N���ꂽ i �̒l.
 */
int hid_ping(int i)
{
 	cmdBuf cmd;
	char buf[256];	// dummy
	int rc;

	memset(&cmd,i,sizeof(cmdBuf));
	cmd.cmd   = CMD_PING ;
	QueryAVR(&cmd,buf,0);

	rc = hidReadBuffer(buf , HidLength );
	if(rc == 0) {
		printf("hidRead error\n");
		exit(1);
	}
	return buf[2] & 0xff;
}
/*********************************************************************
 *	�]�����x�x���`�}�[�N
 *********************************************************************
 *	int cnt  : PING��ł�.
 *	int psize: PING�p�P�b�g�̑傫��(���݂̓T�|�[�g����Ȃ�)
 */
void UsbBench(int cnt,int psize)
{
	int i,n,rc;
	int time1, rate;
	long long total=0;
	int nBytes  = HidLength1 + HidLength ;	//���݌Œ�.
	// 1���Ping�� 7byte��Query��31�o�C�g�̃��^�[��.

   	printf("hid write start\n");
	int time0 = clock();

	for(i=0;i<cnt;i++) {
		n = i & 0xff;
		rc = hid_ping(n);
		if(rc != n) {
			printf("hid ping mismatch error (%x != %x)\n",rc,n);
			exit(1);
		}
		total += nBytes;
	}

	time1 = clock() - time0;
	if (time1 == 0) {
		time1 = 2;
	}
	rate = (int)((total * 1000) / time1);

	if (total > 1024*1024) {
	   	printf("hid write end, %8.3lf MB/%8.2lf s,  %5.2lf kB/s\n",
	   		(double)total/(1024*1024), (double)time1/1000, (double)rate/1024);
	} else 	if (total > 1024) {
	   	printf("hid write end, %8.3lf kB/%8.2lf s,  %5.2lf kB/s\n",
	   		 (double)total/1024, (double)time1/1000, (double)rate/1024);
	} else {
	   	printf("hid write end, %8d B/%8d ms,  %5d B/s\n", (int)total, time1, rate);
   	}
}
/*********************************************************************
 *	�^�[�Q�b�g���������[���_���v����.
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *	int            cnt :�ǂݎ��T�C�Y.
 */
void UsbDump(int adr,int arena,int cnt)
{
	unsigned char buf[256];
	int size;
	int rc;
	while(cnt) {
		size = cnt;
		if(size>=8) size = 8;
		rc = dumpmem(adr,arena,size,buf);	//�������[���e�̓ǂݏo��.

		if(rc !=size) return;
		memdump_print(buf,size,adr);		//���ʈ�.
		adr += size;
		cnt -= size;
	}
}
/*********************************************************************
 *	�^�[�Q�b�g���������[�A���ǂݏo��
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *	uchar         *buf :�ǂݎ��o�b�t�@.
 *	int           size :�ǂݎ��T�C�Y.
 *�߂�l
 *	-1    : ���s
 *	���̒l: ����.
 */
int UsbRead(int adr,int arena,uchar *buf,int size)
{
	int rc = size;
	int len;
	while(size) {
		len = size;
		if(len >= 8) len = 8;
		int rc = dumpmem(adr,arena,len,buf);
		if( rc!= len) {
			return -1;
		}
		adr  += len;	// �^�[�Q�b�g�A�h���X��i�߂�.
		buf  += len;	// �ǂݍ��ݐ�o�b�t�@��i�߂�.
		size -= len; 	// �c��T�C�Y�����炷.
	}
	return rc;
}
/*********************************************************************
 *	�^�[�Q�b�g���������[�P�o�C�g�ǂݏo��
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *�߂�l
 *	�������[���e�̒l.
 */
int UsbPeek(int adr,int arena)
{
	unsigned char buf[256];
	int size=1;
	int rc = UsbRead(adr,arena,buf,size);
	if( rc != size) {
		return -1;
	}
	return buf[0];
}
static	int	poll_port = 0;

/*********************************************************************
 *	�^�[�Q�b�g���������[1�o�C�g�A���ǂݏo���̃Z�b�g�A�b�v
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *�߂�l
 *	�������[���e�̒l.
 */
int UsbSetPoll_slow(int adr,int arena)
{
	poll_port = adr;
	return 1;
}
/*********************************************************************
 *	�^�[�Q�b�g���������[1�o�C�g�A���ǂݏo��
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *�߂�l
 *	�������[���e�̒l.
 */
int UsbPoll_slow()
{
	return UsbPeek(poll_port,0);
}

/*********************************************************************
 *	�^�[�Q�b�g���������[1�o�C�g�A���ǂݏo���̃Z�b�g�A�b�v
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *�߂�l
 *	�������[���e�̒l.
 */
int UsbSetPoll(int adr,int arena)
{
	char buf[128];
	char rd_buf[128];

	if( pollcmd_implemented == 0 ) {
		return UsbSetPoll_slow(adr,arena);
	}

	memset(buf,0,sizeof(buf));

	buf[0] = ID4;
	buf[1] = HIDASP_SET_PAGE;
	buf[2] = 0;		// page_mode
	buf[3] = adr;	// page_addr
	buf[4] = adr>>8;// page_addr_h

	if( QueryAVR( (cmdBuf*) buf, rd_buf , 0) == 0) return 0;	//���s.
	return 1;
}
/*********************************************************************
 *	�^�[�Q�b�g���������[1�o�C�g�A���ǂݏo��
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *�߂�l
 *	�������[���e�̒l.
 */
int UsbPoll()
{
	char buf[128];	// dummy
	int rc;

	if( pollcmd_implemented == 0 ) {
		return UsbPoll_slow();
	}

	rc = hidReadPoll(buf , LENGTH4 ,ID4);
	if(rc == 0) {
#if	0
		printf("hidRead error\n");
		exit(1);
#else
		return -1;
#endif
	}
	return buf[1] & 0xff;
}




/*********************************************************************
 *	�^�[�Q�b�g���̃������[���e�i�P�o�C�g�j������������.
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *	int           data :�������݃f�[�^.
 *	int           mask :�������ݗL���r�b�g�}�X�N.
 *����
 *  mask  = 0 �̏ꍇ�͑S�r�b�g�L�� (����������)
 *	mask != 0 �̏ꍇ�́Amask�r�b�g1�ɑΉ�����data�̂ݍX�V���A���͕ύX���Ȃ�.
 */
void UsbPoke(int adr,int arena,int data,int mask)
{
	int data0 ,data1;
	if(mask == 0) {
		// �}�X�N�s�v�̒�����.
		data0 = data;
		data1 = 0;
	}else{
		// �}�X�N�������܂ޏ�������.
		// �������݃f�[�^�̗L�������� mask �̃r�b�g�� 1 �ɂȂ��Ă��镔���̂�.
		// mask �̃r�b�g�� 0 �ł��镔���́A���̏���ێ�����.

		data0 = data & mask;	// OR���.
		data1 = 0xff &(~mask);	// AND���.

		// �}�X�N�������݂̃��W�b�N�͈ȉ�.
		// *mem = (*mem & data1) | data0;
	}

	pokemem(adr,arena,data0,data1);
}
/*********************************************************************
 *	�P�r�b�g�̏�������
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *	int            bit :�������݃f�[�^�i1bit�j 0 �������� 1
 *	int           mask :�������ݗL���r�b�g�}�X�N.
 */
void UsbPoke_b(int adr,int arena,int bit,int mask)
{
	int data=0;
	if(mask == 0) {
		//�P�o�C�g�̏�������.
		UsbPoke(adr,arena,bit,0);
	}else{
		//�P�r�b�g�̏�������.
		if(bit) data = 0xff;
		UsbPoke(adr,arena,data,mask);
	}
}

void UsbCheckPollCmd()
{
	pollcmd_implemented = 1;			// �Ƃ肠������������Ă���Ɖ���.
	{
		int gtccr = portAddress("gtccr");	// LSB�̂�1��0�̃|�[�g.
		int data;
		int rc = UsbSetPoll(gtccr,0);
		if(rc==0) {					// ���s.
			pollcmd_implemented = 0;		// ��������Ă��Ȃ��Ɣ���.
			return ;
		}

		data = UsbPoll();
		if((data == (-1)) || (data & 0xfe)) {
			pollcmd_implemented = 0;		// ��������Ă��Ȃ��Ɣ���.
		}
	}
}

/*********************************************************************
 *	������
 *********************************************************************
 */
int UsbInit(int verbose,int enable_bulk, char *serial)
{
	verbose_mode = verbose;
	if(	hidasp_init(serial) & HIDASP_MODE_ERROR) {
    	printf("error: [%s] device not found!\n", serial);
    	return 0;
	} else {
		UsbCheckPollCmd();
		return 1;	// OK.
	}
}



/*********************************************************************
 *	�I��
 *********************************************************************
 */
int UsbExit(void)
{
	hidasp_close();
	return 0;
}


