/**********************************************************************
 *	CUI Clock : Sample Main
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
#if	0
void task_regist(void)
{
	init_tsk();
//	reg_tsk(ID_main ,main_task_tone);	// ����.
//	reg_tsk(ID_sound,sound_task);
}
#endif

#define	CPUCLK	3579545
/**********************************************************************
 *	���C��
 **********************************************************************
 */
int	main(void)
{
#if	0
	//	CPU�̃N���b�N��27kHz���x�ɗ��Ƃ�.
	{
		CLKPR = 0b10000000;	// System CLK�̕ύX��v������(4clk�ȓ���)
		CLKPR = 0b00000111;	// System CLK��128����.
	//	CLKPR = 0b00000011;	// System CLK��8����. (1/8)fuse�L�����Ɠ���.
	}
#endif

	//	I/O�|�[�g��PB3�ȊO���͂ɂ���pull up.
	PORTA=0b11111111;
	DDRA =0b00000000;

//	PB3=LED PB2=POWER 1=OUT
	PORTB=0b00000000;
	DDRB =0b00001100;

	PORTD=0b11111111;
	DDRD =0b00000000;

//	3,579,545/8 = 447443.125		 3.58MHz / 8 ���̎���:
//				  4474.43125 -> 4474 0.000096390 ���������Ȃ�.
	timer_init(CPUCLK/8/100 - 1); // 1/100�b (100Hz) �����Ŋ��荞��.
	USART_Init(9600);

	sei();
//	task_regist();

	while(1) {
//		MCUCR = 0b00000000;	// Idle!
		MCUCR = 0b01000000;	// standby!
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
