/*******************************************************************************
 *			usbRS2313 :	  USB to RS232C	serial port	for	ATtiny2313
 *******************************************************************************
 *������:
 *
 *
 *
 *
 *
 *
 *
 *******************************************************************************
 */
#include "hardware.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/eeprom.h>
#include "usbdrv.h"


/*******************************************************************************
 *	Switches
 *******************************************************************************
 */
#define	OPTIMIZE_SIZE		(1)		// Enable global register assignment

#if	OPTIMIZE_SIZE
#define	REGISTER	register
#define	ASM( reg )	asm( reg )
#else
#define	REGISTER
#define	ASM( reg )
#endif


/*******************************************************************************
 *	deviceDescriptor / configDescriptor for CDC Device.
 *******************************************************************************
 */
#include "usbdesc.h"


/*******************************************************************************
 *	Static Work
 *******************************************************************************
 */

//EEPROM 0�Ԓn�Ɋ�����Ă��邱�Ƃ����҂����R�[�h�Ȃ̂Œ���
EEMEM uint8_t saved_mode[]={0x80, 0x25, 0,	0, 0, 0, 8};  /* default: 9600 bps,	8n1 */

uchar	modeBuffer[7];
// =	{0x80, 0x25, 0,	0, 0, 0, 8};  /* default: 9600 bps,	8n1	*/

REGISTER uchar	requestType     ASM("r4");
REGISTER uchar  sendEmptyFrame	ASM("r5");	// = 1;

#if	USE_DCD_REPORTING
REGISTER uchar	intr3Status     ASM("r6");
#endif

//	����:	register r7 �ȍ~�� usart.h ���Ŏg�p����.
//	*****************************************************************************


/*******************************************************************************
 *	Serial Port Control
 *******************************************************************************
 */
#include "usart.h"


//	����!! ����memCopy�� (char)len <= 0 �Ȃ牽�����Ȃ�.
void memCopy(uchar *d,uchar *s,uchar len);		// SRAM to SRAM copy


/* CDC class requests: */
enum {
	SEND_ENCAPSULATED_COMMAND =	0,
	GET_ENCAPSULATED_RESPONSE,
	SET_COMM_FEATURE,
	GET_COMM_FEATURE,
	CLEAR_COMM_FEATURE,
	SET_LINE_CODING	= 0x20,
	GET_LINE_CODING,
	SET_CONTROL_LINE_STATE,
	SEND_BREAK
};

/* defines for 'requestType' */
#define	REQUEST_TYPE_LINE_CODING	0	/* CDC GET/SET_LINE_CODING */

/* ------------------------------------------------------------------------- */

static void mode_read_eeprom(void)
{
	uint8_t *p=modeBuffer, i, ee_adr;

	i=7;
	ee_adr=0;	//ee_adr=(uint8_t)saved_mode;
	do {
		eeprom_busy_wait();
		EEAR = ee_adr++;
		EECR |= (1<<EERE);
		*p++=EEDR;
	} while (--i);
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

/******************************************************************
 *	�f�t�H���g�ȊO�̃R���g���[���]���������ɌĂяo�����G���g���[.
 ******************************************************************
 *	�x���_���N�G�X�g�������̓N���X���N�G�X�g�ɉ�������.
 *	����   :usbRequset_t *rq;      ���N�G�X�g�p�P�b�g�\����.
 *          �ڍׂ� usbdrv.h ���Q��.
 *			����rq->bRequest �ɃR�}���h�ԍ������邱�Ƃ�����.
 *	�߂�l :uchar         replyLen
 *			�o�C�g����Ԃ����ꍇ�� usbMsgPtr�ɃA�h���X�����Ă���.
 *			0xff ��Ԃ����ꍇ�� usbFunctionRead()���Ăяo�����.
 *
 */
uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t	*rq	= (void	*)data;
	uchar			rqType = rq->bmRequestType & USBRQ_TYPE_MASK;

	if(rqType == USBRQ_TYPE_CLASS){	   /* class	request	type */
		if(rq->bRequest	== GET_LINE_CODING){
			mode_read_eeprom();
			usbMsgPtr = modeBuffer;
			return 7;
//			requestType	= REQUEST_TYPE_LINE_CODING;
//			return 0xff;
		}
		if(rq->bRequest == SET_LINE_CODING){
			requestType	= REQUEST_TYPE_LINE_CODING;
			return 0xff;
		}

#	if USE_DCD_REPORTING
		if(rq->bRequest	== SET_CONTROL_LINE_STATE){
			/* Report serial state (carrier	detect). On	several	Unix platforms,
			 * tty devices can only	be opened when carrier detect is set.
			 */
			intr3Status	= 2;
		}
#	endif

#if	0	//kuga ���ꂪ�����rs232��host�ɑ��M���鏈����lock����??
		/*  Prepare bulk-in endpoint to respond to early termination   */
		if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_HOST_TO_DEVICE) {
			sendEmptyFrame  = 1;
		}
#endif
	}
	return 0;
}



