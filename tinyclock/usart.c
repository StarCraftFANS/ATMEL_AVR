/******************************************************************
 *	USART ����M���W���[��
 ******************************************************************
 *	�{�[���[�g�ݒ� (U2X���g�p)
	static	void USART_init(baud);
 *
 *	�q�r�Q�R�Q�b����M�^�X�N.
	static	void USART_RecvTask(void);
	static	void USART_SendTask(void);

 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "timer.h"
#include "usart.h"

#define	ASTERISK_GUARD		(0)	// (1):�R�}���h�̑O��'*'��K�{�Ƃ���.
								// '*p'
								// '*t' �Ȃ�.

/******************************************************************
 *	
 ******************************************************************
 */
//#define MYUBRR F_CPU/16/BAUD-1 	/* �ړIUBRR�l */


uchar USART_txbuf[TXBUF_SIZE];// �q�r�Q�R�Q�b���M�o�b�t�@.
uchar USART_txptr;			   // 
uchar USART_txend;			   // 

uchar USART_rxbuf[RXBUF_SIZE];	// ��MFIFO.
uchar USART_rxptr;				// FIFO�������ݒn�_.
 char USART_rxc;

uchar txp;
uchar rxp;

#define	CPUCLK	3579545
#define	CLKDIV	8

unsigned int	div_x;
static  char    weeks[]="SunMonTueWedThuFriSat";
extern	char	cmd_tick;			//���b����print����.
extern	int   	adj_count;			//�����l

#if	0
/*
//	-----------------------------------------------------------------------
//
//	ubrr = ( r / baud ) - 1 �����߂�.
//	����:
//	   baud: �{�[���[�g���̂���.
//	�ϐ�:
//	   r:    �萔�F USART�ɗ^����N���b�N��
//	   i:	 �{�[���[�g�̋t��. UBRR�ɃZ�b�g����l.
//	����:
//	   (��-1) => i
//	���l:
//		����Z�̓R�[�h�ʂ������̂ŁA�����Z���[�v�Ŏ���.
//	-----------------------------------------------------------------------
 */
int baud_calc(int baud)
{
	int r = CPUCLK/CLKDIV/8;	// 

	int i=-2;	// �����l=-1����,����Ɍ��ʂ� -1 ������.
	do {
		i++;
		r -= baud;
	}while(r>=0);
	return i;
}
#endif


//	baud_rate�� ubrr�ɃZ�b�g����l�ɕϊ�����.
//void baud_calc(int baud_l,uchar baud_h);	 // cpy.S

/******************************************************************
 *	�t�r�`�q�s�̏�����.
 ******************************************************************
 */
/******************************************************************
 *	�{�[���[�g�ݒ�.
 *  ����: int baud  �{�[���[�g
 ******************************************************************
 */
//static 
void USART_Init(int baud)
{
	uchar csrc=0x0e;	// 8N2
	int brr;

//	brr = baud_calc(baud);
	brr=5;	// (55930 / 9600) - 1 �̋ߎ��l; 55930��CPU�N���b�N/8== (3.579545MHz/8����)/8.

	UBRRH = brr>>8; 	/* �ްڰĐݒ�(����޲�) */
	UBRRL = brr;     	/* �ްڰĐݒ�(�����޲�) */
	UCSRC = csrc;		/* �ڰь`���ݒ�(8�ޯ�,2�į�� �ޯ�) */
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(1<<UDRIE);      /* ����M���� | RX���荞�݋��� */
	UCSRA = (1<<U2X);	  // �v���X�P�[���� 1/16 �� 1/8 �ɕύX.

	USART_rxptr = 0;
	USART_txptr = 0;
	USART_txend = 0;
}

/******************************************************************
 *	�f�o�b�O�p.
 ******************************************************************
 */
void USART_SendDirect(char c)
{
	if ( !(UCSRA & (1<<UDRE)) ) {
		return ;	/* 2:���M�ޯ̧�󂫑ҋ@ */
	}
    UDR = c; 		/* ���M */
}
char USART_RecvCh(void)
{
	return USART_rxc;
}

void putch1(uchar ch)
{
	USART_txbuf[txp++]=ch;
}
void putflush(void)
{
	USART_txend=0;
	USART_txptr=0;

	USART_txend=txp;
	USART_SendTask();
}
void putnum1(uchar num)
{
	putch1('0'+num);
}
void putnum2(uchar num)
{
	char n=0;
	while(num>=10) {
		num-=10;
		n++;
	}
	putnum1(n);
	putnum1(num);
}

void putweek(uchar week)
{
	char *p=&weeks[week*3];
	char i;
	for(i=0;i<3;i++) {
		putch1(*p++);
	}
}

void putcrlf()
{
	putch1(0x0d);
	putch1(0x0a);
}
void USART_Tick(void)
{
	txp = 0;

	putweek(cur.week);
		putch1(' ');
	putnum2(cur.hour);
		putch1(':');
	putnum2(cur.minute);
		putch1(':');
	putnum2(cur.sec);

	putcrlf();
	putflush();
}

void print_alarm(void)
{
	txp = 0;

		putch1(' ');
	putnum2(alarm.hour);
		putch1(':');
	putnum2(alarm.minute);
		putch1(':');
	putnum2(alarm.sec);

	putcrlf();
	putflush();
}

void USART_prompt(void)
{
	txp = 0;
	putcrlf();
	putch1('>');
	putflush();
}

