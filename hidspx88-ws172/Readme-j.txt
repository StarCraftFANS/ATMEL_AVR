                                                             2008�N11��12��

        USB�ڑ������̃h���C�o�C���X�g�[���s�v��AVR���C�^�iHIDaspx�j
        �P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P

�y0�z �͂��߂�

HIDaspx �́A�h���C�o�̃C���X�g�[�����s�v�� USB �ڑ������� AVR ���C�^�ł��B
�ʓr�񋟂���� hidmon �𗘗p����΁A�ėp�� USB-IO �Ƃ��Ă����p�ł��܂��B

�r�l������̍쐬���ꂽ HIDasp �����ɁAiruka ����� senshu �������ŉ��ǂ��s
�Ȃ��܂����B���̐��ʂ������܂����̂ŁA2008�N 9�� 22�������J���J�n����
���BHIDaspx �́A�r�l������̌��J���Ă�����̂Ƃ͓��e���قȂ�Afirmware, 
hidspx �R�}���h���Ɍ݊����͂���܂���̂ŋ�ʂ��Ĉ����Ă��������B

�� HIDaspx�́A�G�C�`�E�A�C�E�f�B�[ �A�X�y�b�N�X�Ƃ��ǂ݂��������B

�y0�z �A�[�J�C�u�̍\��

.\
����bin
��	libusb0.dll
��	hidspx-gcc.exe	�c MinGW-GCC�ŃR���p�C����������
��	hidspx.exe	�c Borland C++ ver 5.5.1�ŃR���p�C����������
��	hidspx.ini	�c hidspx�̏������t�@�C��
��	fuse.txt	�c FUSE�����ڍׂɕ\������ׂ̃e�L�X�g�t�@�C��
��	fuse_en.txt
��	fuse_j.txt
����bin
��  ����firmware	�c main.hex�iHIDaspx�p�j, main_libusb.hex�iMacOS, Linux�p�j
��			�c main-small.hex(USB-IO��p�̃t�@�[���E�F�A)
�b�@�@�@�@�@�@�@�@�@�@�@�c genserial.awk�i�V���A����񐶐��c�[���j
�b
����firmware		�c HIDasp�̃t�@�[���E�F�A�\�[�X
����src			�c hidspx�̃\�[�X(MinGW, Borland C++���p)
��  ����libusb
��      ����bin
��      ����examples
��      ����include
��      ����lib
��          ����bcc
��          ����dynamic
��          ����gcc
��          ����msvc
��          ����msvc_x64
����circuit		�c ��H�}


�y1�z hidspx�R�}���h�̃C���X�g�[�����@

�@hidspx�̃C���X�g�[���ɂ́A���ʂȃC���X�g�[���͕s�v�ł��B
bin �f�B���N�g���ɂ���t�@�C�������s�\�ȃf�B���N�g���ɃR�s�[���邱�Ƃŗ�
�p�\�ɂȂ�܂��B

**************************** �d�v ****************************
2007/03/20  11:33               43,520 libusb0.dll
2008/10/30  12:29              115,712 hidspx.exe
2008/10/30  12:29               67,584 hidspx-gcc.exe
2008/09/09  12:54               17,408 usbtool.exe
2008/09/05  10:39                  289 hidspx.ini
2007/10/31  23:01               19,207 fuse.txt
2007/10/31  23:00               21,160 fuse_en.txt
2007/10/31  23:01               19,207 fuse_j.txt
**************************** �d�v ****************************

�����̃t�@�C���́A���݂Ɋ֌W���Ă��܂��̂ŁAhidspx.exe �����łȂ��ꊇ����
�R�s�[���Ă��������B

�R�s�[�R���h���ׁAsetup.bat�t�@�C����p�ӂ��܂����B

 setup.bat �����s����� c:\hidmon �ɕK�v�ȃt�@�C�����R�s�[���܂��B
 setup c:\bin �̂悤�Ɏw�肷��΁Ac:\bin �ɕK�v�ȃt�@�C�����R�s�[���܂��B

���ϐ�������ł�����́Ahidspx\bin �Ƃ����f�B���N�g���� PATH ���ϐ��ɓo
�^����ƁA�Ȍ��ɃR�}���h���w��ł��֗����p�ł��܂��B

�Ȃ��A2 �̎��s�t�@�C���ihidspx-gcc.exe, hidspx.exe�j�̋@�\�͓����ł��B
1 �����ȏ�̃e�X�g���Ԓ��ɖ�肪�����������Ƃ���ABorland C++ �ŃR���p�C��
���� hidspx.exe ��W�����s�t�@�C���Ƃ��Č��J���܂��B����Ɉُ킪���鎞�ɂ́A
�]���̕W���ɂ��Ă��� hidspx-gcc.exe ���g���Ă݂Ă��������B

===========================�u�⑫�v===========================
���݂̂Ƃ���Ahidspx �̃R�}���h�����g���Ă��܂����A����� avrspx �̕ʖ��ł��B
�i���O�͔C�ӂɕύX�ł��܂����A�������Ȃ����O�����Ă��������B�ݒ�t�@�C��
�́A�R�}���h��.ini �ɂȂ�܂��B�j

avrspx �ł͂Ȃ��Ahidspx �ɂ��Ă���̂� ������ avrspx �̗��p�҂ɍ�����^����
���AHIDaspx �Ή��𖾎����邽�߂ł��Bhidspx �� avrspx �S�Ă̋@�\�𗘗p�ł��A
�����̉��ǂ��s���Ă��܂��B�������A�Ӑ}���Ȃ�����p�i�s��j��������\��
���F���Ƃ͂����܂���B���̂��߁A�\���ȓ��쌟�؂��I����܂ł́Aavrspx �̖���
�������Ɍ��J�܂��̂ŁA���������������B
==============================================================

�y2�z ����

bin/main.hex �� Tiny2313 �ɏ�������ł��������D�q���[�Y�ݒ�́C�O�t���� 12MHz
�N���X�^���ɍ��킹�A�ȉ��̂悤�ɐݒ肵�܂��B

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


�� ���܂ł́APD2����̃N���b�N�o�͂�L���ɂ��Ă��܂����B���̋@�\�𗘗p������
   �ꍇ�ɂ́ALow=0xBF �Ɛݒ肵�܂��B
   
   ���̔łł́APD2 �͊J���Œʏ�̃��C�^�AGND �Ɛڑ������ USB-IO �Ƃ��ċ@�\
   ���܂��B���C�^���[�h�ł� PB2 �iLED ���p) �[�q���� 1MHz �̋�`�g���o�͂�
   �܂��̂ŁAPD2 �i12MHz�j�����L�͈͂ɗ��p�ł��܂��B

�@hidspx �𗘗p�ł�����́A�H��o�׏�Ԃ� ATtiny2313 �ɑ΂��A�ȉ��̃R�}���h
�� FUSE �ݒ肪�\�ł��B�������A�O���ɔ��U���ڑ����Ă��Ȃ����ɂ��̐ݒ��
�s���ƁA���U���ڑ�����܂� ISP �����̃��C�^�ɂ��ǂݏ������o���Ȃ��Ȃ��
���̂ŁA�����ӂ��������B

