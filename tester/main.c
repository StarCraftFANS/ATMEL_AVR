/**********************************************************************
	���v
 **********************************************************************
	
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "config.h"
#include "task.h"
#include "taskdef.h"
#include "timer.h"


#define	R220_OFFSET	22			// AVR��PULLUP ������R �� 220���̒�R�덷�̘a��.
#define	R1K_OFFSET	22			// AVR��PULLUP ������R ��  1K���̒�R�덷�̘a��.
#define	R10K_OFFSET	150			// AVR��PULLUP ������R �� 10K���̒�R�덷�̘a��.

#define	PULLUP_OFFSET	4		// AVR��PULLUP �d���� Vcc�Ƃ̌덷digit (5V/1024)
#define	PROBE_OFFSET	1		// �v���[�u���̓�����R��.

#define	TEST			0
#define	EVERY_SEC_TEST	0
#define	EVERY_SEC		1

/**********************************************************************
 *	define
 **********************************************************************
 */
void quit(void);				// bootloader�ɖ߂�.
void led_task(void);			// LED�_���h���C�o�[
void led_set_number(int data);	// LED�\���f�[�^�̃Z�b�g.
void led_set_number2(int hh,int mm);	// LED�\���f�[�^�̃Z�b�g.
void led_set_pattern(uchar *data);// LED�\���p�^�[���̃Z�b�g.
void led_set_comma(uchar digit);// �����ʒu��2���ڂɍ��킹��.
void led_set_zero_supress(void);// ��ʌ��̃[��������.
void wait_usec(uchar n);
/**********************************************************************
 *	work
 **********************************************************************
 */
#define		   MAX_SAMPLE		16
#define		   MAX_SAMPLE_SFT	4
uint  ain_data[MAX_SAMPLE];
uchar ain_cnt = 0;
uint  ain_val;		// ���ϒl.

ulong Rval;			// ��R�l. �P�ʂ̓�.
uint  r_val;		// ��R�l. (4���ȓ�����)
uchar r_exp;		// ��R�l �����_�ʒu.
uchar r_mode;		// ��R����:���݂�PULL-UP���[�h(0=220,1=1K,2=10K)
uchar v_mode;		// 0=������ / 1=�d������ / 255=�N������.
uchar v_cnt0;

enum {
	MODE_R220=0,
	MODE_R1K =1,
	MODE_R10K=2,
	MODE_VOLT=3,
};

#define	PIN_R220	1
#define	PIN_R1K		6
#define	PIN_R10K	7

/**********************************************************************
 *	A/D�ϊ��̎��s.
 **********************************************************************
 */
static	int adc_exec(void)
{
	uchar c;
	uint  d;
	ADCSRA |= 0b01000000;	// ADSC (start)

	do {
		c = ADCSRA;
	} while(c & 0b01000000);
	//	�ϊ��I��
	d  = ADCL;
	d |=(ADCH<<8);
	return d;
}

/**********************************************************************
 *	A/D�ϊ� task.
 **********************************************************************
 */
static	void adc_task(void)
{
	uint d = adc_exec();
	ain_data[ain_cnt] = d;
	ain_cnt++;
	if(	ain_cnt >= MAX_SAMPLE) {
		ain_cnt = 0;
	}
}

/**********************************************************************
 *	PORTD �̎w��bit �� Hi ���x���o�͂ɂ���.
 **********************************************************************
 */
static	void set_HiD(uchar bit)
{
	uchar d = 1<<bit;
	PORTD |=  d;			// Hi
	DDRD  |=  d;			// PDx: H
}
/**********************************************************************
 *	PORTD �̎w��bit �� �I�[�v�� �ɂ���.
 **********************************************************************
 */
static	void set_HiZ(uchar bit)
{
	uchar d = ~(1<<bit);
	PORTD &= d;				//      Lo
	DDRD  &= d;				// PDx: Hi-Z
}

/**********************************************************************
 *	PULL UP ���[�h��؂�ւ���.
 **********************************************************************
 */
static	void set_pullup_mode(uchar mode)
{
	uchar pintbl[4]={PIN_R220,PIN_R1K,PIN_R10K,PIN_R10K};
	uchar i;

	r_mode = mode;

	for(i=0;i<3;i++) {
		if(i==mode) {
			set_HiD(pintbl[i]);
		}else{
			set_HiZ(pintbl[i]);
		}
	}
}

