/* hidasp.c
 * original: binzume.net
 * modify: senshu , iruka
 * 2008-09-22 : for HIDaspx.
 * 2008-11-07 : for HIDaspx, hidmon, bootmon88
 */

#define HIDMON			1
#define DEBUG 		   	0		// for DEBUG

#define DEBUG_PKTDUMP  	0		// HID Report�p�P�b�g���_���v����.
#define DUMP_PRODUCT   	0		// ������,���i����\��.
#define	JTAG_DUMP		0		// JTAG�R�}���h��HEX�_���v.

#define CHECK_COUNT		1		// 4: Connect���� Ping test �̉�.
#define BUFF_SIZE	256

#include <windows.h>
#include <stdio.h>
#include "usbhid.h"
#include "monit.h"
#include "hidasp.h"
#include "firmware/hidcmd.h"
#include "portlist.h"

#ifndef __GNUC__
#pragma comment(lib, "setupapi.lib")
#endif

//
//	cmd.
//
enum {
	CMD_PING 	= HIDASP_TEST,
	CMD_SETPAGE = HIDASP_SET_PAGE,
	CMD_POKE = HIDASP_POKE,
	CMD_PEEK = HIDASP_PEEK,
};


//  obdev
static int MY_VID = 0x16c0;				/* 5824 in dec, stands for VOTI */
static int MY_PID = 0x05df;				/* 1503 in dec, obdev's free PID */

#if	HIDMON88
	#define	MY_Manufacturer	"AVRetc"
	#define	MY_Product		"bmon"
#elif (HIDASPX || HIDMON)
	#define	MY_Manufacturer	"YCIT"
	#define	MY_Product		"HIDaspx"
#else
	#define	MY_Manufacturer	"obdev.at"
	#define	MY_Product		"HIDBoot"
#endif

//	MY_Manufacturer,MY_Product ��define ���O���ƁAVID,PID�݂̂̏ƍ��ɂȂ�.
//	�ǂ��炩���͂����ƁA���̏ƍ����ȗ�����悤�ɂȂ�.
static int found_hidaspx;
static int serial_check_counts;

//extern 
int hidmon_mode=1;


// HID API (from w2k DDK)
_HidD_GetAttributes 		HidD_GetAttributes;
_HidD_GetHidGuid 			HidD_GetHidGuid;
_HidD_GetPreparsedData 		HidD_GetPreparsedData;
_HidD_FreePreparsedData 	HidD_FreePreparsedData;
_HidP_GetCaps 				HidP_GetCaps;
_HidP_GetValueCaps 			HidP_GetValueCaps;
_HidD_GetFeature 			HidD_GetFeature;
_HidD_SetFeature 			HidD_SetFeature;
_HidD_GetManufacturerString	HidD_GetManufacturerString;
_HidD_GetProductString 		HidD_GetProductString;
_HidD_GetSerialNumberString HidD_GetSerialNumberString;	// add by senshu

HINSTANCE hHID_DLL = NULL;		// hid.dll handle
HANDLE hHID = NULL;				// USB-IO dev handle
HIDP_CAPS Caps;

static	int dev_id      = 0;	// �^�[�Q�b�gID: 0x55 �������� 0x5a ���������e.
static	int have_isp_cmd = 0;	// ISP����̗L��.

//----------------------------------------------------------------------------
//--------------------------    Tables    ------------------------------------
//----------------------------------------------------------------------------
//  HID Report �̃p�P�b�g�̓T�C�Y���� 3��ޗp�ӂ���Ă���.
#if HIDMON88
	#define	REPORT_ID1			1	// 8  REPORT_COUNT(6)
	#define	REPORT_ID2			3	// 40 REPORT_COUNT(38)
	#define	REPORT_ID3			2	// 32 REPORT_COUNT(30)

	#define	REPORT_LENGTH1		7	// REPORT_COUNT(6)
	#define	REPORT_LENGTH2		39	// 40 REPORT_COUNT(38)
	#define	REPORT_LENGTH3		132	// REPORT_COUNT(131)

	//	�ő�̒��������� HID ReportID,Length
	#define	REPORT_IDMAX		REPORT_ID2
	#define	REPORT_LENGTHMAX	REPORT_LENGTH2
#else
	#define	REPORT_ID1			1	// 8  REPORT_COUNT(6)
	#define	REPORT_ID2			2	// 32 REPORT_COUNT(30)
	#define	REPORT_ID3			3	// 40 REPORT_COUNT(38)

	#define	REPORT_LENGTH1		7	// 8  REPORT_COUNT(6)
	#define	REPORT_LENGTH2		31	// 32 REPORT_COUNT(30)
	#define	REPORT_LENGTH3		39	// 40 REPORT_COUNT(38)

	#define	PAGE_WRITE_LENGTH	32	// Page Write�ł�32byte�P�ʂ̓]����S�|����.
									// Length5���7�o�C�g���Ȃ��l�ł���K�v������.

	//	�ő�̒��������� HID ReportID,Length
	#define	REPORT_IDMAX		REPORT_ID3
	#define	REPORT_LENGTHMAX	REPORT_LENGTH3
#endif


#if	DEBUG_PKTDUMP
static	void memdump(char *msg, char *buf, int len);
#endif
/*
 * wrapper for HidD_GetFeature / HidD_SetFeature.
 */
