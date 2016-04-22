/* asm.S */

#define BAUDRATE_H ( ( ( F_CPU / 16 ) >> 16 ) & 0xff )
#define BAUDRATE_M ( ( ( F_CPU / 16 ) >> 8 ) & 0xff )
#define BAUDRATE_L ( ( F_CPU / 16 ) & 0xff )

/*
 *	void memCopy(*d,*s,len);		// SRAM to SRAM copy
 *
 *	����: len��1�`127�܂�.	(tiny2313��SRAM��128�o�C�g�����Ȃ��̂�.)
 *		  len <= 0 �̂Ƃ��͉������Ȃ�.
 */

/*************************************************************/
	.text
	.global	memCopy
	.type	memCopy, @function

memCopy:
	cp r1,r20	/* __zero_reg__,r20 */
	brge .L4

	movw r26,r24
	movw r30,r22

.L3:
	ld		r24,Z+
	st		X+,r24
	subi	r20,1
	brne	.L3
.L4:
	ret


#include <avr/io.h>		//@@kuga


/*************************************************************/
/*
 *	void  baud_calc(int baud_l,uchar baud_h);
 *
 *	�{�[���[�g���� bbr�̒l�����߂�.
 *	UBRRL,UBRRH�ɐݒ肷��.
 *
 */
	.global	baud_calc
	.type	baud_calc, @function
//	-----------------------------------------------------------------------
//
//	ubrr = ( r / baud ) - 1 �����߂�.
//	����:
//	   baud:  r22:r25:r24  17bit����. �{�[���[�g���̂���.(r22�͉���1bit�̂ݎg�p)
//	�ϐ�:
//	   r:     r22:r21:r20  24bit����  �萔�F USART�ɗ^����N���b�N��/2.
//	   i:	      r19:r18  16bit����. �{�[���[�g�̋t��. UBRR�ɃZ�b�g����l.
//	����:
//	   (��-1) => i �� UBRR �ɏ�������Ŗ߂�(void)
//	-----------------------------------------------------------------------

baud_calc:
// �p�����[�^���o�b�t�@�[�������[���璼�ڈ������悤�ɕύX�i�R�[�h�G���A�팸�j
	ldi	r30,lo8(modeBuffer)
	ldi	r31,hi8(modeBuffer)
	ldd	r24,Z+0
	ldd	r25,Z+1
	ldd	r22,Z+2

	// 24bit ��baud�� >>1 ����.
	lsr r22
	ror r25
	ror r24

	// r = 12000000/8/2; 	USART�ɗ^����N���b�N��/2.(U2X=1�̂Ƃ�)
	// 0x0f4240 = 16000000/8/2 = 1000000
	ldi r20,BAUDRATE_L
	ldi r21,BAUDRATE_M
	ldi r22,BAUDRATE_H

	// i = (-2);		�ȉ��̃��[�v�ł�i�̏����l�� (-1).
	ldi r18,lo8(-2)	//  �A���A���ʂ����Ƃ�-1����̂ŁA������(-2)��^����.
	ldi r19,hi8(-2)

	// i = (r/b) �̊���Z�������Z���[�v�Ŏ��s����.
.L8:
	// i++;
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))

	// r = r - b;	   24bit�̈����Z�����s.
	sub r20,r24
	sbc r21,r25
	sbc r22,r1		// baud��16bit�Ȃ̂� bit23:16 �� �[���Œ�.
	brcc .L8		// �܂�������.
.L9:
	out _SFR_IO_ADDR(UBRRH),r19
	out _SFR_IO_ADDR(UBRRL),r18

	ret

#if	0	/* ����͎g��Ȃ� */

#define	int24 long

int baud_calc(int baud)
{
	int24 r = 12000000/8/2;
	baud /= 2;
	baud &= 0xffff;

	int i=-2;	// �����l=-1����,����Ɍ��ʂ� -1 ������.
	do {
		i++;
		r -= baud;
	}while(r>=0);
	return i;
}
#endif

;*****
;REGISTER uint8_t RxRp ASM("r8");	//��M�f�[�^�ǂݍ���
;REGISTER uint8_t RxWp ASM("r9");	//��M�f�[�^��������
;REGISTER uint8_t TxRp ASM("r10");	//���M�f�[�^�ǂݍ���
;REGISTER uint8_t TxWp ASM("r11");	//���M�f�[�^��������
;*****

	.global	USART_getc
	.type	USART_getc, @function

//
//  �N���`�J����?xWp,?x,Rp�����荞�݋֎~�ɂ����X�V���邽�߂�
//	�A�Z���u���֐��Ƃ���
//

USART_getc:
	mov	r25,r8
	ldi	r30,lo8(USART_rxbuf)
	ldi	r31,hi8(USART_rxbuf)
	add	r30,r8
;	adc	r31,r1		//ram��128�����Ȃ��̂ŌJ��オ��Ȃ�
	ld	r24,Z
	inc	r25
	andi	r25,(RXBUF_SIZE-1)
	mov	r8,r25
	eor	r25,r25		//uchar��Ԃ��̂ɕK�v���H
	ret

	.global	USART_putc
	.type	USART_putc, @function

