/**********************************************************************
 *	bootloadHID ���g�p����ꍇ�̍ċN�����[�`��
 **********************************************************************
 *	void quit()  : �A�v���P�[�V�������I�����āA�u�[�g���[�_�[�ɖ߂�.
 *		ATmega88 �Ɍ���.
 *		bootloadHID����N�������ꍇ�Ɍ���.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>


#ifndef	__AVR_ATtiny2313__

static	void (*usrfunc)();

void jump()
{
	usrfunc();
}

#endif

/**********************************************************************
 *	
 **********************************************************************
 */
void quit(void)
{
#ifndef	__AVR_ATtiny2313__

	PCICR  = 0;	// PCINT All Disable.
	PCMSK0 = 0;	// PCINT0  Mask off.
	PCMSK1 = 0;	// PCINT8  Mask off.
	PCMSK2 = 0;	// PCINT16 Mask off.

	TIMSK1 = 0;	// TOIE1=0 (Overflow Interrupt)

	cli();

	// ATmega88�ȊO�� bootloader�̃A�h���X�𒲐�����K�v������܂�.

	usrfunc	= (void*) 0xc00;	// 0x1800 / 2
	jump();
#endif
}

/**********************************************************************
 *	
 **********************************************************************
 */
