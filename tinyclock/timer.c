/**********************************************************************
 *	16bit TIMER1 ���g�p�������荞�݃n���h���[.
 **********************************************************************
 *	timer_init()
 *	timer_wait()
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "task.h"
#include "timer.h"
#include "usart.h"

volatile unsigned int   timer_count;			//�N�����Ă���̎���.(tick)
		 unsigned int   adj_count;			//�����l

char	trigger_flag;				//�A���[�����Ԃ�������1.
char	cmd_tick = 0;				//���b����print����.

#define	TEST			0			//1�ɂ���Ɩ����O�b��POWER_ON����.
#define	ONLY_WEEKDAY	1			//�y����POWER_ON���Ȃ�.

//volatile uchar timer_tick;			//�^�C�}�[���荞�ݔ����t���O.


Time cur;
Time alarm;
uchar sec10;	// 0..9

/**********************************************************************
 *	������.
 **********************************************************************
 */
void timer_init(int cnt)
{
	cur.week=1;
	cur.hour=9;
	cur.minute=0;
	cur.sec=0;

	alarm.hour=0;
	alarm.minute=45;
	alarm.sec=61;			// �ݒ肷��܂Ŗ����ɂ���.

	timer_count = 0;
	adj_count = 10000;		// ���� 1/10000 �����x���悤�ɂ���.
//	adj_count = 0xffff;		// 1/65535 �����x�点��.

	TCCR1A = 0b00000011;	// Port�o�͂�off
	TCCR1B = 0b00011001;	// WGM=1111 (����PWM) CS1=001 (CLK/1)

	OCR1AH = (cnt >> 8);
	OCR1AL =  cnt;

#ifdef	__AVR_ATtiny2313__
	TIMSK = 0b10000000;		// ATtiny2313 : TOIE1=1 (Overflow Interrupt)
#else
	TIMSK1 = 0b00000001;	// ATmega88   : TOIE1=1 (Overflow Interrupt)
#endif
}

#if	0
/**********************************************************************
 *	���Ɋ��荞�ނ܂ő҂�.
 **********************************************************************
 */
uchar timer_wait(void)
{
	while(timer_tick == 0) { /* nothing */ };
	timer_tick --;
	return timer_tick;
}
/**********************************************************************
 *
 **********************************************************************
 */
int timer_gettime(void)
{
	return timer_count;
}
#endif
/**********************************************************************
 *	timer : ���ݎ������X�V����.
 **********************************************************************
 */
void add_cur_time()
{
	// LED��Z���_��������.
#if	1
	if((cur.msec == 0)&&(sec10==0)) {
			PORTB &= ~0x08;
	}else{
		PORTB |=  0x08;
	}
#endif

	cur.msec++;		// 0..9
	if(	cur.msec < 100) return;
	cur.msec = 0;



	if(	cmd_tick ) {
		USART_Tick();
	}

#if	1
	sec10++;
	if(	sec10 >= 10) {sec10=0;}
#endif

	cur.sec++;
	if(	cur.sec < 60) return;

	cur.sec = 0;
	sec10 = 0;
	cur.minute++;
	if(	cur.minute < 60) return;

	cur.minute = 0;
	cur.hour++;
	if(	cur.hour < 24) return;

	cur.hour = 0;
	cur.week++;
	if(	cur.week < 7) return;

	cur.week = 0;
}

/**********************************************************************
 *	timer : �A���[�������ƍ��v�����ꍇ�̏���.
 **********************************************************************
 */
void trigger_alarm()
{
	PORTB |= 0x04;
	trigger_flag=1;
}

/**********************************************************************
 *	timer : �A���[�������ƍ��v���Ȃ��ꍇ�̏���.
 **********************************************************************
 */
void trigger_alarm_stop()
{
	if(	trigger_flag ) {
		trigger_flag=0;
		PORTB &= ~0x04;
	}
}
/**********************************************************************
 *	timer : �A���[�������Ɣ�r����.
 **********************************************************************
 */
int compare_alarm()
{
#if	TEST
	if(cur.sec    != alarm.sec)    return 0;
	return 1;
#else
	if(cur.hour   != alarm.hour)   return 0;
	if(cur.minute != alarm.minute) return 0;
	if(cur.sec    != alarm.sec)    return 0;
//	if(cur.msec   != alarm.msec)   return 0;

#if	ONLY_WEEKDAY				//�y����POWER_ON���Ȃ�.
	if(cur.week == 0) return 0;	// Sun
	if(cur.week == 6) return 0;	// Sat
#endif

	return 1;
#endif
}

#if	0
/**********************************************************************
 *	timer : �����̐i�ݕ���␳����.( 1 / adj_count �����x�点��.)
 **********************************************************************
 */
char add_adj_count()
{
	timer_count++;
	if(	timer_count >= adj_count) {
		timer_count=0;
		return 1;
	}
	return 0;
}
#endif
/**********************************************************************
 *	timer : ���荞�݃n���h���[.
 **********************************************************************
 */
ISR(TIMER1_OVF_vect)
{
	char adj=0,hit;

//	adj = add_adj_count();			//���ԕ␳����.
	timer_count++;
	if(	timer_count >= adj_count) {
		timer_count=0;
		adj=1;
	}

	if(adj==0) {
		add_cur_time();				// ���v�� 1/100 �b�����Z.
		hit = compare_alarm();		// �A���[�������Ƃ̔�r.
		if(hit) {
			trigger_alarm();		//  �A���[���炷.
		}else{
			trigger_alarm_stop();	//  �A���[���~�߂�.
		}
	}

	USART_cmdproc();
}


#if	0
ISR(TIMER1_OVF_vect)
{
	timer_count ++;
	if(	timer_count==0) {
		timer_count_high++;
	}
	timer_tick ++;

	led_tick++;
	if(	led_tick>=100) {
		led_tick=0;
	}
}
#endif
/**********************************************************************
 *	
 **********************************************************************
 */
