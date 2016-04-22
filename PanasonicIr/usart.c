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
void putcrlf()
{
	putch1(0x0d);
	putch1(0x0a);
}
void USART_prompt(void)
{
	txp = 0;
	putcrlf();
	putch1('>');
	putflush();
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