char spskip(void)
{
	char c;
	while(1) {
		c=USART_rxbuf[rxp];
		if(c!=' ') return c;
		rxp++;
	}

}
char getch1(void)
{
	char c;
	c=USART_rxbuf[rxp];
	if(c!=0x0d) rxp++;
	return c;
}
char getnum1(void)
{
	char c=getch1();
	c -= '0';
	return c;
}
char getnum2(void)
{
	uchar c1,c2;
	c1 = getnum1();
	if(c1<10) {
		c2 = getnum1();
		if(c2<10) {
			return c1*10 + c2;
		}else{
			rxp--;
			return c1;
		}
	}
	return -1;
}
void set_time(void)
{
	char c;
	spskip();
	c = getnum2();if(c<0) return;

	if(getch1()!=':')return;

	cur.hour = c;

	c = getnum2();if(c<0) return;
	cur.minute = c;

	cur.sec   = 0;

	c = getch1(); if(c!=':')return;
	c = getnum2();if(c<0) return;
	cur.sec   = c;

}
void set_alarm(void)
{
	char c;
	spskip();
	c = getnum2();if(c<0) return;
	if(getch1()!=':')return;

	alarm.hour = c;

	c = getnum2();if(c<0) return;
	alarm.minute = c;

	alarm.sec   = 0;
	c = getch1(); if(c!=':')return;
	c = getnum2();if(c<0) return;
	alarm.sec   = c;

}
void set_week(void)
{
	spskip();
	char c = getnum1();
	if(getch1()!=':')return;

	if(c<7) {
		cur.week = c;
		USART_Tick();
	}
}
int div10(unsigned int d)
{
	uchar i;
	uchar rc=0;
	for(i=0;i<10;i++) {
		if(	div_x >= d) {
			div_x -= d;
			rc++;
		}
	}
	return rc;
}
void putn10(unsigned int d)
{
	putnum1(div10(d));
}
void print_adjust(void)
{
	txp = 0;
	div_x=adj_count;
	putn10(10000);
	putn10(1000);
	putn10(100);
	putn10(10);
	putnum1(div_x);
	putcrlf();
//	putnum2(alarm.hour);
//	putcrlf();
	putflush();
}

void set_adjust(void)
{
	char c;
	spskip();
	c = getnum1();if(c<0) return;
	div_x = c;
	while(1) {
		c = getnum1();if(c<0) break;
		div_x = div_x * 10 + c;
	}
	adj_count = div_x;
}
void USART_cmdproc(void)
{
	uchar c;
	if(USART_RecvCh() != 0x0d) return;

	USART_rxc=0;

	rxp=0;

#if	ASTERISK_GUARD
	c=getch1();
	if(c!='*') {
		USART_prompt();	
		USART_rxptr = 0;	//���̂P�s���͂��s�Ȃ�.
		return;
	}
#endif

	c=getch1();	//	c = USART_rxbuf[rxp++];
	if(c=='s') {
		set_time();
		USART_Tick();
	}
	if(c=='a') {
		set_alarm();
		print_alarm();
	}
	if(c=='j') {
		set_adjust();
		print_adjust();
	}
	if(c=='w') {
		set_week();
	}
	if(c=='p') {
		USART_Tick();
	}
	if(c=='t') {
		cmd_tick ^= 1;
	}
	
	USART_rxptr = 0;	//���̂P�s���͂��s�Ȃ�.
}

/******************************************************************
 *	�q�r�Q�R�Q�b���M�^�X�N.
 ******************************************************************
 */
//static 
void USART_SendTask(void)
{
	if(	USART_txptr < USART_txend ) {	//���M�o�b�t�@�Ƀf�[�^���c���Ă���Ȃ�,
		//
		//	�P�������M���s��.
		//
		if ( !(UCSRA & (1<<UDRE)) ) {
			return ;	/* 2:���M�ޯ̧�󂫑ҋ@ */
		}
	    UDR = USART_txbuf[USART_txptr++];     			/* ���M */
	}
	return ;			/* 0:���M�o�b�t�@�� */
}

/******************************************************************
 *	USART ���M���荞��.
 ******************************************************************
 */
ISR(USART_UDRE_vect)
{
	USART_SendTask();
}


/******************************************************************
 *	USART ��M���荞��.
 ******************************************************************
 */
ISR(USART_RX_vect)
{
	if( UCSRA & (1<<RXC) ) {					// ��M�����H .
		uchar rxchr = UDR;    					// ��M�f�[�^�擾.
		USART_rxbuf[USART_rxptr] = rxchr;		// FIFO�̏������݃|�C���^.
		USART_rxc = rxchr;
		USART_SendDirect(rxchr);

		// FIFO��t�Ȃ�A�|�C���^���X�V���Ȃ�.
		if(	USART_rxptr < (RXBUF_SIZE-1)) {
			USART_rxptr++;
		}
	}
}
/******************************************************************
 *	USART��M���荞��.�i�G�R�[�o�b�N�e�X�g�j
 ******************************************************************
 */
#if	0
ISR(USART_RX_vect)
{
	if( UCSRA & (1<<RXC) ) {					// ��M�����H .
		uchar rxchr = UDR;    					// ��M�f�[�^�擾.
		USART_SendDirect(rxchr);
	}
}
#endif

/******************************************************************
 *	
 ******************************************************************
 */
