/*=============================================================================
*
*							�_�E�����[�h�^�A�b�v���[�h
*
===============================================================================
/ Copyright (C) 1997-2007 Sota. All rights reserved.
/
/ Redistribution and use in source and binary forms, with or without 
/ modification, are permitted provided that the following conditions 
/ are met:
/
/  1. Redistributions of source code must retain the above copyright 
/     notice, this list of conditions and the following disclaimer.
/  2. Redistributions in binary form must reproduce the above copyright 
/     notice, this list of conditions and the following disclaimer in the 
/     documentation and/or other materials provided with the distribution.
/
/ THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
/ IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
/ OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
/ IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
/ INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
/ BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF 
/ USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
/ ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
/ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
/ THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/============================================================================*/

/* ���̃\�[�X�͈ꕔ�AWS_FTP Version 93.12.05 �̃\�[�X���Q�l�ɂ��܂����B */
/* �X���b�h�̍쐬�^�I���Ɋւ��āA����a�쐬�̃p�b�`��g�ݍ��݂܂����B */

/*
	�ꕔ�A�������̂��߂̃R�[�h�ǉ� by H.Shirouzu at 2002/10/02
*/

#define	STRICT
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <mbstring.h>
#include <time.h>
#include <winsock.h>
#include <windowsx.h>
#include <commctrl.h>
#include <process.h>

#include "common.h"
#include "resource.h"

#define SET_BUFFER_SIZE

/* Add by H.Shirouzu at 2002/10/02 */
#undef BUFSIZE
#define BUFSIZE			(32 * 1024)
#define SOCKBUF_SIZE	(256 * 1024)
/* End */

#define TIMER_DISPLAY		1		/* �\���X�V�p�^�C�}��ID */
#define DISPLAY_TIMING		500		/* �\���X�V���� 0.5�b */

#define ERR_MSG_LEN			1024


/* �폜�m�F�_�C�A���O�̏�� */
typedef struct {
	int Cur;
	TRANSPACKET *Pkt;
} MIRRORDELETEINFO;


/*===== �v���g�^�C�v =====*/

static void DispTransPacket(TRANSPACKET *Pkt);
static void EraseTransFileList(void);
static ULONG WINAPI TransferThread(void *Dummy);
static int MakeNonFullPath(TRANSPACKET *Pkt, char *CurDir, char *Tmp);
static int DownLoadNonPassive(TRANSPACKET *Pkt, int *CancelCheckWork);
static int DownLoadPassive(TRANSPACKET *Pkt, int *CancelCheckWork);
static int DownLoadFile(TRANSPACKET *Pkt, SOCKET dSkt, int CreateMode, int *CancelCheckWork);
static void DispDownloadFinishMsg(TRANSPACKET *Pkt, int iRetCode);
static int DispUpDownErrDialog(int ResID, HWND hWnd, char *Fname);
static BOOL CALLBACK UpDownErrorDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static int SetDownloadResume(TRANSPACKET *Pkt, int ProcMode, LONGLONG Size, int *Mode, int *CancelCheckWork);
static BOOL CALLBACK NoResumeWndProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static int DoUpLoad(SOCKET cSkt, TRANSPACKET *Pkt);
static int UpLoadNonPassive(TRANSPACKET *Pkt);
static int UpLoadPassive(TRANSPACKET *Pkt);
static int UpLoadFile(TRANSPACKET *Pkt, SOCKET dSkt);
static int TermCodeConvAndSend(TERMCODECONVINFO *tInfo, SOCKET Skt, char *Data, int Size, int Ascii);
static void DispUploadFinishMsg(TRANSPACKET *Pkt, int iRetCode);
static int SetUploadResume(TRANSPACKET *Pkt, int ProcMode, LONGLONG Size, int *Mode);
static LRESULT CALLBACK TransDlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
static void DispTransferStatus(HWND hWnd, int End, TRANSPACKET *Pkt);
static void DispTransFileInfo(TRANSPACKET *Pkt, char *Title, int SkipButton, int Info);
static int GetAdrsAndPort(char *Str, char *Adrs, int *Port, int Max);
static int IsSpecialDevice(char *Fname);
static int MirrorDelNotify(int Cur, int Notify, TRANSPACKET *Pkt);
static BOOL CALLBACK MirrorDeleteDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static void SetErrorMsg(char *fmt, ...);

/*===== ���[�J���ȃ��[�N =====*/

static HANDLE hTransferThread;
static int fTransferThreadExit = FALSE;

static HANDLE hRunMutex;				/* �]���X���b�h���s�~���[�e�b�N�X */
static HANDLE hListAccMutex;			/* �]���t�@�C���A�N�Z�X�p�~���[�e�b�N�X */

static int TransFiles = 0;				/* �]���҂��t�@�C���� */
static TRANSPACKET *TransPacketBase = NULL;	/* �]���t�@�C�����X�g */

static int Canceled;		/* ���~�t���O YES/NO */
static int ClearAll;		/* �S�Ē��~�t���O YES/NO */

static int ForceAbort;		/* �]�����~�t���O */
							/* ���̃t���O�̓X���b�h���I��������Ƃ��Ɏg�� */

static LONGLONG AllTransSizeNow;	/* ����̓]���œ]�������T�C�Y */
static time_t TimeStart;	/* �]���J�n���� */

static int KeepDlg = NO;	/* �]�����_�C�A���O�������Ȃ����ǂ��� (YES/NO) */
static int MoveToForeground = NO;		/* �E�C���h�E��O�ʂɈړ����邩�ǂ��� (YES/NO) */

static char CurDir[FMAX_PATH+1] = { "" };
static char ErrMsg[ERR_MSG_LEN+7];

/*===== �O���Q�� =====*/

/* �ݒ�l */
extern int SaveTimeStamp;
extern int RmEOF;
// extern int TimeOut;
extern int FwallType;
extern int MirUpDelNotify;
extern int MirDownDelNotify;
extern int FolderAttr;
extern int FolderAttrNum;


/*----- �t�@�C���]���X���b�h���N������ ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

int MakeTransferThread(void)
{
	DWORD dwID;

	hListAccMutex = CreateMutex( NULL, FALSE, NULL );
	hRunMutex = CreateMutex( NULL, TRUE, NULL );

	ClearAll = NO;
	ForceAbort = NO;

	fTransferThreadExit = FALSE;
	hTransferThread = (HANDLE)_beginthreadex(NULL, 0, TransferThread, 0, 0, &dwID);
	if (hTransferThread == NULL)
		return(FAIL); /* XXX */

	return(SUCCESS);
}


/*----- �t�@�C���]���X���b�h���I������ ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void CloseTransferThread(void)
{
	Canceled = YES;
	ClearAll = YES;
	ForceAbort = YES;

	fTransferThreadExit = TRUE;
	while(WaitForSingleObject(hTransferThread, 10) == WAIT_TIMEOUT)
	{
		BackgrndMessageProc();
		Canceled = YES;
	}
	CloseHandle(hTransferThread);

	ReleaseMutex( hRunMutex );

	CloseHandle( hListAccMutex );
	CloseHandle( hRunMutex );
	return;
}


/*----- �]������t�@�C���������X�g�ɒǉ����� --------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		TRANSPACKET **Base : ���X�g�̐擪
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int AddTmpTransFileList(TRANSPACKET *Pkt, TRANSPACKET **Base)
{
	TRANSPACKET *Pos;
	TRANSPACKET *Prev;
	int Sts;

	Sts = FAIL;
	if((Pos = malloc(sizeof(TRANSPACKET))) != NULL)
	{
		memcpy(Pos, Pkt, sizeof(TRANSPACKET));
		Pos->Next = NULL;

		if(*Base == NULL)
			*Base = Pos;
		else
		{
			Prev = *Base;
			while(Prev->Next != NULL)
				Prev = Prev->Next;
			Prev->Next = Pos;
		}
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �]������t�@�C����񃊃X�g���N���A���� --------------------------------
*
*	Parameter
*		TRANSPACKET **Base : ���X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void EraseTmpTransFileList(TRANSPACKET **Base)
{
	TRANSPACKET *Pos;
	TRANSPACKET *Next;

	Pos = *Base;
	while(Pos != NULL)
	{
		Next = Pos->Next;
		free(Pos);
		Pos = Next;
	}
	*Base = NULL;
	return;
}


/*----- �]������t�@�C����񃊃X�g����P�̏�����菜�� --------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		TRANSPACKET **Base : ���X�g�̐擪
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int RemoveTmpTransFileListItem(TRANSPACKET **Base, int Num)
{
	TRANSPACKET *Pos;
	TRANSPACKET *Prev;
	int Sts;

	Sts = FAIL;
	Pos = *Base;
	if(Num == 0)
	{
		*Base = Pos->Next;
		free(Pos);
		Sts = SUCCESS;
	}
	else
	{
		while(Pos != NULL)
		{
			Prev = Pos;
			Pos = Pos->Next;
			if(--Num == 0)
			{
				Prev->Next = Pos->Next;
				free(Pos);
				Sts = SUCCESS;
				break;
			}
		}
	}
	return(Sts);
}


/*----- �]������t�@�C������]���t�@�C�����X�g�ɓo�^���� --------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void AddTransFileList(TRANSPACKET *Pkt)
{
	DispTransPacket(Pkt);

	WaitForSingleObject(hListAccMutex, INFINITE);

	if(AddTmpTransFileList(Pkt, &TransPacketBase) == SUCCESS)
	{
		if((strncmp(Pkt->Cmd, "RETR", 4) == 0) ||
		   (strncmp(Pkt->Cmd, "STOR", 4) == 0))
		{
			TransFiles++;
			PostMessage(GetMainHwnd(), WM_CHANGE_COND, 0, 0);
		}
	}
	ReleaseMutex(hListAccMutex);

	return;
}


/*----- �]���t�@�C������]���t�@�C�����X�g�ɒǉ����� ------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		TRANSPACKET **Base : ���X�g�̐擪
*
*	Return Value
*		�Ȃ�
*
*	Note
*		Pkt���̂����X�g�ɘA������
*----------------------------------------------------------------------------*/