int get_pullup_R(void)
{
	uint regtbl[3]={
		  220+R220_OFFSET,
		 1000+R1K_OFFSET,
		10000+R10K_OFFSET,
	};
	return regtbl[r_mode];
}
/**********************************************************************
 *	PULL UP ���[�h�������Ő؂�ւ���.
 **********************************************************************
 */
#define	R10000 5000
#define	R1000  500
static void change_mode(void)
{
	uchar mode;
	if(r_exp == 255)   {mode = MODE_R10K; }else 
	if(Rval >= R10000) {mode = MODE_R10K; }else 
	if(Rval >= R1000 ) {mode = MODE_R1K;  }else 
    { mode = MODE_R220; }

	if(r_mode != mode) {
		set_pullup_mode(mode);
	}
}
/**********************************************************************
 *	�`/�c ���͂̕��ς����.
 **********************************************************************
 */
static void measure(void)
{
	uint d=0;
	uchar i;
	for(i=0;i<MAX_SAMPLE;i++ ) {
		d += ain_data[i];
	}
	d >>= MAX_SAMPLE_SFT;	// 16�Ŋ���.
	ain_val = d;
}
/**********************************************************************
 *	�`/�c ���͂̕��ς����.
 **********************************************************************
 */
static void measure_volt(void)
{
	Rval = ain_val;
	Rval = (500 * Rval ) / 1024;	// 5.000 V
	r_val = Rval;
	r_exp = 2;
}
/**********************************************************************
 *	��R�l�v�Z.
 **********************************************************************

                     Vin
	Rval = Rref * ---------
                  Vcc - Vin
 */
static void calculate(void)
{
	ulong Vin;
	ulong Vcc;
	ulong Rref;

	Rref = get_pullup_R();
	if( ain_val >=(1023 - PULLUP_OFFSET )) {
		r_val = 9999;
		r_exp = 255;
		return ;
	}
	Vin =    ain_val;
	Vcc = (1023 - PULLUP_OFFSET ) - ain_val ;

	Rval = ( Vin * Rref ) / Vcc;
	if(	Rval>=PROBE_OFFSET) {
		Rval-=PROBE_OFFSET;
	}

	r_exp = 0;				//�܂��A�����_�� 0 ���ڂƂ���.
	if(Rval < 1000) {		// 1k������.
		r_exp = 3;
		r_val = Rval;
		return;
	}
	
	if(Rval < 10000) {		// 10k������.
		r_exp = 2;
		r_val = (Rval/10);
		return;
	}
	
	if(Rval < 100000) {		// 100k������.
		r_exp = 1;
		r_val = (Rval/100);
		return;
	}
	
	if(Rval < 1000000) {		// 1000k������.
		r_exp = 0;
		r_val = (Rval/1000);
		return;
	}
	
		r_val = 9999;
		r_exp = 255;
		return ;
}
/**********************************************************************
 *	result		���ʕ\��.
 **********************************************************************
 */
static void disp_result()
{
	static	uchar minus_data[4]={
		0b01000000,	// -
		0b01000000,	// -
		0b01000000,	// -
		0b01000000,	// -
	};

	if(r_exp==255) {
		led_set_pattern(minus_data);
		return;
	}

	led_set_number(r_val);	// LED�ɐݒ肷��.
	led_set_comma(r_exp);	// �����ʒu��2���ڂɍ��킹��.
	led_set_zero_supress();	// ��ʌ��̃[��������.
}
/**********************************************************************
 *	task
 **********************************************************************
 */
static void volt_task(void)
{
	measure();
	measure_volt();
	disp_result();	//���ʂ�LED�ɃZ�b�g.
	sleep_tsk(48);	//128 mS�x��.
}
/**********************************************************************
 *	�d�����[�h�ֈڍs?
 **********************************************************************
 */
static void check_volt_mode(void)
{
	if(	v_mode == 255) {	//���[�h����?
		v_cnt0++;
		if(v_cnt0==3) {		//�N����R��ڂ̌v��.
			if(r_exp != 3) {	// 1k�������łȂ�.
				v_mode = 0;		// ohm_mode �ɌŒ�;
			}else{				// 1k���ȏ�.
				v_mode = 1;		// volt_mode�ɌŒ�;
				set_pullup_mode(MODE_VOLT);		// ohm����pullup���O��.
			}
		}
	}
}
/**********************************************************************
 *	task
 **********************************************************************
 */
