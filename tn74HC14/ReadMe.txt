
                 AVR ���g�p�������U��.
                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

�� �T�v

  ����́AATtiny2313���g�p�����A���U��ł��B


�� �������

  ����8MHz�œ��삵�A1MHz���x�̔��U�o�͂�PB0�ɏo���܂��B
  PD5��GND�ڑ��̏ꍇ�́A���g����250kHz�ɂȂ�܂��B

  Fuse��Xtal�ɐݒ肵�āA�N���b�N��8MHz�ɂ���ΐ��m�ȏo�͂ɂȂ�܂��B

�� �r���h��

-	�R���p�C����avr-gcc 4.3.3 ���g�p���܂����B
-	4.x�n��AVR-gcc�́A�ɗ͊֐��C�����C���W�J���邱�ƂŃp�t�H�[�}���X�𓾂Ă��܂��B
	���Atiny2313�ł́A�������[�I�[�o�[����̂� -finline-limit=1 �I�v�V������^���ĉ�����܂��B


�� �z���}

                  ATtiny2313
                  ___    ___
RESET            [1  |__| 20] Vcc
PD0(RxD)         [2       19] PB7(SCK)
PD1(TxD)         [3       18] PB6(MISO)
XTAL2            [4       17] PB5(MOSI)
XTAL1            [5       16] PB4              
PD2              [6       15] PB3
PD3              [7       14] PB2
PD4              [8       13] PB1           
PD5              [9       12] PB0           
GND              [10      11] PD6           
                  ~~~~~~~~~~~               

   ---------------------------------------
   XTAL:    XTAL1,2 => Crystal 3.579545MHz
   PB3:     ����m�F�pLED ( LED�̕Е���1k�����o�R����+5V)
   PB2:     �d������pTr  ( �A���[�������ɂP�b On�ɂȂ�Tr)
   PD0(RxD) USART --- USB�o�R�Ȃǂ�PC�Ɍq��
   PD1(TxD) USART --- USB�o�R�Ȃǂ�PC�Ɍq��
   RESET:   10k�����x��5V Pullup����.
   Vcc:     5V
   GND:     GND
   ---------------------------------------



�� FUSE�ݒ�

�q���[�Y�̐ݒ��́A�ȉ��̂悤�ɐݒ肵�܂��B

Low: 11100100
     ||||++++-- CKSEL[3:0] �V�X�e���N���b�N�I��
     ||++-- SUT[1:0] �N������
     |+-- CKOUT (0:PD2�ɃV�X�e���N���b�N���o��)
     +-- CKDIV8 �N���b�N���������l (1:1/1, 0:1/8)

High:11-11011
     |||||||+-- RSTDISBL (RESET�s�� 1:�L��, 0:����(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:�펞ON, 1:�ʏ�)
     ||+-- SPIEN (1:ISP�֎~, 0:ISP����) ��Parallel���̂�
     |+-- EESAVE (������EEPROM�� 1:����, 0:�ێ�)
     +-- DWEN (On-Chip�f�o�b�O 1:����, 0:�L��)

Ext: -------1
            +-- SPMEN (SPM���� 1:����, 0:�L��)

Cal: 93 89