void AppendTransFileList(TRANSPACKET *Pkt)
{
	TRANSPACKET *Pos;

	WaitForSingleObject(hListAccMutex, INFINITE);

	if(TransPacketBase == NULL)
		TransPacketBase = Pkt;
	else
	{
		Pos = TransPacketBase;
		while(Pos->Next != NULL)
			Pos = Pos->Next;
		Pos->Next = Pkt;
	}

	while(Pkt != NULL)
	{
		DispTransPacket(Pkt);

		if((strncmp(Pkt->Cmd, "RETR", 4) == 0) ||
		   (strncmp(Pkt->Cmd, "STOR", 4) == 0))
		{
			TransFiles++;
			PostMessage(GetMainHwnd(), WM_CHANGE_COND, 0, 0);
		}
		Pkt = Pkt->Next;
	}

	ReleaseMutex(hListAccMutex);
	return;
}


/*----- �]���t�@�C������\������ --------------------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DispTransPacket(TRANSPACKET *Pkt)
{
	if((strncmp(Pkt->Cmd, "RETR", 4) == 0) || (strncmp(Pkt->Cmd, "STOR", 4) == 0))
		DoPrintf("TransList Cmd=%s : %s : %s", Pkt->Cmd, Pkt->RemoteFile, Pkt->LocalFile);
	else if(strncmp(Pkt->Cmd, "R-", 2) == 0)
		DoPrintf("TransList Cmd=%s : %s", Pkt->Cmd, Pkt->RemoteFile);
	else if(strncmp(Pkt->Cmd, "L-", 2) == 0)
		DoPrintf("TransList Cmd=%s : %s", Pkt->Cmd, Pkt->LocalFile);
	else if(strncmp(Pkt->Cmd, "MKD", 3) == 0)
	{
		if(strlen(Pkt->LocalFile) > 0)
			DoPrintf("TransList Cmd=%s : %s", Pkt->Cmd, Pkt->LocalFile);
		else
			DoPrintf("TransList Cmd=%s : %s", Pkt->Cmd, Pkt->RemoteFile);
	}
	else
		DoPrintf("TransList Cmd=%s", Pkt->Cmd);
	return;
}


/*----- �]���t�@�C�����X�g���N���A���� ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void EraseTransFileList(void)
{
	TRANSPACKET *New;
	TRANSPACKET *Next;
	TRANSPACKET *NotDel;
	TRANSPACKET Pkt;

	NotDel = NULL;

	WaitForSingleObject(hListAccMutex, INFINITE);
	New = TransPacketBase;
	while(New != NULL)
	{
		/* �Ō��"BACKCUR"�͕K�v�Ȃ̂ŏ����Ȃ� */
		if(strcmp(New->Cmd, "BACKCUR") == 0)
		{
			if(NotDel != NULL)
				free(NotDel);
			NotDel = New;
			New = New->Next;
			NotDel->Next = NULL;
		}
		else
		{
			Next = New->Next;
			free(New);
			New = Next;
		}
	}
	TransPacketBase = NotDel;
	TransFiles = 0;
	PostMessage(GetMainHwnd(), WM_CHANGE_COND, 0, 0);
	ReleaseMutex(hListAccMutex);

	strcpy(Pkt.Cmd, "GOQUIT");
	AddTransFileList(&Pkt);
	return;
}


/*----- �]�����_�C�A���O�������Ȃ��悤�ɂ��邩�ǂ�����ݒ� --------------------
*
*	Parameter
*		int Sw : �]�����_�C�A���O�������Ȃ����ǂ��� (YES/NO)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void KeepTransferDialog(int Sw)
{
	KeepDlg = Sw;
	return;
}


/*----- ���ݓ]�������ǂ�����Ԃ� ----------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X (YES/NO=�]�����ł͂Ȃ�)
*----------------------------------------------------------------------------*/

int AskTransferNow(void)
{
	return(TransPacketBase != NULL ? YES : NO);
}


/*----- �]������t�@�C���̐���Ԃ� --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �]������t�@�C���̐�
*----------------------------------------------------------------------------*/

int AskTransferFileNum(void)
{
	return(TransFiles);
}


/*----- �]�����E�C���h�E��O�ʂɏo�� ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void GoForwardTransWindow(void)
{
	MoveToForeground = YES;
	return;
}


/*----- �]���\�P�b�g�̃J�����g�f�B���N�g������������ ------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void InitTransCurDir(void)
{
	strcpy(CurDir, "");
	return;
}


/*----- �t�@�C���]���X���b�h�̃��C�����[�v ------------------------------------
*
*	Parameter
*		void *Dummy : �g��Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static ULONG WINAPI TransferThread(void *Dummy)
{
	TRANSPACKET *Pos;
	HWND hWndTrans;
	char Tmp[FMAX_PATH+1];
	int CwdSts;
	int GoExit;
	int Down;
	int Up;
	int DelNotify;

	hWndTrans = NULL;
	Down = NO;
	Up = NO;
	GoExit = NO;
	DelNotify = NO;

	while((TransPacketBase != NULL) ||
		  (WaitForSingleObject(hRunMutex, 200) == WAIT_TIMEOUT))
	{
		if(fTransferThreadExit == TRUE)
			break;

		WaitForSingleObject(hListAccMutex, INFINITE);
		memset(ErrMsg, NUL, ERR_MSG_LEN+7);

		Canceled = NO;

		if(TransPacketBase != NULL)
		{
			ReleaseMutex(hListAccMutex);
			if(hWndTrans == NULL)
			{
				if((strncmp(TransPacketBase->Cmd, "RETR", 4) == 0) ||
				   (strncmp(TransPacketBase->Cmd, "STOR", 4) == 0) ||
				   (strncmp(TransPacketBase->Cmd, "MKD", 3) == 0) ||
				   (strncmp(TransPacketBase->Cmd, "L-", 2) == 0) ||
				   (strncmp(TransPacketBase->Cmd, "R-", 2) == 0))
				{
					hWndTrans = CreateDialog(GetFtpInst(), MAKEINTRESOURCE(transfer_dlg), HWND_DESKTOP, (DLGPROC)TransDlgProc);
					if(MoveToForeground == YES)
						SetForegroundWindow(hWndTrans);
					ShowWindow(hWndTrans, SW_SHOWNOACTIVATE);
				}
			}
			TransPacketBase->hWndTrans = hWndTrans;

			if(hWndTrans != NULL)
			{
				if(MoveToForeground == YES)
				{
					SetForegroundWindow(hWndTrans);
					MoveToForeground = NO;
				}
			}

			if(hWndTrans != NULL)
				SendMessage(hWndTrans, WM_SET_PACKET, 0, (LPARAM)TransPacketBase);

			/* �_�E�����[�h */
			if(strncmp(TransPacketBase->Cmd, "RETR", 4) == 0)
			{
				/* �s���ȃp�X�����o */
				if(CheckPathViolation(TransPacketBase) == NO)
				{
					/* �t���p�X���g��Ȃ����߂̏��� */
					if(MakeNonFullPath(TransPacketBase, CurDir, Tmp) == SUCCESS)
					{
						if(strncmp(TransPacketBase->Cmd, "RETR-S", 6) == 0)
						{
							/* �T�C�Y�Ɠ��t���擾 */
							DoSIZE(TransPacketBase->RemoteFile, &TransPacketBase->Size);
							DoMDTM(TransPacketBase->RemoteFile, &TransPacketBase->Time);
							strcpy(TransPacketBase->Cmd, "RETR ");
						}

						Down = YES;
//						if(DoDownLoad(AskTrnCtrlSkt(), TransPacketBase, NO) == 429)
//						{
//							if(ReConnectTrnSkt() == SUCCESS)
								DoDownLoad(AskTrnCtrlSkt(), TransPacketBase, NO, &Canceled);
//						}
					}
				}
			}
			/* �A�b�v���[�h */
			else if(strncmp(TransPacketBase->Cmd, "STOR", 4) == 0)
			{
				/* �t���p�X���g��Ȃ����߂̏��� */
				if(MakeNonFullPath(TransPacketBase, CurDir, Tmp) == SUCCESS)
				{
					Up = YES;
//					if(DoUpLoad(AskTrnCtrlSkt(), TransPacketBase) == 429)
//					{
//						if(ReConnectTrnSkt() == SUCCESS)
							DoUpLoad(AskTrnCtrlSkt(), TransPacketBase);
//					}
				}
			}
			/* �t�H���_�쐬�i���[�J���܂��̓z�X�g�j */
			else if(strncmp(TransPacketBase->Cmd, "MKD", 3) == 0)
			{
				DispTransFileInfo(TransPacketBase, MSGJPN078, FALSE, YES);

				if(strlen(TransPacketBase->RemoteFile) > 0)
				{
					/* �t���p�X���g��Ȃ����߂̏��� */
					CwdSts = FTP_COMPLETE;

					strcpy(Tmp, TransPacketBase->RemoteFile);
					if(ProcForNonFullpath(Tmp, CurDir, hWndTrans, 1) == FAIL)
					{
						ClearAll = YES;
						CwdSts = FTP_ERROR;
					}

					if(CwdSts == FTP_COMPLETE)
					{
						Up = YES;
						CommandProcTrn(NULL, "MKD %s", Tmp);
						/* ���łɃt�H���_������ꍇ������̂ŁA */
						/* �����ł̓G���[�`�F�b�N�͂��Ȃ� */

					if(FolderAttr)
						CommandProcTrn(NULL, "%s %03d %s", AskHostChmodCmd(), FolderAttrNum, Tmp);
					}
				}
				else if(strlen(TransPacketBase->LocalFile) > 0)
				{
					Down = YES;
					DoLocalMKD(TransPacketBase->LocalFile);
				}
			}
			/* �f�B���N�g���쐬�i��Ƀz�X�g���j */
			else if(strncmp(TransPacketBase->Cmd, "R-MKD", 5) == 0)
			{
				DispTransFileInfo(TransPacketBase, MSGJPN079, FALSE, YES);

				/* �t���p�X���g��Ȃ����߂̏��� */
				if(MakeNonFullPath(TransPacketBase, CurDir, Tmp) == SUCCESS)
				{
					Up = YES;
					CommandProcTrn(NULL, "%s%s", TransPacketBase->Cmd+2, TransPacketBase->RemoteFile);

					if(FolderAttr)
						CommandProcTrn(NULL, "%s %03d %s", AskHostChmodCmd(), FolderAttrNum, TransPacketBase->RemoteFile);
				}
			}
			/* �f�B���N�g���폜�i��Ƀz�X�g���j */
			else if(strncmp(TransPacketBase->Cmd, "R-RMD", 5) == 0)
			{
				DispTransFileInfo(TransPacketBase, MSGJPN080, FALSE, YES);

				DelNotify = MirrorDelNotify(WIN_REMOTE, DelNotify, TransPacketBase);
				if((DelNotify == YES) || (DelNotify == YES_ALL))
				{
					/* �t���p�X���g��Ȃ����߂̏��� */
					if(MakeNonFullPath(TransPacketBase, CurDir, Tmp) == SUCCESS)
					{
						Up = YES;
						CommandProcTrn(NULL, "%s%s", TransPacketBase->Cmd+2, TransPacketBase->RemoteFile);
					}
				}
			}
			/* �t�@�C���폜�i��Ƀz�X�g���j */
			else if(strncmp(TransPacketBase->Cmd, "R-DELE", 6) == 0)
			{
				DispTransFileInfo(TransPacketBase, MSGJPN081, FALSE, YES);

				DelNotify = MirrorDelNotify(WIN_REMOTE, DelNotify, TransPacketBase);
				if((DelNotify == YES) || (DelNotify == YES_ALL))
				{
					/* �t���p�X���g��Ȃ����߂̏��� */
					if(MakeNonFullPath(TransPacketBase, CurDir, Tmp) == SUCCESS)
					{
						Up = YES;
						CommandProcTrn(NULL, "%s%s", TransPacketBase->Cmd+2, TransPacketBase->RemoteFile);
					}
				}
			}
			/* �f�B���N�g���쐬�i��Ƀ��[�J�����j */
			else if(strncmp(TransPacketBase->Cmd, "L-MKD", 5) == 0)
			{
				DispTransFileInfo(TransPacketBase, MSGJPN082, FALSE, YES);

				Down = YES;
				DoLocalMKD(TransPacketBase->LocalFile);
			}
			/* �f�B���N�g���폜�i��Ƀ��[�J�����j */
			else if(strncmp(TransPacketBase->Cmd, "L-RMD", 5) == 0)
			{
				DispTransFileInfo(TransPacketBase, MSGJPN083, FALSE, YES);

				DelNotify = MirrorDelNotify(WIN_LOCAL, DelNotify, TransPacketBase);
				if((DelNotify == YES) || (DelNotify == YES_ALL))
				{
					Down = YES;
					DoLocalRMD(TransPacketBase->LocalFile);
				}
			}
			/* �t�@�C���폜�i��Ƀ��[�J�����j */
			else if(strncmp(TransPacketBase->Cmd, "L-DELE", 6) == 0)
			{
				DispTransFileInfo(TransPacketBase, MSGJPN084, FALSE, YES);

				DelNotify = MirrorDelNotify(WIN_LOCAL, DelNotify, TransPacketBase);
				if((DelNotify == YES) || (DelNotify == YES_ALL))
				{
					Down = YES;
					DoLocalDELE(TransPacketBase->LocalFile);
				}
			}
			/* �J�����g�f�B���N�g����ݒ� */
			else if(strcmp(TransPacketBase->Cmd, "SETCUR") == 0)
			{
				if(AskShareProh() == YES)
				{
					if(strcmp(CurDir, TransPacketBase->RemoteFile) != 0)
					{
						if(CommandProcTrn(NULL, "CWD %s", TransPacketBase->RemoteFile)/100 != FTP_COMPLETE)
						{
							DispCWDerror(hWndTrans);
							ClearAll = YES;
						}
					}
				}
				strcpy(CurDir, TransPacketBase->RemoteFile);
			}
			/* �J�����g�f�B���N�g����߂� */
			else if(strcmp(TransPacketBase->Cmd, "BACKCUR") == 0)
			{
				if(AskShareProh() == NO)
				{
					if(strcmp(CurDir, TransPacketBase->RemoteFile) != 0)
						CommandProcTrn(NULL, "CWD %s", TransPacketBase->RemoteFile);
					strcpy(CurDir, TransPacketBase->RemoteFile);
				}
			}
			/* �����I���̂��߂̒ʒm */
			else if(strcmp(TransPacketBase->Cmd, "GOQUIT") == 0)
			{
				GoExit = YES;
			}

			/*===== �P�̏����I��� =====*/

			if(ForceAbort == NO)
			{
				WaitForSingleObject(hListAccMutex, INFINITE);
				if(ClearAll == YES)
					EraseTransFileList();
				else
				{
					if((strncmp(TransPacketBase->Cmd, "RETR", 4) == 0) ||
					   (strncmp(TransPacketBase->Cmd, "STOR", 4) == 0))
					{
						TransFiles--;
						PostMessage(GetMainHwnd(), WM_CHANGE_COND, 0, 0);
					}
					Pos = TransPacketBase;
					TransPacketBase = TransPacketBase->Next;
					free(Pos);
				}
				ClearAll = NO;
				ReleaseMutex(hListAccMutex);

				if(BackgrndMessageProc() == YES)
				{
					WaitForSingleObject(hListAccMutex, INFINITE);
					EraseTransFileList();
					ReleaseMutex(hListAccMutex);
				}
			}
		}
		else
		{
			DelNotify = NO;

			ReleaseMutex(hListAccMutex);
			if(KeepDlg == NO)
			{
				if(hWndTrans != NULL)
				{
					DestroyWindow(hWndTrans);
					hWndTrans = NULL;

					if(GoExit == YES)
					{
						SoundPlay(SND_TRANS);

						if(AskAutoExit() == NO)
						{
							if(Down == YES)
								PostMessage(GetMainHwnd(), WM_REFRESH_LOCAL_FLG, 0, 0);
							if(Up == YES)
								PostMessage(GetMainHwnd(), WM_REFRESH_REMOTE_FLG, 0, 0);
						}
						Down = NO;
						Up = NO;
					}
				}
			}
			BackgrndMessageProc();

			if(GoExit == YES)
			{
				PostMessage(GetMainHwnd(), WM_COMMAND, MAKEWPARAM(MENU_AUTO_EXIT, 0), 0);
				GoExit = NO;
			}
		}
	}
	return 0;
}