> hidspx -d4 -fL0xff -fH0xdb -fX0xff <ENTER>


�y3�z �g����

�@�ȉ��ɊȒP�Ȏg�p����Љ�܂��B�ڍׂȎg�����́Aavrx-tool.txt���������������B

�� 1030�ňȍ~�́AFUSE Low��0xFF�ɐݒ肷�邱�Ƃ����E�߂��܂��B

�� USB-IO�Ƃ��ė��p����
USB-IO �Ƃ��ė��p����ꍇ�ɂ́APD2 �� GND �Ɍq���ł��������BUSB-IO �Ƃ��Ďg
���Ă��鑕�u�ɑ΂��Ahidspx �𗘗p����ƁA�ȉ��̂悤�ɃG���[�ɂȂ�܂��B
����́AUSB-IO �Ƃ��ė��p���Ă��� HIDaspx ���A����ăv���O���}�i���C�^�j��
���đ��삷��̂������ׂł��B

 >hidspx -rp
 Error: HIDaspx is USB-IO mode.
 HIDaspx(0000) not found.


�� AVR���C�^�iProgrammer�j�Ƃ��ė��p����
USB-IO �@�\�𗘗p���Ȃ��ꍇ�ɂ́APD2 ����̃n�[�h�E�F�A�ύX�͕s�v�ł��B
�Ȃ����C�^���[�h�ł́APB2 �� �^�[�Q�b�g�}�C�R���� �~�ϗp�N���b�N (1MHz) ��
���ċ@�\���邽�߁APB2(PWR LED) �� LED �͂�┖���_�����܂��B

hidspx �Łu-ph�v���w�肷��� HIDaspx �����p�ł��܂��BHIDaspx ��p�Ɏg���ꍇ
�ɂ́Ahidspx.ini �� -ph �������Ă����΁A���̎w��͏ȗ��ł��܂��B

> hidspx -ph -d4 test.hex

hidspx.ini �ł� -d4 ��ݒ肵�Ă��܂��B-d �Ńf�B���C���w��ł��A���l�͎��Ԃ�
��Ⴕ�Ēx���Ȃ�܂��B1MHz �� RC ���U���[�h�ł� -d4 �ȏ���w�肵�Ă��������B
���̒l�́A�����ɓ��삷�� AVR �}�C�R���ł͏����Ȓl���w��ł��A16MHz �ȏ�̎�
�g���œ��삷�� AVR �}�C�R���ɂ� -d0 ���w��ł��܂��B�w��l���������قǁAR/W
�̑��x�����サ�܂��B

-dN�̎w��̖ڈ��́A�ȉ��̂Ƃ���ł��B

No	FUSE Low	-d�̒l	���U���g��	���l
0	----	        -d0	16MHz�ȏ�	�O���N���X�^��/�Z���~�b�N���U�q
1	-fL11100100	-d1	8MHz		14CK+65ms
2	-fL11100010	-d2	4MHz		14CK+65ms
3	-fL01100100	-d4	1MHz		�H��o�גl
4	-fL01100010	-d5	500kHz		14CK+65ms
5	-fL11100110	-d17	128kHz		14CK+65ms
6	-fL01100110	-d120	16kHz		118, 119�ł͕s����


iruka ����ɂ��v�����ʂ������܂��B�i�ŐV�̔łł́A����������\�����サ
�Ă��邱�Ƃ�����܂��j

	�R�}���h	�]�����e		���ǑO	��������
