/**********************************************************************
 *	PWM Sound Driver
 **********************************************************************
 * TIMER0�ɂ�鍂���o�v�l�i8bit D/A�j�o��
 *
 *	void sound_init(void)	PWM ������.
 *	void sound_mix(void)	�~�L�V���O.
 *
 *
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "task.h"
#include "sound.h"
#include "sinewave.h"

#define	CH_MAX	4
#define	SUSTAIN	40			// ���ʂ� ���x�� 63�Ŏn�߂āA40�Ŏx�����[�h�ɂ���.
							// ���Ղ������ꂽ��A40 �ȉ����}���Ɍ���������.

uint  waveptr[CH_MAX];		// sine table�̓ǂݏo��pointer.
uint  wavepitch[CH_MAX];	// waveptr �ɉ��Z���釙�l.
uchar wavevol[CH_MAX];		// ����.
uchar wavedecay[CH_MAX];	// ������.

/**********************************************************************
 *	������
 **********************************************************************
 */
void sound_init(void)
{
	uchar i;

	SPK_OUT_DDR |= SPK_OUT_BIT_MASK;	// PD6��out.
	TCCR0A = 0b10000011;	//COM0A=��r��v��Low,����PWM
	TCCR0B = 0b00000001;
	OCR0A  = 128;

	for(i=0;i<CH_MAX;i++) {
		wavepitch[i]=0;
		waveptr[i]=0;
	}
}

/**********************************************************************
 *	8bit D/A�o��.
 **********************************************************************
 */
#if	0
static	void sound_out(uchar x)
{
	OCR0A = x;
}
#else
// timer.c ���Ɏ���.
void sound_out(uchar x);
#endif

#ifdef	__AVR_ATtiny2313__
//
//   c * ( gain / 256 ) �̑e���v�Z�����s.
//
//static 
char amplitude(char c , uchar gain)
{
	char val=0;
	c>>=2;if(gain & 0b1000000) val=c;
	c>>=1;if(gain & 0b100000) val+=c;
	c>>=1;if(gain & 0b10000) val+=c;
	c>>=1;if(gain & 0b1000) val+=c;
	c>>=1;if(gain & 0b100) val+=c;
	c>>=1;if(gain & 0b10) val+=c;
	c>>=1;if(gain & 0b1) val+=c;
	return val;
}
#endif

/**********************************************************************
 *	�`���l���~�L�V���O.
 **********************************************************************
 */
static	void sound_mix(void)
{
	uchar i,p;
	char  out=0;
	char  c;
	int   v;
	for(i=0;i<CH_MAX;i++) {
		uchar off=wavevol[i];
		waveptr[i] += wavepitch[i];		//�s�b�`���W�X�^�����Z.
		p = (waveptr[i] >> 8) & 0x3f;	//sine wave table�̓ǂݏo���|�C���^.
#if	0
		p += wave_offset;				//�t�F�[�Y���|���Ă݂�.
#else
		if(off < 30) off = 30;
		p += (off-30);					//������h�炷.
#endif
		c = __LPM( &sinewave[p] );
#ifdef	__AVR_ATtiny2313__
		v = amplitude(c , wavevol[i]);	//���ʂ���Z.
		c = v ;
#else
		v = ((c * wavevol[i]) >> 8);	//���ʂ���Z.
		c = v ;
#endif
		out += c;						//�~�L�V���O.
	}
	sound_out(out + 0x80);
}

/**********************************************************************
 *	���U����炷�^�X�N.
 **********************************************************************
 */
void sound_task(void)
{
	sound_mix();
}

/**********************************************************************
 *	���U����炷.
 **********************************************************************
 *	ch : (0..3)    �����`���l��.
 *	p  : (0..2047) �s�b�`�萔.
 *	v  : (0..255)  ����
 */
void sound_play(uchar ch,uint p,uchar v)
{
	if(ch<CH_MAX) {
		wavepitch[ch]=p;
		wavevol[ch]=v;
		wavedecay[ch]=1;
	}
}

/**********************************************************************
 *	���U���̌����ʂ��w�肷��.
 **********************************************************************
 *	p  : (0..2047) �s�b�`�萔.
 *	d  : tick������̌�����.
 */
void sound_setdecay(uint p,uchar d)
{
	uchar i;
	for(i=0;i<CH_MAX;i++) {		//�Sch�������Ă���̂̓_�T������.
		if( wavepitch[i]==p ) {
			wavedecay[i]=d;
		}
	}
}
/**********************************************************************
 *	���U��������������. ����I�ɌĂԂ���.
 **********************************************************************
 */
void sound_decay()
{
	static uchar n=0;
	static uchar m=0;
	uchar i,d;
	char v;

	n--;
	if(n & 0x1f) return;

	for(i=0;i<CH_MAX;i++) {
		v = wavevol[i];								//	���݂̉���.
		if( v ) {
			d = wavedecay[i];						//	������.
			if((d==1) && (v == SUSTAIN)) continue;	//  �T�X�e�B�����x���ŕێ�������.
			v -= d;									//  ���������́A���͂��ꂾ��.
			if(v<0) v = 0;							//  ���ʂ𕉂ɂ�����剹�ʂɖ߂�̂�0�ɂ���.
			wavevol[i]=v;							//  �����߂�.
		}
	}
#if	0
	// m�͋��g,wave_offset�͎O�p�g.
	m++; m &= 0x7f;
	if(m>=64) wave_offset = 128-m;
	else      wave_offset = m;
#endif
//	if(wave_offset==0) wave_offset = 1;

//	wave_offset += 128;
}
/**********************************************************************
 *	���U����S���~�߂�.
 **********************************************************************
 */
void sound_stop_all()
{
	uchar i;
	for(i=0;i<CH_MAX;i++) {
		wavepitch[i]=0;
		waveptr[i]=0;
		wavevol[i]=0;
		wavedecay[i]=1;
	}
}
/**********************************************************************
 *	
 **********************************************************************
 */
