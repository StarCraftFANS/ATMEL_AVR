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

volatile uchar timer_tick;			//�^�C�}�[���荞�ݔ����t���O.

#define	TIMER_DA_OUT	1			//�^�C�}�[���荞�݂̃^�C�~���O��D/A�o�͂�L����.

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

#if	PS2KBD_USE_INTERRUPT
//  use PCINT8 hardhandler
#else
//	use TIMER1 softhandler
	kbd_int_handler();
#endif

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