-------------------------------------------------------------------------
hidspx -d0 -rp >XX.hex	8kB Read		8.39�b	2.22�b(3,690B/�b)
hidspx -d0 test.hex	8kB Write/Verify	10.94�b	4.86�b(1.64kB/�b)
hidspx -d1 -rp >XX.hex	8kB Read		8.40�b	3.12�b(2,625B/�b)
hidspx -d1 test.hex	8kB Write/Verify	10.96�b	7.06�b(1.13kB/�b)
hidspx -d4 -rp >XX.hex	8kB Read		8.39�b	4.63�b(1,770B/�b)
hidspx -d4 test.hex	8kB Write/Verify	13.26�b	9.98�b(821B/�b)
-------------------------------------------------------------------------

�y4�z HIDaspx�̍������iUSB-HUB���p�̑E�߁j

�@AT90S2313 �̂悤�ɁA�y�[�W�����݋@�\�̖��� AVR �}�C�R���ł͏����݂Ɏ��Ԃ�
������܂��BUSB 2.0 HUB ������ɐڑ����ď����݂��s�������ʂł��B

�� hidmon�ɂ��x���`�}�[�N����
AVR> bench
hid write start
hid write end, 38000 bytes/11172 ms,  3401 bytes/s

�� Write&Verify����
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:25.703

���ɁA1,000 �~�ȓ��Ŕ����� HUB ����āA���l�̌v�����s���Ă݂܂��B

�� hidmon�ɂ��x���`�}�[�N����
>hidmon
AVR> bench
hid write start
hid write end, 38000 bytes/2468 ms,  15397 bytes/s

�� Write&Verify����
>timeit hidspx -d2 2kb.hex
Elapsed Time:     0:00:09.546

HUB �����ɔ�ׂāA1/2 �ȉ��ɒZ�k����A10�b�����ŏ����݁��ƍ����������܂��B

���̂悤�ɋ��^�C�v�� AVR �}�C�R�������p�ł��܂����A�v���ȏ����݂���]�����
���ɂ́A�V�^�C�v�̃}�C�R�����̗p���Ă��������B�����������T�C�Y�� ATtiny2313
�ł� 1�b���x�Ŋ������܂��B