#if 0	//	���M�����݂���߂��̂ł������߂�
//
//	���M�̋󂫂��Ȃ��ꍇ�͂����A��悤�ɕύX���ׂ����H�H
//	���MBUFF��8�o�C�g�����Ƃ�Ȃ��AUSB�p�P�b�g�T�C�Y�Ɠ���Ȃ̂�
//	�����I�Ȑ��䂪�ł��Ȃ��Arsbuf 16�o�C�g�ɕύX����ƃX�^�b�N���s��������
//
USART_putc:
	mov	r25,r11
	inc	r25
	andi	r25,0x0F	//(TXBUF_SIZE-1)
.L5:
	cp	r10,r25	//���M�o�b�t�@�[�̋󂫊m�F
	breq	.L5		//�󂫖����󂫂��ł���܂ő҂�
	ldi	r30,lo8(rsbuf)
	ldi	r31,hi8(rsbuf)
	add	r30,r11		//TxWp
;	adc	r31,r1		//ram��128�����Ȃ��̂ŌJ��オ��Ȃ�
	st	Z,r24
	mov	r11,r25
	sbi	_SFR_IO_ADDR(UCSRB),UDRIE
	ret

//
//
//USB�̊����݂�j�Q���Ȃ��悤,�����݋֎~���Ԃ͋ɗ͒Z������
//���G���g�����g�ł͂Ȃ��̂Ŏ��g�̊����݂��֎~�������Ŋ��荞�݋���
//
#endif

	.text
    .global USART_RX_vect
    .type   USART_RX_vect, @function


USART_RX_vect:
	cbi	_SFR_IO_ADDR(UCSRB),RXCIE
	out	_SFR_IO_ADDR(GPIOR0),r18
	sei			//�ۗ������݂������Ă�sei�̎��̖��߂͊����܂ꂸ���s�����
	in	r18,_SFR_IO_ADDR(UDR)	//�f�[�^�����܂ł͊����݋֎~�Ŏ��s
	in	r2,_SFR_IO_ADDR(SREG)	//r2=SREG�Z�[�u���W�X�^
;	push	r30
;	push	r31
	out	_SFR_IO_ADDR(GPIOR1),r30	//�X�^�b�N���[���Ȃ�Ȃ��悤
	out	_SFR_IO_ADDR(GPIOR2),r31	//IO�Ƀ��W�X�^�ۑ�
	ldi	r30,lo8(USART_rxbuf)
	ldi	r31,hi8(USART_rxbuf)
	add	r30,r9		//RxWp
;	adc	r31,r1		//ram��128�����Ȃ��̂ŌJ��オ��Ȃ�
	st	Z,r18
	mov	r18,r9
	inc	r18
	andi	r18,(RXBUF_SIZE-1)
	cpse	r8,r18		//�o�b�t�@�[FULL�Ȃ�
	 mov	r9,r18		//�|�C���^�̍X�V�����Ȃ�(��M�f�[�^�͏㏑���Ŕj�������)

	in	r31,_SFR_IO_ADDR(GPIOR2)	;	pop	r31
	in	r30,_SFR_IO_ADDR(GPIOR1)	;	pop	r30

	in	r18,_SFR_IO_ADDR(GPIOR0)
	out	_SFR_IO_ADDR(SREG),r2
	cli	//���̎��_�Ŏ�M�����݂��ۗ������Ƃ܂���
	sbi	_SFR_IO_ADDR(UCSRB),RXCIE	//�ēx��M�����݂�����
	reti
;
#if 0		//���M�����ݎg�p���~�i���\���P�����Ȃ��j

	.text

	.global USART_UDRE_vect
	.type	USART_UDRE_vect, @function


USART_UDRE_vect:
	cbi	_SFR_IO_ADDR(UCSRB),UDRIE
	sei
	in	r3,_SFR_IO_ADDR(SREG)
	cp	r10,r11		//���M�f�[�^�����邩�m�F
	breq	.L6		//���M�f�[�^�Ȃ�
;	push	r30
;	push	r31
	out	_SFR_IO_ADDR(OCR0A),r30	//�X�^�b�N���[���Ȃ�Ȃ��悤
	out	_SFR_IO_ADDR(OCR0B),r31	//IO�Ƀ��W�X�^�ۑ�(�^�C�}�[0��r���W�X�^��]�p)
	ldi	r30,lo8(rsbuf)
	ldi	r31,hi8(rsbuf)
	add	r30,r10		//TxRp
;	adc	r31,r1		//ram��128�����Ȃ��̂ŌJ��オ��Ȃ�
	ld	r31,Z
	out	_SFR_IO_ADDR(UDR),r31
	mov	r31,r10
	inc	r31
	andi	r31,0x0F	//(TXBUF_SIZE-1)
	mov	r10,r31
	in	r31,_SFR_IO_ADDR(OCR0B)	;	pop	r31
	in	r30,_SFR_IO_ADDR(OCR0A)	;	pop	r30
	out	_SFR_IO_ADDR(SREG),r3
	cli	//�����܂Ŏ��s�̊ԂɊ����܂�Ă��̊Ԃɑ��M�͊������Ă��邩���m��Ȃ�
	sbi	_SFR_IO_ADDR(UCSRB),UDRIE
	reti
.L6:		//���M�����݋֎~�̂܂܏I��
	out	_SFR_IO_ADDR(SREG),r3
	reti

#endif




/*************************************************************/