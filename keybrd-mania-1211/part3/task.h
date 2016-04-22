/**********************************************************************
 *	Tiny Task Controler
 **********************************************************************
 *	�l����:
 *	�E�X�P�W���[���̓��E���h���r���̂݁B
 *	�E�m���E�v���G���v�e�B�u�B
 *	�E���炩����task_id�ƁA���̎��s�֐����`�i������mapping�j���Ă������ƁB
 *	�E���荞�݃��[�`���Ƃ͊�{�I�ɖ��֌W.(�t�H�A�O���E���h����)�B
 *	�E�^�X�N���̃R���e�L�X�g�́A�֐��|�C���^�����ł���B
 *	  �E���W�X�^�R���e�L�X�g�Ȃǂ͈�؎����Ȃ��B
 *	  �E�֐��|�C���^�������ɂ��؂�ւ���s�ׂ͂n�j�B
 *	  �E�X�^�b�N�G���A�͑S�^�X�N�ɑ΂��ĂP�ł��苤�L�B
 *	�E�^�X�N���܂킷���߂ɂ� dispatch_task() �̎��������Ăяo���B
 *	�E�X���[�v�ȊO�ɁA���������T�[�r�X�����͖����B
 *	�E�^�X�N�L���[�⃊�X�g�\���������Ȃ��̂ŁA���܂葽�����̃^�X�N�����Ȃ��Ȃ��B
 *	  �z�萔�����������P�U�ȓ��B
 */

#ifndef	_task_h_
#define	_task_h_


#ifndef __ASSEMBLER__
#ifndef uchar
#define uchar   unsigned char
#endif
#ifndef uint
#define uint   unsigned int
#endif
#ifndef schar
#define schar   signed char
#endif
#endif




#ifdef	__AVR_ATtiny2313__
#define	MAX_TASKS	2
typedef	uchar	UCNT;
#else
//	ATmega88
#define	MAX_TASKS	4
typedef	uint	UCNT;
#endif
/**********************************************************************
 *	TASK STATE
 **********************************************************************
 */
enum {
	TSTAT_DORMANT  = 0,	//�x�~.
	TSTAT_RUNNABLE = 1,	//���s��.
	TSTAT_WAIT     = 2,	//�ҋ@.
    TSTAT_SLEEP    = 4,	//�x��.
};	//���̂Ƃ���0��1�����L��.

/**********************************************************************
 *	TASK CONTROL BLOCK
 **********************************************************************
 */
typedef	struct {
	uchar stat;			//�^�X�N�X�e�[�g.(enum)
	UCNT  sleep_cnt;	//�x���J�E���g 0=�o�� !0=�x����
	volatile void (*func)(void);	//�֐��G���g���[.
} TCB;

extern	TCB   task_tcb[MAX_TASKS];	//
extern	uchar cur_tsk;				//���ݎ��s����id;

/**********************************************************************
 *	TASK CONTROL FUNCTION
 **********************************************************************
 */


#define	reg_tsk(id_,userfunc_) {	\
	 task_tcb[id_].func = userfunc_;		\
	 task_tcb[id_].sleep_cnt = 0;			\
	 task_tcb[id_].stat = TSTAT_RUNNABLE;	\
	}
//��:���Ƃ�RUNNABLE�ɂ���.


#define	kill_tsk(id_) {		\
	 task_tcb[id].stat = 0;				\
	 task_tcb[id].func = NULL;			\
	}
//��:���stat���N���A����.


#define	sleep_id_tsk(id_,count_)	{	\
	 task_tcb[id_].sleep_cnt = count_;	\
	 task_tcb[id_].stat = TSTAT_SLEEP;	\
	}
//��:�C��task�̎��̋N�����Ԃ�ݒ肷��.

#define	sleep_tsk(count_)	{	\
	sleep_id_tsk( cur_tsk , count_ );}
//��:�����̎��̋N�����Ԃ�ݒ肷��.

#define	get_tsk_id()	(cur_tsk)


//	TCB�̏�����.
void init_tsk(void);
void dispatch_tsk(void);


#endif	//_task_h_
