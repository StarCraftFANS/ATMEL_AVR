@echo off
REM 2008-11-13 (c) written by senshu
REM set COPY=echo (copy)
set COPY=copy

set PROJECT=hidmon
REM �R�s�[��̃f�B���N�g�����Z�b�g����
set TARGET=C:\bin

if "%1"=="" (
REM �����Ȃ��i�W���ݒ�j
) else (
REM HELP�̃`�F�b�N
  if "%1"=="HELP" goto HELP
  if "%1"=="?" goto HELP
  if "%1"=="help" goto HELP
  set TARGET=%1
)

echo ================= %PROJECT% ���Z�b�g�A�b�v���܂� ====================
echo %TARGET%�t�H���_ �ɃR�s�[����]����ꍇ�́A[Y]��啶���œ��͂̂��ƁB
SET ANS=n
SET /P ANS="���ɃZ�b�g�A�b�v�����܂���[Y/n]"
IF %ANS%==Y (
  GOTO START
) ELSE (
  GOTO WHERE
)

:WHERE
SET TARGET=
SET /P TARGET="��]����t�H���_������͂��Ă�������:"
SET ANS=n
SET /P ANS="%TARGET%�t�H���_�ɃZ�b�g�A�b�v�����܂����H [Y/n]"
IF %ANS%==Y (
  GOTO START
) ELSE (
  echo ================= %PROJECT% �̃Z�b�g�A�b�v�𒆒f ====================
  GOTO END
)

:START
echo �K�v�ȃt�@�C���Q��%TARGET%�ɃR�s�[���܂��B

REM Check
IF NOT EXIST %TARGET% ( 
  mkdir %TARGET%
)
echo ==== [COPY to %TARGET%] ====
echo hidmon88.exe, bootloadHID.exe
%COPY% hidmon88.exe %TARGET% > NUL
%COPY% bootloadHID.exe %TARGET% > NUL
echo ==== %PROJECT% setup finished ====
goto END

:HELP
  echo ����� %PROJECT%�̃Z�b�g�A�b�v�v���O�����ł���A�X�V���ɂ����p�ł��܂��B
  echo   setup [��]����t�H���_��]
  echo �̂悤�Ɏg���܂��B
  echo [��]����t�H���_��]���ȗ�����ƕW���Ƃ���%TARGET%�ɃR�s�[���܂��B

:END
pause