/*----- �t���p�X���g��Ȃ��t�@�C���A�N�Z�X�̏��� ------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���p�P�b�g
*		char *Cur : �J�����g�f�B���N�g��
*		char *Tmp : ��Ɨp�G���A
*
*	Return Value
*		int �X�e�[�^�X(SUCCESS/FAIL)
*
*	Note
*		�t���p�X���g��Ȃ����́A
*			���̃��W���[������ CWD ���s�Ȃ��A
*			Pkt->RemoteFile �Ƀt�@�C�����̂ݎc���B�i�p�X���͏����j
*----------------------------------------------------------------------------*/

static int MakeNonFullPath(TRANSPACKET *Pkt, char *Cur, char *Tmp)
{
	int Sts;

	Sts = ProcForNonFullpath(Pkt->RemoteFile, Cur, Pkt->hWndTrans, 1);
	if(Sts == FAIL)
		ClearAll = YES;

	return(Sts);
}




/*----- �_�E�����[�h���s�Ȃ� --------------------------------------------------
*
*	Parameter
*		SOCKET cSkt : �R���g���[���\�P�b�g
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		int DirList : �f�B���N�g�����X�g�̃_�E�����[�h(YES/NO)
*
*	Return Value
*		int �����R�[�h
*
*	Note
*		���̃��W���[���́A�t�@�C���ꗗ�̎擾�Ȃǂ��s�Ȃ��ۂɃ��C���̃X���b�h
*		������Ă΂��B���C���̃X���b�h����Ă΂�鎞�� Pkt->hWndTrans == NULL�B
*----------------------------------------------------------------------------*/

int DoDownLoad(SOCKET cSkt, TRANSPACKET *Pkt, int DirList, int *CancelCheckWork)
{
	int iRetCode;
	char Reply[ERR_MSG_LEN+7];

	Pkt->ctrl_skt = cSkt;
	if(IsSpecialDevice(GetFileName(Pkt->LocalFile)) == YES)
	{
		iRetCode = 500;
		SetTaskMsg(MSGJPN085, GetFileName(Pkt->LocalFile));
		DispDownloadFinishMsg(Pkt, iRetCode);
	}
	else if(Pkt->Mode != EXIST_IGNORE)
	{
		if(Pkt->Type == TYPE_I)
			Pkt->KanjiCode = KANJI_NOCNV;

		iRetCode = command(Pkt->ctrl_skt, Reply, CancelCheckWork, "TYPE %c", Pkt->Type);
		if(iRetCode/100 < FTP_RETRY)
		{
			if(Pkt->hWndTrans != NULL)
			{
				AllTransSizeNow = 0;

				if(DirList == NO)
					DispTransFileInfo(Pkt, MSGJPN086, TRUE, YES);
				else
					DispTransFileInfo(Pkt, MSGJPN087, FALSE, NO);
			}

			if(BackgrndMessageProc() == NO)
			{
				if(AskPasvMode() != YES)
					iRetCode = DownLoadNonPassive(Pkt, CancelCheckWork);
				else
					iRetCode = DownLoadPassive(Pkt, CancelCheckWork);
			}
			else
				iRetCode = 500;
		}
		else
			SetErrorMsg(Reply);
	}
	else
	{
		DispTransFileInfo(Pkt, MSGJPN088, TRUE, YES);
		SetTaskMsg(MSGJPN089, Pkt->RemoteFile);
		iRetCode = 200;
	}
	return(iRetCode);
}


