                                                                      2008�N11��27��

sorry,Japanese Only.

   ���̃\�[�X�͂܂� �����łł��B

�� bootloadHID ��

  �@����́AAVR-USB �̃��t�@�����X�v���W�F�N�g�ł���AbootloadHID �Ƀ������[
  �ǂݏ����̃R�}���h��ǉ���������ł��B11/10 ����AATmega88/mega168 �̗���
  �œ��삷��悤�ɂȂ�܂����B


�� �o���邱��

  bootloader �@�\�́AbootloadHID �ƌ݊���������A������ hidmon88 �Ƃ��Ă��g
  ���܂��B

  �g�����Ƃ��ẮA 
    �K���ȃA�v���P�[�V�������쐬��bootloadHID �œ]�����܂��B
    hidmon88 �Ŏ��s���w�����܂��B
    �����āARAM �L�^�������e�́Ahidmon88 �œǂݎ�邱�Ƃ��\�ł��B

�� �A�[�J�C�u�̍\��

.\
����bin
��  ����firmware
����commandline
����develop
����firmware
��  ����usbdrv
����src
����test
    ����usbdrv


�EATmega168�p
   text    data     bss     dec     hex filename
      0    2038       0    2038     7f6 main168-12.hex
      0    1970       0    1970     7b2 main168-20.hex
      0    2024       0    2024     7e8 main168-RC.hex
      0    2122       0    2122     84a main168-test-20.hex
      0    2540       0    2540     9ec main168-test-RC.hex

�EATmega88�p
   text    data     bss     dec     hex filename
      0    2016       0    2016     7e0 main88-12.hex
      0    1948       0    1948     79c main88-20.hex
      0    2012       0    2012     7dc main88-RC.hex
      0    2472       0    2472     9a8 main88-test-RC.hex

�� -12, -20 �̓N���X�^�����U�q�̎��g�� (MHz)�A-RC �� RC ���U (12.8MHz) ����
   �����܂��Btest ���t�������̂́A���p�̈�œ��삷��t�@�[���ihidmon88 �@�\�j
   �ł��B�n�[�h�E�F�A�ɂ��킹�A�K�؂ɏ�������ł��������B

�� FUSE�i�N���X�^�����U��̏ꍇ�j


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
#Ext: -----000
#          ||+-- BOOTRST [0]BootLoader�����s
#          ++-- BOOTSZ[1:0] �u�[�g�̈�T�C�Y�i2kB�j
#
###############################################################################

�� ��H�}�Ȃ�

  Readme88.txt���������������B


�� ����̖��_

  bootloader �ŏ������񂾒���Ahidmon �� 0�` 0x1800 �Ԓn�����Ă� 0xff ����
  �\������Ȃ��s�������܂����B�ʂ� AVR ���C�^�[�œǂށA���邢�� USB ��
  �}���Ȃ����ƁA����ɏ������܂�Ă��邱�Ƃ͊m�F�ł��܂��B
  --> 08/11/02 kuga�l�̂������ɂ��C���ł��܂����B���肪�Ƃ��������܂����B

  test/ �ȉ��� hidmon88 �iATmega88/168 �p�̃��j�^�v���O�����j��[�߂Ă��܂��B
  ����͉��p�̈�œ��삷��R�[�h�̗�ł��B���̉��p�̈�ł́Abootloader �@�\
  �͎g���܂���B

  �R���p�C���� WinAVR-2006-0422.7z ���g�p���Ă��܂��B����ȊO�� WinAVR �ł�
  hex �̗e�ʂ� 2k ���I�[�o�[����\��������܂��B�i���Ȃ��Ƃ� gcc-3 �x�[�X
  �� WinAVR ���g���K�v������܂��j

  �͂��ɃI�[�o�[����ꍇ�� Vendor,Product ��������k�߂Ď������Ă��������B


�� �g���\��

  �ǂݏ������郁�����[�̎�ނ𑝂₷�B--> �ς�.
  POLL/GRAPH�R�}���h�̃T�|�[�g�B      --> �ς�.
  GO/REBOOT �R�}���h�̃T�|�[�g�B      --> �������̂�.

�� senshu�̍s������ƃ���

hidasp.c �͂��Ȃ�ύX���܂����B�K�؂ȍ����͎��Ȃ��Ǝv���܂��B�i���R�FISP
�@�\�Ɋւ��镔�����㔼�Ɉړ��������߁j

1. genserial.awk ��bootmon88�ɂ��g����悤�ɏC�����܂����B

2. hidmon88�R�}���h���C�����A-p:XXXX �ɂ��A�V���A���ԍ��w����\�ɂ��܂���

3. hidasp.c ���ŐV�ɂ��܂���

4. �N���X�^���i20MHz, 12MHz�j �̗����œ��삷�邱�Ƃ��m�F���܂���(m88��)

  avr-size main.bin (20MHz�̏ꍇ)
   text    data     bss     dec     hex filename
   1960       8      55    2023     7e7 main.bin
   
   
  avr-size main.bin(12MHz�̏ꍇ)
   text    data     bss     dec     hex filename
   2028       8      55    2091     82b main.bin

  AVR> bench(USB-HUB�L��)
  hid write start
  hid write end, 138000 bytes/5000 ms,  27600 bytes/s

  hid write start(USB-HUB�Ȃ�)
  hid write end, 138000 bytes/24000 ms,  5750 bytes/s

5. develop, bin �̃f�B���N�g����ǉ����Ahidmon,hidspx �Ɨގ��̍\���ɕύX�B

6. firmware����Makefile���C���i2008-11-10�j

   make MCU=m168 �̂悤�� make ����ƁAATmega168 �p�� main168.hex �𐶐����A
   �ȗ����ɂ́AATmega88 �p�� main88.hex �𐶐����܂��B
   
   F_CPU=12800000 ���w�肷��ƁARC���U��p�̃R�[�h�𐶐����܂��B
   F_CPU=12000000 ���w�肷��ƁA�N���X�^��(12MHz)�p�̃R�[�h�𐶐����܂��B
   F_CPU=20000000 ���w�肷��ƁA�N���X�^��(20MHz)�p�̃R�[�h�𐶐����܂��B

   �܂��Amake help �Ŏg����@�\�����X�g���܂��B�������ݗp�� AVR ���C�^�� 
   hidspx ���g���Ă���ꍇ�ɂ́A�ȉ��Ɏ����S�ẴR�}���h���@�\���܂��B

    make help
    ^^^^^^^^^
    make [MCU=m168] ... Build HEX file
    make [MCU=m168] readfuse ... Read FUSE info.
    make [MCU=m168] readflash ... Read flash
    make [MCU=m168] flash ... Write flash
    make [MCU=m168] fuse ... Write fuse
    make [MCU=m168] lock ... Write LOCKBIT
    make [MCU=m168] clean ... Clean project
    make [MCU=m168] install ... install HEX file

7. firmware�̃V���A�����
   HIDaspx�Ƃ̏d���������ׁA'0001'�ɂ��Ă��܂��B
   hidmon88 -p1 �Őڑ����Ă��������B(hidmon88�̏ȗ����̒l��'0001'�ɂ��܂���)

8. hidasp.c���C�����Ahidmon88���p���bootloadHID�ɐ�����s����������܂����B

9. iruka����̋��͂ɂ��AATmega168�ł�2kB�ȓ��Ɏ��܂�悤�ɂȂ�܂����B
   �኱�ł����A�󂫂�����܂��B

    avr-objcopy -j .text -j .data -O ihex main.bin main168.hex
    avr-size main.bin
      text    data     bss     dec     hex filename
      2032       2      55    2089     829 main.bin

10. vendor, product�̕�����̏ƍ��𖳌��ɂ��AVID, PID�ƃV���A�����̏ƍ����s���܂��B

  VID=0x16c0 PID=0x05df vendor="YCIT" product="HIDaspx" serial="0000"
  VID=0x16c0 PID=0x05df vendor="AVRetc" product="bmon" serial="0001"

11. test/�f�B���N�g���ɉ��p�̈�p��firm��p�ӂ��܂����B (iruka����̉���)

    hidmon88�̏ȗ����̃V���A�������u0000�v�ˁu0001�v�ɕύX
    >hidmon88�N�����ɁuTARGET DEV_ID=89�v��\��

12. ����RC���U���g���̎��������@�\���L���ɂȂ�܂����B

    kuga ����̃A�h�o�C�X�ɂ��Amega88/168 �̗����ŁA�������œ��삵�܂��B
    FUSE �ݒ���ȉ��Ɏ����܂��BPB0 �ւ̎��g���ݒ�͓���m�F�p�ł�����A����
    ���m�F�ł�����A�����ɂ��邱�Ƃ��ł��܂��B

    RC���U�p��FUSE�ݒ�(ATmega88/168���p)

	Low: 10100010 (0xA2)
	     ||||++++-- CKSEL[3:0] �V�X�e���N���b�N�I��
	     ||++-- SUT[1:0] �N������
	     |+-- CKOUT (0:PB0�ɃV�X�e���N���b�N���o��)
	     +-- CKDIV8 �N���b�N���������l (1:1/1, 0:1/8)
	
	High:11-11101 (0xDD)
	     |||||+++-- BODLEVEL[2:0] (111:��, 110:1.8V, 101:2.7V, 100:4.3V)
	     ||||+-- EESAVE (������EEPROM�� 1:����, 0:�ێ�)
	     |||+-- WDTON (1:WDT�ʏ퓮��, 0:WDT�펞ON)
	     ||+-- SPIEN (1:ISP�֎~, 0:ISP����) ��Parallel���̂�
	     |+-- DWEN (On-Chip�f�o�b�O 1:����, 0:�L��)
	     +-- RSTDISBL (RESET�s�� 1:�L��, 0:����(PC6))
	
	Ext: -----000 (0xF8)
	          ||+-- BOOTRST ���f�[�^�V�[�g�Q��
	          ++-- BOOTSZ[1:0] ���f�[�^�V�[�g�Q��


13. ���p�̈�i����RC���U���[�h�j�œ��삷��hidmon88�p�̃t�@�[����ǉ����܂����B

    iruka ����̍쐬�����A�[�J�C�u�ɓY�t����Ă��܂������A���̗���s����
    �s�\���Ȃ��̂ƂȂ��Ă��܂����Btest �f�B���N�g���ɂ́A���p�̈� (ATmega88
    �ł� 0000�` 17ff �܂Łj�œ��삷��v���O�����ł��B

    �����ŁAhidmon88 �̋@�\��L���ɂ��� HEX �t�@�C���𐶐��\�ɂ��܂����B
    �u�[�g���[�_�̉��p�̈�̎��s�@�\���g���΁AbootloadHID �ł��̃v���O����
    ���������ݎ��s�ł��܂����A���ꂪ�ʓ|�ȏꍇ�ɂ́AAVR ���C�^�ŏ������݁A
    FUSE ext �� 0xF9�i���[�g���[�_�@�\�𖳌����j �ɐݒ肵�Ă��������B

    main88-test-RC.hex   0000 - 099B
    main168-test-RC.hex  0000 - 09DF

    �Ȃ��A���p�̈�œ��삷��R�[�h�ɂ́A2kB �Ƃ��������͂���܂���B
    ATmega88 �� 8kB, ATmega168 �� 16kB �܂ł̃T�C�Y�̃R�[�h���쐬�\�ł��B

14. hidmon88��bench�R�}���h�ŌJ��Ԃ���������Ȑ��l�i��100����j���w�莞�A
    �\�����s���ɂȂ�̂��C��

 �� ����RC���U��ł̓���ɂ��āi���肢�j

    RC ���U���[�h�ł́A3.3V ���M�����[�^�Ȃǂň��艻�����d���œ��삳����K
    �v������܂��B�܂��AbootloadHID ���������Ă��܂����AFLASH �������̍X�V
    �ŃG���[����������ꍇ������A���p�I�ɂ���ɂ͉��ǂ��K�v�ł��B

15. RC���U���[�h�ł̓�������ǁikuga���񂩂�̒�āj

�Efirmware. test�̗����̃R�[�h�Ɉȉ��̏C�������{

  -usbconfig ���� osctune �}�N�����폜���A avrusb-20081022 �ɂ��� osctune.h
   �� include �ɕύX�B(osctune.h �� OSCCAL �ւ� 0x20 �̉��Z���폜)

  - 1 �T�C�N���O�̃^�C�}�[�̒l���o���Ă����K�v������̂ŁAmain.c ���� 
   lastTimer0Value �ϐ���ǉ��B

16. test���[�h�̓�����m�F���܂����i1125�ȑO�̔łɂ͕s�����j

test ���[�h�Ő��������t�@�[���E�G�A�́Ahidmon88 �@�\�ȊO�̃u�[�g���[�_�@
�\���c���Ă����ׁA����ɕs�������܂����B����́A���p�̈�ɏ������ރe�X
�g�ς݃t�@�[����p�ӂ��܂����B

hidmon88�ŕ\�������uTARGET DEV_ID=XX�v�̈Ӗ��́A�ȉ��̂Ƃ���ł��B

#define	DEV_ID_FUSION		0x55
#define	DEV_ID_STD		0x5a
#define	DEV_ID_MEGA88		0x88
#define	DEV_ID_MEGA88_USERMODE	0x89

������J����HEX�t�@�C���̃T�C�Y�B

�� bootloader + hidmon88
   text    data     bss     dec     hex filename
      0    2038       0    2038     7f6 main168-12.hex
      0    1970       0    1970     7b2 main168-20.hex

      0    2016       0    2016     7e0 main88-12.hex
      0    1948       0    1948     79c main88-20.hex

�� RC���U���[�h�p�ibootloader�̂݁j
   text    data     bss     dec     hex filename
      0    2010       0    2010     7da main88-RC.hex
      0    2022       0    2022     7e6 main168-RC.hex

�� hidmon88�̂݁i�e�X�g�p�F���p�̈�œ���j
   text    data     bss     dec     hex filename
      0    1968       0    1968     7b0 main88-test-20.hex
      0    2036       0    2036     7f4 main168-test-20.hex

      0    2384       0    2384     950 main88-test-RC.hex
      0    2452       0    2452     994 main168-test-RC.hex

RC ���[�h�ł�����m�F���s���Ă��܂��B�ȉ��� BAT �t�@�C���𗘗p����΁A��r
�I�ȒP�ɍ\���ł��܂��B�Ȃ��A����r���ł̔��U���̕ύX�𒆎~���܂����B

17. registor�����ĂɃ~�X������A�C�����܂����B
�܂��A�e�X�g�p��BAT�t�@�C����ǉ����܂����B

==> prog-m168-20.bat <==
@echo off
@echo ATmega168 setting FUSE (Xtal 20MHz mode).
hidspx -d10 -e
hidspx -d10 -FL0xFF -FH0xDD -FX0xF8
@echo Write bootloader and hidmon88 Firmware
hidspx -d1 main168-20.hex
echo "bootmonHID main168-test-20.hex"

==> prog-m168-rc.bat <==
@echo off
@echo ATmega168 setting FUSE (RC mode).
hidspx -d10 -e
hidspx -d10 -FL0xA2 -FH0xDD -FX0xF8
@echo Write Bootloader)not include hidmon88)
hidspx -d1 main168-RC.hex
echo "bootmonHID main168-test-RC.hex"

==> prog-m88-20.bat <==
@echo off
@echo ATmega88 setting FUSE (Xtal 20MHz mode).
hidspx -d10 -e
hidspx -d10 -FL0xFF -FH0xDD -FX0xF8
@echo Write Bootloader and hidmon88
hidspx -d1 main88-20.hex
@echo Write bootloader and hidmon88 Firmware
echo "bootmonHID main88-test-20.hex"

==> prog-m88-rc.bat <==
@echo off
@echo ATmega88 setting FUSE (RC mode).
hidspx -d10 -e
hidspx -d10 -FL0xA2 -FH0xDD -FX0xF8
@echo Write bootloader(not include hidmon88)
hidspx -d1 main88-RC.hex
echo "bootmonHID main88-test-RC.hex"

�� RC���U���[�h�̕]���@

1. ATmega88(168)������ȓd���œ��삳����
2. prog-m88-rc.bat���g���āA�t�@�[���E�F�A���������ށB
   �iXtal���ڑ�����Ă��Ă��ARC���U�ւ̐؂�ւ��͉\�ŁA���O���s�v�j
3. bootmonHID main88-test-RC.hex �ŉ��p�̈�Ƀe�X�g�p�̃��j�^����������
4. PD5��GND�ɐڑ����AUSB�R�l�N�^�𔲍����i���邢��AVR�}�C�R����RESET�j����
5. hidmon88 �Ń��j�^�����삷�邱�Ƃ��m�F����

  >hidmon88
   ^^^^^^^^
  TARGET DEV_ID=89  ... 89�́A���p�̈�œ����\���܂�
  AVR>

�� PD5 �̐؂�ւ����A���ʓ|�ł����AXtal �œ��삳����ꍇ�ɂ́A���j�^�@�\
   ���u�[�g�̈�Ɋ܂߂邱�Ƃ��o����̂ŁA�f�o�b�O���� PD5 �̐؂�ւ�����́A
   hidmon88 �Ŏw���\�ł��B�igo boot �Ɠ��͂���j

���̏ꍇ�ɂ́A�e�X�g�p�̃v���O�����Ƃ��� hidmon88 �p�̃t�@�[���E�F�A������
���݂܂������A�Ǝ��ɍ쐬�����v���O�������u�[�g�̈���������������imega88:6kB,
mega168:14kB�j�ɏ������ݎ��s�ł��܂��B�܂�A��x���̊���p�ӂł���ƁAAVR
���C�^�������Ƃ��A�v���O�����J�����\�ł��B�f�o�b�O�����������v���O�����́A
PD5 �� GND �ɂ���΁A���ʂȑ�����s��Ȃ��Ƃ� RESET ����Ɏ��s�ł��܂��B

=================================
�ǋL 2009-2-6:
  ���̃o�[�W�����́Abootmon-1127.zip ��WSN#172��ɑΉ����������̂ł��B
  X'tal�̎��g���� �eMakefile���Ŏw��ł��܂����A�ȗ��l�Ƃ���12MHz�ł�Hex
  �𐶐����Ă��܂��B

============== EOF ==============
