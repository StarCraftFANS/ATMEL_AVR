/*********************************************************************
 *	H I D   B L A S T E R
 *********************************************************************
 *API:
 */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ERROR_OK					(0)
#define	LOG_ERROR(...)
#define	HAVE_STDINT_H

#include "helper/command.h"
#include "hidblast.h"

extern	struct jtag_interface hidblast_interface;

static	struct jtag_command **jtag_command_arg;

/*********************************************************************
 *	*jtag_command_arg �����擾
 *********************************************************************
 */
struct  jtag_command *get_command_arg(void)
{
	return *jtag_command_arg;
}
/*********************************************************************
 *	������
 *********************************************************************
 */
DLL_int get_if_spec(struct jtag_command **q)
{
	jtag_command_arg = q;
	return (int) &hidblast_interface;
}
/*********************************************************************
 *
 *********************************************************************
 */

/*
 * DLL�G���g���|�C���g�֐��̌^�ƓT�^�I�ȃX�P���g���R�[�h
 */
BOOL APIENTRY					/* int __stdcall */
DllMain(HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID pParam)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		/* �����ɃO���[�o���ϐ����̏������R�[�h������ */
		/* ��loaddll ��DLL�����[�h�������͂��������s����� */
		break;

	case DLL_PROCESS_DETACH:
		/* �����ɃO���[�o���ϐ����̌�n���R�[�h������ */
		/* ��freedll ��DLL���A�����[�h�������͂��������s����� */
		break;

	case DLL_THREAD_ATTACH:
		/* �����ɃX���b�h���ɕK�v�ȕϐ����̏������R�[�h������ */
		break;

	case DLL_THREAD_DETACH:
		/* �����ɃX���b�h���ɕK�v�ȕϐ����̌�n���R�[�h������ */
		break;
	}
	return TRUE;
}
/*********************************************************************
 *
 *********************************************************************
 */