/*----- �ʏ탂�[�h�Ńt�@�C�����_�E�����[�h ------------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

static int DownLoadNonPassive(TRANSPACKET *Pkt, int *CancelCheckWork)
{
	int iRetCode;
	int iLength;
	SOCKET data_socket = INVALID_SOCKET;   // data channel socket
	SOCKET listen_socket = INVALID_SOCKET; // data listen socket
	char Buf[1024];
	int CreateMode;
	struct sockaddr_in saSockAddr1;
	char Reply[ERR_MSG_LEN+7];

	if((listen_socket = GetFTPListenSocket(Pkt->ctrl_skt, CancelCheckWork)) != INVALID_SOCKET)
	{
		if(SetDownloadResume(Pkt, Pkt->Mode, Pkt->ExistSize, &CreateMode, CancelCheckWork) == YES)
		{
			sprintf(Buf, "%s%s", Pkt->Cmd, Pkt->RemoteFile);
			iRetCode = command(Pkt->ctrl_skt, Reply, CancelCheckWork, "%s", Buf);
			if(iRetCode/100 == FTP_PRELIM)
			{
				if(SocksGet2ndBindReply(listen_socket, &data_socket) == FAIL)
				{
					iLength = sizeof(saSockAddr1);
					data_socket = do_accept(listen_socket, (struct sockaddr *)&saSockAddr1, (int *)&iLength);

					if(shutdown(listen_socket, 1) != 0)
						ReportWSError("shutdown listen", WSAGetLastError());
					listen_socket = DoClose(listen_socket);

					if(data_socket == INVALID_SOCKET)
					{
						SetErrorMsg(MSGJPN280);
						ReportWSError("accept", WSAGetLastError());
						iRetCode = 500;
					}
					else
						DoPrintf("Skt=%u : accept from %s port %u", data_socket, inet_ntoa(saSockAddr1.sin_addr), ntohs(saSockAddr1.sin_port));
				}

				if(data_socket != INVALID_SOCKET)
				{
					iRetCode = DownLoadFile(Pkt, data_socket, CreateMode, CancelCheckWork);
//					data_socket = DoClose(data_socket);
				}
			}
			else
			{
				SetErrorMsg(Reply);
				SetTaskMsg(MSGJPN090);
				listen_socket = DoClose(listen_socket);
				iRetCode = 500;
			}
		}
		else
			iRetCode = 500;
	}
	else
	{
		iRetCode = 500;
		SetErrorMsg(MSGJPN279);
	}
	DispDownloadFinishMsg(Pkt, iRetCode);

	return(iRetCode);
}


/*----- Passive���[�h�Ńt�@�C�����_�E�����[�h ---------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

static int DownLoadPassive(TRANSPACKET *Pkt, int *CancelCheckWork)
{
	int iRetCode;
	SOCKET data_socket = INVALID_SOCKET;   // data channel socket
	char Buf[1024];
	int CreateMode;
	char Adrs[20];
	int Port;
	int Flg;
	char Reply[ERR_MSG_LEN+7];

	iRetCode = command(Pkt->ctrl_skt, Buf, CancelCheckWork, "PASV");
	if(iRetCode/100 == FTP_COMPLETE)
	{
		if(GetAdrsAndPort(Buf, Adrs, &Port, 19) == SUCCESS)
		{
			if((data_socket = connectsock(Adrs, Port, MSGJPN091, CancelCheckWork)) != INVALID_SOCKET)
			{
				if(setsockopt(data_socket, IPPROTO_TCP, TCP_NODELAY, (LPSTR)&Flg, sizeof(Flg)) == SOCKET_ERROR)
					ReportWSError("setsockopt", WSAGetLastError());

				if(SetDownloadResume(Pkt, Pkt->Mode, Pkt->ExistSize, &CreateMode, CancelCheckWork) == YES)
				{
					sprintf(Buf, "%s%s", Pkt->Cmd, Pkt->RemoteFile);
					iRetCode = command(Pkt->ctrl_skt, Reply, CancelCheckWork, "%s", Buf);
					if(iRetCode/100 == FTP_PRELIM)
					{
						iRetCode = DownLoadFile(Pkt, data_socket, CreateMode, CancelCheckWork);
//						data_socket = DoClose(data_socket);
					}
					else
					{
						SetErrorMsg(Reply);
						SetTaskMsg(MSGJPN092);
						data_socket = DoClose(data_socket);
						iRetCode = 500;
					}
				}
				else
					iRetCode = 500;
			}
			else
				iRetCode = 500;
		}
		else
		{
			SetErrorMsg(MSGJPN093);
			SetTaskMsg(MSGJPN093);
			iRetCode = 500;
		}
	}
	else
		SetErrorMsg(Buf);

	DispDownloadFinishMsg(Pkt, iRetCode);

	return(iRetCode);
}


/*----- �_�E�����[�h�̎��s ----------------------------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		SOCKET dSkt : �f�[�^�\�P�b�g
*		int CreateMode : �t�@�C���쐬���[�h (CREATE_ALWAYS/OPEN_ALWAYS)
*
*	Return Value
*		int �����R�[�h
*
*	Note
*		�]���̌o�ߕ\����
*			�_�C�A���O���o��(Pkt->hWndTrans!=NULL)�ꍇ�A�C���^�[�o���^�C�}�Ōo�߂�\������
*			�_�C�A���O���o���Ȃ��ꍇ�A���̃��[�`������DispDownloadSize()���Ă�
*----------------------------------------------------------------------------*/

