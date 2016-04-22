/**********************************************************************
 *	PS2 Keyboard Library : Sample Main
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
#include "sound.h"

#define	CH_MAX	4

/**********************************************************************
 *	define
 **********************************************************************
 */
#define	ESCAPE	0x1b		// [ESC]

#define	VOL		63			// ��������.

void quit();				// bootloader�ɖ߂�.

void main_break(uchar mode);
void sound_play(uchar ch,uint p,uchar v);
void sound_setdecay(uint p,uchar d);
void sound_decay();


void tone_send(uchar c)
{
}

void tone_decay(uchar c)
{
}

/**********************************************************************
 *	task
 **********************************************************************
 */
void main_task_tone(void)
{
	sleep_tsk(20);		//���̋N���� 20tick �҂�.
}

/**********************************************************************
 *	
 **********************************************************************
 */
