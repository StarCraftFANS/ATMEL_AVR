/**********************************************************************
 *	���[���X�Ō��A���S���Y��.
 **********************************************************************
 *	uchar morse_stat()
 *	void morse_send(uchar c)
 *	void morse_task()
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "task.h"
#include "timer.h"
#include "morse.h"
#include "sound.h"


#include "morsetab.h"	//�����\.


//	�����A�Z���̒����萔.
#define	MORSE_SPEED1	32
#define	MORSE_SPEED3	(MORSE_SPEED1*3+4)	//�̊���A������������������.


//	��ԕϐ�:
uchar morse_char;	// ASCII CODE
uchar morse_pat;	// pattern (MSB���� morse_cnt �r�b�g�����L��)
					// bit��0�Ȃ璷��,1�Ȃ�Z��.

uchar morse_cnt;	// counter (1..5�܂ŁA0�Ȃ�Ō��I��)
uchar morse_sub;	// sub count 0:mark 1:space 2:end of job

void  keyon(uchar c)
{
	if(c) {
		sound_play(0,0x280,0xff);
	}else{
		sound_play(0,0,0);
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
void morse_init(void)
{
	TX_OUT_PORT |= TX_OUT_BIT_MASK;	// PD7��H.
//	TX_OUT_DDR  |= TX_OUT_BIT_MASK;	// PD7��out.
}

/**********************************************************************
 *	��������.(CW ���U)
 **********************************************************************
 */
static	void morse_out_mark(void)
{
	TX_OUT_PORT &= ~TX_OUT_BIT_MASK;	// PD7��L.(�d�gon)
	TX_OUT_DDR  |= TX_OUT_BIT_MASK;		// PD7��out.
	keyon(1);							//�T�E���h���[�`����On.
}
/**********************************************************************
 *	���𗣂�.(CW ��~)
 **********************************************************************
 */
static	void morse_out_space(void)
{
	TX_OUT_DDR  &= ~TX_OUT_BIT_MASK;	// PD7��in.
	TX_OUT_PORT |= TX_OUT_BIT_MASK;		// PD7��H.(pull up)
	keyon(0);							//�T�E���h���[�`����Off.
}

/**********************************************************************
 *	�R�}���h��̂̉ۂ�Ԃ�.
 **********************************************************************
 *	�߂�l :
 *		0  : �R�}���h��t�\.
 *	   !0  : ���ݑŌ�����ASCII ����(1��)
 */
uchar morse_stat(void)
{
	return morse_char;
}

/**********************************************************************
 *	�R�}���h��̏��� sub.
 **********************************************************************
 *	c = �Ō����� ASCII�R�[�h .
 *	i = �Ō�����R�[�h�̃e�[�u���ԍ�.(Alphabet 26�� + Numeric 10�� �̂ǂꂩ)
 */
static void morse_send_idx(uchar c,uchar i)
{
	uchar d = __LPM(&morse_tab[i]);
	morse_char = c;
	morse_pat = d;
	morse_cnt = d & 7;
	morse_sub = 0;
}
/**********************************************************************
 *	�R�}���h��̏���.
 **********************************************************************
 *	c = �Ō����� ASCII�R�[�h .
 */
void morse_send(uchar c)
{
	if((c>='A')&&(c<='Z')) {morse_send_idx(c,c-'A'   );return;}
	if((c>='a')&&(c<='z')) {morse_send_idx(c,c-'a'   );return;}
	if((c>='0')&&(c<='9')) {morse_send_idx(c,c-'0'+26);return;}
	if((c==' ')) {morse_send_idx(c,'e'-'a');return;}
}

/**********************************************************************
 *	���̋N�����Ԃ��Z�b�g����.
 **********************************************************************
 *	len = 0   �����҂�.
 *	len !=0   �Z���҂�.
 */
void sleep_key(uchar len)
{
	uchar    c;
	if(!len){c = MORSE_SPEED3;}
	else	{c = MORSE_SPEED1;}

	sleep_tsk(c << 5);
}
/**********************************************************************
 *	���[���X�Ō��^�X�N.
 **********************************************************************
 */
void morse_task(void)
{
	uchar keylen;
	if(morse_char==0) return;	//�Ō���������:

	switch(morse_sub) {	//�T�u�V�[�P���X�ɂ�蕪�򏈗�:
	 case 0:	// KEY_ON ==========
		keylen= morse_pat & 0x80; 	//�Ō��p�^�[��.�����Ȃ�0�A�Z���Ȃ�1.
				morse_pat<<=1; 		//�Ō��p�^�[�����P�V�t�g.
				morse_cnt--;		//�Ō������P����.

		if(morse_char != ' ') {		//�󔒈ȊO.
			morse_out_mark();		//�L�[����.
		}

		morse_sub = 1;				//���V�[�P���X�Ɉڍs.
		sleep_key(keylen);			//�N���܂ł̑҂����ԃZ�b�g.
	 	break;
	 case 1:	// KEY_OFF ========
		morse_out_space();			//�L�[����.

		morse_sub = 2;				//���V�[�P���X�Ɉڍs.
		sleep_key(1);
	 	break;
	 case 2:	// STOP BIT =======
		if(	morse_cnt == 0 ) {		//�S���Ō�����?
			morse_sub = 3;			//���V�[�P���X�Ɉڍs.
			sleep_key(0);			//�N���܂ł̑҂����ԃZ�b�g.
		}else{
			morse_sub=0;			//�V�[�P���X�E���Z�b�g.
		}
	 	break;
	 default:
	 case 3:	// JOB END ========	�Ō��I��.
		morse_sub = 0;
		morse_char = 0;				//���̑Ō���t�\�ɂ���.
	 	break;
	}
}
/**********************************************************************
 *	
 **********************************************************************
 */