static int DownLoadFile(TRANSPACKET *Pkt, SOCKET dSkt, int CreateMode, int *CancelCheckWork)
{
	int iNumBytes;
	char Buf[BUFSIZE];
	char Buf2[BUFSIZE+3];
	HANDLE iFileHandle;
	SECURITY_ATTRIBUTES Sec;
	DWORD Writed;
	CODECONVINFO cInfo;
	int Continue;
//	fd_set ReadFds;
//	struct timeval Tout;
//	struct timeval *ToutPtr;
	int iRetCode;
	int TimeOutErr;
	char TmpBuf[ONELINE_BUF_SIZE];
	DWORD dwFileAttributes;

#ifdef SET_BUFFER_SIZE
/* Add by H.Shirouzu at 2002/10/02 */
	int buf_size = SOCKBUF_SIZE;
	for ( ; buf_size > 0; buf_size /= 2)
		if (setsockopt(dSkt, SOL_SOCKET, SO_RCVBUF, (char *)&buf_size, sizeof(buf_size)) == 0)
			break;
/* End */
#endif

	Pkt->Abort = ABORT_NONE;

	Sec.nLength = sizeof(SECURITY_ATTRIBUTES);
	Sec.lpSecurityDescriptor = NULL;
	Sec.bInheritHandle = FALSE;

	dwFileAttributes = GetFileAttributes(Pkt->LocalFile);
	if (dwFileAttributes != INVALID_FILE_ATTRIBUTES && (dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
		// �ǂݎ���p
		if (MessageBox(GetMainHwnd(), MSGJPN296, MSGJPN086, MB_YESNO) == IDYES) {
			// ����������
			SetFileAttributes(Pkt->LocalFile, dwFileAttributes ^ FILE_ATTRIBUTE_READONLY);
		}
	}

	if((iFileHandle = CreateFile(Pkt->LocalFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &Sec, CreateMode, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
	{
		if(CreateMode == OPEN_ALWAYS)
			SetFilePointer(iFileHandle, 0, 0, FILE_END);

		if(Pkt->hWndTrans != NULL)
		{
			TimeStart = time(NULL);
			SetTimer(Pkt->hWndTrans, TIMER_DISPLAY, DISPLAY_TIMING, NULL);
		}

		InitCodeConvInfo(&cInfo);
		cInfo.KanaCnv = Pkt->KanaCnv;

		/*===== �t�@�C������M���郋�[�v =====*/
		while((Pkt->Abort == ABORT_NONE) && (ForceAbort == NO))
		{
//			FD_ZERO(&ReadFds);
//			FD_SET(dSkt, &ReadFds);
//			ToutPtr = NULL;
//			if(TimeOut != 0)
//			{
//				Tout.tv_sec = TimeOut;
//				Tout.tv_usec = 0;
//				ToutPtr = &Tout;
//			}
//			iNumBytes = select(0, &ReadFds, NULL, NULL, ToutPtr);
//			if(iNumBytes == SOCKET_ERROR)
//			{
//				ReportWSError("select", WSAGetLastError());
//				if(Pkt->Abort == ABORT_NONE)
//					Pkt->Abort = ABORT_ERROR;
//				break;
//			}
//			else if(iNumBytes == 0)
//			{
//				SetErrorMsg(MSGJPN094);
//				SetTaskMsg(MSGJPN094);
//				Pkt->Abort = ABORT_ERROR;
//				break;
//			}

			if((iNumBytes = do_recv(dSkt, Buf, BUFSIZE, 0, &TimeOutErr, CancelCheckWork)) <= 0)
			{
				if(TimeOutErr == YES)
				{
					SetErrorMsg(MSGJPN094);
					SetTaskMsg(MSGJPN094);
					if(Pkt->hWndTrans != NULL)
						ClearAll = YES;
					if(Pkt->Abort == ABORT_NONE)
						Pkt->Abort = ABORT_ERROR;
				}
				else if(iNumBytes == SOCKET_ERROR)
				{
					if(Pkt->Abort == ABORT_NONE)
						Pkt->Abort = ABORT_ERROR;
				}
				break;
			}

			/* �����R�[�h�ϊ� */
			if(Pkt->KanjiCode != KANJI_NOCNV)
			{
				cInfo.Str = Buf;
				cInfo.StrLen = iNumBytes;
				cInfo.Buf = Buf2;
				cInfo.BufSize = BUFSIZE+3;
				do
				{
					if(Pkt->KanjiCode == KANJI_JIS)
						Continue = ConvJIStoSJIS(&cInfo);
					else
						Continue = ConvEUCtoSJIS(&cInfo);
					if(WriteFile(iFileHandle, Buf2, cInfo.OutLen, &Writed, NULL) == FALSE)
						Pkt->Abort = ABORT_DISKFULL;
				}
				while((Continue == YES) && (Pkt->Abort == ABORT_NONE));
			}
			else
			{
				if(WriteFile(iFileHandle, Buf, iNumBytes, &Writed, NULL) == FALSE)
					Pkt->Abort = ABORT_DISKFULL;
			}

			Pkt->ExistSize += iNumBytes;
			if(Pkt->hWndTrans != NULL)
				AllTransSizeNow += iNumBytes;
			else
			{
				/* �]���_�C�A���O���o���Ȃ����̌o�ߕ\�� */
				DispDownloadSize(Pkt->ExistSize);
			}

			if(BackgrndMessageProc() == YES)
				ForceAbort = YES;
		}

		/* �����c�����f�[�^���������� */
		if(Pkt->KanjiCode != KANJI_NOCNV)
		{
			cInfo.Buf = Buf2;
			cInfo.BufSize = BUFSIZE+3;
			FlushRestData(&cInfo);
			if(WriteFile(iFileHandle, Buf2, cInfo.OutLen, &Writed, NULL) == FALSE)
				Pkt->Abort = ABORT_DISKFULL;
		}

		/* �O���t�\�����X�V */
		if(Pkt->hWndTrans != NULL)
		{
			KillTimer(Pkt->hWndTrans, TIMER_DISPLAY);
			DispTransferStatus(Pkt->hWndTrans, YES, Pkt);
			TimeStart = time(NULL) - TimeStart + 1;
		}
		else
		{
			/* �]���_�C�A���O���o���Ȃ����̌o�ߕ\�������� */
			DispDownloadSize(-1);
		}

		/* �t�@�C���̃^�C���X�^���v�����킹�� */
		if((SaveTimeStamp == YES) &&
		   ((Pkt->Time.dwLowDateTime != 0) || (Pkt->Time.dwHighDateTime != 0)))
		{
			SetFileTime(iFileHandle, &Pkt->Time, &Pkt->Time, &Pkt->Time);
		}

		CloseHandle(iFileHandle);

		if(iNumBytes == SOCKET_ERROR)
			ReportWSError("recv",WSAGetLastError());
	}
	else
	{
		SetErrorMsg(MSGJPN095, Pkt->LocalFile);
		SetTaskMsg(MSGJPN095, Pkt->LocalFile);
		Pkt->Abort = ABORT_ERROR;
	}


	if(shutdown(dSkt, 1) != 0)
		ReportWSError("shutdown", WSAGetLastError());

	DoClose(dSkt);

	if(ForceAbort == NO)
	{
		/* Abort���z�X�g�ɓ`���� */
		if(Pkt->Abort != ABORT_NONE && iFileHandle != INVALID_HANDLE_VALUE)
		{
			SendData(Pkt->ctrl_skt, "\xFF\xF4\xFF", 3, MSG_OOB, CancelCheckWork);	/* MSG_OOB�ɒ��� */
			SendData(Pkt->ctrl_skt, "\xF2", 1, 0, CancelCheckWork);
			command(Pkt->ctrl_skt, NULL, CancelCheckWork, "ABOR");
		}
	}

	iRetCode = ReadReplyMessage(Pkt->ctrl_skt, Buf, 1024, CancelCheckWork, TmpBuf);

//#pragma aaa
//DoPrintf("##DOWN REPLY : %s", Buf);

	if(Pkt->Abort == ABORT_DISKFULL)
	{
		SetErrorMsg(MSGJPN096);
		SetTaskMsg(MSGJPN096);
	}
	if(iRetCode >= FTP_RETRY)
		SetErrorMsg(Buf);
	if(Pkt->Abort != ABORT_NONE)
		iRetCode = 500;

	return(iRetCode);
}


/*----- �_�E�����[�h�I���^���~���̃��b�Z�[�W��\�� ----------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		int iRetCode : �����R�[�h
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DispDownloadFinishMsg(TRANSPACKET *Pkt, int iRetCode)
{
	char Fname[FMAX_PATH+1];

	if(ForceAbort == NO)
	{
		if((iRetCode/100) >= FTP_CONTINUE)
		{
			strcpy(Fname, Pkt->RemoteFile);

#if defined(HAVE_OPENVMS)
			/* OpenVMS�̏ꍇ�A��f�B���N�g���ֈړ������550 File not found�ɂȂ���
			 * �G���[�_�C�A���O��G���[���b�Z�[�W���o��̂ŉ������Ȃ� */
			if (AskHostType() == HTYPE_VMS)
				return;
#endif

			if((strncmp(Pkt->Cmd, "NLST", 4) == 0) || (strncmp(Pkt->Cmd, "LIST", 4) == 0))
			{
				SetTaskMsg(MSGJPN097);
				strcpy(Fname, MSGJPN098);
			}
			else if((Pkt->hWndTrans != NULL) && (TimeStart != 0))
				SetTaskMsg(MSGJPN099, TimeStart, Pkt->ExistSize/TimeStart);
			else
				SetTaskMsg(MSGJPN100);

			if(Pkt->Abort != ABORT_USER)
			{
				if(DispUpDownErrDialog(downerr_dlg, Pkt->hWndTrans, Fname) == NO)
					ClearAll = YES;
			}
		}
		else
		{
			if((strncmp(Pkt->Cmd, "NLST", 4) == 0) || (strncmp(Pkt->Cmd, "LIST", 4) == 0))
				SetTaskMsg(MSGJPN101, Pkt->ExistSize);
			else if((Pkt->hWndTrans != NULL) && (TimeStart != 0))
				SetTaskMsg(MSGJPN102, TimeStart, Pkt->ExistSize/TimeStart);
			else
				SetTaskMsg(MSGJPN103, Pkt->ExistSize);
		}
	}
	return;
}


/*----- �_�E�����[�h�^�A�b�v���[�h�G���[�̃_�C�A���O��\�� --------------------
*
*	Parameter
*		int RedID : �_�C�A���O�{�b�N�X�̃��\�[�XID
*		HWND hWnd : �������ݒ��_�C�A���O�̃E�C���h�E
*		char *Fname : �t�@�C����
*
*	Return Value
*		int �X�e�[�^�X (YES=���~/NO=�S�Ē��~)
*----------------------------------------------------------------------------*/

static int DispUpDownErrDialog(int ResID, HWND hWnd, char *Fname)
{
	if(hWnd == NULL)
		hWnd = GetMainHwnd();

	SoundPlay(SND_ERROR);
	return(DialogBoxParam(GetFtpInst(), MAKEINTRESOURCE(ResID), hWnd, UpDownErrorDialogProc, (LPARAM)Fname));
}


/*----- �_�E�����[�h�G���[�^�A�b�v���[�h�G���[�_�C�A���O�̃R�[���o�b�N --------
*
*	Parameter
*		HWND hDlg : �E�C���h�E�n���h��
*		UINT message : ���b�Z�[�W�ԍ�
*		WPARAM wParam : ���b�Z�[�W�� WPARAM ����
*		LPARAM lParam : ���b�Z�[�W�� LPARAM ����
*
*	Return Value
*		BOOL TRUE/FALSE
*----------------------------------------------------------------------------*/

static BOOL CALLBACK UpDownErrorDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, UPDOWN_ERR_FNAME, WM_SETTEXT, 0, (LPARAM)lParam);
			SendDlgItemMessage(hDlg, UPDOWN_ERR_MSG, WM_SETTEXT, 0, (LPARAM)ErrMsg);
			return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
					EndDialog(hDlg, YES);
					break;

				case IDCANCEL :
					EndDialog(hDlg, NO);
					break;
			}
			return(TRUE);
	}
    return(FALSE);
}


/*----- �_�E�����[�h�̃��W���[���̏������s�� ----------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		iont ProcMode : �������[�h(EXIST_xxx)
*		LONGLONG Size : ���[�h�ς݂̃t�@�C���̃T�C�Y
*		int *Mode : �t�@�C���쐬���[�h (CREATE_xxxx)
*
*	Return Value
*		int �]�����s�����ǂ���(YES/NO=���̃t�@�C���𒆎~/NO_ALL=�S�Ē��~)
*
*	Note
*		Pkt->ExistSize�̃Z�b�g���s�Ȃ�
*----------------------------------------------------------------------------*/

static int SetDownloadResume(TRANSPACKET *Pkt, int ProcMode, LONGLONG Size, int *Mode, int *CancelCheckWork)
{
	int iRetCode;
	int Com;
	char Reply[ERR_MSG_LEN+7];
	char Tmp[40];

	Com = YES;

	Pkt->ExistSize = 0;
	*Mode = CREATE_ALWAYS;

	if(ProcMode == EXIST_RESUME)
	{
		iRetCode = command(Pkt->ctrl_skt, Reply, CancelCheckWork, "REST %s", MakeNumString(Size, Tmp, FALSE));
		if(iRetCode/100 < FTP_RETRY)
		{
			/* ���W���[�� */
			if(Pkt->hWndTrans != NULL)
				Pkt->ExistSize = Size;
			*Mode = OPEN_ALWAYS;
		}
		else
		{
			Com = DialogBox(GetFtpInst(), MAKEINTRESOURCE(noresume_dlg), Pkt->hWndTrans, NoResumeWndProc);
			if(Com != YES)
			{
				if(Com == NO_ALL)		/* �S�Ē��~ */
					ClearAll = YES;
				Pkt->Abort = ABORT_USER;
			}
		}
	}
	return(Com);
}


/*----- resume�G���[�_�C�A���O�̃R�[���o�b�N ----------------------------------
*
*	Parameter
*		HWND hDlg : �E�C���h�E�n���h��
*		UINT message : ���b�Z�[�W�ԍ�
*		WPARAM wParam : ���b�Z�[�W�� WPARAM ����
*		LPARAM lParam : ���b�Z�[�W�� LPARAM ����
*
*	Return Value
*		BOOL TRUE/FALSE
*----------------------------------------------------------------------------*/

static BOOL CALLBACK NoResumeWndProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_INITDIALOG :
			return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
					EndDialog(hDlg, YES);
					break;

				case IDCANCEL :
					EndDialog(hDlg, NO);
					break;

				case RESUME_CANCEL_ALL :
					EndDialog(hDlg, NO_ALL);
					break;
			}
            return(TRUE);
	}
	return(FALSE);
}



