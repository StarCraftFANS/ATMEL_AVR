/**********************************************************************
 *	InfraLED Remocon
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "config.h"
#include "timer.h"
#include "usart.h"

/**********************************************************************
 *	define
 **********************************************************************
 */
void USART_Init(int baud);
void keybd_init();

#if	0
void task_regist(void)
{
	init_tsk();
//	reg_tsk(ID_main ,main_task_tone);	// ����.
//	reg_tsk(ID_sound,sound_task);
}
#endif
#define	CPUCLK	4000000		//3579545
/**********************************************************************
 *	���C��
 **********************************************************************
 */
int	main(void)
{
	//	PortA=LED
	PORTA=0b00000010;
	DDRA =0b00000011;	// PA0�͐ԊOLED���_���o�� / PA1 �̓`�F�b�N�pLED���_���o��.
	//	PortB=weak pullup(in)
	PORTB=0b11111111;	// �S��Pull-UP
	DDRB =0b00000000;
	//	PortD=pull-down(sel only)
	PORTD=0b10000011;	// PD0,PD1,PD7��Pull-UP
	DDRD =0b00000000;

	//�d�͐ߖ�.
	ACSR=0x80;	//	�A�i���O��r��̓d�͂�Off
	

	keybd_init();
	timer_init(CPUCLK/8/20 - 1); // 1/20�b (20Hz) �����Ŋ��荞��.
//	USART_Init(9600);

	sei();
//	task_regist();

	while(1) {
//		MCUCR = 0b00000000;	// Idle!
		MCUCR = 0b00100000;	// standby!
		asm("sleep");

//		timer_wait();
//		dispatch_tsk();

	}
	return 0;
}

/**********************************************************************
 *	
 **********************************************************************
 */
