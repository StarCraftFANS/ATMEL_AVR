/******************************************************************
 *	USART ����M���W���[��
 ******************************************************************
 *	�{�[���[�g�ݒ� (U2X���g�p)
	static	void USART_init(uchar baud_h);
 *
 *	RS232C����M�^�X�N.
	static	void USART_RecvTask(void);
	static	void USART_SendTask(void);
 */


/******************************************************************
 *
 ******************************************************************
 */
//#define F_CPU 12000000      	/* MCU�N���b�N���g�� */
//#define BAUD 9600             	/* �ړIUSART�{�[���[�g���x */
//#define MYUBRR F_CPU/16/BAUD-1 	/* �ړIUBRR�l */


#ifndef TXBUF_SIZE			// ���M�o�b�t�@�T�C�Y
#error Define TXBUF_SIZE in Makefile
#endif
#ifndef RXBUF_SIZE			// ��MFIFO�T�C�Y.
#error Define RXBUF_SIZE in Makefile
#endif

uchar rsbuf[TXBUF_SIZE];// RS232C���M�o�b�t�@.
// REGISTER uchar rsptr ASM("r7");	 // ���o���n�_ (0..3) �A4 �̎��͋�.

uchar USART_rxbuf[RXBUF_SIZE];	// ��MFIFO.
//REGISTER uchar USART_rxptr ASM("r7");	// FIFO�������ݒn�_.

REGISTER uint8_t RxRp ASM("r8");	//��M�f�[�^�ǂݍ���
REGISTER uint8_t RxWp ASM("r9");	//��M�f�[�^��������

REGISTER uint8_t TxRp ASM("r10");	//���M�f�[�^�ǂݍ���
REGISTER uint8_t TxWp ASM("r11");	//���M�f�[�^��������

//#define	 USART_default_baud  19200,0


//	baud_rate�� ubrr�ɃZ�b�g����l�ɕϊ�����.
void baud_calc(void);	 // asm.S

uchar USART_getc(void);
static void USART_putc(uchar);

/******************************************************************
 *	USART�̏�����.
 ******************************************************************
 */
/******************************************************************
 *	�{�[���[�g�ݒ�.
 *  ����: int baud_l  �{�[���[�g(����16bit)         [15:0]
 *      uchar baud_h  �{�[���[�g(��� 8bit) [23:16]
 ******************************************************************
 */
static void USART_Init(void)
{
	uchar csrc=0x0e;	// 8N2

	baud_calc();
//	UBRRH = brr>>8; 		/* �{�[���[�g�ݒ�(��ʃo�C�g) */
//	UBRRL = brr;     	/* �{�[���[�g�ݒ�(���ʃo�C�g) */
	UCSRC = csrc;		/* �t���[���`���ݒ�(8�r�b�g,2�X�g�b�v �r�b�g) */
//	UCSRB = (1<<RXEN)|(1<<TXEN);      /* ����M���� */
	UCSRA = (1<<U2X);	// �v���X�P�[���� 1/16 �� 1/8 �ɕύX.

//	rsptr = TXBUF_SIZE;

	RxRp = RxWp = 0;
	TxRp = TxWp = 0;
	UCSRB = (1<<RXEN)|(1<<TXEN) | (1<<RXCIE);      /* ����M���� & ��M�����݋���*/
}

#if 0	//@@kuga �폜
/******************************************************************
 *	RS232C��M�^�X�N.
 ******************************************************************
 */
static void USART_RecvTask(void)
{
	while( UCSRA & (1<<RXC) ) {					// ��M����? .
		uchar rxchr = UDR;    					// ��M�f�[�^�擾.
		USART_rxbuf[USART_rxptr] = rxchr;		// FIFO�̏������݃|�C���^.
		// FIFO��t�Ȃ�A�|�C���^���X�V���Ȃ�.
		if(	USART_rxptr < (RXBUF_SIZE-1)) {
			USART_rxptr++;
		}
	}
}

/******************************************************************
 *	RS232C���M�^�X�N.
 ******************************************************************
 */
static void USART_SendTask(void)
{
	if(	rsptr < TXBUF_SIZE ) {
		//���M�o�b�t�@�Ƀf�[�^���c���Ă���Ȃ�,
		//
		//	1�������M���s��.
		//
		if ( !(UCSRA & (1<<UDRE)) ) {
			return ;	/* 2:���M�o�b�t�@�󂫑ҋ@ */
		}
	    UDR = rsbuf[rsptr++];     			/* ���M */
	}else{
        if( usbAllRequestsAreDisabled() ) {
            usbEnableAllRequests();
        }
	}
	return ;			/* 0:���M�o�b�t�@�� */
}

#endif


static void USART_SendTask(void)
{
	if ( (TxWp!=TxRp) && (UCSRA & (1<<UDRE)) ) {
		UDR=rsbuf[TxRp++];
		TxRp &= (TXBUF_SIZE-1);
	   if( usbAllRequestsAreDisabled() ) {
			uchar used=(TxWp-TxRp) & (TXBUF_SIZE-1);
	 		if ( used<=(TXBUF_SIZE-HW_CDC_PACKET_SIZE-1) ) {		//���M�ɋ󂠂�?
	            usbEnableAllRequests();
	        }
		}
	}
}


static void USART_putc(uchar data)
{
	rsbuf[TxWp]=data;
	TxWp++;
	TxWp &= (TXBUF_SIZE-1);
}


