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

#define	CH_MAX	4

#include "keyname.h"
#include "tonecode.h"

/**********************************************************************
 *	define
 **********************************************************************
 */
#define	ESCAPE	0x1b		// [ESC]

#define	VOL		63			// ��������.


static	char tone_offset=0;
#define	PITCH_OFFSET	tone_offset

void quit();				// bootloader�ɖ߂�.

void main_break(uchar mode);
void sound_play(uchar ch,uint p,uchar v);
void sound_setdecay(uint p,uchar d);
void sound_decay();


void tone_send(uchar c)
{
	static uchar ch=0;
	{
		c += PITCH_OFFSET;	// 1 Octave UP,
		uchar *p = (uchar *) &pitch_table[c];
		uchar l = __LPM( p );
		uchar h = __LPM( p+1 );
		sound_play(ch,l+(h<<8),VOL);
		ch++;ch &= 3;
	}
}

void tone_decay(uchar c)
{
	c += PITCH_OFFSET;	// 1 Octave UP,
	uchar *p = (uchar *) &pitch_table[c];
	uchar l = __LPM( p );
	uchar h = __LPM( p+1 );
	sound_setdecay(l+(h<<8),4);
}

#define	PREFIX_E0		0x1
#define	PREFIX_E1		0x2			// [Pause]=E1 14,77,E1 14,F0 77
#define	PREFIX_F0		0x4

static  uchar prefix;				// Holds last scan code prefix
/**********************************************************************
 *	task
 **********************************************************************
 */
void main_task_tone(void)
{
	static uchar lastkey=0xff;

	uchar pre = prefix;			// ��O�̃R�[�h���g���R�[�h�������Ă���?
	uchar c = kbd_getcode();	// PS2 keyboard���� ASCII�R�[�h���擾����.
	uchar t;					// ���̍���.

	sleep_tsk(20);		//���̋N���� 20tick �҂�.

	sound_decay();

	if( c == 0 ) return;
	if( c == 0xe0 ) {			//�g���V�[�P���X.
		prefix |= PREFIX_E0;
		return ;
	}else if(c == 0xe1) {
		prefix |= PREFIX_E1;
		return ;
	}else if(c == 0xf0) {		//�����ꂽ��񂪌㑱.
		prefix |= PREFIX_F0;
		return ;
	}

	prefix = 0;
	if( pre & PREFIX_E0) {	// �g���V�[�P���X��SCAN CODE��MSB�𗧂Ăċ�ʂ���.
		c |= SKEY_EXTEND;
	}
	if( pre & PREFIX_E1) {	// �g���V�[�P���X��SCAN CODE��MSB�𗧂Ăċ�ʂ���.
		c |= SKEY_EXTEND;
	}
	if( pre & PREFIX_F0) {		// �����ꂽ�ꍇ�̏���.
		t = __LPM( &tonecode_table[c & 0x7f] );
		if( t ) {
			tone_decay(t);
		}
		if( lastkey == c) {
			lastkey = 0xff;	//�����ꂽ�̂ŁA�Ō�̃L�[�����Z�b�g.
		}
		return ;
	}



#ifndef	__AVR_ATtiny2313__
	if( c == SKEY_ESCAPE) {	// [ESC]�������ꂽ.
		quit();				// bootloader �ɖ߂�.
	}
	if( c == SKEY_F1) {		// [F1]�������ꂽ.
		main_break(1);		// main() �ɖ߂�. ==> ���[�h�ؑ�.
	}
#endif
	if( c == SKEY_UP) {		// [��]�������ꂽ.
		tone_offset++;
	}
	if( c == SKEY_DOWN) {	// [��]�������ꂽ.
		tone_offset--;
	}
	if( c == SKEY_SPACE) {	// [SPACE]�������ꂽ.
		sound_stop_all();	// �����~�߂�.
	}

	if( lastkey == c ) return;	// KEY REPEAT�͖���.
	else {lastkey = c;}

	if(c >= 0x80) return;

	t = __LPM( &tonecode_table[c & 0x7f] );
	if( t ) {
		tone_send(t);
	}

}

/**********************************************************************
 *	
 **********************************************************************
 */
