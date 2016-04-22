#ifndef	usart_h_
#define	usart_h_
/******************************************************************
 *	USART ����M���W���[��
 ******************************************************************
 *	�{�[���[�g�ݒ� (U2X���g�p)
	static	void USART_init(uchar baud_h);
 *
 *	�q�r�Q�R�Q�b����M�^�X�N.
	static	void USART_RecvTask(void);
	static	void USART_SendTask(void);
 */

#define	TXBUF_SIZE	14			// ���M�o�b�t�@�T�C�Y. 4�Ɍ���.
#define	RXBUF_SIZE	14			// ��MFIFO�T�C�Y.

void USART_Init(int baud);

// ATtiny2313�� RAM�����Ȃ��̂ŁAUSART�̃o�b�t�@��|�C���^��
//	���̂܂܌��J���ăA�v��������Q�Ƃ���g�����ɂȂ��Ă���.

// tinyclock�ł́AUSART�̎󂯓n���͍s�P�ʂɂȂ邽�߁A������FIFO
//	�I�Ȉ��������Ȃ��ŁA��s���܂����珈������p�^�[���ōs�Ȃ�.

//	��M�n.
char USART_RecvCh(void);		//�Ō�Ɏ�M�����������擾����.
extern	uchar USART_rxbuf[];			// ��M�o�b�t�@.
extern	uchar USART_rxptr;				// FIFO�������ݒn�_.

//	���M�n.
void USART_SendTask(void);		//���M�o�b�t�@�̃f�[�^���P�����Â��M.
extern	uchar USART_txbuf[];			// �q�r�Q�R�Q�b���M�o�b�t�@.
extern	uchar USART_txptr;			   	// 
extern	uchar USART_txend;			   	// 

//	�f�o�b�O�p.
void USART_SendDirect(char c);	//USART�ɒ���1�������M����.
void USART_Tick();
void USART_cmdproc();

/******************************************************************
 *	
 ******************************************************************
 */
#endif
