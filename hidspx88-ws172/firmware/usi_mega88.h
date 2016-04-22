/*------------------------------------------------------------------------
 *		ATmega88�p usi_trans()�֐�.
 *------------------------------------------------------------------------
�֐��ꗗ:

 void  portInit(void)			�N����Port������;
 void  ispConnect()             SPI�ڑ�;
 void  ispDisconnect()          SPI�J��;
 uchar usi_trans(char cData)	SPI�]��1�o�C�g���s;

 *------------------------------------------------------------------------
 */

//	PORTB PIN Setting
#define	ISP_DDR		DDRB
#define	ISP_OUT		PORTB
//#define	ISP_DDR_DEFAULT	0x0f	/* PB7-4=in PB3-0=out */
//#define	ISP_OUT_DEFAULT	0x0b	/* PB7-4=0, ISP_RDY=0 */

//	PORTB PIN ASSIGN
#define	ISP_SCK		5			/* Target SCK */
#define	ISP_MISO	4			/* Target MISO */
#define	ISP_MOSI	3			/* Target MOSI */
#define	ISP_RST		2			/* Target RESET */
#define	ISP_RDY		1			/* Green LED */
#define	ISP_RED		0			/* RED LED */

/*------------------------------------------------------------------------
	SPI���䃌�W�X�^���}�X�^�[���[�h�ŏ���������.
    -d0 | fOSC/4  ... 3MHz
    -d1 | fOSC/8  ... 1.5MHz
    -d2 | fOSC/16 ... 750kHz
    -d3 | fOSC/32 ... 375kHz
    -d4 | fOSC/64 ... 187kHz
    -d5 | fOSC/64 ... 187kHz
    -d6 | fOSC/128...  93kHz

    -d7�ȍ~��-d6�Ɠ����ł�.  
    -d4��d5������������ɂȂ��Ă��܂����A�n�[�h�E�F�A�̎d�l�ł�.
    -d255 �́A8bit�̃I�[�o�t���[���ɂ��A6MHz�ݒ�ɂȂ�܂�.
 *------------------------------------------------------------------------
 */

void SPI_MasterInit(void)
{
//	ISP_DDR=(1<<ISP_MOSI)|(1<<ISP_SCK);  /*MOSI,SCK=�o�́A���͓��͂ɐݒ� */
//	SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);   /*SPI���A�}�X�^�A16�����ɐݒ� */
//	SPSR=(1<<SPI2X);

	uchar spr_10x=(wait+1);	// 0..6
	if(spr_10x>=8) spr_10x = 7;
	SPCR=(1<<SPE)|(1<<MSTR)|  ((spr_10x>>1) & 3);	// bit 2,1
	SPSR= spr_10x & 1;								// bit 0
}

//------------------------------------------------------------------------
//uchar SPI_MasterTransmit(char cData)
//	SPI�]�����P�o�C�g�����s����.

uchar usi_trans(char cData)
{
    SPDR=cData;                  /*�f�[�^���M�J�n*/
	while(!(SPSR&(1<<SPIF)));    /*�]�������܂őҋ@*/
    return SPDR;                 /*��M�f�[�^*/
}

//------------------------------------------------------------------------
void ispDelay()
{
	delay_10us(1);
}
//------------------------------------------------------------------------
void ispConnect() {
  /* all ISP pins are inputs before */
  /* now set output pins */
  ISP_DDR |= (1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI);

  /* reset device */
  ISP_OUT &= ~(1 << ISP_RST);   /* RST low */
  ISP_OUT &= ~(1 << ISP_SCK);   /* SCK low */

  /* positive reset pulse > 2 SCK (target) */
  ispDelay();
  ISP_OUT |= (1 << ISP_RST);    /* RST high */
  ispDelay();                
  ISP_OUT &= ~(1 << ISP_RST);   /* RST low */

  SPI_MasterInit();
}

//------------------------------------------------------------------------
static void ispDisconnect() {
  
  /* set all ISP pins inputs */
  ISP_DDR &= ~((1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI));
  /* switch pullups off */
  ISP_OUT &= ~((1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI));

  /* disable hardware SPI */
  SPCR = 0;
}

static	void ispSckPulse(void)
{
//	ISP_DDR=(1<<ISP_MOSI)|(1<<ISP_SCK);  /*MOSI,SCK=�o�́A���͓��͂ɐݒ� */
  /* disable hardware SPI */
  SPCR = 0;

	ISP_OUT |= (1 << ISP_SCK);		/* SCK high */
//	delay_10us(100);				//@@x  ispDelay(); -> 1ms
    ispDelay();
	ISP_OUT &= ~(1 << ISP_SCK);		/* SCK Low */
    ispDelay();

  SPI_MasterInit();
}

#if	0
static void	initForUsbConnectivity(void)
{
	uchar i = 0;
	usbInit();
	/* enforce USB re-enumerate: */
	usbDeviceDisconnect();	/* do this while interrupts	are	disabled */
	do{				/* fake	USB	disconnect for > 250 ms	*/
//		WDT_reset();
		_delay_ms(1);
	}while(--i);
	usbDeviceConnect();
	sei();
}
#endif

static void portInit(void)
{
#if	USE_WSN172_BOARD
	// ws#172
	PORTD |= (1<<PD5);					/* PD5�� USB D-�� pullup */
	DDRD = 0;    		    			/* all input */
	DDRB = ~USBMASK;        			/* all outputs except USB data */
	programmer_mode = 1;				//PIND & (1<<PD2);
#else
	// hand-made
	PORTD |= (1<<PD5);					/* PD5�� USB D-�� pullup */
	DDRD = ~USBMASK;        			/* all outputs except USB data */
	programmer_mode = 1;				//PIND & (1<<PD2);
#endif
}