//----------------------------------------------------------------------------
/*
 *	HID�f�o�C�X���� HID Report ���擾����.
 *	�󂯎�����o�b�t�@�͐擪��1�o�C�g�ɕK��ReportID�������Ă���.
 *
 *	id �� Length �̑g�̓f�o�C�X���Œ�`���ꂽ���̂łȂ���΂Ȃ�Ȃ�.
 *
 *	�߂�l��HidD_GetFeature�̖߂�l( 0 = ���s )
 *
 */
static int hidRead(HANDLE h, char *buf, int Length, int id)
{
	int rc;
	buf[0] = id;
	rc = HidD_GetFeature(h, buf, Length);
#if	DEBUG_PKTDUMP
	memdump("RD", buf, Length);
	printf("id=%d Length=%d rc=%d\n",id,Length,rc);
#endif
	return rc;
}

int	hidReadPoll(char *buf, int Length, int id)
{
	int rc;
	buf[0] = id;
	rc = HidD_GetFeature(hHID, buf, Length);
#if	DEBUG_PKTDUMP
	memdump("RD", buf, Length);
#endif
	return rc;
}


/*
 *	HID�f�o�C�X�� HID Report �𑗐M����.
 *	���M�o�b�t�@�̐擪��1�o�C�g��ReportID �����鏈����
 *	���̊֐����ōs���̂ŁA�擪1�o�C�g��\�񂵂Ă�������.
 *
 *	id �� Length �̑g�̓f�o�C�X���Œ�`���ꂽ���̂łȂ���΂Ȃ�Ȃ�.
 *
 *	�߂�l��HidD_SetFeature�̖߂�l( 0 = ���s )
 *
 */
static int hidWrite(HANDLE h, char *buf, int Length, int id)
{
	int rc;
	buf[0] = id;
	rc = HidD_SetFeature(h, buf, Length);
#if	DEBUG_PKTDUMP
	memdump("WR", buf, Length);
#endif
	return rc;
}

/*
 *	hidWrite()���g�p���āA�f�o�C�X���� buf[] �f�[�^�� len �o�C�g����.
 *	�������������ʂ��āAReportID�������I������.
 *
 *	�߂�l��HidD_SetFeature�̖߂�l( 0 = ���s )
 *
 */
int	hidWriteBuffer(char *buf, int len)
{
	int report_id = 0;
	int length    = 0;

	if (len <= REPORT_LENGTH1) {
		length= REPORT_LENGTH1;report_id = REPORT_ID1;
	} else if (len <= REPORT_LENGTH2) {
		length= REPORT_LENGTH2;report_id = REPORT_ID2;
	} else if (len <= REPORT_LENGTH3) {
		length= REPORT_LENGTH3;report_id = REPORT_ID3;
	}

	if( report_id == 0) {
		// �K�؂Ȓ������I���ł��Ȃ�����.
		fprintf(stderr, "Error at hidWriteBuffer. len=%d\n",len);
#if DLL
		return 0;
#else
		exit(1);
		return 0;
#endif
	} else {
		return hidWrite(hHID, buf, length, report_id);
	}
}

/*
 *	hidRead()���g�p���āA�f�o�C�X������ buf[] �f�[�^�� len �o�C�g�擾����.
 *	�������������ʂ��āAReportID�������I������.
 *
 *	�߂�l��HidD_GetFeature�̖߂�l( 0 = ���s )
 *
 */
int	hidReadBuffer(char *buf, int len)
{
	int report_id = 0;
	int length    = 0;

	if (len <= REPORT_LENGTH1) {
		length= REPORT_LENGTH1;report_id = REPORT_ID1;
	} else if (len <= REPORT_LENGTH2) {
		length= REPORT_LENGTH2;report_id = REPORT_ID2;
	} else if (len <= REPORT_LENGTH3) {
		length= REPORT_LENGTH3;report_id = REPORT_ID3;
	}

	if( report_id == 0) {
		// �K�؂Ȓ������I���ł��Ȃ�����.
		fprintf(stderr, "Error at hidWriteBuffer. len=%d\n",len);
#if DLL
		return 0;
#else
		exit(1);
		return 0;
#endif
	} else {
		return hidRead(hHID, buf, length, report_id);
	}
}
/*
 *	hidWrite()���g�p���āA�f�o�C�X����4�o�C�g�̏��𑗂�.
 *	4�o�C�g�̓���� cmd , arg1 , arg2 , arg 3 �ł���.
 *  ReportID��ID1���g�p����.
 *
 *	�߂�l��HidD_SetFeature�̖߂�l( 0 = ���s )
 *
 */
int hidCommand(int cmd,int arg1,int arg2,int arg3)
{
	unsigned char buf[BUFF_SIZE];

	memset(buf , 0, sizeof(buf) );

	buf[1] = cmd;
	buf[2] = arg1;
	buf[3] = arg2;
	buf[4] = arg3;

	return hidWrite(hHID, buf, REPORT_LENGTH1, REPORT_ID1);
}

