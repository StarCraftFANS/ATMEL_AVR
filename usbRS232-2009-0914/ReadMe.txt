                                                            2008�N11��09���i���Łj
                                                            2009�N 1��14��
                                                            2009�N 3�� 9��
                                                            2009�N 9��14��


�� usbRS232 �̏Љ�

  ����́AATtiny2313���g�����AUSB�V���A���ϊ���ł��B
  CDC(�R�~���j�P�[�V�����E�f�o�C�X�E�N���X) �Ŏ������Ă��܂��̂� Windows �W
  ���� CDC ���z COM �|�[�g�h���C�o�[�ɂ��A"COMx:" (x �� 1�` 9 �܂�) �|�[
  �g�Ɍ�����͂��ł��B

�� �A�[�J�C�u�̍\��

.\
����bin
��  ����firmware
����develop
����Firmware
    ����usbdrv

�� �z���}

 * 	��H�}�́AHIDasp(x)�AHIDsph�̂��̂Ƌ��ʂł��B

                  ATtiny2313
                  ___    ___
RESET            [1  |__| 20] Vcc
PD0(RxD)         [2       19] PB7(SCK)
PD1(TxD)         [3       18] PB6(MISO)
XTAL2            [4       17] PB5(MOSI)
XTAL1            [5       16] PB4
PD2              [6       15] PB3(BUSY LED)
PD3(D+)          [7       14] PB2
PD4(D-)          [8       13] PB1
PD5(USB pullup)  [9       12] PB0
GND              [10      11] PD6
                  ~~~~~~~~~~~

�E����()�̏�����Ă���pin�ƁAGNC,Vcc�ɔz�������Ă��������B

�EGND,Vcc�Ԃɂ̓p�X�R��(10uF�ȏ�)�����Ă��������B

�EXTAL1,XTAL2�Ԃ� 12MHz �̐����U���q�����t���܂��B

�EPB5,PB6,PB7 �́AISP(�C���E�V�X�e���E�v���O���~���O) �[�q�ŁA�O���ɂ��� AVR
  ���C�^�[�Ƃ̐ڑ����o����悤�ɂ���ƁA���ł��t�@�[���E�F�A�̍X�V���o��
  ��̂ŕ֗��ł��B

�EUSB D+,D-�́A68�����x�̒�R���o�R����USB�R�l�N�^�ɔz�����܂��B

�EUSB D- ��USB�R�l�N�^���[�q��1.5k����5V��������3.3V��pull up���܂��B

�EVcc �� 5V �쓮������ꍇ�� USB D+,D- �̃R�l�N�^�[�q���Ƀc�F�i�[ Di �����
  �� 3.3V ���x�ɃN�����v���܂��B

�ERxD,TxD�͂�����TTL���x���ł��B(�ʏ��Hi�ŁA�X�^�[�g�r�b�g��Lo�ɂȂ�܂��B)

   ---------------------------------------
   SPI:     PB7-5
   USB:     PD4   ===> USB D-
            PD3   ===> USB D+
            PD5   ===> USB D- pullup (Option)
            PB3   ===> BUSY LED (Option)
   XTAL:    XTAL1,2 => Crystal 12MHz
   ---------------------------------------

�� firmware��FUSE�ݒ�

HIDaspx �̉�H�œ��삳����ꍇ�ɂ́A2 �� HEX �t�@�C���ibin/firmware/main.
hex �� bin/firmware/main.eep�j �� Tiny2313 �ɏ�������ł��������D
�q���[�Y�ݒ莞�́A�K�� 12MHz �̃N���X�^����ڑ����A�ݒ肵�Ă��������B

 >hidspx main.hex main.eep
  ^^^^^^^^^^^^^^^^^^^^^^^^
 Detected device is ATtiny2313.
 Erase Flash memory.
 Write   Flash: 2002/2002 B
 Verify  Flash: 2002/2002 B
 Passed.
 Write  EEPROM:    7/   7 B
 Verify EEPROM:    7/   7 B
 Passed.

 >hidspx -fL0xFF -fH0xDB -fX0xFF
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Low: 11111111 (0xFF)
     ||||++++-- CKSEL[3:0] �V�X�e���N���b�N�I��
     ||++-- SUT[1:0] �N������
     |+-- CKOUT (0:PD2�ɃV�X�e���N���b�N���o��)
     +-- CKDIV8 �N���b�N���������l (1:1/1, 0:1/8)

High:11-11011 (0xDB)
     |||||||+-- RSTDISBL (RESET�s�� 1:�L��, 0:����(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:�펞ON, 1:�ʏ�)
     ||+-- SPIEN (1:ISP�֎~, 0:ISP����) ��Parallel���̂�
     |+-- EESAVE (������EEPROM�� 1:����, 0:�ێ�)
     +-- DWEN (On-Chip�f�o�b�O 1:����, 0:�L��)

Ext: -------1 (0xFF)
            +-- SPMEN (SPM���� 1:����, 0:�L��)



�� �e�X�g

�Ew32term.exe��Windows�̃R�}���h���C���œ��삷��ȈՃ^�[�~�i���\�t�g�ł��B
�Eteraterm���ł������͏o���܂��B

    ATtiny2313 �� pin 2(RxD) �� pin 3(TxD) ��ڑ�����΁A�G�R�[�o�b�N�e�X�g
    ���o���܂��B

�EWindows Vista �ł́AUSB �� LowSpeed Bulk �]����������Ă��Ȃ����߁A���̂�
  �܂ł͓��삵�܂���B

�� �r���h���̒���

�E�r���h�ɂ� WinAVR-20060421 ���g�p���܂��B�ŐV�� avr-gcc(gcc4 �x�[�X�̂���
  �S��) �ł̓R�[�h�T�C�Y���傫���Ȃ� 2kB �ɓ���܂���B

�EAVR-Wiki�ɂ���AWinAVR-20060421����movw���߂��g����悤�ɂ�����
  WinAVR-20060421.7z ���g�p����ƁA����Ƀ������[�T�C�Y��ߖ�ł��܂��B

�Ehardware.h  �� HW_LED_PIN �̒l��ς���ƁABUSY LED��PB0�`3�̂����ꂩ�ɕύX�ł��܂�.

�Eusbconfig.h ��
    #define HWPIN_USB_IS_D_4_3	0
    #define HWPIN_USB_IS_D_3_2	1
  �̐ݒ��ς���ƁAUSB D-,D+�̐ڑ��� PORTD 3,2 �ɕύX���邱�Ƃ��ł��܂��B

 PD2 �� USB �ʐM�ɗ��p���鎞�ɂ́AFUSE �ݒ�ɒ��ӂ��A�N���b�N�o�͋@�\�𖳌�
 �ɂ���K�v������܂��B

�� �r���h���̒���

������ build-all.bat ���������������B12/16MHz �p�� HEX �t�@�C�������̃R�}
���h�̎��s�Ő����ł��܂��BRXBUF��DCD�̓R���p�C�����Ɏw��ł���悤�ɂ��܂����B
�������A�e�X�g���s���Ă���܂���̂ŁA�������Ƀe�X�g�����肢�ł���΍K���ł��B

==== build-all.bat �̓��e ====
@echo off

:FCPU12
make clean
make
make install

REM Program:2008 bytes (98.0% Full)��2kB�ȓ��Ɏ��܂�
:FCPU16DCD
make DCD=1 F_CPU=16000000 clean
make DCD=1 F_CPU=16000000
make DCD=1 F_CPU=16000000 install

avr-size ../bin/firmware/*.hex
:END
================================

�� ���������t�@�C�����ɂ���

main-12.* 12MHz����p
main-16.* 16MHz����p

   text    data     bss     dec     hex filename
      0       7       0       7       7 main-12.eep
      0    2002       0    2002     7d2 main-12.hex
      0       7       0       7       7 main-16.eep
      0    2008       0    2008     7d8 main-16.hex
      0    2016       0    2016     7e0 save.hex

AVR�}�C�R���ɏ������ޏꍇ�ɂ́Amain-12.hex, main-12.eep�𓯎���
��������ł��������B

�� ���p���x�ƒ��ӓ_

  0. ����̉���ŁA16MHz�p�̃��W���[���𐶐��ł��܂��B
     16MHz �p�ɃR���p�C�����s���ƁAProgram:2008 bytes (98.0% Full) �� 2kB
     �ȓ��Ɏ��܂邱�Ƃ��m�F�ł��܂����B����ɂ��A�኱�̋@�\�̒ǉ���C����
     �\�ɂȂ�ƍl���܂��B

  1. UCHI �z�X�g (intel �n) �ł� 38,400bps �ŕ����������m�F����Ă���A19,200bps
     �����E�ł��BUCHI �z�X�g�ɑ΂��ẮAPC-USB �f�o�C�X�Ԃ� HighSpeed-HUB
     ��ڑ����APC-Hub �Ԃ� 480Mbps �Őڑ����邱�Ƃő��x���P�̉\���������
     �����A���݂̂Ƃ��듮�삪�m�F����Ă���̂� VIA �� USB �`�b�v�ł݂̂ł��B

  2. OHCI �z�X�g (AMD,SiS,NEC �n) �ł� 60,000bps �ŕ����������m�F����Ă��܂�
     �̂ŁA57,600bps �����E�ł��B

  3. UCHI �z�X�g�ɑ΂��ẮAPC-USB �f�o�C�X�Ԃ� HighSpeed-HUB ��ڑ����A
     PC-Hub �Ԃ� 480Mbps �Őڑ����邱�Ƃő��x���P�̉\��������܂��B�i���m�F�j


�� �Ɛ�

    �܂��A���낢��ȕ]�����s���Ă��܂���̂ŕs���������\���������ł��B
	���M�o�b�t�@�t���̏���������܂���B
	�t���[�R���g���[��������܂���B

    �{�[���[�g�� 600bps ���� 12800bps �̊Ԃ̔C�ӂ̋����l�����܂��B
            (baudrate�̉���1�r�b�g�͖�������܂��B)
    MIDI�̂悤��31250bps �ɐݒ肷�邱�Ƃ��\�ł��B

    �{�[���[�g���W�X�^�̐ݒ莮�� 16bit ���x�ł����v�Z���Ă��܂���̂ŁA�ݒ�
    �l�ɂ���Ă͌덷��������ꍇ������܂��B

    �v�Z��  UBRR = (USARTCLOCK / (baudrate/2)) - 1;
                    USARTCLOCK = 12000000 / 8 / 2 ;
	-----------------------------------------------


�� �ӎ�

  LowSpeed AVR-USB��CDC�N���X�̎����́ARecursion Co., Ltd.
     http://www.recursion.jp/avrcdc/indexj.html
  �̓c���C���̎�ɂ��ŏ��ɍs���܂����B
  ���̂��ƂɂȂ����A�C�f�B�A��2005�N���̐ΐ싱�㎁�̎����ɑk��܂��B

  �܂��AAVR-USB(http://www.obdev.at/products/avrusb/index.html)
  �̃��t�@�����X�v���W�F�N�g�̂ЂƂł���A
     AVR-Doper - an STK500 Compatible AVR Programmer
  �̃\�[�X������̃t���[�����[�N�Ɏg�킹�Ă������܂����B

 ��L�̐�B�̕��X�ɐ[���h�ӂƊ��ӂ��q�ׂ����Ǝv���܂��B


�� �J���҃���

���̃A�[�J�C�u�́Akuga�����2008-1028�ł����ɍ쐬���Ă��܂��B

�@usbRS232���C�����܂����B

  (1) �o�b�t�@�|�C���^�̈����o�O�C��
  (2) ���M�����荞�݂���|�[�����O�ɕύX
   �@�@(���荞�݂ɕύX���Ă����ʂ��Ȃ��̂ŃR�[�h�k����D��)
  (3) �ʐM�p�����[�^��EEPROM�ɕۑ�
�@�@�@�iRAM�ɕۑ������ɁA���̕����M�o�b�t�@�[�Ɋ����Ă�)
  (4) ���M�o�b�t�@�[ 8->16�Ɋg��
  (5) 12MHz, 16MHz�̕ύX���\�ɂ��܂����B
  (6) V-USB�̉���𔽉f���܂����i16MHz�ł̓��삪���퉻�j


