                                                               2008-11-28
                       �L�[�{�[�h�}�j�A�E Part III
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~

�� �T�v

  �{�v���O�����́A�h�a�l-�o�b�݊��@�p�ɍL���g�p����Ă���PS/2�d�l��
�L�[�{�[�h�f�o�C�X���`�s�l�d�k�@�`�u�q�}�C�R���ɐڑ����āA���͋@��
�Ƃ��Ďg�p���邱�Ƃ�ړI�Ƃ��Ă��܂��B

  �ꉞ�ėp�I�Ȑ��^��ڎw���Ă��܂����A�ėp���̖����Ƃ���͂��e�͂��������B


�� �g�����^�e�X�g���@

  �g�d�w�t�@�C���Apart3/main88-12.hex ��ATmega88 �ɏĂ��܂��B
  ATmega88�́A���L�z���}���Q�l�ɔz�����Ă��������B
  �d���i�T�u�j��^���A�ڑ���̃L�[�{�[�h�������ƁA�X�s�[�J�[����
�@���������o���琳��ł��B

�� �z���}
                          ATmega88/168
                         ___    ___
    (PCINT14/_RESET)PC6 [1  |__| 28] PC5(ADC5/SCL/PCINT13) 
       (PCINT16/RxD)PD0 [2       27] PC4(ADC4/SDA/PCINT12)
       (PCINT17/TxD)PD1 [3       26] PC3(ADC3/PCINT11)
      (PCINT18/INT0)PD2 [4       25] PC2(ADC2/PCINT10)
 (PCINT19/OC2B/INT1)PD3 [5       24] PC1(ADC1/PCINT9)  ======> PS/2 keyb DATA
    (PCINT20/XCK/T0)PD4 [6       23] PC0(ADC0/PCINT8)  ======> PS/2 keyb CLK
                    Vcc [7       22] GND
                    GND [8       21] AREF
(PCINT6/XTAL1/TOSC1)PB6 [9       20] AVcc
(PCINT7/XTAL2/TOSC2)PB7 [10      19] PB5(SCK/PCINT5)
   (PCINT21/OC0B/T1)PD5 [11      18] PB4(MISO/PCINT3)
 (PCINT22/OC0A/AIN0)PD6 [12      17] PB3(MOSI/OC2A/PCINT3)
      (PCINT23/AIN1)PD7 [13      16] PB2(_SS/OC1B/PCINT2)
  (PCINT0/CLKO/ICP1)PB0 [14      15] PB1(OC1A/PCINT1)
                         ~~~~~~~~~~

   ---------------------------------------
   XTAL:    XTAL1,2 => Crystal 12MHz
            PC0 =====> PS/2 keyb CLK  (PCINT8)
            PC1 =====> PS/2 keyb DATA
            PD6 =====> Sound Output(PWM)
            PD7 =====> CW Output(DC Level,L=MAKE H=BRAKE)
   ---------------------------------------

�� FUSE

############################## ATMega88/168 ##############################
# Fuse Low byte: (0xFF)
#Low: 11111111
#     ||||++++-- CKSEL[3:0] �V�X�e���N���b�N�I��
#     ||++-- SUT[1:0] �N������
#     |+-- CKOUT (0:PB0�ɃV�X�e���N���b�N���o��)
#     +-- CKDIV8 �N���b�N���������l (1:1/1, 0:1/8)
#
# Fuse High byte: (0xDD)
#High:11-11101 
#     |||||+++-- BODLEVEL[2:0] (111:��, 110:1.8V, 101:2.7V, 100:4.3V)
#     ||||+-- EESAVE (������EEPROM�� 1:����, 0:�ێ�)
#     |||+-- WDTON (1:WDT�ʏ퓮��, 0:WDT�펞ON)
#     ||+-- SPIEN (1:ISP�֎~, 0:ISP����) ��Parallel���̂�
#     |+-- DWEN (On-Chip�f�o�b�O 1:����, 0:�L��)
#     +-- RSTDISBL (RESET�s�� 1:�L��, 0:����(PC6))
#
# Fuse Extend byte: (0xF8)
#Ext: -----001
#          ||+-- BOOTRST ���f�[�^�V�[�g�Q�� (1=���p�̈�A0=�u�[�g���[�_�[)
#          ++-- BOOTSZ[1:0] ���f�[�^�V�[�g�Q��
#
###############################################################################

�� CLOCK

  �K�v�Ȃ�΁Afirmware/Makefile ���̋L�q��ύX���Ă��������B

	#F_CPU = 12000000
	F_CPU = 20000000

�� �J���̃q���g

  AVR-USB�̃��t�@�����X�v���W�F�N�g�ł���AbootloadHID �܂��́AbootloadHID ��
  ���j�^�@�\��t�������ٍ� bootmon ��p����ƁA�`�u�q���C�^���g�p���邱�ƂȂ�
  USB�o�R�Ńv���O�����̍X�V�Ǝ��s���s�����Ƃ��o����̂ŃR���p�C�����q�t�m��
�@�J��Ԃ����y�ɏo����悤�ɂȂ�܂��B���������������B


�� �f�B���N�g���\��

   keybrd-mania --+-- part1/ �o�b�W�W�O�PmkII�r�q�L�[�{�[�h�⊮�v��(PIC16F84A)
                  |
                  +-- part2/ �L�[�{�[�h�܂ɂ�][ (PIC16F84A)
                  |            (�ь�p�L�[�{�[�h���h�a�l�݊��@�ɐڑ����Ďg����)
                  +-- part3/ �ėp PS2 �L�[�{�[�h���C�u���� (ATmega88)
                  |
                  +-- part4/ �ėp PS2 �L�[�{�[�h���C�u���� (ATtiny2313)

   ���ӁFpart1,part2 �͂قڂQ�O���I�̈╨�ł��B(�ꕔ2001�N������timestamp����)


�� ���ۏ؁E�Ɛӏ���

  �������āi���邢�͓K���ȃt���[�����[�N�Ƃ��āj�����R�ɂ��g�����������B
  ���p�A�񏤗p�͖₢�܂���B

  �A���A���̃v���O�������^�p���邱�Ƃɂ���Ĕ������������Ȃ鑹�Q�ɂ��Ă��A
  ����҂���щ��ǎ҂͂��̐ӂ𕉂�Ȃ����̂Ƃ����Ă����������B
  ���̏����ɓ��ӂł��Ȃ����͎g�p�������Ȃ����̂Ƃ����Ă��������܂��B


�� �Q�l����

  �`�s�l�d�k�Ђ̃A�v���P�[�V�����m�[�g�R�P�R�Ԃ��Q�Ƃ����Ē����܂����B



###### EOF ######
