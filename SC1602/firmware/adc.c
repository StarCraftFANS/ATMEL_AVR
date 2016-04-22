/**********************************************************************
 *	
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "task.h"

void adc_init(void)
{
	// A/D�ϊ� MUX���W�X�^�ݒ�.
	ADMUX = 0b00000101;		// Aref,ADC5
//	ADMUX = 0b11000101;		// internal ref,ADC5
	// A/D�ϊ� �d��On, A/D Clock = 1/64 ck
	ADCSRA = 0b10000110;
	// AIN�ɑ΂���f�W�^�����͋֎~.
	DIDR0  = 0b00000001;
}

/**********************************************************************
 *	A/D�ϊ��̎��s.
 **********************************************************************
 */
int adc_exec(void)
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