//
//	mask ��   0 �̏ꍇ�́A addr �� data0 ��1�o�C�g��������.
//	mask �� ��0 �̏ꍇ�́A addr �� data0 �� mask �̘_���ς���������.
//		�A���A���̏ꍇ�� mask bit�� 0 �ɂȂ��Ă��镔���ɉe����^���Ȃ��悤�ɂ���.
//
//	��:	PORTB �� 8bit �� data����������.
//		hidPokeMem( PORTB , data , 0 );
//	��:	PORTB �� bit2 ������ on
//		hidPokeMem( PORTB , 1<<2 , 1<<2 );
//	��:	PORTB �� bit2 ������ off
//		hidPokeMem( PORTB ,    0 , 1<<2 );
//
int hidPokeMem(int addr,int data0,int mask)
{
	unsigned char buf[BUFF_SIZE];
	memset(buf , 0, sizeof(buf) );

	buf[1] = HIDASP_POKE;
	buf[2] = 0;
	buf[3] = addr;
	buf[4] =(addr>>8);
	if( mask ) {
		buf[5] = data0 & mask;
		buf[6] = ~mask;
	}else{
		buf[5] = data0;
		buf[6] = 0;
	}
	return hidWrite(hHID, buf, REPORT_LENGTH1, REPORT_ID1);
}

int hidPeekMem(int addr)
{
	unsigned char buf[BUFF_SIZE];
	memset(buf , 0, sizeof(buf) );

	buf[1] = HIDASP_PEEK;
	buf[2] = 1;
	buf[3] = addr;
	buf[4] =(addr>>8);

	hidWrite(hHID, buf, REPORT_LENGTH1, REPORT_ID1);
	hidReadBuffer( buf, REPORT_LENGTH1 );
	return buf[1];
}

#if HIDMON88
static void hidSetDevCaps(void)
{
	hidCommand(HIDASP_GET_CAPS,0,0,0);	// bootloadHID���擾���ׂ������o�b�t�@�ɒu��.
}
#endif

#define	USICR			0x2d	//
#define	DDRB			0x37	// PB4=RST PB3=LED
#define	DDRB_WR_MASK	0xf0	// ����\bit = 1111_0000
#define	PORTB			0x38	// PB4=RST PB3=LED
#define	PORTB_WR_MASK	0		// 0 �̏ꍇ��MASK���Z�͏ȗ�����A������.

#define HIDASP_RST		0x10	// RST bit

/*
 *	LED�̐���. (hidasp.h)
#define HIDASP_RST_H_GREEN	0x18	// RST����,LED OFF
#define HIDASP_RST_L_BOTH	0x00	// RST���s,LED ON
#define HIDASP_SCK_PULSE 	0x80	// RST-L SLK-pulse ,LED ON	@@kuga
 */


static void hidSetStatus(int ledstat)
{
	static int once = 0;
	int ddrb;
	if (have_isp_cmd) {
		hidCommand(HIDASP_SET_STATUS,0,ledstat,0);	// cmd,portd(&0000_0011),portb(&0001_1111),0
	}else{
		if (once == 0 && !hidmon_mode) {
//			fprintf(stderr, "Warnning: Please update HIDaspx firmware.\n");
			fprintf(stderr, "Warnning: Please check HIDaspx mode.\n");
			once++;
		}

		if (hidmon_mode) {
			ddrb = 0xff;			// DDRB = 0xff : 1���o�̓s�� ������bit3-0�͉e�����Ȃ�.
			hidPokeMem(PORTB,ledstat,PORTB_WR_MASK);
			hidPokeMem(DDRB ,ddrb   ,DDRB_WR_MASK);
		} else if(ledstat & HIDASP_RST) {	// RST����.
			ddrb = 0x10;			// PORTB �S����.
			hidPokeMem(USICR,0      ,0);
			hidPokeMem(PORTB,ledstat,PORTB_WR_MASK);
			hidPokeMem(DDRB ,ddrb   ,DDRB_WR_MASK);
		}else{
			// RST��L�ɂ���.
			ddrb = 0xd0;			// DDRB 1101_1100 : 1���o�̓s�� ������bit3-0�͉e�����Ȃ�.
			hidPokeMem(USICR,0      ,0);
			hidPokeMem(DDRB ,ddrb   ,DDRB_WR_MASK);
			hidPokeMem(PORTB,ledstat,PORTB_WR_MASK);
			hidPokeMem(PORTB,ledstat|HIDASP_RST,PORTB_WR_MASK);	// RST�͂܂�Hi
			hidPokeMem(PORTB,ledstat,PORTB_WR_MASK);	// RST�͂��Ƃ�L��.
			hidPokeMem(USICR,0x1a   ,0);
		}
	}
}

