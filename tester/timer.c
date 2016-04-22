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

volatile int   timer_count;			//�N�����Ă���̎���.(tick)
volatile int   timer_count_high;	//�N�����Ă���̎���.(tick/65536)

volatile char  time_hh;				//����
volatile char  time_mm;				//����
volatile char  time_ss;				//����
volatile int   time_tick;			//����

volatile uchar timer_tick;			//�^�C�}�[���荞�ݔ����t���O.

#define	TIME_INTERVAL	2000		//2KHz

#define	TIMER_DA_OUT	0			//�^�C�}�[���荞�݂̃^�C�~���O��D/A�o�͂�L����.

#if		TIMER_DA_OUT				//�^�C�}�[���荞�݂̃^�C�~���O��D/A�o�͂�L����.
static	 uchar timer_daout;			//�^�C�}�[���荞�݂̃^�C�~���O��D/A�o��.
#endif


void kbd_int_handler(void);

/**********************************************************************
 *	������.
 **********************************************************************
 */
void timer_init(int cnt)
{
	timer_count = 0;

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
uchar timer_gettick(void)
{
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
/**********************************************************************
 *	timer : ���荞�݃n���h���[.
 **********************************************************************
 */
ISR(TIMER1_OVF_vect)
{
#if		TIMER_DA_OUT				//�^�C�}�[���荞�݂̃^�C�~���O��D/A�o�͂�L����.
	OCR0A = timer_daout;
#endif
	timer_count ++;
	if(	timer_count==0) {
		timer_count_high++;
	}
	timer_tick ++;

//	-------------
	time_tick++;
	if( time_tick >= TIME_INTERVAL ) {
		time_tick = 0;
		time_ss++;
		if( time_ss >= 60 ) {
			time_ss =0;
			time_mm++;
			if( time_mm >= 60 ) {
				time_mm =0;
				time_hh++;
				if( time_hh >= 24 ) {
					time_hh =0;
				}
			}
		}
	}

}
/**********************************************************************
 *	
 **********************************************************************
 */
void wait_usec(uchar n)
{
	do {
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
		asm("nop");
	}while(--n);
}
/**********************************************************************
 *
 **********************************************************************
 */

#if		TIMER_DA_OUT				//�^�C�}�[���荞�݂̃^�C�~���O��D/A�o�͂�L����.
void sound_out(uchar x)
{
	timer_daout = x;
}
#endif
/**********************************************************************
 *	
 **********************************************************************
 */