/*----- �A�b�v���[�h���s�Ȃ� --------------------------------------------------
*
*	Parameter
*		SOCKET cSkt : �R���g���[���\�P�b�g
*		TRANSPACKET *Pkt : �]���t�@�C�����
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

static int DoUpLoad(SOCKET cSkt, TRANSPACKET *Pkt)
{
	int iRetCode;
	char Reply[ERR_MSG_LEN+7];

	Pkt->ctrl_skt = cSkt;

	if(Pkt->Mode != EXIST_IGNORE)
	{
		if(CheckFileReadable(Pkt->LocalFile) == SUCCESS)
		{
			if(Pkt->Type == TYPE_I)
				Pkt->KanjiCode = KANJI_NOCNV;

			iRetCode = command(Pkt->ctrl_skt, Reply, &Canceled, "TYPE %c", Pkt->Type);
			if(iRetCode/100 < FTP_RETRY)
			{
				if(Pkt->Mode == EXIST_UNIQUE)
					strcpy(Pkt->Cmd, "STOU ");

				if(Pkt->hWndTrans != NULL)
					DispTransFileInfo(Pkt, MSGJPN104, TRUE, YES);

				if(BackgrndMessageProc() == NO)
				{
					if(AskPasvMode() != YES)
						iRetCode = UpLoadNonPassive(Pkt);
					else
						iRetCode = UpLoadPassive(Pkt);
				}
				else
					iRetCode = 500;
			}
			else
				SetErrorMsg(Reply);

			/* �����ύX */
			if((Pkt->Attr != -1) && ((iRetCode/100) == FTP_COMPLETE))
				command(Pkt->ctrl_skt, Reply, &Canceled, "%s %03X %s", AskHostChmodCmd(), Pkt->Attr, Pkt->RemoteFile);
		}
		else
		{
			SetErrorMsg(MSGJPN105, Pkt->LocalFile);
			SetTaskMsg(MSGJPN105, Pkt->LocalFile);
			iRetCode = 500;
			Pkt->Abort = ABORT_ERROR;
			DispUploadFinishMsg(Pkt, iRetCode);
		}
	}
	else
	{
		DispTransFileInfo(Pkt, MSGJPN106, TRUE, YES);
		SetTaskMsg(MSGJPN107, Pkt->LocalFile);
		iRetCode = 200;
	}
	return(iRetCode);
}


/*----- �ʏ탂�[�h�Ńt�@�C�����A�b�v���[�h ------------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

static int UpLoadNonPassive(TRANSPACKET *Pkt)
{
	int iRetCode;
	int iLength;
	SOCKET data_socket = INVALID_SOCKET;   // data channel socket
	SOCKET listen_socket = INVALID_SOCKET; // data listen socket
	char Buf[1024];
	struct sockaddr_in saSockAddr1;
	int Resume;
	char Reply[ERR_MSG_LEN+7];

	if((listen_socket = GetFTPListenSocket(Pkt->ctrl_skt, &Canceled)) != INVALID_SOCKET)
	{
		SetUploadResume(Pkt, Pkt->Mode, Pkt->ExistSize, &Resume);
		if(Resume == NO)
			sprintf(Buf, "%s%s", Pkt->Cmd, Pkt->RemoteFile);
		else
			sprintf(Buf, "%s%s", "APPE ", Pkt->RemoteFile);

		iRetCode = command(Pkt->ctrl_skt, Reply, &Canceled, "%s", Buf);
		if((iRetCode/100) == FTP_PRELIM)
		{
			if(SocksGet2ndBindReply(listen_socket, &data_socket) == FAIL)
			{
				iLength=sizeof(saSockAddr1);
				data_socket = do_accept(listen_socket,(struct sockaddr *)&saSockAddr1, (int *)&iLength);

				if(shutdown(listen_socket, 1) != 0)
					ReportWSError("shutdown listen", WSAGetLastError());
				listen_socket = DoClose(listen_socket);

				if(data_socket == INVALID_SOCKET)
				{
					SetErrorMsg(MSGJPN280);
					ReportWSError("accept", WSAGetLastError());
					iRetCode = 500;
				}
				else
					DoPrintf("Skt=%u : accept from %s port %u", data_socket, inet_ntoa(saSockAddr1.sin_addr), ntohs(saSockAddr1.sin_port));
			}

			if(data_socket != INVALID_SOCKET)
			{
				iRetCode = UpLoadFile(Pkt, data_socket);
				data_socket = DoClose(data_socket);
			}
		}
		else
		{
			SetErrorMsg(Reply);
			SetTaskMsg(MSGJPN108);
			listen_socket = DoClose(listen_socket);
			iRetCode = 500;
		}
	}
	else
	{
		SetErrorMsg(MSGJPN279);
		iRetCode = 500;
	}
	DispUploadFinishMsg(Pkt, iRetCode);

	return(iRetCode);
}


/*----- Passive���[�h�Ńt�@�C�����A�b�v���[�h ---------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

static int UpLoadPassive(TRANSPACKET *Pkt)
{
	int iRetCode;
	SOCKET data_socket = INVALID_SOCKET;   // data channel socket
	char Buf[1024];
	char Adrs[20];
	int Port;
	int Flg;
	int Resume;
	char Reply[ERR_MSG_LEN+7];

	iRetCode = command(Pkt->ctrl_skt, Buf, &Canceled, "PASV");
	if(iRetCode/100 == FTP_COMPLETE)
	{
		if(GetAdrsAndPort(Buf, Adrs, &Port, 19) == SUCCESS)
		{
			if((data_socket = connectsock(Adrs, Port, MSGJPN109, &Canceled)) != INVALID_SOCKET)
			{
				if(setsockopt(data_socket, IPPROTO_TCP, TCP_NODELAY, (LPSTR)&Flg, sizeof(Flg)) == SOCKET_ERROR)
					ReportWSError("setsockopt", WSAGetLastError());

				SetUploadResume(Pkt, Pkt->Mode, Pkt->ExistSize, &Resume);
				if(Resume == NO)
					sprintf(Buf, "%s%s", Pkt->Cmd, Pkt->RemoteFile);
				else
					sprintf(Buf, "%s%s", "APPE ", Pkt->RemoteFile);

				iRetCode = command(Pkt->ctrl_skt, Reply, &Canceled, "%s", Buf);
				if(iRetCode/100 == FTP_PRELIM)
				{
					iRetCode = UpLoadFile(Pkt, data_socket);

					data_socket = DoClose(data_socket);
				}
				else
				{
					SetErrorMsg(Reply);
					SetTaskMsg(MSGJPN110);
					data_socket = DoClose(data_socket);
					iRetCode = 500;
				}
			}
			else
			{
				SetErrorMsg(MSGJPN281);
				iRetCode = 500;
			}
		}
		else
		{
			SetErrorMsg(Buf);
			SetTaskMsg(MSGJPN111);
			iRetCode = 500;
		}
	}
	else
		SetErrorMsg(Buf);

	DispUploadFinishMsg(Pkt, iRetCode);

	return(iRetCode);
}


/*----- �A�b�v���[�h�̎��s ----------------------------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		SOCKET dSkt : �f�[�^�\�P�b�g
*
*	Return Value
*		int �����R�[�h
*
*	Note
*		�]���̌o�ߕ\���́A�C���^�[�o���^�C�}�Ōo�߂�\������
*		�]���_�C�A���O���o���Ȃ��ŃA�b�v���[�h���邱�Ƃ͂Ȃ�
*----------------------------------------------------------------------------*/

static int UpLoadFile(TRANSPACKET *Pkt, SOCKET dSkt)
{
	DWORD iNumBytes;
	HANDLE iFileHandle;
	SECURITY_ATTRIBUTES Sec;
	char Buf[BUFSIZE];
	char Buf2[BUFSIZE+3];
	CODECONVINFO cInfo;
	TERMCODECONVINFO tInfo;
	int Continue;
	char *EofPos;
	int iRetCode;
#if 0
	int TimeOutErr;
#endif
	char TmpBuf[ONELINE_BUF_SIZE];
	DWORD Low;
	DWORD High;

#ifdef SET_BUFFER_SIZE
/* Add by H.Shirouzu at 2002/10/02 */
	int buf_size = SOCKBUF_SIZE;
	for ( ; buf_size > 0; buf_size /= 2)
		if (setsockopt(dSkt, SOL_SOCKET, SO_SNDBUF, (char *)&buf_size, sizeof(buf_size)) == 0)
			break;
/* End */
#endif

	Pkt->Abort = ABORT_NONE;

	Sec.nLength = sizeof(SECURITY_ATTRIBUTES);
	Sec.lpSecurityDescriptor = NULL;
	Sec.bInheritHandle = FALSE;

	if((iFileHandle = CreateFile(Pkt->LocalFile, GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE, &Sec, OPEN_EXISTING, 0, NULL)) != INVALID_HANDLE_VALUE)
	{
		if(Pkt->hWndTrans != NULL)
		{
			Low = GetFileSize(iFileHandle, &High);
			Pkt->Size = MakeLongLong(High, Low);

			High = (DWORD)HIGH32(Pkt->ExistSize);
			Low = (DWORD)LOW32(Pkt->ExistSize);
			SetFilePointer(iFileHandle, Low, &High, FILE_BEGIN);

			AllTransSizeNow = 0;
			TimeStart = time(NULL);
			SetTimer(Pkt->hWndTrans, TIMER_DISPLAY, DISPLAY_TIMING, NULL);
		}

		InitCodeConvInfo(&cInfo);
		cInfo.KanaCnv = Pkt->KanaCnv;
		InitTermCodeConvInfo(&tInfo);

		/*===== �t�@�C���𑗐M���郋�[�v =====*/
		while((Pkt->Abort == ABORT_NONE) &&
			  (ForceAbort == NO) &&
			  (ReadFile(iFileHandle, Buf, BUFSIZE, &iNumBytes, NULL) == TRUE))
		{
			if(iNumBytes == 0)
				break;

			/* EOF���� */
			EofPos = NULL;
			if((RmEOF == YES) && (Pkt->Type == TYPE_A))
			{
				if((EofPos = memchr(Buf, 0x1A, iNumBytes)) != NULL)
					iNumBytes = EofPos - Buf;
			}

			/* �����R�[�h�ϊ� */
			if(Pkt->KanjiCode != KANJI_NOCNV)
			{
				cInfo.Str = Buf;
				cInfo.StrLen = iNumBytes;
				cInfo.Buf = Buf2;
				cInfo.BufSize = BUFSIZE+3;
				do
				{
					if(Pkt->KanjiCode == KANJI_JIS)
						Continue = ConvSJIStoJIS(&cInfo);
					else
						Continue = ConvSJIStoEUC(&cInfo);

					if(TermCodeConvAndSend(&tInfo, dSkt, Buf2, cInfo.OutLen, Pkt->Type) == FAIL)
					{
						Pkt->Abort = ABORT_ERROR;
							break;
					}
				}
				while(Continue == YES);
			}
			else
			{
				if(TermCodeConvAndSend(&tInfo, dSkt, Buf, iNumBytes, Pkt->Type) == FAIL)
					Pkt->Abort = ABORT_ERROR;
			}

			Pkt->ExistSize += iNumBytes;
			if(Pkt->hWndTrans != NULL)
				AllTransSizeNow += iNumBytes;

			if(BackgrndMessageProc() == YES)
				ForceAbort = YES;

			if(EofPos != NULL)
				break;
		}

		if((ForceAbort == NO) && (Pkt->Abort == ABORT_NONE))
		{
			/* ����c�����f�[�^�𑗐M */
			if(Pkt->KanjiCode != KANJI_NOCNV)
			{
				cInfo.Buf = Buf2;
				cInfo.BufSize = BUFSIZE+3;
				FlushRestData(&cInfo);

				if(TermCodeConvAndSend(&tInfo, dSkt, Buf2, cInfo.OutLen, Pkt->Type) == FAIL)
					Pkt->Abort = ABORT_ERROR;
			}

			tInfo.Buf = Buf2;
			tInfo.BufSize = BUFSIZE+3;
			FlushRestTermCodeConvData(&tInfo);
			if(SendData(dSkt, Buf2, tInfo.OutLen, 0, &Canceled) == FAIL)
				Pkt->Abort = ABORT_ERROR;
		}

		/* �O���t�\�����X�V */
		if(Pkt->hWndTrans != NULL)
		{
			KillTimer(Pkt->hWndTrans, TIMER_DISPLAY);
			DispTransferStatus(Pkt->hWndTrans, YES, Pkt);
			TimeStart = time(NULL) - TimeStart + 1;
		}
		CloseHandle(iFileHandle);
	}
	else
	{
		SetErrorMsg(MSGJPN112, Pkt->LocalFile);
		SetTaskMsg(MSGJPN112, Pkt->LocalFile);
		Pkt->Abort = ABORT_ERROR;
	}

	if(shutdown(dSkt, 1) != 0)
		ReportWSError("shutdown", WSAGetLastError());

#if 0
	/* clean up */
	while(do_recv(dSkt, Buf, BUFSIZE, 0, &TimeOutErr, &Canceled) > 0)
		;
#endif

	iRetCode = ReadReplyMessage(Pkt->ctrl_skt, Buf, 1024, &Canceled, TmpBuf);

//#pragma aaa
//DoPrintf("##UP REPLY : %s", Buf);

	if(iRetCode >= FTP_RETRY)
		SetErrorMsg(Buf);

	if(Pkt->Abort != ABORT_NONE)
		iRetCode = 500;

	return(iRetCode);
}


