                                                             2008�N11��21��

        USB�ڑ������̃h���C�o�C���X�g�[���s�v��AVR���C�^�iHIDaspx�j
        �P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P

�� ���̃A�[�J�C�u�́Asenshu���񂪒��S�ƂȂ��ĊJ�����Ă���HIDaspx��
   ATmega88 �ɑΉ����������̂ł��B


�� ���݂̖��_/�a�t�f���

   �f�o�C�X�F��(ispConnect)���s���S�Ȃ��߁A�^�[�Q�b�gAVR��F�����Ȃ����Ƃ�����܂��B

   �������݃f�o�C�X��MISO,MOSI,SCK���o�̓��[�h�ŏՓ˂���ꍇ�A�������߂Ȃ����Ƃ�����܂��B

   ISP�N���b�N�̓n�[�h�E�F�A�N���b�N�����������Ă��Ȃ����߁A�ᑬ�^�[�Q�b�g�ɑΉ����Ă��܂���B
    -d0 | fOSC/4  ... 3MHz
    -d1 | fOSC/8  ... 1.5MHz
    -d2 | fOSC/16 ... 750kHz
    -d3 | fOSC/32 ... 375kHz
    -d4 | fOSC/64 ... 187kHz
    -d5 | fOSC/64 ... 187kHz
    -d6 | fOSC/128...  93kHz

    -d7�ȍ~��-d6�Ɠ����ł�.  d4��d5������������ɂȂ��Ă��܂����A������n�[�h�E�F�A�̎d�l�ł��B



�� �z��
                          ATmega88/168
                         ___    ___
    (PCINT14/_RESET)PC6 [1  |__| 28] PC5(ADC5/SCL/PCINT13)
       (PCINT16/RxD)PD0 [2       27] PC4(ADC4/SDA/PCINT12)
       (PCINT17/TxD)PD1 [3       26] PC3(ADC3/PCINT11)
      (PCINT18/INT0)PD2 [4       25] PC2(ADC2/PCINT10)
 (PCINT19/OC2B/INT1)PD3 [5       24] PC1(ADC1/PCINT9)
    (PCINT20/XCK/T0)PD4 [6       23] PC0(ADC0/PCINT8)
                    Vcc [7       22] GND
                    GND [8       21] AREF
(PCINT6/XTAL1/TOSC1)PB6 [9       20] AVcc                          �������ݑ�AVR-chip
(PCINT7/XTAL2/TOSC2)PB7 [10      19] PB5(SCK/PCINT5)      -------> SCK
   (PCINT21/OC0B/T1)PD5 [11      18] PB4(MISO/PCINT3)     -------> MISO
 (PCINT22/OC0A/AIN0)PD6 [12      17] PB3(MOSI/OC2A/PCINT3)-------> MOSI
      (PCINT23/AIN1)PD7 [13      16] PB2(_SS/OC1B/PCINT2) -------> RESET
  (PCINT0/CLKO/ICP1)PB0 [14      15] PB1(OC1A/PCINT1)
                         ~~~~~~~~~~
   ---------------------------------------
   USB:     PB0     ===> USB D-
            PB1,PD2 ===> USB D+
   XTAL:    XTAL1,2 => Crystal 12MHz
   ---------------------------------------

   �^�[�Q�b�g�Ƃ�MISO,MOSI�̓N���X�ڑ��̕K�v�͂���܂���B�X�g���[�g�ł��肢���܂��B

   �^�[�Q�b�g�Ɛڑ�����S�{�̃��C����100�����x�̒�R��ʂ��Đڑ�����ق������S�ł��B

   USB D+,D-��75�����x�̒�R��ʂ���PD2,PD3�Ƃ��ꂼ��ڑ����A�����D-��1.5k����ʂ���
   �v���A�b�v����K�v������܂��B

   AVcc,AREF�͓K�؂ɏ������Ă��������B
   (Vcc�������h�g���ʂ���AVcc�ɐڑ����܂��B)

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
#          ||+-- BOOTRST ���f�[�^�V�[�g�Q��
#          ++-- BOOTSZ[1:0] ���f�[�^�V�[�g�Q��
#
###############################################################################

�� CLOCK

	F_CPU = 12000000 �ŁA���肢���܂��B

#################
2009-0206�ǋL:

�EwsNak #172��ɑΉ����܂����B

�g�����F
�i�P�jbootmon-ws172 ���g�p���āAATmega88�̉��p�̈��HIDaspx�t�@�[������������ł��������B
      ��F
      C:hidspx-ws172> bootloadhid.exe firmware\main.hex
�i�Q�j�N���̂������́A�����ʓ|�ł����A�^�N�g�X�C�b�`��'L'�������Ȃ���t�r�a�̃P�[�u��
�@�@�@��ڑ����܂��BPWR/BUSY�̂k�d�c���_�ł��n�߂���AHIDaspx�t�@�[�����N�������؋��ł��B
�i�R�j���ȃv���O���~���O�p��JP2 (1-2) �W�����p�[���ڑ�����Ă�����A�O���Ă����܂��B
�i�S�j�K���ȃv���O�����Ώ�AVR�}�C�R����ڑ����āAsrc/hidspx-gcc.exe ���N�����܂��B
      ��F
      C:hidspx-ws172/src> hidspx.exe -r
�i�T�jAVR�}�C�R���̎�ނ��\���o�����琬���ł��B

-----------------

���ӓ_�F

�E�g�v �r�o�h�ɂ����Ή����Ă��܂���̂ŁA�N���b�N�̒x��AVR�`�b�v��F���ł��܂���B
�Ebootmon�̎x�z������AHIDaspx�ɐ؂�ւ��邽�߂Ƀ^�N�g�X�C�b�`'L'�������Ă��A
�@�p�\�R����USB�̐؂�ւ��i�t�@�[���̍����ւ��j��F�����܂���̂ŁAhidspx-gcc.exe ��
�@���܂������Ă���܂���B
�@�Ώ��@�Ƃ��ẮAWindows�̃f�o�C�X�}�l�[�W���[�Ɍ���Ă��铖�YHID�f�o�C�X
�@�@�q���[�}���C���^�[�t�F�[�X�f�o�C�X
�@�@  |
      +-- USB�q���[�}���C���^�[�t�F�[�X�f�o�C�X ��������
�@���}�E�X�E�N���b�N�ň�x�����ɂ��A�ĂїL���ɂ���ƁAAVR�̓d���͓������܂܂ŃP�[�u��
�@�𔲂����������̂Ɠ������ʂ�������̂�hidspx-gcc.exe���g����悤�ɂȂ�܂��B

�@	�i���̂܂܂ł͔��ɕs�ւȂ̂ŁA���Ǘ\��ł��j

�E�ʂ̕��@�Ƃ��ẮA�^�N�g�X�C�b�`'L'���W�����p�[�ɂ��ĕ��Ă����Ƃ�����肩�����A
�@���邢��ATmega88��ext-fuse��ύX���āA��ɉ��p�v���O�������N������悤�ɂ�����@
�@������܂��B
�@�i�A��ext-fuse�̕ύX���s���ƁA�t�@�[���E�F�A�̍X�V��bootmon(bootloadHID.exe)��
�@�@�g���Ȃ��Ȃ�A�O����AVR���C�^���K�v�ɂȂ�܂��j


�Ehidspx-gcc.exe �Ɠ����f�B���N�g���� hidspx-gcc.ini ��p�ӂ��āA�ȉ��̓��e��
�@�ݒ肵�Ă��������B

;hidspx-gcc.ini
-d5
-ph:0001
;------

###### EOF ######





