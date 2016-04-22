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
#include "ps2keyb.h"
#include "morse.h"

/**********************************************************************
 *	define
 **********************************************************************
 */
#define	ESCAPE	0x1b		// [ESC]
#define	F1		0xf1		// [F1]

void quit();				// bootloader�ɖ߂�.

static uchar main_mode = 0;	// 0:tone 1:morse
static uchar break_flag;
void main_task_tone(void);

/**********************************************************************
 *	message function
 **********************************************************************
 */
void main_break(uchar mode)
{
	main_mode = mode;
	break_flag = 1;
}
/**********************************************************************
 *	task
 **********************************************************************
 */
void main_task(void)
{
	if(morse_stat()==0) {			// �Ō��^�X�N����.
		uchar c = kbd_getchar();	// PS2 keyboard���� ASCII�R�[�h���擾����.
		if( c ) {					// �����L�[���͂�������.
			if( c == ESCAPE) {			// [ESC]�������ꂽ.
				quit();						// bootloader �ɖ߂�.
			}else if( c == F1) {		// [F1]�������ꂽ.
				main_break(0);				// ���Տ����ɖ߂�.
			}else{
				morse_send(c);		// ���[���X�����𔭐M����.(c=ASCII CODE)
			}
		}
	}
	sleep_tsk(20);		//20tick �҂�.
}


void task_regist(void)
{
	init_tsk();

	if(main_mode) {
		// ���[���X.
		reg_tsk(ID_main ,main_task);
		reg_tsk(ID_sound,sound_task);
		reg_tsk(ID_sub  ,morse_task);
	}else{
		// ����.
		reg_tsk(ID_main ,main_task_tone);
		reg_tsk(ID_sound,sound_task);
	}
}
/**********************************************************************
 *	���C��
 **********************************************************************
 */
int	main(void)
{
	kbd_init();
	sound_init();
	morse_init();
						// �ȉ��A20MHz���̎���:
//	timer_init(400-1);	// 20  ��S(50    kHz)�����Ŋ��荞��.
	timer_init(256-1);	// 12.8��S(78.125kHz)�����Ŋ��荞��.
	sei();

	while(1) {
		sound_stop_all();
		task_regist();

		break_flag=0;
		do {
			timer_wait();
			dispatch_tsk();
		}while(break_flag==0);
  	}

	return 0;
}

/**********************************************************************
 *	
 **********************************************************************
 */
