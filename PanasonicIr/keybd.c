/**********************************************************************
 *	�ԊO�������R���E�L�[�X�L����
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "task.h"
#include "timer.h"
#include "usart.h"

void	timer_init(int cnt);
void	led_on();
void	led_off();

void	delay10();
void	delay8(char n);
uchar	set_code(uchar *buf,uchar maker_id,uchar code_id);

#define	MAX_SCAN_COLUM	7

#define	LED_ON		PORTA
#define	LED_PORT	0x01		// ��LED
#define	ILD_PORT	0x02		// �ԊOLED

#define	PBMASK 0x0f				// PB0:3 �̂ݗL��.

uchar scan_cnt=0;				// 0�`4
uchar trig_key=0;				// �����ꂽ�L�[�ɑΉ�����index�ԍ�.(0�`55)
uchar trig_keycnt=0;			// �����ꂽ�L�[�̌�(������ĂȂ����0.�����ꂽ��1)
								// ���������ꂽ�Ƃ���2�ȏ�ɂȂ�̂ŁA���̏ꍇ�̓L�[���̂Ă�.
uchar old_key=0;				// �O��� key index�ԍ�.(0�`55)

uchar maker_id=0;
uchar maker_cnt=0;

uchar kb_stat[MAX_SCAN_COLUM];	// 7 x 8 = 56bit(key) ���̉����ꂽ���.
uchar ild_data[6];				// �ԊOLED���瑗�o����f�[�^��.


//----------------------------------------
#define	ILD_PIN		0x01				// �ԊOLED�̃|�[�g  PORTA.0
#define	ild_on()	PORTA |=  ILD_PIN;	// �ԊOLED��_��������. (���_��)
#define	ild_off()	PORTA &= ~ILD_PIN;	// �ԊOLED������������.
//----------------------------------------

#define	PN	16	//14	//18					// �ԊOLED (38kHz) �p���X�̐�. (1T������)

/*
static uchar pulldn_tbl[8]={
	0x01,0x02,0x04,0x08,
	0x10,0x20,0x40,0x80,
};
*/

/**********************************************************************
 *	���̃e�[�u���Ɠ��l.
 *	index i(i=0�`7)�ɑ΂��ADDRD �ɏ������ޒl���v�Z����. 
 **********************************************************************
 */
uchar pulldn_tbl(uchar i)
{
	uchar m=0x01;
	while(i) {
		m<<=1;
		i--;
	}
	return m;
}


/**********************************************************************
 *	n x 2ms�҂�.
 **********************************************************************
 */
void delay2ms(uchar n)
{
	uchar i;
	for(i=0;i<n;i++) {
		delay8(250);	//2mS
	}
}

/**********************************************************************
 *	LED(����) �_��
 **********************************************************************
 */
void LED_pulse(uchar m)
{
	uchar i;
	for(i=0;i<m;i++) {
		led_on();
		delay2ms(25);	// 0.5�b.
		led_off();
		delay2ms(25);	// 0.5�b.
	}
}

void print_maker()
{
	if(maker_id) {
		LED_pulse(2);
	}else{
		LED_pulse(10);
	}
}

/**********************************************************************
 *	�ԊO��: 1T ����38kHz�M���𑗏o
 **********************************************************************
 *	38KHz = 13.1uS on + 13.1uS off
 * CPU Clk= 500kHz = 2uS
 *		3 Cycle On + 10 Cycle Off : Total 13 Cycle = 26nS (38kHz)
000001ac <ILD_pulse>:
 1ac:	d8 9a       	sbi	0x1b, 0	; 27
 1ae:	00 00       	nop
 1b0:	00 00       	nop
 1b2:	d8 98       	cbi	0x1b, 0	; 27
 1b4:	00 00       	nop
 1b6:	00 00       	nop
 1b8:	00 00       	nop
 1bc:	00 00       	nop
 1be:	00 00       	nop
	...
 1c0:	81 50       	subi	r24, 0x01	; 1
 1c2:	a1 f7       	brne	.-24     	; 0x1ac <ILD_pulse>
 1c4:	08 95       	ret


 */
//	500kHz Version
void ILD_pulse(uchar m)
{
	do {
		ild_on();		// 2:
		asm("nop");		// 1:
		asm("nop");		// 1:

		ild_off();		// 2:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
	}while(--m);		// subi + bne = 3:
}

#if	0	// 1MHz Version
void ILD_pulse(uchar m)
{
	uchar i;
	for(i=0;i<m;i++) {
		ild_on();
		delay10();
		ild_off();
		delay10();
	}
}
#endif
/**********************************************************************
 *	�ԊO��: 1T ���̎��ԑ҂�.
 **********************************************************************
 */
void ILD_pause(uchar m)
{
	do {
		ild_off();		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:

		ild_off();		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
	}while(--m);		// subi + bne = 3:
}
/**********************************************************************
 *	�ԊO��: 1bit ���̃f�[�^�𑗏o
 **********************************************************************
 */
void ILD_bit(uchar bit)
{
	ILD_pulse(PN);
	ILD_pause(PN);
	if(bit) {			// bit�� 1 �̏ꍇ�̓|�[�Y���Ԃ��R�{���.
		ILD_pause(PN);
		ILD_pause(PN);
	}
}
/**********************************************************************
 *	�ԊO��: 1byte���̃f�[�^�𑗏o
 **********************************************************************
 */
