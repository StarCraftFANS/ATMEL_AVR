�� �T�v

   ����́AOpenOCD �� DLL �n�b�N�i�����j�ł��B

   �����A�O���� DLL ��u���āAJTAG�A�_�v�^�[�̃h���C�o�[�̕������������݂Ă��܂��B

�� ���݂̃X�e�[�^�X

   ���ɒᑬ�ł����A�Ƃ肠�����AATtiny2313���g�p����JTAG�A�_�v�^�[�������Ă��܂��B


�� ������

   WindowsXP��p���܂��B

   HIDaspx �n�[�h�E�F�A��p�ӂ��āA�t�@�[���E�F�A�̂݁A���L�̂��̂ɍ����ւ��Ă����܂��B

     hid_blaster/firmware/main-12.hex

   �����́A
   ATtiny2313        ARM��� JTAG�[�q
          PB7  ------------- TCK
          PB6  ------------- TDI
          PB5  ------------- TDO
          PB4  ------------- TMS
  �ƂȂ�܂��B����ȊO��nTRST�s���Ȃǂ͂܂��T�|�[�g���Ă��܂���B(USB-Blaster�Ɠ��l)
   (��L�S�{�̐��͒����ł����삵�܂������A100�����x�̒����R�����ăV���[�g���̂�h���悤�ɂ���
�@�@�ق������S�ł�)

   ARM����3.3V�Ȃ̂ŁAAVR���̓d���ɒ��ӂ��Ă��������B(ATtiny2313����3.3V�ɂ���K�v������܂�)

   hidblast/ �f�B���N�g���� ocd.bat ���邢�́A ocd2.bat ���N�����āAopenocd.exe�̓f���o�����b�Z�[�W
   ���m�F���邱�Ƃ��o���܂��B

   ����ɐڑ��o���Ă���悤�ł�����Atelnet�� localhost:4444 �Ԃɐڑ����āAOpenOCD�R�}���h�����s����
   �݂Ă��������B


�� �f�B���N�g���\��

 hid_blaster-+- �\�[�X.
             |
             +--helper\   �w�b�_�[�t�@�C��.
             +--jtag\     �w�b�_�[�t�@�C��.
             |
             +--firmware\ ATtiny2313���̃t�@�[���E�F�A (main-12.hex���Ă��܂�)
             |
             +--openocd_patch\  openocd�{�̑��쐬�p�̉����_


�� �v���O�����̍ăr���h���@

   WindowsXP���MinGW-gcc�R���p�C����p����make���Ă��������B
   make����ƁAhidblast.dll ���쐬����܂��B

   openocd.exe�{�̂��ăr���h������@�́A�ȉ���URL���Q�Ƃ��Ă��������B

-http://hp.vector.co.jp/authors/VA000177/html/2010-09.html
   
   ����̉��������\�[�X��openocd_patch/ �f�B���N�g���ɒu���Ă��܂��B

   Linux��ł̃r���h�I�v�V�����́A����Ȋ����ł��B
   $ ./configure \
       --build=i686-pc-linux-gnu \
       --host=i586-mingw32msvc \
       --enable-dummy

   �o���オ���� openocd.exe �{�̂́A�h���C�o�[�Ƃ��āA����f�B���N�g���ɑ��݂��� hidblast.dll ��
   �N�����ɌĂяo���܂��B(���݂��Ȃ���΁Adummy�h���C�o�[�݂̂��g�ݍ��܂�܂�)


�� ����̖��_

   �܂��x���ł��B
   HIDmon / HIDaspx �̋@�\���c���Ă͂��܂����A�R�[�h�T�C�Y�ߖ�ɂ�鑬�x�ቺ������܂��B

�� ���C�Z���X

   OpenOCD�̔z�z���C�Z���X�ɏ����܂��B


�� �W�]

   hidblast.dll �t�@�C����(���͂�)�����ւ��邾���ŁA����f�o�C�X���T�|�[�g�\�ɂȂ�܂��B

   hidblast.dll �̃G���g���[�|�C���g�́A
      DLL_int get_if_spec(struct jtag_command **q);
   �����ł��B������struct jtag_command **q��q�ɂ́Aopenocd�{�̂�jtag_command_queue�Ƃ���
   �O���[�o���ϐ��̃A�h���X��n���܂��B
   �߂�l�́A(int�ɂȂ��Ă��܂���) �h���C�o�[�L�q�\���̂̃A�h���X�ɂȂ�܂��B