/*----- �o�b�t�@�̓��e�����s�R�[�h�ϊ����đ��M --------------------------------
*
*	Parameter
*		TERMCODECONVINFO *tInfo : ���s�R�[�h�ϊ��p�P�b�g
*		SOCKET Skt : �\�P�b�g
*		char *Data : �f�[�^
*		int Size : �f�[�^�̃T�C�Y
*		int Ascii : ���[�h�@�@(TYPE_xx)
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

static int TermCodeConvAndSend(TERMCODECONVINFO *tInfo, SOCKET Skt, char *Data, int Size, int Ascii)
{
	char Buf3[BUFSIZE*2];
	int Continue;
	int Ret;

	Ret = SUCCESS;

// CR-LF�ȊO�̉��s�R�[�h��ϊ����Ȃ����[�h�͂����֒ǉ�
	if(Ascii == TYPE_A)
	{
		tInfo->Str = Data;
		tInfo->StrLen = Size;
		tInfo->Buf = Buf3;
		tInfo->BufSize = BUFSIZE*2;
		do
		{
			Continue = ConvTermCodeToCRLF(tInfo);
			if((Ret = SendData(Skt, Buf3, tInfo->OutLen, 0, &Canceled)) == FAIL)
				break;
		}
		while(Continue == YES);
	}
	else
		Ret = SendData(Skt, Data, Size, 0, &Canceled);

	return(Ret);
}


/*----- �A�b�v���[�h�I���^���~���̃��b�Z�[�W��\�� ----------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		int iRetCode : �����R�[�h
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DispUploadFinishMsg(TRANSPACKET *Pkt, int iRetCode)
{
	if(ForceAbort == NO)
	{
		if((iRetCode/100) >= FTP_CONTINUE)
		{
			if((Pkt->hWndTrans != NULL) && (TimeStart != 0))
				SetTaskMsg(MSGJPN113, TimeStart, Pkt->ExistSize/TimeStart);
			else
				SetTaskMsg(MSGJPN114);

			if(Pkt->Abort != ABORT_USER)
			{
				if(DispUpDownErrDialog(uperr_dlg, Pkt->hWndTrans, Pkt->LocalFile) == NO)
					ClearAll = YES;
			}
		}
		else
		{
			if((Pkt->hWndTrans != NULL) && (TimeStart != 0))
				SetTaskMsg(MSGJPN115, TimeStart, Pkt->ExistSize/TimeStart);
			else
				SetTaskMsg(MSGJPN116);
		}
	}
	return;
}


/*----- �A�b�v���[�h�̃��W���[���̏������s�� ----------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		iont ProcMode : �������[�h(EXIST_xxx)
*		LONGLONG Size : �z�X�g�ɂ���t�@�C���̃T�C�Y
*		int *Mode : ���W���[�����s�����ǂ��� (YES/NO)
*
*	Return Value
*		int �X�e�[�^�X = YES
*
*	Note
*		Pkt->ExistSize�̃Z�b�g���s�Ȃ�
*----------------------------------------------------------------------------*/

static int SetUploadResume(TRANSPACKET *Pkt, int ProcMode, LONGLONG Size, int *Mode)
{
	Pkt->ExistSize = 0;
	*Mode = NO;
	if(ProcMode == EXIST_RESUME)
	{
		if(Pkt->hWndTrans != NULL)
		{
			Pkt->ExistSize = Size;
			*Mode = YES;
		}
	}
	return(YES);
}


/*----- �]�����_�C�A���O�{�b�N�X�̃R�[���o�b�N --------------------------------
*
*	Parameter
*		HWND hDlg : �E�C���h�E�n���h��
*		UINT message : ���b�Z�[�W�ԍ�
*		WPARAM wParam : ���b�Z�[�W�� WPARAM ����
*		LPARAM lParam : ���b�Z�[�W�� LPARAM ����
*
*	Return Value
*		BOOL TRUE/FALSE
*----------------------------------------------------------------------------*/

static LRESULT CALLBACK TransDlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	RECT RectDlg;
	RECT RectPar;
	HMENU hMenu;
	static TRANSPACKET *Pkt;

	switch(Msg)
	{
		case WM_INITDIALOG :
			GetWindowRect(hDlg, &RectDlg);
			RectDlg.right -= RectDlg.left;
			RectDlg.bottom -= RectDlg.top;
			GetWindowRect(GetMainHwnd(), &RectPar);
			MoveWindow(hDlg,
				((RectPar.right + RectPar.left) / 2) - (RectDlg.right / 2),
				((RectPar.bottom + RectPar.top) / 2) - (RectDlg.bottom / 2),
				RectDlg.right,
				RectDlg.bottom,
				FALSE);

			hMenu = GetSystemMenu(hDlg, FALSE);
			EnableMenuItem(hMenu, SC_CLOSE, MF_GRAYED);
			break;

		case WM_COMMAND :
			switch(LOWORD(wParam))
			{
				case TRANS_STOP_NEXT :
					ClearAll = YES;
					break;

				case TRANS_STOP_ALL :
					ClearAll = YES;
					/* ������ break �͂Ȃ� */

				case IDCANCEL :
					Pkt->Abort = ABORT_USER;
					Canceled = YES;
					break;
			}
			break;

		case WM_TIMER :
			if(wParam == TIMER_DISPLAY)
			{
				if(MoveToForeground == YES)
					SetForegroundWindow(hDlg);
				MoveToForeground = NO;
				KillTimer(hDlg, TIMER_DISPLAY);
				DispTransferStatus(hDlg, NO, Pkt);
				SetTimer(hDlg, TIMER_DISPLAY, DISPLAY_TIMING, NULL);
			}
			break;

		case WM_SET_PACKET :
			Pkt = (TRANSPACKET *)lParam;
			break;
	}
	return(FALSE);
}