////////////////////////////////////////////////////////////////////////
//             hid.dll �����[�h
static int LoadHidDLL()
{
	hHID_DLL = LoadLibrary("hid.dll");
	if (!hHID_DLL) {
#if 1
		fprintf(stderr, "Error at Load 'hid.dll'\n");
#else
		MessageBox(NULL, "Error at Load 'hid.dll'", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetAttributes = (_HidD_GetAttributes)GetProcAddress(hHID_DLL, "HidD_GetAttributes");
	if (!HidD_GetAttributes) {
#if 1
		fprintf(stderr, "Error at HidD_GetAttributes\n");
#else
		MessageBox(NULL, "Error at HidD_GetAttributes", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetHidGuid = (_HidD_GetHidGuid)GetProcAddress(hHID_DLL, "HidD_GetHidGuid");
	if (!HidD_GetHidGuid) {
#if 1
		fprintf(stderr, "Error at HidD_GetHidGuid\n");
#else
		MessageBox(NULL, "Error at HidD_GetHidGuid", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetPreparsedData =	(_HidD_GetPreparsedData)GetProcAddress(hHID_DLL, "HidD_GetPreparsedData");
	HidD_FreePreparsedData = (_HidD_FreePreparsedData)GetProcAddress(hHID_DLL, "HidD_FreePreparsedData");
	HidP_GetCaps = (_HidP_GetCaps)GetProcAddress(hHID_DLL, "HidP_GetCaps");
	HidP_GetValueCaps =	(_HidP_GetValueCaps) GetProcAddress(hHID_DLL, "HidP_GetValueCaps");

//
	HidD_GetFeature = (_HidD_GetFeature)GetProcAddress(hHID_DLL, "HidD_GetFeature");
	HidD_SetFeature = (_HidD_SetFeature)GetProcAddress(hHID_DLL, "HidD_SetFeature");
	HidD_GetManufacturerString = (_HidD_GetManufacturerString)GetProcAddress(hHID_DLL, "HidD_GetManufacturerString");
	HidD_GetProductString = (_HidD_GetProductString)GetProcAddress(hHID_DLL, "HidD_GetProductString");
	HidD_GetSerialNumberString = (_HidD_GetSerialNumberString)GetProcAddress(hHID_DLL, "HidD_GetSerialNumberString");

#if	DEBUG
	printf("_HidD_GetFeature= %x\n", (int) HidD_GetFeature);
	printf("_HidD_SetFeature= %x\n", (int) HidD_SetFeature);
#endif
	return 1;
}

////////////////////////////////////////////////////////////////////////
// �f�B�o�C�X�̏����擾
static void GetDevCaps()
{
	PHIDP_PREPARSED_DATA PreparsedData;
	HIDP_VALUE_CAPS *VCaps;
	char buf[1024];

	VCaps = (HIDP_VALUE_CAPS *) (&buf);

	HidD_GetPreparsedData(hHID, &PreparsedData);
	HidP_GetCaps(PreparsedData, &Caps);
	HidP_GetValueCaps(HidP_Input, VCaps, &Caps.NumberInputValueCaps,  PreparsedData);
	HidD_FreePreparsedData(PreparsedData);

#if DEBUG
	fprintf(stderr, " Caps.OutputReportByteLength = %d\n", Caps.OutputReportByteLength);
	fprintf(stderr, " Caps.InputReportByteLength = %d\n", Caps.InputReportByteLength);
#endif


#if	REPORT_LENGTH_OVERRIDE
	//������REPORT_COUNT�����݂���Ƃ��͉��LCaps�� 0 �̂܂܂Ȃ̂ŁA�㏑������.
	Caps.OutputReportByteLength = REPORT_LENGTH_OVERRIDE;
	Caps.InputReportByteLength = REPORT_LENGTH_OVERRIDE;
#endif
}

//----------------------------------------------------------------------------
/*
 * 	unicode �� ASCII�ɕϊ�.
 */
static char *uni_to_string(char *t, unsigned short *u)
{
	char *buf = t;
	int c;
	// short �� char �z��ɓ���Ȃ���.
	while (1) {
		c = *u++;
		if (c == 0)
			break;
		if (c & 0xff00)
			c = '?';
		*t++ = c;
	}

	*t = 0;
	return buf;
}

//----------------------------------------------------------------------------
/*  Manufacturer & Product name check.
 *  ���O�`�F�b�N : ����=1  ���s=0 �ǂݎ��s�\=(-1)
 */
static int check_product_string(HANDLE handle, const char *serial, int list_mode)
{
	static int first = 1;
	int i;
	unsigned short unicode[BUFF_SIZE*2];
	char string1[BUFF_SIZE];
	char string2[BUFF_SIZE];
	char string3[BUFF_SIZE];
	char tmp[2][BUFF_SIZE];

	Sleep(20);
	if (!HidD_GetManufacturerString(handle, unicode, sizeof(unicode))) {
		return -1;
	}
	uni_to_string(string1, unicode);

	Sleep(20);
	if (!HidD_GetProductString(handle, unicode, sizeof(unicode))) {
		return -1;
	}
	uni_to_string(string2, unicode);

	// �V���A���ԍ��̃`�F�b�N�������� (2010/02/12 13:24:08)
	serial_check_counts = 0;
	if (serial[0]=='*') {
		//extern 
		int  f_auto_retry=2;		/* for HIDaspx (Auto detect, Retry MAX) */

		for (i=0; i<f_auto_retry; i++) {
			serial_check_counts++;
			Sleep(20);
			if (!HidD_GetSerialNumberString(handle, unicode, sizeof(unicode))) {
				return -1;
			}
			uni_to_string(tmp[i%2], unicode);
			if ((i>0) && ((i%2) == 1)) {
				if (strcmp(tmp[0], tmp[1])==0) {
					strcpy(string3, tmp[0]);	// OK
				} else {
					return -1;
				}
			}
		}
		if (list_mode) {
			if (first) {
				fprintf(stderr,	"VID=%04x, PID=%04x, [%6s], [%7s], serial=[%s] (*)\n",
						MY_VID, MY_PID, string1,  string2, string3);
				first = 0;
			} else {
				fprintf(stderr,	"VID=%04x, PID=%04x, [%6s], [%7s], serial=[%s]\n",
						MY_VID, MY_PID, string1,  string2, string3);
			}
		}
	} else {
		Sleep(20);
		if (!HidD_GetSerialNumberString(handle, unicode, sizeof(unicode))) {
			return -1;
		}
		uni_to_string(string3, unicode);
	}


#ifdef	MY_Manufacturer
	if (strcmp(string1, MY_Manufacturer) != 0)
		return 0;	// ��v����
#endif

#ifdef	MY_Product
	if (strcmp(string2, MY_Product) != 0)
		return 0;	// ��v����
#endif

	found_hidaspx++;

	/* Check serial number */
	if (found_hidaspx) {
		if (strcmp(string3, serial) == 0)
			return 1;		//���v����.
		else if (strcmp(serial ,"*") == 0)
			return 1;		//���v����.
	}

	return 0;	// ��v����

}

////////////////////////////////////////////////////////////////////////
// HID�f�B�o�C�X�ꗗ����USB-IO������
static int OpenTheHid(const char *serial, int list_mode)
{
	int f, i, rc;
	ULONG Needed, l;
	GUID HidGuid;
	HDEVINFO DeviceInfoSet;
	HIDD_ATTRIBUTES DeviceAttributes;
	SP_DEVICE_INTERFACE_DATA DevData;
	PSP_INTERFACE_DEVICE_DETAIL_DATA DevDetail;
	//SP_DEVICE_INTERFACE_DETAIL_DATA *MyDeviceInterfaceDetailData;

	DeviceAttributes.Size = sizeof(HIDD_ATTRIBUTES);
	DevData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	HidD_GetHidGuid(&HidGuid);
#if 1							/* For vista */
	DeviceInfoSet =
		SetupDiGetClassDevs(&HidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#else
	DeviceInfoSet =
		SetupDiGetClassDevs(&HidGuid, "", NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#endif

	f = i = 0;
	while ((rc = SetupDiEnumDeviceInterfaces(DeviceInfoSet, 0, &HidGuid, i++, &DevData))!=0) {
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DevData, NULL, 0, &Needed, 0);
		l = Needed;
		DevDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA *) GlobalAlloc(GPTR, l + 4);
		DevDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DevData, DevDetail, l, &Needed, 0);

		hHID = CreateFile(DevDetail->DevicePath,
						  GENERIC_READ | GENERIC_WRITE,
						  FILE_SHARE_READ | FILE_SHARE_WRITE,
						  NULL, OPEN_EXISTING,
//                        FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING,
						  0, NULL);

		GlobalFree(DevDetail);

		if (hHID == INVALID_HANDLE_VALUE)	// Can't open a device
			continue;
		HidD_GetAttributes(hHID, &DeviceAttributes);

		// HIDasp���ǂ������ׂ�.
		if (DeviceAttributes.VendorID == MY_VID && DeviceAttributes.ProductID == MY_PID) {
			int rc;
			rc = check_product_string(hHID, serial, list_mode);
			if ( rc == 1) {
				f++;				// ��������
				// ���������X�g���� (2010/02/12 13:19:38)
				if (list_mode == 0) {
					break;			// �����F�����́A�ŏ��̌��𗘗p����
				}
			}
		} else {
			// ����������
			CloseHandle(hHID);
			hHID = NULL;
		}
	}
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	return f;
}

#if	DEBUG_PKTDUMP
//----------------------------------------------------------------------------
//  �������[�_���v.
void memdump(char *msg, char *buf, int len)
{
	int j;
	fprintf(stderr, "%s", msg);
	for (j = 0; j < len; j++) {
		fprintf(stderr, " %02x", buf[j] & 0xff);
		if((j & 0x1f)== 31)
			fprintf(stderr, "\n +");
	}
	fprintf(stderr, "\n");
}
#endif


int hidasp_list(char * exe_name)
{
	int r, rc;

	LoadHidDLL();
	found_hidaspx = 0;
	r = OpenTheHid("*", 1);
	if (r == 0) {
		rc = 1;
	} else {
		rc = 0;
	}
	if (found_hidaspx==0) {
		if (serial_check_counts != 0) {
			fprintf(stderr, "%s: HIDaspx(VID=%04x, PID=%04x) serial check error (%d).\n",
				exe_name, MY_VID, MY_PID, serial_check_counts);
		} else {
			fprintf(stderr, "%s: HIDaspx(VID=%04x, PID=%04x) isn't found.\n",
				exe_name, MY_VID, MY_PID);
		}
	}
	if (hHID_DLL) {
		FreeLibrary(hHID_DLL);
	}
	return rc;
}

//----------------------------------------------------------------------------
//  ������.
//----------------------------------------------------------------------------
void chg_vid_pid(int type)
{
	if (type) {	/* NON-free (pro mode) */
		MY_VID = 0x20A0;				/* VID */
		MY_PID = 0x410e;				/* PID */
	} else {
		MY_VID = 0x16c0;				/* 5824 in dec, stands for VOTI */
		MY_PID = 0x05df;				/* 1503 in dec, obdev's free PID */
	}
}

int hidasp_init(int type, const char *serial)
{
	unsigned char rd_data[BUFF_SIZE];
	int i, r, result;

	result = 0;

	chg_vid_pid(type);
	LoadHidDLL();
	if (OpenTheHid(serial, 0) == 0) {
#if DEBUG
		fprintf(stderr, "ERROR: fail to OpenTheHid(%s)\n", serial);
#endif
		return HIDASP_MODE_ERROR;
	}

	GetDevCaps();
//	Sleep(100);

#if DEBUG
	fprintf(stderr, "HIDaspx Connection check!\n");
#endif

	for (i = 0; i < CHECK_COUNT; i++) {
		hidCommand(HIDASP_TEST,(i),0,0);	// Connection test
		r = hidRead(hHID, rd_data ,REPORT_LENGTH1, REPORT_ID1);
#if DEBUG
		fprintf(stderr, "HIDasp Ping(%2d) = %d\n", i, rd_data[1]);
#endif
		if (r == 0) {
			fprintf(stderr, "Error: fail to Read().\n");
			return HIDASP_MODE_ERROR;
		}
		dev_id = rd_data[1];
		if((dev_id != DEV_ID_FUSION)
		 &&(dev_id != DEV_ID_STD)
		 &&(dev_id != DEV_ID_MEGA88)
		 &&(dev_id != DEV_ID_MEGA88_USERMODE)) {
			fprintf(stderr, "Error: fail to ping test. (id = %x)\n",dev_id);
			return HIDASP_MODE_ERROR;
		}

		if (hidmon_mode) {
			fprintf(stderr, "HIDASP:TARGET DEV_ID=%x\n",dev_id);
		}

		if (rd_data[2] != i) {
			fprintf(stderr, "Error: fail to ping test. %d != %d\n", rd_data[2] , i);
			return HIDASP_MODE_ERROR;
		}
	}
	if (!hidmon_mode) {
		hidCommand(HIDASP_SET_STATUS,0,HIDASP_RST_H_GREEN,0);	// RESET HIGH
	}
#if	HIDMON88
		hidCommand(HIDASP_BOOT_RWW,0,0,0);	// Read-While-Write-Section Read Enable.
#endif
	r = hidRead(hHID, rd_data ,REPORT_LENGTH1, REPORT_ID1);
	if( rd_data[1] == 0xaa ) {				// ISP�R�}���h(isp_enable)�����퓮�삵��.
		have_isp_cmd = HIDASP_ISP_MODE;		// ISP����OK.
		result |= HIDASP_ISP_MODE;
	} else if( rd_data[1] == DEV_ID_FUSION ) {
		result |= HIDASP_FUSION_OK;			// NEW firmware
	} else if( rd_data[1] == DEV_ID_STD ) {
		result &= ~HIDASP_FUSION_OK;		// OLD firmware
	} else if( rd_data[1] == DEV_ID_MEGA88 ) {		// USB-IO mode
		result |= HIDASP_USB_IO_MODE;		// ISP����NG.
	} else if( rd_data[1] == DEV_ID_MEGA88_USERMODE ) {		// USB-IO mode
		result |= HIDASP_USB_IO_MODE;		// ISP����NG.
	}
#if DEBUG
	if (result & HIDASP_ISP_MODE) {
		fprintf(stderr, "[ISP CMD] ");
	}
	if (result & HIDASP_FUSION_OK) {
		fprintf(stderr, "[FUSION] ");
	}
	if (result & HIDASP_USB_IO_MODE) {
		fprintf(stderr, "[USB-IO mode] ");
	}
	if (result == 0) {
		fprintf(stderr, "ISP CMD Not support.\n");
	} else {
		fprintf(stderr, "OK.\n");
	}
#endif
	return result;
}

//----------------------------------------------------------------------------
//  �I��.
//----------------------------------------------------------------------------
void hidasp_close()
{
#define HIDASP_NOP 			  0	//����� ISP�̃R�}���h�Ǝv����?

	if (hHID) {
		unsigned char buf[BUFF_SIZE];

		buf[0] = 0x00;
		buf[1] = HIDASP_NOP;
		buf[2] = 0x00;
		buf[3] = 0x00;
		hidasp_cmd(buf, NULL);					// AVOID BUG!
		if (!hidmon_mode) {
			hidSetStatus(HIDASP_RST_H_GREEN);		// RESET HIGH
		}
#if	HIDMON88
		hidSetDevCaps();
#endif
		CloseHandle(hHID);
	}
	if (hHID_DLL) {
		FreeLibrary(hHID_DLL);
	}
	hHID = NULL;
	hHID_DLL = NULL;
}

//----------------------------------------------------------------------------
//  ISP�R�}���h���s.
//----------------------------------------------------------------------------
int hidasp_cmd(const unsigned char cmd[4], unsigned char res[4])
{
	char buf[128];
	int r;

	memset(buf , 0, sizeof(buf) );
	if (res != NULL) {
		buf[1] = HIDASP_CMD_TX_1;
	} else {
		buf[1] = HIDASP_CMD_TX;
	}
	memcpy(buf + 2, cmd, 4);

	r = hidWrite(hHID, buf, REPORT_LENGTH1 , REPORT_ID1);
#if DEBUG
	fprintf(stderr, "hidasp_cmd %02X, cmd: %02X %02X %02X %02X ",
		buf[1], cmd[0], cmd[1], cmd[2], cmd[3]);
#endif

	if (res != NULL) {
		r = hidRead(hHID, buf, REPORT_LENGTH1 , REPORT_ID1);
		memcpy(res, buf + 1, 4);
#if DEBUG
		fprintf(stderr, " --> res: %02X %02X %02X %02X\n",
				res[0], res[1], res[2], res[3]);
#endif
	}

	return 1;
}


//----------------------------------------------------------------------------
//  �^�[�Q�b�g�}�C�R�����v���O�������[�h�Ɉڍs����.
//----------------------------------------------------------------------------
int hidasp_program_enable(int delay)
{
	unsigned char buf[BUFF_SIZE];
	unsigned char res[4];
	int i, rc;
	int tried;	//AVRSP �Ɠ��l�̃v���g�R�����̗p

	rc = 1;
	hidSetStatus(HIDASP_RST_H_GREEN);			// RESET HIGH
	hidSetStatus(HIDASP_RST_L_BOTH);			// RESET LOW (H PULSE)
	hidCommand(HIDASP_SET_DELAY,delay,0,0);		// SET_DELAY
	Sleep(30);				// 30

	for(tried = 0; tried < 3; tried++) {
		for(i = 0; i < 32; i++) {

			buf[0] = 0xAC;
			buf[1] = 0x53;
			buf[2] = 0x00;
			buf[3] = 0x00;
			hidasp_cmd(buf, res);

			if (res[2] == 0x53) {
				rc = 0;					// AVR�}�C�R���Ɠ������m�F
				goto hidasp_program_enable_exit;
			}
			if (tried < 2) {			// 2��܂ł͒ʏ�̓������@
				break;
			}
			// AT90S�p�̓������@�œ��������
			hidSetStatus(HIDASP_SCK_PULSE);			// RESET LOW SCK H PULSE shift scan point
		}
	}

	hidasp_program_enable_exit:
#if DEBUG
	if (rc == 0) {
		fprintf(stderr, "hidasp_program_enable() == OK\n");
	} else  {
		fprintf(stderr, "hidasp_program_enable() == NG\n");
	}
#endif
	return rc;
}




//----------------------------------------------------------------------------
int		ddrb  ;
int		pinb  ;
int		portb ;
/*********************************************************************
 *	������
 *********************************************************************
 */
int UsbInit(int verbose,int enable_bulk, char *serial)
{
	int type = 0;
	if(	hidasp_init(type,serial) & HIDASP_MODE_ERROR) {
		if (verbose) {
	    	fprintf(stderr, "error: [%s] device not found!\n", serial);
    	}
    	return 0;
	} else {
		ddrb  = portAddress("ddrb");
		pinb  = portAddress("pinb");
		portb = portAddress("portb");
		return 1;	// OK.
	}
}

/*********************************************************************
 *	�I��
 *********************************************************************
 */
int UsbExit(void)
{
	hidasp_close();
	return 0;
}

/*********************************************************************
 *	AVR�f�o�C�X�ɏ����R�}���h�𑗂��āA�K�v�Ȃ猋�ʂ��󂯎��.
 *********************************************************************
 *	cmdBuf   *cmd : �����R�}���h
 *	uchar    *buf : ���ʂ��󂯎��̈�.
 *  int reply_len : ���ʂ̕K�v�T�C�Y. (0�̏ꍇ�͌��ʂ��󂯎��Ȃ�)
 *�߂�l
 *		0	: ���s
 *	   !0   : ����
 */
static	int QueryAVR(cmdBuf *cmd,uchar *buf,int size,int reply_len)
{
	int rc = 0;
	int report_id = cmd->report_id;
	char *s;

	rc = hidWriteBuffer((char*)cmd , size);
	if(rc == 0) {
		printf("hidWrite error\n");
		exit(1);
	}
	if(reply_len) {
		rc = hidReadBuffer((char *)cmd , reply_len+1 );
		if(rc == 0) {
			//if(report_id != 4) 
			{
				printf("hidRead error %d\n",report_id);
				exit(1);
			}
		}
		s = (char*) cmd;
		memcpy(buf, s+1 ,reply_len);
	}
    return rc;
}
/*********************************************************************
 *	�^�[�Q�b�g���̃������[���e���擾����
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *	int           size :�ǂݎ��T�C�Y.
 *	unsigned char *buf :�󂯎��o�b�t�@.
 *	����: HID Report Length - 1 ��蒷���T�C�Y�͗v�����Ă͂Ȃ�Ȃ�.
 */
int dumpmem(int adr,int arena,int size,unsigned char *buf)
{
	cmdBuf cmd;

	memset(&cmd,0,sizeof(cmdBuf));
	cmd.cmd   = CMD_PEEK ;	//| arena;
	cmd.size  = size;
	cmd.adr[0]= adr;
	cmd.adr[1]= adr>>8;

	if( QueryAVR(&cmd,buf,REPORT_LENGTH1,1+size) == 0) return 0;	//���s.
	return size;
}

/*********************************************************************
 *	�^�[�Q�b�g���̃������[���e(1byte)���擾����
 *********************************************************************
 */
int peekmem(int adr,int arena)
{
	uchar buf[64];
	dumpmem(adr,arena,1,buf);
	return buf[0];
}

/*********************************************************************
 *	�^�[�Q�b�g���̃������[���e(1�o�C�g)������������.
 *********************************************************************
 *	int            adr :�^�[�Q�b�g���̃������[�A�h���X
 *	int          arena :�^�[�Q�b�g���̃������[���(���ݖ��g�p)
 *	int          data0 :�������݃f�[�^.      (OR)
 *	int          data1 :�������݃r�b�g�}�X�N.(AND)
 *����:
 *	�t�@�[�����̏������݃A���S���Y���͈ȉ��̂悤�ɂȂ��Ă���̂Œ���.

	if(data1) {	//�}�X�N�t�̏�������.
		*adr = (*adr & data1) | data0;
	}else{			//�ׂ���������.
		*adr = data0;
	}

 */
int	pokemem(int adr,int arena,int data0,int data1)
{
    cmdBuf cmd;
	char buf[16];	// dummy

	memset(&cmd,0,sizeof(cmdBuf));
	cmd.cmd   = CMD_POKE ;	//| arena;
	cmd.size  = 1;
	cmd.adr[0]= adr;
	cmd.adr[1]= adr>>8;
	cmd.data[0] = data0;
	cmd.data[1] = data1;
	return QueryAVR(&cmd,buf,REPORT_LENGTH1,0);
}

/*********************************************************************
 *	JTAG�R�}���h���s
 *********************************************************************
 */
static	uchar j_buf[128];
static	int	  j_index=0;
static	int	  j_anchor=0;

//#define	MAX_JCMD_SIZE	28
#define	MAX_JCMD_SIZE	(REPORT_LENGTH3-1)	//

/*********************************************************************
 *	JTAG�R�}���h���s
 *********************************************************************
 */
int jcmd_write_nb(uchar *stream,int size,uchar *result)
{
    cmdBuf cmd;
	char *t;
	int  rsize=0;
	if(result!=NULL) rsize = size;

#if	JTAG_DUMP
	{int i;
	printf("JTAG:");
	for(i=0;i<size;i++) {
		printf(" %02x",stream[i]);
	}
	printf("\n");
	}
#endif

	t = (char*)&cmd;
	memset(t,0,sizeof(cmdBuf));
	cmd.cmd   = HIDASP_JTAG_WRITE;
	memcpy(t+2,stream,size);

	return QueryAVR(&cmd,result,2+size,rsize);
}

/*********************************************************************
 *	
 *********************************************************************
 */
int jcmd_write_flash()
{
	if(	j_index ) {
		jcmd_write_nb(j_buf,j_index,NULL);
		j_index=0;
	}
	return 0;
}

/*********************************************************************
 *	JTAG�R�}���h��A�����鏈��.
 *********************************************************************
 */
int jcmd_add(uchar *stream,int size)
{
	int c1,c2;
	// ����A��: �����f�[�^����̏ꍇ.
	if(	j_index == 0) {
		j_anchor = 0;				//���O�̃R���g���[�����[�h�̏ꏊ.
		memcpy(j_buf,stream,size);	//�ۃR�s�[����.
		j_index = size;
		return size;
	}
	// ����łȂ��A��: �����f�[�^�����݂���ꍇ.
	c1 = j_buf[j_anchor];
	c2 = stream[0];
	if((c1 & 0x80) && (c2 & 0x80)) {	//�����Ƃ�bitbang�X�g���[���̏ꍇ�͂P�̃X�g���[���ɂ��Ă��܂�.
		memcpy(j_buf+j_index-1,stream+1,size-1);	// �����f�[�^��NUL�^�[�~�l�[�g�ʒu��bitbang���R�s�[.
		j_buf[j_anchor] = 0x80 | ((c1&0x7f)+(c2&0x7f));	//��������������.
		j_index += size-2;							// �ڑ�������NUL������bitbang����R�[�h���P�Â���̂�-2
		// anchor�̈ʒu�͕ς��Ȃ�.
		return j_index;
	}
	
	// ����łȂ��A��: bitbang���m�Ŗ����ꍇ.
	{
		j_anchor = j_index-1;
		memcpy(j_buf+j_anchor,stream,size);		// �����f�[�^��NUL�^�[�~�l�[�g�ʒu�ɃR�s�[.
		j_index += size-1;						// �ڑ�������NUL�������P����̂�-1
		return j_index;
	}
}

/*********************************************************************
 *	
 *********************************************************************
 */
int jcmd_write(uchar *stream,int size,uchar *result)
{
	//�ǂݏ����𔺂��ꍇ�̓o�b�t�@�s��.
	if(result) {
		jcmd_write_flash();
		return jcmd_write_nb(stream,size,result);
	}

	// �A����̃T�C�Y��MAX_JCMD_SIZE���z����̂��������Ă���ꍇ��
	//   ��������f���o��.
	if( (j_index+size)>=MAX_JCMD_SIZE ) {
		jcmd_write_flash();
	}

	// �A�������s����.
	jcmd_add(stream,size);
	return j_index;
}

/*********************************************************************
 *	
 *********************************************************************
 */
int get_portb(void)
{
	jcmd_write_flash();
	return peekmem(pinb,AREA_RAM);
}
/*********************************************************************
 *	
 *********************************************************************
 */
void set_portb(int d)
{
	pokemem(portb,AREA_RAM,d,0);
}
/*********************************************************************
 *	
 *********************************************************************
 */
void set_ddrb(int d)
{
	pokemem(ddrb,AREA_RAM,d,0);
}

/*********************************************************************
 *	
 *********************************************************************
 */
