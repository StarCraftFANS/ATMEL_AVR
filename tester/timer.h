/**********************************************************************
 *	16bit TIMER1 ���g�p�������荞�݃n���h���[.
 **********************************************************************
 *	timer_init()
 *	timer_wait()
 */
#ifndef _timer_h_
#define _timer_h_

volatile char  time_hh;				//����
volatile char  time_mm;				//����
volatile char  time_ss;				//����
volatile int   time_tick;			//����


void  timer_init(int cnt);
uchar timer_wait(void);
uchar timer_gettick(void);
int   timer_gettime(void);

#endif  //_timer_h_
/**********************************************************************
 *	
 **********************************************************************
 */