/*----- �]���X�e�[�^�X��\�� --------------------------------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		int End : �]���������������ǂ��� (YES/NO)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DispTransferStatus(HWND hWnd, int End, TRANSPACKET *Pkt)
{
	time_t TotalLap;
	int Per;
	LONGLONG Bps;
	LONGLONG Transed;
	char Num1[40];
	char Num2[40];
	char Tmp[80];
	char Str[80];
	char *Pos;

	if(hWnd != NULL)
	{
		SendMessage(hWnd, WM_GETTEXT, 79, (LPARAM)Str);
		if((Pos = strchr(Str, ')')) != NULL)
			Pos ++;
		else
			Pos = Str;
		sprintf(Tmp, "(%d)%s", AskTransferFileNum(), Pos);
		SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)Tmp);

		if(Pkt->Abort == ABORT_NONE)
		{
			if(End == NO)
			{
				TotalLap = time(NULL) - TimeStart + 1;

				Bps = 0;
				if(TotalLap != 0)
					Bps = AllTransSizeNow / TotalLap;
				Transed = Pkt->Size - Pkt->ExistSize;

				if(Pkt->Size <= 0)
					sprintf(Tmp, "%d ", Pkt->ExistSize);
				else if(Pkt->Size < 1024)
					sprintf(Tmp, "%s / %s ", MakeNumString(Pkt->ExistSize, Num1, TRUE), MakeNumString(Pkt->Size, Num2, TRUE));
				else
					sprintf(Tmp, "%sk / %sk ", MakeNumString(Pkt->ExistSize/1024, Num1, TRUE), MakeNumString(Pkt->Size/1024, Num2, TRUE));
				strcpy(Str, Tmp);

				if(Bps == 0)
					sprintf(Tmp, "( 0 B/S )");
				else if(Bps < 1000)
					sprintf(Tmp, "( %s B/S )", MakeNumString(Bps, Num1, TRUE));
				else
					sprintf(Tmp, "( %s.%02d KB/S )", MakeNumString(Bps/1000, Num1, TRUE), (int)((Bps%1000)/10));
				strcat(Str, Tmp);

				if((Bps > 0) && (Pkt->Size > 0) && (Transed >= 0))
				{
					sprintf(Tmp, "  %d:%02d", (int)((Transed/Bps)/60), (int)((Transed/Bps)%60));
					strcat(Str, Tmp);
				}
				else
					strcat(Str, "  ??:??");
			}
			else
				strcpy(Str, MSGJPN117);
		}
		else
			strcpy(Str, MSGJPN118);

		SendDlgItemMessage(hWnd, TRANS_STATUS, WM_SETTEXT, 0, (LPARAM)Str);

		if(Pkt->Size <= 0)
			Per = 0;
		else if(Pkt->Size < 1024*1024)
			Per = (int)(Pkt->ExistSize * 100 / Pkt->Size);
		else
			Per = (int)((Pkt->ExistSize/1024) * 100 / (Pkt->Size/1024));
		SendDlgItemMessage(hWnd, TRANS_TIME_BAR, PBM_SETPOS, Per, 0);
	}
	return;
}


/*----- �]������t�@�C���̏���\�� ------------------------------------------
*
*	Parameter
*		TRANSPACKET *Pkt : �]���t�@�C�����
*		char *Title : �E�C���h�E�̃^�C�g��
*		int SkipButton : �u���̃t�@�C���𒆎~�v�{�^���̗L�� (TRUE/FALSE)
*		int Info : �t�@�C������\�����邩�ǂ��� (YES/NO)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DispTransFileInfo(TRANSPACKET *Pkt, char *Title, int SkipButton, int Info)
{
	char Tmp[40];

	if(Pkt->hWndTrans != NULL)
	{
		EnableWindow(GetDlgItem(Pkt->hWndTrans, IDCANCEL), SkipButton);

		sprintf(Tmp, "(%d)%s", AskTransferFileNum(), Title);
		SendMessage(Pkt->hWndTrans, WM_SETTEXT, 0, (LPARAM)Tmp);
		SendDlgItemMessage(Pkt->hWndTrans, TRANS_STATUS, WM_SETTEXT, 0, (LPARAM)"");

		SendDlgItemMessage(Pkt->hWndTrans, TRANS_TIME_BAR, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
		SendDlgItemMessage(Pkt->hWndTrans, TRANS_TIME_BAR, PBM_SETSTEP, 1, 0);
		SendDlgItemMessage(Pkt->hWndTrans, TRANS_TIME_BAR, PBM_SETPOS, 0, 0);

		if(Info == YES)
		{
			DispStaticText(GetDlgItem(Pkt->hWndTrans, TRANS_REMOTE), Pkt->RemoteFile);
			DispStaticText(GetDlgItem(Pkt->hWndTrans, TRANS_LOCAL), Pkt->LocalFile);

			if(Pkt->Type == TYPE_I)
				SendDlgItemMessage(Pkt->hWndTrans, TRANS_MODE, WM_SETTEXT, 0, (LPARAM)MSGJPN119);
			else if(Pkt->Type == TYPE_A)
				SendDlgItemMessage(Pkt->hWndTrans, TRANS_MODE, WM_SETTEXT, 0, (LPARAM)MSGJPN120);

			if(Pkt->KanjiCode == KANJI_NOCNV)
				SendDlgItemMessage(Pkt->hWndTrans, TRANS_KANJI, WM_SETTEXT, 0, (LPARAM)MSGJPN121);
			else if(Pkt->KanjiCode == KANJI_JIS)
				SendDlgItemMessage(Pkt->hWndTrans, TRANS_KANJI, WM_SETTEXT, 0, (LPARAM)MSGJPN122);
			else if(Pkt->KanjiCode == KANJI_EUC)
				SendDlgItemMessage(Pkt->hWndTrans, TRANS_KANJI, WM_SETTEXT, 0, (LPARAM)MSGJPN123);
		}
		else
		{
			SendDlgItemMessage(Pkt->hWndTrans, TRANS_REMOTE, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(Pkt->hWndTrans, TRANS_LOCAL, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(Pkt->hWndTrans, TRANS_MODE, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(Pkt->hWndTrans, TRANS_KANJI, WM_SETTEXT, 0, (LPARAM)"");
		}
	}
	return;
}


/*----- PASV�R�}���h�̖߂�l����A�h���X�ƃ|�[�g�ԍ��𒊏o --------------------
*
*	Parameter
*		char *Str : PASV�R�}���h�̃��v���C
*		char *Adrs : �A�h���X�̃R�s�[�� ("www.xxx.yyy.zzz")
*		int *Port : �|�[�g�ԍ����Z�b�g���郏�[�N
*		int Max : �A�h���X������̍ő咷
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

static int GetAdrsAndPort(char *Str, char *Adrs, int *Port, int Max)
{
	char *Pos;
	char *Btm;
	int Sts;

	Sts = FAIL;

	Pos = strchr(Str, '(');
	if(Pos != NULL)
	{
		Pos++;
		Btm = strchr(Pos, ',');
		if(Btm != NULL)
		{
			Btm++;
			Btm = strchr(Btm, ',');
			if(Btm != NULL)
			{
				Btm++;
				Btm = strchr(Btm, ',');
				if(Btm != NULL)
				{
					Btm++;
					Btm = strchr(Btm, ',');
					if(Btm != NULL)
					{
						if((Btm - Pos) <= Max)
						{
							strncpy(Adrs, Pos, Btm - Pos);
							*(Adrs + (Btm - Pos)) = NUL;
							ReplaceAll(Adrs, ',', '.');

							Pos = Btm + 1;
							Btm = strchr(Pos, ',');
							if(Btm != NULL)
							{
								Btm++;
								*Port = (atoi(Pos) * 0x100) + atoi(Btm);
								Sts = SUCCESS;
							}
						}
					}
				}
			}
		}
	}
	return(Sts);
}


/*----- Windows�̃X�y�V�����f�o�C�X���ǂ�����Ԃ� -----------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*
*	Return Value
*		int �X�e�[�^�X (YES/NO)
*----------------------------------------------------------------------------*/

static int IsSpecialDevice(char *Fname)
{
	int Sts;

	Sts = NO;
	if((_stricmp(Fname, "CON") == 0) ||
	   (_stricmp(Fname, "PRN") == 0) ||
	   (_stricmp(Fname, "AUX") == 0) ||
	   (_strnicmp(Fname, "CON.", 4) == 0) ||
	   (_strnicmp(Fname, "PRN.", 4) == 0) ||
	   (_strnicmp(Fname, "AUX.", 4) == 0))
	{
		Sts = YES;
	}
	return(Sts);
}


/*----- �~���[�����O�ł̃t�@�C���폜�m�F --------------------------------------
*
*	Parameter
*		int Cur
*		int Notify
*		TRANSPACKET *Pkt
*
*	Return Value
*		BOOL TRUE/FALSE
*----------------------------------------------------------------------------*/

static int MirrorDelNotify(int Cur, int Notify, TRANSPACKET *Pkt)
{
	MIRRORDELETEINFO DelInfo;
	HWND hWnd;

	if(((Cur == WIN_LOCAL) && (MirDownDelNotify == NO)) ||
	   ((Cur == WIN_REMOTE) && (MirUpDelNotify == NO)))
	{
		Notify = YES_ALL;
	}

	if(Notify != YES_ALL)
	{
		DelInfo.Cur = Cur;
		DelInfo.Pkt = Pkt;
		hWnd = Pkt->hWndTrans;
		if(hWnd == NULL)
			hWnd = GetMainHwnd();
		Notify = DialogBoxParam(GetFtpInst(), MAKEINTRESOURCE(delete_dlg), hWnd, MirrorDeleteDialogCallBack, (LPARAM)&DelInfo);
	}
	return(Notify);
}


/*----- �~���[�����O�ł̃t�@�C���폜�_�C�A���O�̃R�[���o�b�N ------------------
*
*	Parameter
*		HWND hDlg : �E�C���h�E�n���h��
*		UINT message : ���b�Z�[�W�ԍ�
*		WPARAM wParam : ���b�Z�[�W�� WPARAM ����
*		LPARAM lParam : ���b�Z�[�W�� LPARAM ����
*
*	Return Value
*		BOOL TRUE/FALSE
*----------------------------------------------------------------------------*/

static BOOL CALLBACK MirrorDeleteDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static MIRRORDELETEINFO *DelInfo;
	switch (iMessage)
	{
		case WM_INITDIALOG :
			DelInfo = (MIRRORDELETEINFO *)lParam;

			if(DelInfo->Cur == WIN_LOCAL)
			{
				SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM)MSGJPN124);
				SendDlgItemMessage(hDlg, DELETE_TEXT, WM_SETTEXT, 0, (LPARAM)DelInfo->Pkt->LocalFile);
			}
			else
			{
				SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM)MSGJPN125);
				SendDlgItemMessage(hDlg, DELETE_TEXT, WM_SETTEXT, 0, (LPARAM)DelInfo->Pkt->RemoteFile);
			}
			return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
					EndDialog(hDlg, YES);
					break;

				case DELETE_NO :
					EndDialog(hDlg, NO);
					break;

				case DELETE_ALL :
					EndDialog(hDlg, YES_ALL);
					break;

				case IDCANCEL :
					ClearAll = YES;
					EndDialog(hDlg, NO_ALL);
					break;
			}
            return(TRUE);
	}
	return(FALSE);
}





static void SetErrorMsg(char *fmt, ...)
{
	va_list Args;

	if(strlen(ErrMsg) == 0)
	{
		va_start(Args, fmt);
		wvsprintf(ErrMsg, fmt, Args);
		va_end(Args);
	}
	return;
}






/*----- �_�E�����[�h���̕s���ȃp�X���`�F�b�N ----------------------------------
*
*	Parameter
*		TRANSPACKET *packet : �_�E�����[�h���
*
*	Return Value
*		int YES=�s���ȃp�X/NO=���Ȃ��p�X
*----------------------------------------------------------------------------*/
int CheckPathViolation(TRANSPACKET *packet)
{
	int result = NO;
	char *msg;

	if((strncmp(packet->RemoteFile, "..\\", 3) == 0) ||
	   (strncmp(packet->RemoteFile, "../", 3) == 0) ||
	   (strstr(packet->RemoteFile, "\\..\\") != NULL) ||
	   (strstr(packet->RemoteFile, "/../") != NULL))
	{
		msg = malloc(strlen(MSGJPN297) + strlen(packet->RemoteFile) + 1);
		if(msg)
		{
			sprintf(msg, MSGJPN297, packet->RemoteFile);
			MessageBox(GetMainHwnd(), msg, MSGJPN086, MB_OK);
			free(msg);
		}
		result = YES;
	}
	return(result);
}


