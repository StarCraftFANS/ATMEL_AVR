/*------------------------------------------------------------------------
 *		ATtiny2313�p usi_trans()�֐�.
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
#define	ISP_DDR_DEFAULT	0x0f	/* PB7-4=in PB3-0=out */
#define	ISP_OUT_DEFAULT	0x0b	/* PB7-4=0, ISP_RDY=0 */

//	PORTB PIN ASSIGN
#define	ISP_SCK		7			/* Target SCK */
#define	ISP_MOSI	6			/* Target MISO */
#define	ISP_MISO	5			/* Target MOSI */
#define	ISP_RST		4			/* Target RESET */
#define	ISP_RDY		3			/* Green LED */
#define	ISP_RED		2			/* RED LED */

/* ------------------------------------------------------------------------- */
/* -----------------------------  USI Transfer  ---------------------------- */
/* ------------------------------------------------------------------------- */


//
//	wait:
//		0 =  3clk    2 MHz
//		1 =  4clk  1.5 MHz
//		2 =  9clk  666 kHz
//		3 = 21clk  285 kHz
//		4 = 33clk  181 kHz
//	   10 =129clk   46 kHz
//	   20 =249clk   23 kHz
//	   50 =609clk  9.8 kHz
//
//		2�ȏ�� 9 + (12 * wait) clk �iCALL-RET���̗p�����ꍇ�����l�j
//
static uchar usi_trans(uchar data){
	USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK);	//by kuga
	USIDR=data;
	USISR=(1<<USIOIF);
	if(wait==0) {
#if 1	// by kuga
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
	    ISP_OUT |= (1<<ISP_SCK); ISP_OUT &= ~(1<<ISP_SCK);
#else
		uchar CR0=(1<<USIWM0)|(1<<USICS1)|(1<<USITC);
		USICR=CR0;
		uchar CR1=(1<<USIWM0)|(1<<USICS1)|(1<<USITC)|(1<<USICLK);
		//�������1clk�Ȃ̂Ł������ nop�K�v.
					asm("nop");	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;	DLY_2clk();
		USICR=CR0;	DLY_2clk();	USICR=CR1;//DLY_2clk(); :else ��rjmp�ő�p.
#endif
	}else if(wait==1) {
		do{
			USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
		} while(!(USISR&(1<<USIOIF)));
	}else{
		do {
			uchar d=wait;		// 12clk * (wait-2)
			while(d != 2) {		// 1+1: cpi,breq
				delay_7clk();	// 3+4: call,ret, code �팸�̂��߁Acall-ret �𗘗p
				d--;			// 1+2: subi,rjmp
			}
			USICR=(1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
		} while(!(USISR&(1<<USIOIF)));
	}
	USICR=0;		/* SCK���|�[�g�ɖ߂��Ă��� by kuga */
	return USIDR;
}


#if INCLUDE_ISP_CMD

static	void ispConnect(void)
{
	/* all ISP pins are inputs before */
	/* now set output pins */
	ISP_DDR = (1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI) | ISP_DDR_DEFAULT;

	/* reset device */
	ISP_OUT &= ~(1 << ISP_RST);		/* RST low */
	ISP_OUT &= ~(1 << ISP_SCK);		/* SCK low */

	/* positive reset pulse > 2 SCK (target) */
	delay_10us(6);					//@@x  ispDelay();
	ISP_OUT |= (1 << ISP_RST);		/* RST high */
	delay_10us(100);				//@@x  ispDelay(); -> 1ms
	ISP_OUT &= ~((1 << ISP_RST)|(1 << ISP_RDY));	/* RST low , LED Low*/
}

static	void ispDisconnect(void)
{
	/* set all ISP pins inputs */
	//ISP_DDR &= ~((1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI));
	ISP_DDR = ISP_DDR_DEFAULT;		/* PB7-4=in PB3-0=out */

	/* switch pullups off */
	//ISP_OUT &= ~((1 << ISP_RST) | (1 << ISP_SCK) | (1 << ISP_MOSI));
	ISP_OUT = 0b00001011;			// PB7-4=Hi-Z,  PB3-0=Hi, LED(PWR)������
}

static	void ispSckPulse(void)
{
	ISP_OUT |= (1 << ISP_SCK);		/* SCK high */
	delay_10us(100);				//@@x  ispDelay(); -> 1ms
	ISP_OUT &= ~(1 << ISP_SCK);		/* SCK Low */
}
#endif


static void portInit(void)
{
#if STARTUP_DELAY
	/* Power ON delay (300ms) */
	for (wait=0; wait<150; wait++) {
		delay_10us(200);		// 2ms
	}
#endif
#if USB_IO_MODE_ENABLE					/* USB-IO�ł́A����������PORTB����̓��[�h */

	/* PD5�� USB D-, PD2��mode�W�����p�p pullup */
	PORTD |= ((1<<PD6)|(1<<PD5)|(1<<PD2)|(1<<PD1)|(1<<PD0));
	delay_10us(10);						/* �l�����肷��܂ŁA100u�b�҂� */

	/* all outputs except (USB data, PD2, PD1, PD0) */
	DDRD = ~(USBMASK | (1<<PD6)| (1<<PD2)|(1<<PD1)|(1<<PD0));

	programmer_mode = PIND & (1<<PD2);
	if (programmer_mode) {				/* PD2�W�����p�����̎��ɂ́AAVR���C�^�p�̐ݒ�ɂ��� */
		ISP_OUT = ISP_OUT_DEFAULT;		/* PB3 LED(PWR) ON, PB2 LED(ACC) OFF */
		ISP_DDR = ISP_DDR_DEFAULT;		/* PB7-4=in, PB3-0=out */

#ifdef	tn2313
		TCCR0A =(1<<COM0A0)|(1<<WGM01);	//  COM0A=01 : ��r��v��OC0A�g�O��.
										//  WGM210 = 010 : ��r��v�^�C�}�[�A�J�E���^�N���A.
		TCCR0B = (1<<CS00);				//  fclk 1/1 ����.
										//  TIMER0 �̎�����ݒ肷��.
		OCR0A = 5;						//  (12MHz / 2) / (5+1) = 1MHz
#endif

	}
#else
	PORTD |= (1<<PD5);					/* PD5�� USB D-�� pullup */
	DDRD = ~USBMASK;        			/* all outputs except USB data */
	ISP_OUT = ISP_OUT_DEFAULT;			/* PB3 LED(PWR) ON, PB2 LED(ACC) OFF */
	ISP_DDR = ISP_DDR_DEFAULT;			/* PB7-4=in PB3-0=out */
#endif
}