static void ohm_task(void)
{
	measure();
	calculate();
	disp_result();	//���ʂ�LED�ɃZ�b�g.

	change_mode();
	check_volt_mode();
	sleep_tsk(48);	//128 mS�x��.
}

static char mod10(char n)
{
	while(n>=10) {
		n -= 10;
	}
	return n;
}

/**********************************************************************
 *	task
 **********************************************************************
 */
static void main_task(void)
{
//	led_set_number2(time_hh,time_mm);	// LED�ɐݒ肷��.
	led_set_number2(time_mm,time_ss);	// LED�ɐݒ肷��.

	if(time_tick < (2000/2)) {
		led_set_comma(2);	// �����ʒu��2���ڂɍ��킹��.
	}

}
/**********************************************************************
 *	task
 **********************************************************************
 */
static void sound_task(void)
{
	static uchar beep0;
	uchar beep=0;
	
	if(beep0) {
		PIND  = 0b00000001;			// PD1: flip
	}

#if	TEST
	// 10�b
	if(mod10(time_ss) == 0) {
#else
	// 10��
	if((time_ss == 0) && (mod10(time_mm)==0)) {
#endif
		if(time_mm) {
			if(time_tick & 0x100) {
				beep=1;
			}
		}
	}


#if	EVERY_SEC_TEST
	//���b
//	if((time_ss == 0)) 
	if(time_tick <2) {
			beep=1;
	}
#endif

#if	EVERY_SEC
	//���b
	if((time_mm == 0)&&(time_ss < 5)) 
	if(time_tick <2) {
			beep=1;
	}
#endif

	if((beep0==0) && (beep!=0)) {
		DDRD |= 0b00000001;			// PD0: out
	}
	if((beep0!=0) && (beep==0)) {
		DDRD &= ~0b00000001;			// PD0: in
		PIND &= ~0b00000001;			// PD0: LO
	}

	beep0=beep;
}

/**********************************************************************
 *	task
 **********************************************************************
 */
static void task_regist(void)
{
//	reg_tsk(ID_adc, adc_task);
	reg_tsk(ID_adc ,sound_task);
	reg_tsk(ID_main,main_task);
	reg_tsk(ID_led, led_task);
}

/**********************************************************************
 *	
 **********************************************************************
 */
static void port_init(void)
{
	//�|�[�g�������W�X�^�̏�����.
	PORTC=0;
	DDRC =0b11111110;		// PC0 == AIN0

	PORTD=0b00110000;		// PD7,PD6,PD1 == R10K,R1K,R220
	DDRD =0b00110000;

	PORTB=0;
	DDRB =0b00111111;

	// A/D�ϊ� MUX���W�X�^�ݒ�.
	ADMUX = 0b00000000;
	// A/D�ϊ� �d��On, A/D Clock = 1/64 ck
	ADCSRA = 0b10000110;
	// AIN�ɑ΂���f�W�^�����͋֎~.
	DIDR0  = 0b00000001;


#if	0	// ��r����g�p���鏉����.
	//A/D�ϊ��p���W�X�^�ݒ�.
	ADCSRA = 0b1000_0000;
	//�A�i���O��r�평����.
	ADCSRB = 0b0000_0000;
	//��d��1.1V�Ɣ�r / Timer1�ߊlOn. / ������G�b�W�ł̊��荞��.
	ACSR   = 0b0100_0110;
	//AIN1�ɑ΂���f�W�^�����͋֎~.
	DIDR1  = 0b0000_0010;
#endif


}
/**********************************************************************
 *	���C��
 **********************************************************************
 */
int	main(void)
{
	port_init();
						// �ȉ��A20MHz���̎���:
//	timer_init(41666-1);// 2083��S(480.0  Hz)�����Ŋ��荞��.
	timer_init(10000-1);//  500��S (2 KHz)�����Ŋ��荞��.
//	timer_init(256-1);	// 12.8��S(78.125kHz)�����Ŋ��荞��.
	sei();

	task_regist();
	set_pullup_mode(MODE_R1K);
	v_mode = 255;		// ���[�h����.
	v_cnt0 = 0;
	do {
		timer_wait();
		dispatch_tsk();
	}while(1);

	return 0;
}

/**********************************************************************
 *	
 **********************************************************************
 */