void ILD_byte(uchar byte)
{
	uchar i;
	for(i=0;i<8;i++) {
		ILD_bit(byte & 1);
		byte >>= 1;
	}
}
/**********************************************************************
 *	�ԊO��: �v���A���u��(sync�M��)
 **********************************************************************
 */
void ILD_sync()
{
	ILD_pulse(PN*8);
	ILD_pause(PN*4);
}
/**********************************************************************
 *	�ԊO��: �|�X�g�A���u��(stop�M��)
 **********************************************************************
 */
void ILD_stop()
{
	ILD_pulse(PN);
	ILD_pause(PN*8);
	ILD_pause(PN*8);
}
/**********************************************************************
 *	�ԊO��: �����R���M��(6byte) ���M.
 **********************************************************************
 *	���o�f�[�^�́A���炩���� ild_data[6] �ɐݒ肳��Ă�����̂Ƃ���.
 */
void ILD_send(void)
{
	ILD_sync();	// Preamble
	uchar i;
	for(i=0;i<6;i++) {
		ILD_byte(ild_data[i]);	// 1byte���M
	}
	ILD_stop();	// Postamble
}

/**********************************************************************
 *	�L�[�X�L����: ������
 **********************************************************************
 */
void scan_init()
{
	trig_key=0xff;
	trig_keycnt=0;
	scan_cnt=0;
	return;
}

//
//	���[�J�[�؂�ւ��́A�I�t�^�C�}�[������.
//

/**********************************************************************
 *	�L�[�X�L����: �����ꂽ�L�[�l�ɑ΂���A�N�V����(�M�����M)�����s
 **********************************************************************
 */
void keybd_exec(uchar key)
{
#if	1
	if(key == 30) {	// OFF_TIMER�L�[.
		if(old_key==key) {
			maker_cnt++;
			if(	maker_cnt>=10) {	//��8�b OFF_TIMER�������ꂽ��
				maker_cnt=0;
				maker_id ^= 1;	//���[�J�[ID�؂�ւ�.
				print_maker();
				old_key=0;
			}
		}
		return;
	}
	if(old_key!=key) {maker_cnt=0;}
#endif

#if	1
	if(key == 24) {	// (POWER)
		if(old_key==key) {
			return;		// POWER�{�^���̓��s�[�g���Ȃ�.
		}
	}
#endif

	// �L�[�}�g���N�X�R�[�h����A�����R���R�[�h(6byte)�ɕϊ�.
	// �߂�lrc�́A�`���l���R�[�h(1-12,���̑�) 0�Ȃ疳���L�[.
	uchar rc=set_code(ild_data,maker_id,key);

	if(rc) {
		led_on();
		ILD_send();	// 6byte �ԊO�����o.
		led_off();
	}

	if(key == 24) {	// (POWER)
		ILD_send();	// POWER�{�^���͔O�̂��ߍđ�����.
	}
}

/**********************************************************************
 *	�L�[�X�L����: �P��(8bit)���̃f�[�^������
 **********************************************************************
 */
void keybd_trigger(uchar idx,uchar trig)
{
	uchar i,bit=0x01;
	for(i=0;i<8;i++,bit<<=1) {		// bit��LSB����MSB�܂Ō�������.
		if(trig & bit) {
			trig_key= idx + i*8;	//�}�g���N�X�ԍ�. 0�`39
			trig_keycnt++;			//���o���ꂽ�����L�[�̑���.
		}
	}
}

/**********************************************************************
 *	�L�[�X�L����: ������
 **********************************************************************
 */
void keybd_init(void)
{
	scan_init();
	old_key=0xff;
}


/**********************************************************************
 *	�L�[�X�L����: �莞����
 **********************************************************************
 */
void keybd_scan()
{
	uchar i,c,trig;

	//
	//	��� ��������Ƃ̏���.
	//
	if(	scan_cnt>=MAX_SCAN_COLUM) {
		if(	trig_keycnt==1 ) {			//���o�L�[���P�̏ꍇ�A
			keybd_exec(trig_key);		//���̃L�[�ɑΉ����鏈�������s����.
		}
		old_key=trig_key;
		scan_init();		//���̎���̂��߂̕ϐ�������.
		return;
	}
	//
	//	��������.
	//
	i=scan_cnt;
	{
//		DDRD=pulldn_tbl(i+2);	// �v���_�E�����ʐ���Low�o�͂�1�{�̂݃Z�b�g����.(PD2-6)
		DDRD=pulldn_tbl(i);		// �v���_�E�����ʐ���Low�o�͂�1�{�̂݃Z�b�g����.(PD0-6)

		delay8(2);
		scan_cnt++;

		c = PINB;c ^= 0xff;  c &= PBMASK;
		trig = (c & (kb_stat[i] ^ PBMASK));	//�g���K���o.

//		if(trig) keybd_trigger(i,trig);		//�����ꂽ����݂̂ɔ���.
		if(c)    keybd_trigger(i,c);		//������Ă���Ώ�ɔ���.

		kb_stat[i]=c;
	}
}
/**********************************************************************
 *	
 **********************************************************************
 */