�����̌��ʂ���AUSB HUB �����ɂ悢�d�������Ă��邱�Ƃ��킩��܂��B���^
�C�v�� AVR �}�C�R���Ɍ��炸�A�قƂ�ǂ̏ꍇ�A�������x�͊i�i�Ɍ��サ�܂��B
hidmon �� 15kB/s �𒴂������p�ӂ���Α��x����ɂȂ���܂��BUSB-HUB ��
���肵�A�������ė��p���Ă݂Ă͂������ł��傤���B

===============================================================================

�y5�z ���C�Z���X�i�r�l������ɓ����j
AVR USB�ɏ����AGPL2�Ƃ��܂��D

�y6�z �ӎ�
�@HIDaspx �̊J���ɂ́A�����̕����狦�͂����������܂����B�����ɁA�S��芴��
�̈ӂ�\���܂��B

    1. �r�l������  �c�c HIDasp�̊J���҂ł��BHIDasp�Ȃ����Ă�HIDaspx�͂���܂���B
    2. iruka����   �c�c ��������iruka����̐��ʂł��B
    3. kuga����    �c�c ��AVR�̃T�|�[�g�ƃR���p�N�g���APD5�ɂ��Đڑ��@�\�B
    4. RAIN����    �c�c �V�A�C�R���̍쐬�Ɗe��̓���e�X�g�B
    5. ��i����    �c�c ������HIDaspx����ʂ��ė��p�\�Ȃ��Ƃ��m�F�B
    6. kuman����   �c�c �������璍�ڂ��Ă��������A�����̏��������������܂����B
    7. TAD����A�{�O����Amitu����Aakakusa����A���񂳂�Akawana����
                   �c�c ����񍐂�e��̃R�����g�����������܂����B

  ----- * ----- * ----- * ----- * ----- * ----- * ----- * ----- * -----

�y�t�^�z �J���Ҍ����̃���

[0] 2008-10-17�`10-22�̍�Ɠ��e

�@������� HIDaspx ������ PC �ɍ����������p�ł���@�\����������ׁA�V���A
���ԍ��̐����c�[���� hidspx, hidmon �̋@�\�g�����s���܂����B���̋@�\������
���邽�߁A�ȉ��̍�Ƃ��s���Ă��܂��B

 1. �V���A����񐶐��c�[���̍쐬�igawk�ŋL�q�j
 2. hidspx�R�}���h��-ph[:XXXX]�ɃV���A�����w��@�\��ǉ�
 3.          �V   �� -ph? �Ń��X�g�ł���@�\��ǉ�
 4. hidmon �ɂ����l�̋@�\��ǉ�
 5.          �V   �� -p? �Ń��X�g�ł���@�\��ǉ�
 6. hidmon.dll�� �V���A�������w��ł���@�\��ǉ�
 7. �h�L�������g�̌�����
 8. ���ۂ̊��ł̓���m�F

[2] firmware, hidspx-src �̗��p�@

 1. CMD prompt ��� make ���g����build����

 2. �w�b�_�t�@�C���Ȃǂ�ύX�����ꍇ�ɂ́Amake clean ���make���邱�ƁB
    �~�X�������ׁArebuild.bat ��p�ӂ��Ă���Brebuild �����s����ƁA����
    ��K�����s�ł���B�Ȃ��A��Ƃ��ς񂾂�Acleanup �����s����ƁA��ƃt�@
    �C�����ꊇ���č폜�ł���B

 3. �G���[�����������ꂽ�t�@�[���E�F�A����s�t�@�C���́Abin�̉��ɃR�s�[�����

 4. �X�V�R���h���ׁA�Ō��develop�f�B���N�g����build-all.bat�����s����

[3] �V���A�����̐����c�[���̎g����

HIDaspx �p�̃V���A���f�[�^�iHEX �t�@�C���j�𐶐�����c�[����p�ӂ��܂����B

�@���̃c�[���� gawk(GNU awk �Ƃ����C���^�[�v���^�j ���g���ē��삵�܂��BAVR
�}�C�R���̗��p�҂ɍL�����y���Ă��� WinAVR �ɂ́Agawk.exe ���t�����Ă��܂����A
gawk �𗘗p�ł��Ȃ����́A�o�O�����Ȃ��P�Ƃœ��삷��A�ؑ�����̈ڐA�� gawk
����肵�Ă��������B

2008�N7��4�� gawk 3.1.6 ���{��Ńo�C�i���o�Ofix
GAWK�̉��ǔł̓���� �� http://www.kt.rim.or.jp/~kbk/

http://www.kt.rim.or.jp/%7ekbk/cgi-bin/download.cgi?dl-file1=gawk-mbcs-win32-20080704.zip

���肵�� gawk-mbcs-win32-20080704.zip ��W�J���Agawk.exe �� hidspx.exe �Ɠ�
���f�B���N�g���ɃR�s�[���܂��B

 2007/10/22  15:50              280,351 ChangeLog
 2007/07/16  23:59               35,147 COPYING
 2008/07/02  16:47              189,434 mbc-diff
 2007/09/25  14:58               83,133 NEWS
 2008/07/03  16:51              729,088 gawk.exe
 2006/12/01  14:26               53,248 igawk.exe
 2008/07/03  17:22                9,588 readme.ja

1028 �ňȍ~�A�u-v adr= �v���w�肷�邱�ƂŁA�C�ӂ̃A�h���X�ɑ΂���V���A����
��𐶐��ł��A�܂��A�����̃V���A�������u���ɐ����\�ł��B

usage: gawk -f genserial.awk -v t={aspx|mon88} start-number  [end-number]
�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P

�g�p��1�F
 > gawk -f genserial.awk 0001

�̂悤�Ɏg���܂��B��������s����ƁA0001 �̃V���A���f�[�^�� HEX �t�@�C��
�i_0001.hex�j�𐶐��ł��܂��B

�g�p��2�F0001�`0100�̃V���A�����̃t�@�C�����ꊇ���Đ������܂��B
         �w��ł���l��1�`9999�܂ŁA����𒴂���ƃG���[�ɂȂ�܂��B

 > gawk -f genserial.awk 1 100


�g�p��3�F16�i����p�啶���̎w����\�ł��B�������͑啶���ɕϊ����܂��B

 > gawk -f genserial.awk ABCD

���̃t�@�C���� main.hex �̃t�@�[���ƂƂ��Ɉȉ��̂悤�ɏ������߂΁AHIDaspx 
�͂��̃V���A���ԍ������������̂ɂȂ�܂��B

 > hidspx main.hex _0001.hex

�� �擪������'_'��ǉ����܂����̂ŁA�ȉ��̂悤�ȑ��삪�\�ł��B
 > del _*.hex
 �ňꊇ���ăV���A�����p�̃t�@�C�����폜�ł��܂��B

�g�p��4�F
 > gawk -f genserial.awk -v t=mon88 0001

bootmon88�p�̃V���A�����𐶐����܂��B

�g�p��5�F
 > gawk -f genserial.awk -v t=1234 0001

1234(10�i��)�Ԓn����n�܂�4���̃V���A�����𐶐����܂��B


[3.1] hidspx�ł̃V���A�����w����@

������̈قȂ�V���A������������ HIDaspx ���g���ē���̊m�F���s���܂����B
���̋@�\�ɂ��A������ HIDaspx ����ʂ��ė��p�ł��܂��B�i��䂾���ŗ��p����
�ꍇ�ɂ́A�V���A�����ݒ�͕s�v�ł��B�j

> hidspx -ph:0001  �ihidspx -ph1 �ł������Ӗ��ɂȂ�܂��j

���̂悤�Ɏw�肷��ƕ����� HIDaspx ����ʂ��Ďg�����Ƃ��ł��܂��B�Ȃ��A-ph
�Ǝw��ŏ����l�� 0000 ��I�����܂��B�V���A������ݒ肵�Ȃ��ꍇ�ɂ́A�]��
�ʂ�̗��p���\�ł��B

>hidspx -ph -r
HIDaspx(0000) not found. ---- -ph �ł́u0000�v��T���܂�

>hidspx -ph? ---- �ڑ�����Ă��� HIDaspx��\�������܂�
Manufacturer: [YCIT], Product: [HIDaspx], serial number: [0123]

>hidspx -ph:1234 -r ---- �ԍ����w�肵��HIDaspx���g���܂�
HIDaspx(1234) not found. ---- 1234��HIDaspx�͑��݂��܂���

>hidspx -ph:0123 -r ---- �������w�肷��΁A���삵�܂�
Detected device is ATtiny2313.
Device Signature  = 1E-91-0A
Flash Memory Size = 2048 bytes
Flash Memory Page = 32 bytes x 64 pages
EEPROM Size       = 128 bytes

[4] �@�\�̕ύX�_

�@�����̊J���҂ɂ��J���͔��U�������ł����AHIDaspx �Ɋւ��ẮA��ՓI�H��
��肭�@�\���Ă��܂��B

       4454 Oct 16 20:19 Makefile
       4540 Oct 16 20:22 Makefile.unix
         52 Oct 16 20:31 cleanup.bat
        144 Oct 17 08:41 rebuild.bat
       1137 Oct 18 18:03 hidconfig.h
      15153 Oct 18 18:04 usbconfig.h

    ���܂� main.c �ɏ����Ă����u�@�\�I��p�̒�`�v���uhidconfig.h�v�ɕ������A
    ���̃\�[�X������Q�Ɖ\�ɂ��܂����B����ɂ���āAhidconfig.h �݂̂��C
    �����邾���ŁA�œK�ȃR�[�h�̍쐬���\�ɂȂ�܂��B

    ���̒ǉ�����bat�t�@�C���̊ȒP�Ȏg������������Ă����܂��B

    0. �ݒ�t���O�́Ahidconfig.h, usbconfig.h �ɐݒ肷��
       �imain.c �ɂ͐ݒ�t���O�͂���܂���j
    1. �ʏ�́Amake �����s����B
    2. �G���[�������Ȃ�A�������@�\���邱�Ƃ��킩��΁Arebuild�����s����ƁA
    clean��ɍăR���p�C�����A2��ނ�HEX�t�@�C���� ../bin/firmware�ɃR�s�[�����
    3. ��Ƃ��I���΁Acleanup�ɂč�ƃt�@�C�����폜����


    2 �� 3 �͈�ɂł��܂����A�t�@�C�����폜����R�}���h�́A��������̂���
    ��ƍl���Ă��܂��B���̕ύX�ŕs�s��������ꍇ�ɂ́A���w�E���������B

    �܂��Airuka ����������Ă��܂����A���݁A�S�Ă̋@�\��L���ɂ��Ă� 2044 
    �o�C�g�ł���A2048 �ȓ��Ɏ��܂��Ă��܂��B

    * senshu 2008-10-18 (�y) 20:23:54 New
      hidspx �� firmware �̐��������Ċm�F���Ă݂܂������ALED �̐���R�}���h
      �̗L���̔���� LED ����̃t�@�[���E�F�A�̕����Ɏ኱�̋^��_������܂��B

      �T�C�Y������Ȃ������̂ŁA�R�����g�A�E�g��������������̂ł����A����
      �͐������ALED ����� MONITOR �R�}���h�ɓ��ꂵ�AISP_CMD �͕K���g�����
      ���ɂ���̂��悢�ƍl���Ă��܂��B

      �F�X�����������ʁA�ȉ��̎d�l�ɂ������ƍl���Ă��܂��B�i1017�ł����l�j

      (1) ISP_CMD �͕K���g�ݍ���
      (2) ISP_CMD�ł�LED�̐���͖����i�t�@�[���ɔC����j
      �@�@�� ���C�^����v���O����(hidspx)�Ŏw�肵�Ă���������
      (3) 2��LED��PC���琧�䂷��ꍇ�AISP_CMD�ł͂Ȃ�MONITOR�@�\�𗘗p����

      ���̎d�l�ɂ���΁A�t�@�[�����R���p�N�g�Ɉێ��ł��A������������܂��B
      ���� 9���Ɍ��J�����t�@�[�����g���Ă���ꍇ�ł����A����͐V��������
      �ɍX�V�����肢���邱�ƂɂȂ�܂��B


[5] �Q�lURL

    * �r�l������(HIDasp�̊J����) �� http://www.binzume.net/library/avr_hidasp.html

    * iruka����̃T�C�g
	 http://hp.vector.co.jp/authors/VA000177/html/A3C8A3C9A3C4A3E1A3F3A3F0.html

    * kuman����̉�H�}���܂ށA���H���|�[�g(��ǂ������߂��܂�)
	http://www.geocities.jp/kuman2600/n6programmer.html#13 (10/12�ǋL����)

    * Objective Development Software GmbH �c AVR-USB�iAVR�}�C�R���p��USB�h���C�o�̃T�C�g�j
	http://www.obdev.at/products/avrusb/index.html


[6] �ύX����
2008-09-22 ... senshu��iruka���񂪉��������{	�������Ή����{

2008-09-24 ... senshu  firmware -d0�I�v�V�����̍œK��
               hidpsx�̃G���[���o���A�̃��b�Z�[�W�o�͂̏C���i���s���ĕ\���j

2008-09-28 ... hidspx�̃R���p�C���I�v�V������-Wall��ǉ����A�x�����b�Z�[�W��
               �΂���C�������{(senshu)
               firmware -d0�I�v�V������USBasp�݊��������߂�(iruka)
               MOSI, SCK �̋���������iHi-Z������j

2008-10-03 ...	- hidspx�̃��b�Z�[�W��HIDaspx�ɓ���
		- Borland C++ �ł̌x�����b�Z�[�W��}�~(avrspx.h���C��)
		- usbtool ��ǉ�
		- kuga����̃A�h�o�C�X�ɂ��AFirmware�T�C�Y��40�o�C�g�k��(1968�o�C�g)
		 avr-size --mcu=attiny2313 main.elf
		   text    data     bss     dec     hex filename
		   1966       4      85    2055     807 main.elf
		- USB��ProductID 0x5dc (libusb device) �Ƃ̋����������ׁA
		  0x5df(HID devide)�ɕύX
		- firmware�̕ύX �i���̂Ƃ���T�|�[�g���Ă���AVR�f�o�C�X�ł́j
		  page_addr �� 256 �ȉ��Ȃ̂ŁAuint16 ���� uint8 �ɍ~�i�B

2008-10-06 ...	- iruka�����10-05�܂ł̏C�����𔽉f�i�قړ������e�ł��j
		- -d2�ȏ�̒l���w�肵�����̕s��́Asenshu�̏C���~�X�ł����B
		 �iiruka����A��ς��萔�����|�����܂����j
		- ATtiny2313�̔F�����s�\�����������Ɋւ��ẮA���̃~�X�ɋN������
		 �V�[�P���X����������H�A���K�؂Ȃ��̂ɂȂ��Ă��܂�
				- firmware�̕ύX  page_addr �� uint16 �ɖ߂��B

2008-10-10 ...	- iruka�����10-09�܂ł̏C�����𔽉f�i�قړ������e�ł��j
		- 2008.10.9a delay_10us�֐����R���p�C�����ɂ���Ă͍œK�������
		  �����Ă����̂��C��. �i�A�Z���u،���Ń����C�g�j
		- 2008.10.9a '-d2'�ȏ�̒x���N���b�N����K����
		- 2008.10.9 ������: flow-control�𓱓�
		- 2008.10.9 12MHz�^�[�Q�b�g�� '-d0'���g����悤�ɕύX(SCLK=2MHz)
		- 2008.10.9 ispConnect/ispDisconnect�𓱓�

2008-10-12 ...	- kuga�����AT90S�V���[�Y�p��ISP�ڍs������ǉ��i90S�V���[�Y���T�|�[�g�j
		- ATtiny2313 ��FUSE�r�b�gSTU�r�b�g�̒l�ɂ���āA�F���Ɏ��s����̂��C��
		  �ihidspx����hwctrl.c spi_reset�֐���delay_ms(10)��ǉ����܂����j
		- FUSE verify�G���[���̕\����K����
		- FUSE ���������֐��̓K�����i�d�v�I�j

2008-10-14 ...	- firmware main.c�uUSICR=0;	/* SCK���|�[�g�ɖ߂��Ă��� */�v��L����
		-- USI�̏�������main�֐�����Ausi_trans�֐��Ɉړ�(SCK�������̌݊�������)
		- usbHidReportDescriptor�̑傫����9�o�C�g�k��(���� 2038�o�C�g)
		- hidmon�Ƃ̐��������l�����A���ɖ߂��܂���(���� 2048�o�C�g)
		- �\�[�X���̃R�����g���C���i�s�����������C���j

2008-10-17 ...	- firmware (HID�łɉ����Alibusb�ł�ǉ��j
		- rebuild.bat , cleanup.bat��ǉ�
		- avrx-tool.txt�i�ڂ����g�����̉���j��ǉ�

2008-10-20 ...	- firmware (HID�łɉ����Alibusb�ł�ǉ��j
		- -ph:XXXX �̃V���A�������T�|�[�g
		- firmware�\�[�X�̃N���[���A�b�v�����{
		- bin �t�H���_�ɃV���A����񐶐��c�[��(genserial.awk)��ǉ�
		- �V�A�C�R���ɕύX(RAIN����쐬)

2008-10-21 ...	- �V���A����񐶐��c�[���̋���
		- ��H�}�t�@�C�����X�V

2008-10-22 ...	- �V���A����񐶐��c�[���̋���(WinAVR�t����gawk�ł�����ɓ���)
		- �������̌�����

2008-10-23 ...	- �V���A����񐶐��c�[���̋���(�����A�p�啶���̗��p���\)
		- AVR-USB�̃h���C�o��2008-10-22�łɍX�V

2008-10-28 ...	- ���萫����̂��߁AHIDaspx�t�@�[���̋N������300m�b�̎��ԑ҂���}������
		- �������̉��M�i���������b�N�����j
		- USB-IO���[�h�̏�����ǉ�����(PORTB����̓��[�h�ɂ���)
		- hidmon�ɂĒ񋟂��Ă����t�@�[���E�G�A��small�ł����̔łɊ܂߂�
		- �\���Ȉ��萫���m�F�ł����̂ŁABorland C++ �ŃR���p�C���������̂� 
		  hidspx.exe�Ƃ��AMinGW-gcc �ŃR���p�C���������̂� hidspx-gcc.exe �Ƃ����B
		- �d�����ďo�͂���郁�b�Z�[�W��1�x�̂݁A�o�͂���悤�ɂ����B

2008-10-30 ...	- USB-IO �� Programmer���[�h����ʂ��Ĉ����悤�ɕύX���܂����B
		- firmware �� hidspx.exe �̗������X�V����K�v������܂��B
		- ��H�}���C�����܂���(PD2��PB2�̈�����ύX�����ׁj
		- HIDaspx�̃y�[�W��PDF�����A�t�������ɒǉ����܂����B

2008-10-31 ...	- �t���̐������ɕs�����������C�����܂����B
		- firmware ���� uint8_t => uchar�ɓ���iAVR-USB�ɍ��킹�܂����j

2008-11-05 ...	- develop�f�B���N�g���ɁAbuil-all.bat��p�ӂ����B
		- ���̃R�}���h�����s����ƁA���J�ɕK�v�ȑS�Ẵt�@�C���𐶐����܂��B
		  �i�}�E�X�̃_�u���N���b�N�ł��@�\���܂��j
		- -ph�w��ŁA0001��1�Ɠ��͉\�ɂ��܂����B���Ȍ��ȗ��p���\�ł��B
		- PORTD (PD6, PD1, PD0)����́i�v���A�b�v����j�Ƃ��ď�����
		  �iakakusa���񂩂�̗v�]�ɂ��ύX�j

2008-11-06 ...	- �V���A����񐶐��c�[����bootmon88�Ή��ɂ����B
		- hidasp.c ��bootmon88�Ƃ̋��p�ɂ����B
		- hidmon, bootmon88�ɍ��킹�邽�߁Ahidspx-src �f�B���N�g���� src �ɕύX�B
		- �ȈՃZ�b�g�A�b�v�c�[���isetup.bat�j��ǉ��B

2008-11-12 ...	- USB-IO���[�h�ŗ��p�����ꍇ�ɂ́A�G���[�Œ��f����̂ł͂Ȃ��A�x���ɗ��߂��B
                  >hidspx -rf
                   Warnning: Please check HIDaspx mode.
                - hidasp.c ���ŐV�ɍX�V
		- vendor, product �̕�����̏ƍ��𖳌��ɂ��AVID, PID �ƃV���A������
		  �ƍ����s���܂��B

		  [���R]
		  �ȉ��̂悤�ɁA�قȂ�vendor����v���_�N�g���ł����p���\�ɂ��邽�߁B

                  VID=0x16c0 PID=0x05df vendor="YCIT" product="HIDaspx" serial="0000"
                  VID=0x16c0 PID=0x05df vendor="AVRetc" product="bmon" serial="0001"

# TAB size = 8�ŕҏW���Ă��܂��B