#if	0	//@@kuga
/*******************************************************************************
 *	Control-In ���s���ɌĂ΂��.
 ******************************************************************
 *	�߂�l�́A�z�X�g�ɓ]������o�C�g��.(<= 8)
 *				0xff�̎���stall
 *	���ꂪ�Ăт������ɂ́A�܂� usbFunctionSetup() �̕Ԃ�l��
 *  0xff�łȂ���΂Ȃ�Ȃ�.
 */
uchar usbFunctionRead(uchar	*data, uchar len)
{
	if(requestType == REQUEST_TYPE_LINE_CODING){
		/* return the "virtual"	configuration */
		mode_read_eeprom();
		memCopy(data, modeBuffer, 7);
		return 7;
	}
	return 0;	/* error ->	terminate transfer */
}
#endif

/*******************************************************************************
 *	Control-Out ���s���ɌĂ΂��.
 *******************************************************************************
 */
uchar usbFunctionWrite(uchar *data,	uchar len)
{
	if(requestType == REQUEST_TYPE_LINE_CODING){
		uint8_t  i,ee_adr,*p;
		/* Don't know why data toggling	is reset when line coding is changed, but it is... */
		USB_SET_DATATOKEN1(USBPID_DATA1);	/* enforce DATA0 token for next	transfer */
		/* store the line configuration	so that	we can return it on	request	*/
		memCopy(modeBuffer, data, 7);

		// mode����EEPROM�ɕۑ�����
		//�W�����C�u�����͎g���Ȃ�		eeprom_write_block(modeBuffer,(void *)ee_adr,7);
		ee_adr=0; i=7; p=modeBuffer;
		do {
			eeprom_busy_wait();
			EECR=(0<<EEPM1)|(0<<EEPM0);
			EEAR=ee_adr++;
			EECR |= (1<<EERE);
			if (EEDR!=*p) {
				EEDR=*p;
				cli();		//EEMPE�Z�b�g����EEPE�Z�b�g�܂ł�4clk�ȓ��Ɏ��s����K�v������
				EECR |= (1<<EEMPE);
				sei();	//sei�̎��̖��߂͊����ݕۗ����ł����s�����
				EECR |= (1<<EEPE);
			}
			p++;
		} while (--i);
//		USART_Init( *((int*)modeBuffer) , modeBuffer[2] );
		USART_Init();
//		return 1;
	}
	return 1;	/* error ->	accept everything until	end	*/
}

/*******************************************************************************
 *	HOST->DEVICE:(Bulk-out) ���s���ɌĂ΂��.
 *******************************************************************************
 */
void usbFunctionWriteOut(uchar *data, uchar	len)
{
#if 1	//@@kuga
//	if (len) {
		do {
			USART_putc(*data++);
		} while (--len);

		len=(TxWp-TxRp) & (TXBUF_SIZE-1);	//�o�b�t�@�[�g�p��
 		if ( len>(TXBUF_SIZE-HW_CDC_PACKET_SIZE-1) ) {		////�c���p�P�b�g�T�C�Y��菬�����ꍇ
			usbDisableAllRequests();	//���̗v�����Ƃ߂Ă���
		}
//	}
#else
//========iruka============
	if (len) {
#if	1
		// �����M�f�[�^������Ȃ�A�����őS�����M����܂ő҂�.
		while( rsptr < TXBUF_SIZE ) {
	//		USART_RecvTask();
			USART_SendTask();
		}
#endif
		// ���M�o�b�t�@�ɗ��߂�.
		rsptr = 8 - len;
		memCopy(rsbuf+rsptr,data,len);
		USART_SendTask();
		if( rsptr < TXBUF_SIZE ) {	//���M�o�b�t�@����łȂ��Ȃ�A�t���[���䂷��.
			usbDisableAllRequests();
		}
	}
//=======iruka=============
#endif
}


/*******************************************************************************
 *	usbSetInterrupt :	�q�r�Q�R�Q��M�f�[�^���z�X�g�o�b�ɑ���.
 *******************************************************************************
 */
static	void setInterrupt_xfer(void)
{
//	static uchar buffer[HW_CDC_PACKET_SIZE];
#define buffer modeBuffer		//modeBuffer��]�p @@kuga

#if 1	//@@kuga
	uchar rxcnt=(RxWp-RxRp) & (RXBUF_SIZE-1);
	if((rxcnt > 0) || sendEmptyFrame){
		if (rxcnt) {
			uchar i,*p;
			if(rxcnt >= HW_CDC_PACKET_SIZE) {
				rxcnt = HW_CDC_PACKET_SIZE;	//�P��̓]���͂W�o�C�g�܂�.
			}
			p=buffer; i=rxcnt;
			do {
				*p++=USART_getc();
			} while (--i) ;
		}
		usbSetInterrupt(buffer,rxcnt);
		sendEmptyFrame=0;
		if(rxcnt==HW_CDC_PACKET_SIZE) {
			sendEmptyFrame=1;
		}
	}
#else
	uchar rxcnt = USART_rxptr;		//��M������.
	if((rxcnt > 0) || sendEmptyFrame){
		if(rxcnt >= 8) rxcnt = 8;	//�P��̓]���͂W�o�C�g�܂�.

		// ��M�σf�[�^��buffer�ɃR�s�[.
		memCopy(buffer,USART_rxbuf,rxcnt);

		// ��M�σf�[�^(max 8byte) ��USART_rxbuf[]����폜.
		USART_rxptr -= rxcnt;
		memCopy(USART_rxbuf,USART_rxbuf+8,USART_rxptr);	//�o�b�t�@���炵.

		/* send	an empty block after last data block to	indicate transfer end */
		usbSetInterrupt(buffer,rxcnt);
		sendEmptyFrame=0;
		if(rxcnt==8) {sendEmptyFrame=1;}
	}
#endif
}


/*******************************************************************************
 *	usbSetInterrupt3 :
 *******************************************************************************
 */
static	void setInterrupt3(void)
{
#if	USE_DCD_REPORTING
	/* We need to report rx	and	tx carrier after open attempt */
	if(intr3Status != 0	&& usbInterruptIsReady3()){
		static uchar serialStateNotification[8]	= {0xa1, 0x20, 0, 0, 0,	0, 2, 0};
		static uchar serialStateData[2]	= {3, 0};
		if(intr3Status == 2){
			usbSetInterrupt3(serialStateNotification, 8);
		}else{
			usbSetInterrupt3(serialStateData, 2);
		}
		intr3Status--;
	}
#endif
}

static void LED_Control()
{
#if	HW_USE_LED_CONTROL
#if 1	//@@kuga
	if (RxWp!=RxRp)
#else
	if(USART_rxptr)
#endif
	{
		PORTB &= ~(1<<HW_LED_PIN);
	}else{
		PORTB |=  (1<<HW_LED_PIN);
	}
#endif
}
/*******************************************************************************
 *
 *******************************************************************************
 */
static	void hardwareInit()
{
#if 1	// HIDaspx�̉�H�p.
	PORTD |= (1<<PD5) | (1<<PD0);		/* PD5 USB D- pullup  */
#else
	PORTD = 1;				// RxD Pullup
#endif

	DDRD = ~USBMASK;        /* all outputs except USB data */
	DDRB = 0x0f;			/* PB7-4=in PB3-0=out*/

	sendEmptyFrame	= 1;	//@@kuga

#if	USE_DCD_REPORTING
	intr3Status = 0;
#endif
}

/*
 *	*******************************************************************
 * 	��H�}�́A�g�h�c�������i���j�A�g�h�c�������̂��̂Ƌ��ʂł��B
 *	*******************************************************************
 */

/*******************************************************************************
 *	main Loop
 *******************************************************************************
 */
int	main(void)
{
	hardwareInit();
	usbInit();
	mode_read_eeprom();		//eeprom�ɕۑ����Ă���ݒ�������l�Ƃ���
	USART_Init();
//	USART_Init(USART_default_baud);
	sei();
	for(;;){	/* main	event loop */
		usbPoll();
//@@kuga		USART_RecvTask();
		USART_SendTask();
		LED_Control();
		if(	usbInterruptIsReady() ) {
	//		PORTB &= ~(1<<PB2) ;	//@@kuga for debug
			setInterrupt_xfer();		// ��M�f�[�^���z�X�g�o�b�ɓ]��.
	//		PORTB |= (1<<PB2) ;		//@@kuga for debug
		}
		setInterrupt3();				// EndPoint3: DCD_REPORTING
	}
	return 0;
}

