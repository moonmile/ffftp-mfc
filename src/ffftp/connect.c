/*=============================================================================
*
*								�z�X�g�ւ̐ڑ��^�ؒf
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

#define	STRICT
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <mbstring.h>
#include <time.h>
#include <winsock.h>
#include <windowsx.h>
#include <commctrl.h>

#include "common.h"
#include "resource.h"

#include <htmlhelp.h>
#include "helpid.h"


/*===== �v���g�^�C�v =====*/

static BOOL CALLBACK QuickConDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static int SendInitCommand(char *Cmd);
static void AskUseFireWall(char *Host, int *Fire, int *Pasv, int *List);
static void SaveCurrentSetToHistory(void);
static int ReConnectSkt(SOCKET *Skt);
static SOCKET DoConnect(char *Host, char *User, char *Pass, char *Acct, int Port, int Fwall, int SavePass, int Security);
static int CheckOneTimePassword(char *Pass, char *Reply, int Type);
static BOOL CALLBACK BlkHookFnc(void);
static int Socks5MakeCmdPacket(SOCKS5REQUEST *Packet, char Cmd, int ValidIP, ulong IP, char *Host, ushort Port);
static int SocksSendCmd(SOCKET Socket, void *Data, int Size, int *CancelCheckWork);
static int Socks5GetCmdReply(SOCKET Socket, SOCKS5REPLY *Packet);
static int Socks4GetCmdReply(SOCKET Socket, SOCKS4REPLY *Packet);
static int Socks5SelMethod(SOCKET Socket, int *CancelCheckWork);

/*===== �O���Q�� =====*/

extern char FilterStr[FILTER_EXT_LEN+1];
extern char TitleHostName[HOST_ADRS_LEN+1];
extern int CancelFlg;

/* �ݒ�l */
extern char UserMailAdrs[USER_MAIL_LEN+1];
extern char FwallHost[HOST_ADRS_LEN+1];
extern char FwallUser[USER_NAME_LEN+1];
extern char FwallPass[PASSWORD_LEN+1];
extern int FwallPort;
extern int FwallType;
extern int FwallDefault;
extern int FwallSecurity;
extern int FwallResolv;
extern int FwallLower;
extern int FwallDelimiter;
extern int PasvDefault;
extern int QuickAnonymous;

/*===== ���[�J���ȃ��[�N =====*/

static int Anonymous;
static int TryConnect = NO;
static SOCKET CmdCtrlSocket = INVALID_SOCKET;
static SOCKET TrnCtrlSocket = INVALID_SOCKET;
static HOSTDATA CurHost;

/* �ڑ����̐ڑ���ASOCKS�T�[�o�̃A�h���X����ۑ����Ă��� */
/* ���̏���listen�\�P�b�g���擾����ۂɗp���� */
static struct sockaddr_in SocksSockAddr;	/* SOCKS�T�[�o�̃A�h���X��� */
static struct sockaddr_in CurSockAddr;		/* �ڑ���z�X�g�̃A�h���X��� */

static int UseIPadrs;
static char DomainName[HOST_ADRS_LEN+1];




/*----- �z�X�g�ꗗ���g���ăz�X�g�֐ڑ� ----------------------------------------
*
*	Parameter
*		int Type : �_�C�A���O�̃^�C�v (DLG_TYPE_xxx)
*		int Num : �ڑ�����z�X�g�ԍ�(0�`, -1=�_�C�A���O���o��)

*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ConnectProc(int Type, int Num)
{
	int Save;
	int LFSort;
	int LDSort;
	int RFSort;
	int RDSort;

	SaveBookMark();
	SaveCurrentSetToHost();

	if((Num >= 0) || (SelectHost(Type) == YES))
	{
		if(Num >= 0)
			SetCurrentHost(Num);

		/* �ڑ����Ȃ�ؒf���� */
		if(CmdCtrlSocket != INVALID_SOCKET)
			DisconnectProc();

		SetTaskMsg("----------------------------");

		InitPWDcommand();
		CopyHostFromList(AskCurrentHost(), &CurHost);

		if(ConnectRas(CurHost.Dialup, CurHost.DialupAlways, CurHost.DialupNotify, CurHost.DialEntry) == SUCCESS)
		{
			SetHostKanaCnvImm(CurHost.KanaCnv);
			SetHostKanjiCodeImm(CurHost.KanjiCode);
			SetSyncMoveMode(CurHost.SyncMove);

			if((AskSaveSortToHost() == YES) && (CurHost.Sort != SORT_NOTSAVED))
			{
				DecomposeSortType(CurHost.Sort, &LFSort, &LDSort, &RFSort, &RDSort);
				SetSortTypeImm(LFSort, LDSort, RFSort, RDSort);
				ReSortDispList(WIN_LOCAL, &CancelFlg);
			}

			Save = NO;
			if(strlen(CurHost.PassWord) > 0)
				Save = YES;

			DisableUserOpe();
			CmdCtrlSocket = DoConnect(CurHost.HostAdrs, CurHost.UserName, CurHost.PassWord, CurHost.Account, CurHost.Port, CurHost.FireWall, Save, CurHost.Security);
			TrnCtrlSocket = CmdCtrlSocket;

			if(CmdCtrlSocket != INVALID_SOCKET)
			{
				strcpy(TitleHostName, CurHost.HostName);
				DispWindowTitle();
				SoundPlay(SND_CONNECT);

				SendInitCommand(CurHost.InitCmd);

				if(strlen(CurHost.LocalInitDir) > 0)
				{
					DoLocalCWD(CurHost.LocalInitDir);
					GetLocalDirForWnd();
				}
				InitTransCurDir();
				DoCWD(CurHost.RemoteInitDir, YES, YES, YES);

				LoadBookMark();
				GetRemoteDirForWnd(CACHE_NORMAL, &CancelFlg);
			}
			else
				SoundPlay(SND_ERROR);

			EnableUserOpe();
		}
		else
			SetTaskMsg(MSGJPN001);
	}
	return;
}


/*----- �z�X�g������͂��ăz�X�g�֐ڑ� ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void QuickConnectProc(void)
{
	char Tmp[FMAX_PATH+1 + USER_NAME_LEN+1 + PASSWORD_LEN+1 + 2];
	char File[FMAX_PATH+1];

	SaveBookMark();
	SaveCurrentSetToHost();

	if(DialogBoxParam(GetFtpInst(), MAKEINTRESOURCE(hostname_dlg), GetMainHwnd(), QuickConDialogCallBack, (LPARAM)Tmp) == YES)
	{
		/* �ڑ����Ȃ�ؒf���� */
		if(CmdCtrlSocket != INVALID_SOCKET)
			DisconnectProc();

		SetTaskMsg("----------------------------");

		InitPWDcommand();
		CopyDefaultHost(&CurHost);
		if(SplitUNCpath(Tmp, CurHost.HostAdrs, CurHost.RemoteInitDir, File, CurHost.UserName, CurHost.PassWord, &CurHost.Port) == SUCCESS)
		{
			if(strlen(CurHost.UserName) == 0)
			{
				strcpy(CurHost.UserName, Tmp + FMAX_PATH+1);
				strcpy(CurHost.PassWord, Tmp + FMAX_PATH+1 + USER_NAME_LEN+1);
			}

			SetCurrentHost(HOSTNUM_NOENTRY);
			AskUseFireWall(CurHost.HostAdrs, &CurHost.FireWall, &CurHost.Pasv, &CurHost.ListCmdOnly);
			CurHost.FireWall = (int)Tmp[FMAX_PATH+1 + USER_NAME_LEN+1 + PASSWORD_LEN+1];
			CurHost.Pasv = (int)Tmp[FMAX_PATH+1 + USER_NAME_LEN+1 + PASSWORD_LEN+1 + 1];

			SetHostKanaCnvImm(CurHost.KanaCnv);
			SetHostKanjiCodeImm(CurHost.KanjiCode);
			SetSyncMoveMode(CurHost.SyncMove);

			DisableUserOpe();
			CmdCtrlSocket = DoConnect(CurHost.HostAdrs, CurHost.UserName, CurHost.PassWord, CurHost.Account, CurHost.Port, CurHost.FireWall, NO, CurHost.Security);
			TrnCtrlSocket = CmdCtrlSocket;

			if(CmdCtrlSocket != INVALID_SOCKET)
			{
				strcpy(TitleHostName, CurHost.HostAdrs);
				DispWindowTitle();
				SoundPlay(SND_CONNECT);

				InitTransCurDir();
				DoCWD(CurHost.RemoteInitDir, YES, YES, YES);

				GetRemoteDirForWnd(CACHE_NORMAL, &CancelFlg);
				EnableUserOpe();

				if(strlen(File) > 0)
					DirectDownLoadProc(File);
			}
			else
			{
				SoundPlay(SND_ERROR);
				EnableUserOpe();
			}
		}
	}
	return;
}


/*----- �N�C�b�N�ڑ��_�C�A���O�̃R�[���o�b�N ----------------------------------
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

static BOOL CALLBACK QuickConDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static char *Buf;
	int i;
	HISTORYDATA Tmp;

//char Str[HOST_ADRS_LEN+USER_NAME_LEN+INIT_DIR_LEN+5+1];

	switch (iMessage)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, QHOST_HOST, CB_LIMITTEXT, FMAX_PATH, 0);
			SendDlgItemMessage(hDlg, QHOST_HOST, WM_SETTEXT, 0, (LPARAM)"");
			SendDlgItemMessage(hDlg, QHOST_USER, EM_LIMITTEXT, USER_NAME_LEN, 0);
			if(QuickAnonymous == YES)
			{
				SendDlgItemMessage(hDlg, QHOST_USER, WM_SETTEXT, 0, (LPARAM)"anonymous");
				SendDlgItemMessage(hDlg, QHOST_PASS, WM_SETTEXT, 0, (LPARAM)UserMailAdrs);
			}
			else
			{
				SendDlgItemMessage(hDlg, QHOST_USER, WM_SETTEXT, 0, (LPARAM)"");
				SendDlgItemMessage(hDlg, QHOST_PASS, WM_SETTEXT, 0, (LPARAM)"");
			}
			SendDlgItemMessage(hDlg, QHOST_PASS, EM_LIMITTEXT, PASSWORD_LEN, 0);
			SendDlgItemMessage(hDlg, QHOST_FWALL, BM_SETCHECK, FwallDefault, 0);
			SendDlgItemMessage(hDlg, QHOST_PASV, BM_SETCHECK, PasvDefault, 0);
			for(i = 0; i < HISTORY_MAX; i++)
			{
				if(GetHistoryByNum(i, &Tmp) == SUCCESS)
				{
//sprintf(Str, "%s (%s) %s", Tmp.HostAdrs, Tmp.UserName, Tmp.RemoteInitDir);
//SendDlgItemMessage(hDlg, QHOST_HOST, CB_ADDSTRING, 0, (LPARAM)Str);
					SendDlgItemMessage(hDlg, QHOST_HOST, CB_ADDSTRING, 0, (LPARAM)Tmp.HostAdrs);
				}
			}
			Buf = (char *)lParam;
			return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
					SendDlgItemMessage(hDlg, QHOST_HOST, WM_GETTEXT, FMAX_PATH+1, (LPARAM)Buf);
					SendDlgItemMessage(hDlg, QHOST_USER, WM_GETTEXT, USER_NAME_LEN+1, (LPARAM)Buf + FMAX_PATH+1);
					SendDlgItemMessage(hDlg, QHOST_PASS, WM_GETTEXT, PASSWORD_LEN+1, (LPARAM)Buf + FMAX_PATH+1 + USER_NAME_LEN+1);
					*(Buf + FMAX_PATH+1 + USER_NAME_LEN+1 + PASSWORD_LEN+1) = (char)SendDlgItemMessage(hDlg, QHOST_FWALL, BM_GETCHECK, 0, 0);
					*(Buf + FMAX_PATH+1 + USER_NAME_LEN+1 + PASSWORD_LEN+1+1) = (char)SendDlgItemMessage(hDlg, QHOST_PASV, BM_GETCHECK, 0, 0);
					EndDialog(hDlg, YES);
					break;

				case IDCANCEL :
					EndDialog(hDlg, NO);
					break;

//				case QHOST_HOST :
//					if(HIWORD(wParam) == CBN_EDITCHANGE)
//						DoPrintf("EDIT");
//					break;
			}
            return(TRUE);
	}
	return(FALSE);
}


/*----- �w�肵���z�X�g���Ńz�X�g�֐ڑ� ----------------------------------------
*
*	Parameter
*		char *unc : UNC������
*		int Kanji : �z�X�g�̊����R�[�h (KANJI_xxx)
*		int Kana : ���p���ȁ��S�p�ϊ����[�h (YES/NO)
*		int Fkanji : �t�@�C�����̊����R�[�h (KANJI_xxx)
*		int TrMode : �]�����[�h (TYPE_xx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DirectConnectProc(char *unc, int Kanji, int Kana, int Fkanji, int TrMode)
{
	char Host[HOST_ADRS_LEN+1];
	char Path[FMAX_PATH+1];
	char File[FMAX_PATH+1];
	char User[USER_NAME_LEN+1];
	char Pass[PASSWORD_LEN+1];
	int Port;

	SaveBookMark();
	SaveCurrentSetToHost();

	/* �ڑ����Ȃ�ؒf���� */
	if(CmdCtrlSocket != INVALID_SOCKET)
		DisconnectProc();

	SetTaskMsg("----------------------------");

	InitPWDcommand();
	if(SplitUNCpath(unc, Host, Path, File, User, Pass, &Port) == SUCCESS)
	{
		if(strlen(User) == 0)
		{
			strcpy(User, "anonymous");
			strcpy(Pass, UserMailAdrs);
		}

		CopyDefaultHost(&CurHost);

		SetCurrentHost(HOSTNUM_NOENTRY);
		strcpy(CurHost.HostAdrs, Host);
		strcpy(CurHost.UserName, User);
		strcpy(CurHost.PassWord, Pass);
		strcpy(CurHost.RemoteInitDir, Path);
		AskUseFireWall(CurHost.HostAdrs, &CurHost.FireWall, &CurHost.Pasv, &CurHost.ListCmdOnly);
		CurHost.Port = Port;
		CurHost.KanjiCode = Kanji;
		CurHost.KanaCnv = Kana;
		CurHost.NameKanjiCode = Fkanji;
		CurHost.KanaCnv = YES;			/* �Ƃ肠���� */

		SetHostKanaCnvImm(CurHost.KanaCnv);
		SetHostKanjiCodeImm(CurHost.KanjiCode);
		SetSyncMoveMode(CurHost.SyncMove);

		if(TrMode != TYPE_DEFAULT)
		{
			SetTransferTypeImm(TrMode);
			DispTransferType();
		}

		DisableUserOpe();
		CmdCtrlSocket = DoConnect(CurHost.HostAdrs, CurHost.UserName, CurHost.PassWord, CurHost.Account, CurHost.Port, CurHost.FireWall, NO, CurHost.Security);
		TrnCtrlSocket = CmdCtrlSocket;

		if(CmdCtrlSocket != INVALID_SOCKET)
		{
			strcpy(TitleHostName, CurHost.HostAdrs);
			DispWindowTitle();
			SoundPlay(SND_CONNECT);

			InitTransCurDir();
			DoCWD(CurHost.RemoteInitDir, YES, YES, YES);

			GetRemoteDirForWnd(CACHE_NORMAL, &CancelFlg);
			EnableUserOpe();

			if(strlen(File) > 0)
				DirectDownLoadProc(File);
			else
				ResetAutoExitFlg();
		}
		else
		{
			SoundPlay(SND_ERROR);
			EnableUserOpe();
		}
	}
	return;
}


/*----- �z�X�g�̃q�X�g���Ŏw�肳�ꂽ�z�X�g�֐ڑ� ------------------------------
*
*	Parameter
*		int MenuCmd : ���o���q�X�g���Ɋ��蓖�Ă�ꂽ���j���[�R�}���h
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void HistoryConnectProc(int MenuCmd)
{
	HISTORYDATA Hist;
	int LFSort;
	int LDSort;
	int RFSort;
	int RDSort;

	if(GetHistoryByCmd(MenuCmd, &Hist) == SUCCESS)
	{
		SaveBookMark();
		SaveCurrentSetToHost();

		/* �ڑ����Ȃ�ؒf���� */
		if(CmdCtrlSocket != INVALID_SOCKET)
			DisconnectProc();

		SetTaskMsg("----------------------------");

		InitPWDcommand();
		CopyHistoryToHost(&Hist, &CurHost);

		if(ConnectRas(CurHost.Dialup, CurHost.DialupAlways, CurHost.DialupNotify, CurHost.DialEntry) == SUCCESS)
		{
			SetCurrentHost(HOSTNUM_NOENTRY);
			SetHostKanaCnvImm(CurHost.KanaCnv);
			SetHostKanjiCodeImm(CurHost.KanjiCode);
			SetSyncMoveMode(CurHost.SyncMove);

			DecomposeSortType(CurHost.Sort, &LFSort, &LDSort, &RFSort, &RDSort);
			SetSortTypeImm(LFSort, LDSort, RFSort, RDSort);
			ReSortDispList(WIN_LOCAL, &CancelFlg);

			SetTransferTypeImm(Hist.Type);
			DispTransferType();

			DisableUserOpe();
			CmdCtrlSocket = DoConnect(CurHost.HostAdrs, CurHost.UserName, CurHost.PassWord, CurHost.Account, CurHost.Port, CurHost.FireWall, NO, CurHost.Security);
			TrnCtrlSocket = CmdCtrlSocket;

			if(CmdCtrlSocket != INVALID_SOCKET)
			{
				strcpy(TitleHostName, CurHost.HostAdrs);
				DispWindowTitle();
				SoundPlay(SND_CONNECT);

				SendInitCommand(CurHost.InitCmd);

				DoLocalCWD(CurHost.LocalInitDir);
				GetLocalDirForWnd();

				InitTransCurDir();
				DoCWD(CurHost.RemoteInitDir, YES, YES, YES);

				GetRemoteDirForWnd(CACHE_NORMAL, &CancelFlg);
			}
			else
				SoundPlay(SND_ERROR);

			EnableUserOpe();
		}
		else
			SetTaskMsg(MSGJPN002);
	}
	else
		SoundPlay(SND_ERROR);

	return;
}


/*----- �z�X�g�̏������R�}���h�𑗂� ------------------------------------------
*
*	Parameter
*		int Cmd : �������R�}���h�X
*
*	Return Value
*		�Ȃ�
*
*	NOte
*		�������R�}���h�͈ȉ��̂悤�ȃt�H�[�}�b�g�ł��邱��
*			cmd1\0
*			cmd1\r\ncmd2\r\n\0
*----------------------------------------------------------------------------*/

static int SendInitCommand(char *Cmd)
{
	char Tmp[INITCMD_LEN+1];
	char *Pos;

	while(strlen(Cmd) > 0)
	{
		strcpy(Tmp, Cmd);
		if((Pos = strchr(Tmp, '\r')) != NULL)
			*Pos = NUL;
		if(strlen(Tmp) > 0)
			DoQUOTE(Tmp);

		if((Cmd = strchr(Cmd, '\n')) != NULL)
			Cmd++;
		else
			break;
	}
	return(0);
}


/*----- �w��̃z�X�g��FireWall���g���ݒ肩�ǂ�����Ԃ� ------------------------
*
*	Parameter
*		char *Hots : �z�X�g��
*		int *Fire : FireWall���g�����ǂ�����Ԃ����[�N
*		int *Pasv : PASV���[�h��Ԃ����[�N
*		int *List : LIST�R�}���h�̂ݎg�p�t���O
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void AskUseFireWall(char *Host, int *Fire, int *Pasv, int *List)
{
	int i;
	HOSTDATA Tmp;

	*Fire = FwallDefault;
	*Pasv = PasvDefault;
	*List = NO;

	i = 0;
	while(CopyHostFromList(i, &Tmp) == SUCCESS)
	{
		if(strcmp(Host, Tmp.HostAdrs) == 0)
		{
			*Fire = Tmp.FireWall;
			*Pasv = Tmp.Pasv;
			*List = Tmp.ListCmdOnly;
			break;
		}
		i++;
	}
	return;
}


/*----- �ڑ����Ă���z�X�g�̃A�h���X��Ԃ� ------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		char *�z�X�g�̃A�h���X
*----------------------------------------------------------------------------*/

char *AskHostAdrs(void)
{
	return(CurHost.HostAdrs);
}


/*----- �ڑ����Ă���z�X�g�̃|�[�g��Ԃ� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �z�X�g�̃|�[�g
*----------------------------------------------------------------------------*/

int AskHostPort(void)
{
	return(CurHost.Port);
}

/*----- �ڑ����Ă���z�X�g�̃t�@�C�����̊����R�[�h��Ԃ� ----------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �����R�[�h (KANJI_xxx)
*----------------------------------------------------------------------------*/

int AskHostNameKanji(void)
{
	if(AskCurrentHost() != HOSTNUM_NOENTRY)
		CopyHostFromListInConnect(AskCurrentHost(), &CurHost);

	return(CurHost.NameKanjiCode);
}


/*----- �ڑ����Ă���z�X�g�̃t�@�C�����̔��p�J�i�ϊ��t���O��Ԃ� --------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int ���p�J�i��S�p�ɕϊ����邩�ǂ��� (YES/NO)
*----------------------------------------------------------------------------*/

int AskHostNameKana(void)
{
	if(AskCurrentHost() != HOSTNUM_NOENTRY)
		CopyHostFromListInConnect(AskCurrentHost(), &CurHost);

	return(CurHost.NameKanaCnv);
}


/*----- �ڑ����Ă���z�X�g��LIST�R�}���h���[�h��Ԃ� --------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int LIST�R�}���h���[�h (YES/NO)
*----------------------------------------------------------------------------*/

int AskListCmdMode(void)
{
	if(CurHost.HostType == HTYPE_VMS)
		return(YES);
	else
	{
		if(AskCurrentHost() != HOSTNUM_NOENTRY)
			CopyHostFromListInConnect(AskCurrentHost(), &CurHost);
		return(CurHost.ListCmdOnly);
	}
}


/*----- �ڑ����Ă���z�X�g��NLST -R���g�����ǂ�����Ԃ� ------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int NLST -R���g�����ǂ��� (YES/NO)
*----------------------------------------------------------------------------*/

int AskUseNLST_R(void)
{
	if(AskCurrentHost() != HOSTNUM_NOENTRY)
		CopyHostFromListInConnect(AskCurrentHost(), &CurHost);

	return(CurHost.UseNLST_R);
}


/*----- �ڑ����Ă���z�X�g��Chmod�R�}���h��Ԃ� -------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		char *Chmod�R�}���h
*----------------------------------------------------------------------------*/

char *AskHostChmodCmd(void)
{
	if(AskCurrentHost() != HOSTNUM_NOENTRY)
		CopyHostFromListInConnect(AskCurrentHost(), &CurHost);

	return(CurHost.ChmodCmd);
}


/*----- �ڑ����Ă���z�X�g�̃^�C���]�[����Ԃ� --------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �^�C���]�[��
*----------------------------------------------------------------------------*/

int AskHostTimeZone(void)
{
	if(AskCurrentHost() != HOSTNUM_NOENTRY)
		CopyHostFromListInConnect(AskCurrentHost(), &CurHost);

	return(CurHost.TimeZone);
}


/*----- �ڑ����Ă���z�X�g��PASV���[�h��Ԃ� ----------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int PASV���[�h���ǂ��� (YES/NO)
*----------------------------------------------------------------------------*/

int AskPasvMode(void)
{
	return(CurHost.Pasv);
}


/*----- �ڑ����Ă���z�X�g��LNST�t�@�C������Ԃ� ------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		char *�t�@�C�����^�I�v�V����
*----------------------------------------------------------------------------*/

char *AskHostLsName(void)
{
	if(AskCurrentHost() != HOSTNUM_NOENTRY)
		CopyHostFromListInConnect(AskCurrentHost(), &CurHost);

	return(CurHost.LsName);
}


/*----- �ڑ����Ă���z�X�g�̃z�X�g�^�C�v��Ԃ� --------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		char *�t�@�C�����^�I�v�V����
*----------------------------------------------------------------------------*/

int AskHostType(void)
{
	if(AskCurrentHost() != HOSTNUM_NOENTRY)
		CopyHostFromListInConnect(AskCurrentHost(), &CurHost);

	return(CurHost.HostType);
}


/*----- �ڑ����Ă���z�X�g��FireWall���g���z�X�g���ǂ�����Ԃ� ----------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int FireWall���g�����ǂ��� (YES/NO)
*----------------------------------------------------------------------------*/

int AskHostFireWall(void)
{
	return(CurHost.FireWall);
}


/*----- �ڑ����Ă���z�X�g�Ńt���p�X�Ńt�@�C���A�N�Z�X���Ȃ����ǂ�����Ԃ� ----
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �t���p�X�ŃA�N�Z�X���Ȃ� (YES=�t���p�X�֎~/NO)
*----------------------------------------------------------------------------*/

int AskNoFullPathMode(void)
{
	if(CurHost.HostType == HTYPE_VMS)
		return(YES);
	else
		return(CurHost.NoFullPath);
}


/*----- �ڑ����Ă��郆�[�U����Ԃ� --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		char *���[�U��
*----------------------------------------------------------------------------*/

char *AskHostUserName(void)
{
	return(CurHost.UserName);
}


/*----- ���݂̐ݒ���z�X�g�̐ݒ�ɃZ�b�g���� ----------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�J�����g�f�B���N�g���A�\�[�g���@���z�X�g�̐ݒ�ɃZ�b�g����
*----------------------------------------------------------------------------*/

void SaveCurrentSetToHost(void)
{
	int Host;
	char LocDir[FMAX_PATH+1];
	char HostDir[FMAX_PATH+1];

	if(TrnCtrlSocket != INVALID_SOCKET)
	{
		if((Host = AskCurrentHost()) != HOSTNUM_NOENTRY)
		{
			CopyHostFromListInConnect(Host, &CurHost);
			if(CurHost.LastDir == YES)
			{
				AskLocalCurDir(LocDir, FMAX_PATH);
				AskRemoteCurDir(HostDir, FMAX_PATH);
				SetHostDir(AskCurrentHost(), LocDir, HostDir);
			}
			SetHostSort(AskCurrentHost(), AskSortType(ITEM_LFILE), AskSortType(ITEM_LDIR), AskSortType(ITEM_RFILE), AskSortType(ITEM_RDIR));
		}
	}
	return;
}


/*----- ���݂̐ݒ���q�X�g���ɃZ�b�g���� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void SaveCurrentSetToHistory(void)
{
	char LocDir[FMAX_PATH+1];
	char HostDir[FMAX_PATH+1];

	AskLocalCurDir(LocDir, FMAX_PATH);
	AskRemoteCurDir(HostDir, FMAX_PATH);
	strcpy(CurHost.LocalInitDir, LocDir);
	strcpy(CurHost.RemoteInitDir, HostDir);

	CurHost.Sort = AskSortType(ITEM_LFILE) * 0x1000000 | AskSortType(ITEM_LDIR) * 0x10000 | AskSortType(ITEM_RFILE) * 0x100 | AskSortType(ITEM_RDIR);

	CurHost.KanjiCode = AskHostKanjiCode();
	CurHost.KanaCnv = AskHostKanaCnv();

	CurHost.SyncMove = AskSyncMoveMode();

	AddHostToHistory(&CurHost, AskTransferType());
	SetAllHistoryToMenu();

	return;
}


/*----- �R�}���h�R���g���[���\�P�b�g�̍Đڑ� ----------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int ReConnectCmdSkt(void)
{
	int Sts;

	if(CmdCtrlSocket != TrnCtrlSocket)
		do_closesocket(TrnCtrlSocket);
	TrnCtrlSocket = INVALID_SOCKET;

	Sts = ReConnectSkt(&CmdCtrlSocket);

	TrnCtrlSocket = CmdCtrlSocket;

	return(Sts);
}


/*----- �]���R���g���[���\�P�b�g�̍Đڑ� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

//int ReConnectTrnSkt(void)
//{
//	return(ReConnectSkt(&TrnCtrlSocket));
//}


/*----- ����̍Đڑ� ----------------------------------------------------------
*
*	Parameter
*		SOCKET *Skt : �ڑ������\�P�b�g��Ԃ����[�N
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

static int ReConnectSkt(SOCKET *Skt)
{
	char Path[FMAX_PATH+1];
	int Sts;

	Sts = FAIL;

	SetTaskMsg(MSGJPN003);

	DisableUserOpe();
	/* ���݂̃\�P�b�g�͐ؒf */
	if(*Skt != INVALID_SOCKET)
		do_closesocket(*Skt);
	/* �Đڑ� */
	if((*Skt = DoConnect(CurHost.HostAdrs, CurHost.UserName, CurHost.PassWord, CurHost.Account, CurHost.Port, CurHost.FireWall, NO, CurHost.Security)) != INVALID_SOCKET)
	{
		AskRemoteCurDir(Path, FMAX_PATH);
		DoCWD(Path, YES, YES, YES);
		Sts = SUCCESS;
	}
	else
		SoundPlay(SND_ERROR);

	EnableUserOpe();
	return(Sts);
}


/*----- �R�}���h�R���g���[���\�P�b�g��Ԃ� ------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		SOCKET �R�}���h�R���g���[���\�P�b�g
*----------------------------------------------------------------------------*/

SOCKET AskCmdCtrlSkt(void)
{
	return(CmdCtrlSocket);
}


/*----- �]���R���g���[���\�P�b�g��Ԃ� ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		SOCKET �]���R���g���[���\�P�b�g
*----------------------------------------------------------------------------*/

SOCKET AskTrnCtrlSkt(void)
{
	return(TrnCtrlSocket);
}


/*----- �R�}���h�^�]���R���g���[���\�P�b�g�̋��L������ ------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SktShareProh(void)
{
	if(CmdCtrlSocket == TrnCtrlSocket)
	{

//SetTaskMsg("############### SktShareProh");

		CmdCtrlSocket = INVALID_SOCKET;
		ReConnectSkt(&CmdCtrlSocket);
	}
	return;
}


/*----- �R�}���h�^�]���R���g���[���\�P�b�g�̋��L����������Ă��邩�`�F�b�N ----
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X
*			YES=���L����/NO=���L
*----------------------------------------------------------------------------*/

int AskShareProh(void)
{
	int Sts;

	Sts = YES;
	if(CmdCtrlSocket == TrnCtrlSocket)
		Sts = NO;

	return(Sts);
}


/*----- �z�X�g����ؒf --------------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DisconnectProc(void)
{

//SetTaskMsg("############### Disconnect Cmd=%x, Trn=%x", CmdCtrlSocket,TrnCtrlSocket);

	if((CmdCtrlSocket != INVALID_SOCKET) && (CmdCtrlSocket != TrnCtrlSocket))
	{
		DoQUIT(CmdCtrlSocket);
		DoClose(CmdCtrlSocket);
	}

	if(TrnCtrlSocket != INVALID_SOCKET)
	{
		DoQUIT(TrnCtrlSocket);
		DoClose(TrnCtrlSocket);

		SaveCurrentSetToHistory();

		EraseRemoteDirForWnd();
		SetTaskMsg(MSGJPN004);
	}

	TrnCtrlSocket = INVALID_SOCKET;
	CmdCtrlSocket = INVALID_SOCKET;

	DispWindowTitle();
	MakeButtonsFocus();
	ClearBookMark();

	return;
}


/*----- �\�P�b�g�������ؒf���ꂽ�Ƃ��̏��� ------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DisconnectSet(void)
{
	CmdCtrlSocket = INVALID_SOCKET;
	TrnCtrlSocket = INVALID_SOCKET;

	EraseRemoteDirForWnd();
	DispWindowTitle();
	MakeButtonsFocus();
	SetTaskMsg(MSGJPN005);
	return;
}


/*----- �z�X�g�ɐڑ������ǂ�����Ԃ� ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X (YES/NO)
*----------------------------------------------------------------------------*/

int AskConnecting(void)
{
	int Sts;

	Sts = NO;
	if(TrnCtrlSocket != INVALID_SOCKET)
		Sts = YES;

	return(Sts);
}


/*----- �z�X�g�֐ڑ����� ------------------------------------------------------
*
*	Parameter
*		char *Host : �z�X�g��
*		char *User : ���[�U��
*		char *Pass : �p�X���[�h
*		char *Acct : �A�J�E���g
*		int Port : �|�[�g
*		int Fwall : FireWall���g�����ǂ��� (YES/NO)
*		int SavePass : �p�X���[�h���ē��͂������ɕۑ����邩�ǂ��� (YES/NO)
*		int Security : �Z�L�����e�B (SECURITY_xxx, MDx)
*
*	Return Value
*		SOCKET �\�P�b�g
*
*	Note
*		�z�X�g���A���[�U���A�p�X���[�h���w�肳��Ă��Ȃ������Ƃ��́A�ڑ��Ɏg�p
*		�������̂��R�s�[���Ă�����
*			char *Host : �z�X�g��
*			char *User : ���[�U��
*			char *Pass : �p�X���[�h
*			char *Acct : �A�J�E���g
*
*		FireWall�͎��̂悤�ɓ��삷��
*			TYPE1	Connect fire �� USER user(f) �� PASS pass(f) �� SITE host �� USER user(h) ��      PASS pass(h) �� ACCT acct
*			TYPE2	Connect fire �� USER user(f) �� PASS pass(f) ��              USER user(h)@host �� PASS pass(h) �� ACCT acct
*			TYPE3	Connect fire ��                                              USER user(h)@host �� PASS pass(h) �� ACCT acct
*			TYPE4	Connect fire ��                                 OPEN host �� USER user(h) ��      PASS pass(h) �� ACCT acct
*			TYPE5	SOCKS4
*			none	Connect host ��                                              USER user(h) ��      PASS pass(h) �� ACCT acct
*----------------------------------------------------------------------------*/

static SOCKET DoConnect(char *Host, char *User, char *Pass, char *Acct, int Port, int Fwall, int SavePass, int Security)
{
	int Sts;
	int Flg;
	int Anony;
	SOCKET ContSock;
	char Buf[1024];
	char Reply[1024];
	int Continue;
	int ReInPass;
	char *Tmp;
	int HostPort;
	static const char *SiteTbl[4] = { "SITE", "site", "OPEN", "open" };
	char TmpBuf[ONELINE_BUF_SIZE];
	struct linger LingerOpt;

	if(Fwall == YES)
		Fwall = FwallType;
	else
		Fwall = FWALL_NONE;

	TryConnect = YES;
	CancelFlg = NO;
#if 0
//	WSASetBlockingHook(BlkHookFnc);
#endif

	ContSock = INVALID_SOCKET;

	HostPort = Port;
	Tmp = Host;
	if(((Fwall >= FWALL_FU_FP_SITE) && (Fwall <= FWALL_OPEN)) ||
	   (Fwall == FWALL_SIDEWINDER) ||
	   (Fwall == FWALL_FU_FP))
	{
		Tmp = FwallHost;
		Port = FwallPort;
	}

	if(strlen(Tmp) != 0)
	{
		if((ContSock = connectsock(Tmp, Port, "", &CancelFlg)) != INVALID_SOCKET)
		{
			while((Sts = ReadReplyMessage(ContSock, Buf, 1024, &CancelFlg, TmpBuf) / 100) == FTP_PRELIM)
				;

			if(Sts == FTP_COMPLETE)
			{
				Flg = 1;
				if(setsockopt(ContSock, SOL_SOCKET, SO_OOBINLINE, (LPSTR)&Flg, sizeof(Flg)) == SOCKET_ERROR)
					ReportWSError("setsockopt", WSAGetLastError());
#pragma aaa
				Flg = 1;
				if(setsockopt(ContSock, SOL_SOCKET, SO_KEEPALIVE, (LPSTR)&Flg, sizeof(Flg)) == SOCKET_ERROR)
					ReportWSError("setsockopt", WSAGetLastError());
				LingerOpt.l_onoff = 1;
				LingerOpt.l_linger = 90;
				if(setsockopt(ContSock, SOL_SOCKET, SO_LINGER, (LPSTR)&LingerOpt, sizeof(LingerOpt)) == SOCKET_ERROR)
					ReportWSError("setsockopt", WSAGetLastError());
///////


				/*===== �F�؂��s�Ȃ� =====*/

				Sts = FTP_COMPLETE;
				if((Fwall == FWALL_FU_FP_SITE) ||
				   (Fwall == FWALL_FU_FP_USER) ||
				   (Fwall == FWALL_FU_FP))
				{
					if((Sts = command(ContSock, Reply, &CancelFlg, "USER %s", FwallUser) / 100) == FTP_CONTINUE)
					{
						CheckOneTimePassword(FwallPass, Reply, FwallSecurity);
						Sts = command(ContSock, NULL, &CancelFlg, "PASS %s", Reply) / 100;
					}
				}
				else if(Fwall == FWALL_SIDEWINDER)
				{
					Sts = command(ContSock, Reply, &CancelFlg, "USER %s:%s%c%s", FwallUser, FwallPass, FwallDelimiter, Host) / 100;
				}
				if((Sts != FTP_COMPLETE) && (Sts != FTP_CONTINUE))
				{
					SetTaskMsg(MSGJPN006);
					DoClose(ContSock);
					ContSock = INVALID_SOCKET;
				}
				else
				{
					if((Fwall == FWALL_FU_FP_SITE) || (Fwall == FWALL_OPEN))
					{
						Flg = 0;
						if(Fwall == FWALL_OPEN)
							Flg = 2;
						if(FwallLower == YES)
							Flg++;

						if(HostPort == PORT_NOR)
							Sts = command(ContSock, NULL, &CancelFlg, "%s %s", SiteTbl[Flg], Host) / 100;
						else
							Sts = command(ContSock, NULL, &CancelFlg, "%s %s %d", SiteTbl[Flg], Host, HostPort) / 100;
					}

					if((Sts != FTP_COMPLETE) && (Sts != FTP_CONTINUE))
					{
						SetTaskMsg(MSGJPN007, Host);
						DoClose(ContSock);
						ContSock = INVALID_SOCKET;
					}
					else
					{
						Anony = NO;
						if((strlen(User) != 0) || 
						   (InputDialogBox(username_dlg, GetMainHwnd(), NULL, User, USER_NAME_LEN+1, &Anony, IDH_HELP_TOPIC_0000001) == YES))
						{
							if(Anony == YES)
							{
								strcpy(User, "anonymous");
								strcpy(Pass, UserMailAdrs);
							}

							if((Fwall == FWALL_FU_FP_USER) || (Fwall == FWALL_USER))
							{
								if(HostPort == PORT_NOR)
									sprintf(Buf, "%s%c%s", User, FwallDelimiter, Host);
								else
									sprintf(Buf, "%s%c%s %d", User, FwallDelimiter, Host, HostPort);
							}
							else
								strcpy(Buf, User);

							ReInPass = NO;
							do
							{
								Continue = NO;
								if((Sts = command(ContSock, Reply, &CancelFlg, "USER %s", Buf) / 100) == FTP_CONTINUE)
								{
									if((strlen(Pass) != 0) || 
									   (InputDialogBox(passwd_dlg, GetMainHwnd(), NULL, Pass, PASSWORD_LEN+1, &Anony, IDH_HELP_TOPIC_0000001) == YES))
									{
										CheckOneTimePassword(Pass, Reply, Security);

										/* �p�X���[�h���X�y�[�X1�̎��̓p�X���[�h�̎��̂Ȃ��Ƃ��� */
										if(strcmp(Reply, " ") == 0)
											strcpy(Reply, "");

										Sts = command(ContSock, NULL, &CancelFlg, "PASS %s", Reply) / 100;
										if(Sts == FTP_ERROR)
										{
											strcpy(Pass, "");
											if(InputDialogBox(re_passwd_dlg, GetMainHwnd(), NULL, Pass, PASSWORD_LEN+1, &Anony, IDH_HELP_TOPIC_0000001) == YES)
												Continue = YES;
											else
												DoPrintf("No password specified.");
											ReInPass = YES;
										}
										else if(Sts == FTP_CONTINUE)
										{
											if((strlen(Acct) != 0) || 
											   (InputDialogBox(account_dlg, GetMainHwnd(), NULL, Acct, ACCOUNT_LEN+1, &Anony, IDH_HELP_TOPIC_0000001) == YES))
											{
												Sts = command(ContSock, NULL, &CancelFlg, "ACCT %s", Acct) / 100;
											}
											else
												DoPrintf("No account specified");
										}
									}
									else
									{
										Sts = FTP_ERROR;
										DoPrintf("No password specified.");
									}
								}
							}
							while(Continue == YES);
						}
						else
						{
							Sts = FTP_ERROR;
							DoPrintf("No user name specified");
						}

						if(Sts != FTP_COMPLETE)
						{
							SetTaskMsg(MSGJPN008, Host);
							DoClose(ContSock);
							ContSock = INVALID_SOCKET;
						}
						else if((SavePass == YES) && (ReInPass == YES))
						{
							if(DialogBox(GetFtpInst(), MAKEINTRESOURCE(savepass_dlg), GetMainHwnd(), ExeEscDialogProc) == YES)
								SetHostPassword(AskCurrentHost(), Pass);
						}
					}
				}
			}
			else
			{
//#pragma aaa
				SetTaskMsg(MSGJPN009/*"�ڑ��ł��܂���(1) %x", ContSock*/);
				DoClose(ContSock);
				ContSock = INVALID_SOCKET;
			}
		}
	}
	else
	{

		if(((Fwall >= FWALL_FU_FP_SITE) && (Fwall <= FWALL_OPEN)) ||
		   (Fwall == FWALL_FU_FP))
			SetTaskMsg(MSGJPN010);
		else
			SetTaskMsg(MSGJPN011);
	}

#if 0
//	WSAUnhookBlockingHook();
#endif
	TryConnect = NO;

	return(ContSock);
}


/*----- �����^�C���p�X���[�h�̃`�F�b�N ----------------------------------------
*
*	Parameter
*		chat *Pass : �p�X���[�h�^�p�X�t���[�Y
*		char *Reply : USER�R�}���h�𑗂������Ƃ̃��v���C������
*						�^PASS�R�}���h�ő���p�X���[�h��Ԃ��o�b�t�@
*		int Type : �^�C�v (SECURITY_xxx, MDx)
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*
*	Note
*		�����^�C���p�X���[�h�łȂ�����Pass�����̂܂�Reply�ɃR�s�[
*----------------------------------------------------------------------------*/

static int CheckOneTimePassword(char *Pass, char *Reply, int Type)
{
	int Sts;
	char *Pos;
	int Seq;
	char Seed[MAX_SEED_LEN+1];
	int i;

	Sts = SUCCESS;
	Pos = NULL;

	if(Type == SECURITY_AUTO)
	{
		if((Pos = stristr(Reply, "otp-md5")) != NULL)
		{
			Type = MD5;
			SetTaskMsg(MSGJPN012);
		}
		else if((Pos = stristr(Reply, "otp-sha1")) != NULL)
		{
			Type = SHA1;
			SetTaskMsg(MSGJPN013);
		}
		else if(((Pos = stristr(Reply, "otp-md4")) != NULL) || ((Pos = stristr(Reply, "s/key")) != NULL))
		{
			Type = MD4;
			SetTaskMsg(MSGJPN014);
		}
	}
	else
		Pos = GetNextField(Reply);

	if((Type == MD4) || (Type == MD5) || (Type == SHA1))
	{
		/* �V�[�P���X�ԍ��������郋�[�v */
		DoPrintf("Analize OTP");
		DoPrintf("%s", Pos);
		Sts = FAIL;
		while((Pos = GetNextField(Pos)) != NULL)
		{
			if(IsDigit(*Pos))
			{
				Seq = atoi(Pos);
				DoPrintf("Sequence=%d", Seq);

				/* Seed */
				if((Pos = GetNextField(Pos)) != NULL)
				{
					if(GetOneField(Pos, Seed, MAX_SEED_LEN) == SUCCESS)
					{
						/* Seed�͉p�����̂ݗL���Ƃ��� */
						for(i = strlen(Seed)-1; i >= 0; i--)
						{
							if((IsAlpha(Seed[i]) == 0) && (IsDigit(Seed[i]) == 0))
								Seed[i] = NUL;
						}
						if(strlen(Seed) > 0)
						{
							DoPrintf("Seed=%s", Seed);
							Make6WordPass(Seq, Seed, Pass, Type, Reply);
							DoPrintf("Response=%s", Reply);

							/* �V�[�P���X�ԍ��̃`�F�b�N�ƌx�� */
							if(Seq <= 10)
								DialogBox(GetFtpInst(), MAKEINTRESOURCE(otp_notify_dlg), GetMainHwnd(), ExeEscDialogProc);

							Sts = SUCCESS;
						}
					}
				}
				break;
			}
		}

		if(Sts == FAIL)
			SetTaskMsg(MSGJPN015);
	}
	else
	{
		strcpy(Reply, Pass);
		DoPrintf("No OTP used.");
	}
	return(Sts);
}














/*----- �\�P�b�g��ڑ����� ----------------------------------------------------
*
*	Parameter
*		char *host : �z�X�g��
*		int port : �|�[�g�ԍ�
*		char *PreMsg : ���b�Z�[�W�̑O������
*
*	Return Value
*		SOCKET �\�P�b�g
*----------------------------------------------------------------------------*/

SOCKET connectsock(char *host, int port, char *PreMsg, int *CancelCheckWork)
{
	struct sockaddr_in saSockAddr;
	char HostEntry[MAXGETHOSTSTRUCT];
	struct hostent *pHostEntry;
	SOCKET sSocket;
	int Len;
	int Fwall;
	SOCKS4CMD Socks4Cmd;
	SOCKS4REPLY Socks4Reply;
	SOCKS5REQUEST Socks5Cmd;
	SOCKS5REPLY Socks5Reply;

	//////////////////////////////
	// �z�X�g�������Ɛڑ��̏���
	//////////////////////////////

	Fwall = FWALL_NONE;
	if(AskHostFireWall() == YES)
		Fwall = FwallType;

	sSocket = INVALID_SOCKET;

	UseIPadrs = YES;
	strcpy(DomainName, host);
	memset(&CurSockAddr, 0, sizeof(CurSockAddr));
	CurSockAddr.sin_port = htons((u_short)port);
	CurSockAddr.sin_family = AF_INET;
	if((CurSockAddr.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
	{
		// �z�X�g�����w�肳�ꂽ
		// �z�X�g������A�h���X�����߂�
		if(((Fwall == FWALL_SOCKS5_NOAUTH) || (Fwall == FWALL_SOCKS5_USER)) &&
		   (FwallResolv == YES))
		{
			// �z�X�g��������SOCKS�T�[�o�ɔC����
			pHostEntry = NULL;
		}
		else
		{
			// �A�h���X���擾
			SetTaskMsg(MSGJPN016, DomainName);
			pHostEntry = do_gethostbyname(host, HostEntry, MAXGETHOSTSTRUCT, CancelCheckWork);
		}

		if(pHostEntry != NULL)
		{
			memcpy((char *)&CurSockAddr.sin_addr, pHostEntry->h_addr, pHostEntry->h_length);
			SetTaskMsg(MSGJPN017, PreMsg, DomainName, inet_ntoa(CurSockAddr.sin_addr), ntohs(CurSockAddr.sin_port));
		}
		else
		{
			if((Fwall == FWALL_SOCKS5_NOAUTH) || (Fwall == FWALL_SOCKS5_USER))
			{
				UseIPadrs = NO;
				SetTaskMsg(MSGJPN018, PreMsg, DomainName, ntohs(CurSockAddr.sin_port));
			}
			else
			{
				SetTaskMsg(MSGJPN019, host);
				return(INVALID_SOCKET);
			}
		}
	}
	else
		SetTaskMsg(MSGJPN020, PreMsg, inet_ntoa(CurSockAddr.sin_addr), ntohs(CurSockAddr.sin_port));

	if((Fwall == FWALL_SOCKS4) || (Fwall == FWALL_SOCKS5_NOAUTH) || (Fwall == FWALL_SOCKS5_USER))
	{
		// SOCKS���g��
		// SOCKS�ɐڑ����鏀��
		if(Fwall == FWALL_SOCKS4)
		{
			Socks4Cmd.Ver = SOCKS4_VER;
			Socks4Cmd.Cmd = SOCKS4_CMD_CONNECT;
			Socks4Cmd.Port = CurSockAddr.sin_port;
			Socks4Cmd.AdrsInt = CurSockAddr.sin_addr.s_addr;
			strcpy(Socks4Cmd.UserID, FwallUser);
			Len = offsetof(SOCKS4CMD, UserID) + strlen(FwallUser) + 1;
		}
		else
		{
			Len = Socks5MakeCmdPacket(&Socks5Cmd, SOCKS5_CMD_CONNECT, UseIPadrs, CurSockAddr.sin_addr.s_addr, DomainName, CurSockAddr.sin_port);
		}

		memset(&SocksSockAddr, 0, sizeof(SocksSockAddr));
		if((SocksSockAddr.sin_addr.s_addr = inet_addr(FwallHost)) == INADDR_NONE)
		{
			if((pHostEntry = do_gethostbyname(FwallHost, HostEntry, MAXGETHOSTSTRUCT, CancelCheckWork)) != NULL)
				memcpy((char *)&SocksSockAddr.sin_addr, pHostEntry->h_addr, pHostEntry->h_length);
			else
			{
				SetTaskMsg(MSGJPN021, FwallHost);
				return INVALID_SOCKET;
			}
		}
		SocksSockAddr.sin_port = htons((u_short)FwallPort);
		SocksSockAddr.sin_family = AF_INET;
		SetTaskMsg(MSGJPN022, inet_ntoa(SocksSockAddr.sin_addr), ntohs(SocksSockAddr.sin_port));
		// connect�Őڑ�������SOCKS�T�[�o
		memcpy(&saSockAddr, &SocksSockAddr, sizeof(SocksSockAddr));
	}
	else
	{
		// connect�Őڑ�����̂͐ڑ���̃z�X�g
		memcpy(&saSockAddr, &CurSockAddr, sizeof(CurSockAddr));
	}

	/////////////
	// �ڑ����s
	/////////////

	if((sSocket = do_socket(AF_INET, SOCK_STREAM, TCP_PORT)) != INVALID_SOCKET)
	{
		if(do_connect(sSocket, (struct sockaddr *)&saSockAddr, sizeof(saSockAddr), CancelCheckWork) != SOCKET_ERROR)
		{
			if(Fwall == FWALL_SOCKS4)
			{
				Socks4Reply.Result = -1;
				if((SocksSendCmd(sSocket, &Socks4Cmd, Len, CancelCheckWork) != SUCCESS) ||
				   (Socks4GetCmdReply(sSocket, &Socks4Reply) != SUCCESS) || 
				   (Socks4Reply.Result != SOCKS4_RES_OK))
				{
					SetTaskMsg(MSGJPN023, Socks4Reply.Result);
					DoClose(sSocket);
					sSocket = INVALID_SOCKET;
				}
			}
			else if((Fwall == FWALL_SOCKS5_NOAUTH) || (Fwall == FWALL_SOCKS5_USER))
			{
				if(Socks5SelMethod(sSocket, CancelCheckWork) == FAIL)
				{
					DoClose(sSocket);
					sSocket = INVALID_SOCKET;
				}

				Socks5Reply.Result = -1;
				if((SocksSendCmd(sSocket, &Socks5Cmd, Len, CancelCheckWork) != SUCCESS) ||
				   (Socks5GetCmdReply(sSocket, &Socks5Reply) != SUCCESS) || 
				   (Socks5Reply.Result != SOCKS5_RES_OK))
				{
					SetTaskMsg(MSGJPN024, Socks5Reply.Result);
					DoClose(sSocket);
					sSocket = INVALID_SOCKET;
				}

			}

			if(sSocket != INVALID_SOCKET)
				SetTaskMsg(MSGJPN025);
		}
		else
		{
//#pragma aaa
			SetTaskMsg(MSGJPN026/*"�ڑ��ł��܂���(2) %x", sSocket*/);
			DoClose(sSocket);
			sSocket = INVALID_SOCKET;
		}
	}
	else
		SetTaskMsg(MSGJPN027);

	return(sSocket);
}


/*----- ���b�X���\�P�b�g���擾 ------------------------------------------------
*
*	Parameter
*		SOCKET ctrl_skt : �R���g���[���\�P�b�g
*
*	Return Value
*		SOCKET ���b�X���\�P�b�g
*----------------------------------------------------------------------------*/

SOCKET GetFTPListenSocket(SOCKET ctrl_skt, int *CancelCheckWork)
{
    SOCKET listen_skt;
    int iLength;
    char *a,*p;
	struct sockaddr_in saCtrlAddr;
	struct sockaddr_in saTmpAddr;
	SOCKS4CMD Socks4Cmd;
	SOCKS4REPLY Socks4Reply;
	SOCKS5REQUEST Socks5Cmd;
	SOCKS5REPLY Socks5Reply;

	int Len;
	int Fwall;

	Fwall = FWALL_NONE;
	if(AskHostFireWall() == YES)
		Fwall = FwallType;

	if((listen_skt = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != INVALID_SOCKET)
	{
		if(Fwall == FWALL_SOCKS4)
		{
			/*===== SOCKS4���g�� =====*/
			DoPrintf("Use SOCKS4 BIND");
			if(do_connect(listen_skt, (struct sockaddr *)&SocksSockAddr, sizeof(SocksSockAddr), CancelCheckWork) != SOCKET_ERROR)
			{
				Socks4Cmd.Ver = SOCKS4_VER;
				Socks4Cmd.Cmd = SOCKS4_CMD_BIND;
				Socks4Cmd.Port = CurSockAddr.sin_port;
				Socks4Cmd.AdrsInt = CurSockAddr.sin_addr.s_addr;
				strcpy(Socks4Cmd.UserID, FwallUser);
				Len = offsetof(SOCKS4CMD, UserID) + strlen(FwallUser) + 1;

				Socks4Reply.Result = -1;
				if((SocksSendCmd(listen_skt, &Socks4Cmd, Len, CancelCheckWork) != SUCCESS) ||
				   (Socks4GetCmdReply(listen_skt, &Socks4Reply) != SUCCESS) || 
				   (Socks4Reply.Result != SOCKS4_RES_OK))
				{
					SetTaskMsg(MSGJPN028, Socks4Reply.Result);
					DoClose(listen_skt);
					listen_skt = INVALID_SOCKET;
				}

				if(Socks4Reply.AdrsInt == 0)
					Socks4Reply.AdrsInt = SocksSockAddr.sin_addr.s_addr;

				a = (char *)&Socks4Reply.AdrsInt;
				p = (char *)&Socks4Reply.Port;
			}
		}
		else if((Fwall == FWALL_SOCKS5_NOAUTH) || (Fwall == FWALL_SOCKS5_USER))
		{
			/*===== SOCKS5���g�� =====*/
			DoPrintf("Use SOCKS5 BIND");
			if(do_connect(listen_skt, (struct sockaddr *)&SocksSockAddr, sizeof(SocksSockAddr), CancelCheckWork) != SOCKET_ERROR)
			{
				if(Socks5SelMethod(listen_skt, CancelCheckWork) == FAIL)
				{
					DoClose(listen_skt);
					listen_skt = INVALID_SOCKET;
					return(listen_skt);
				}

				Len = Socks5MakeCmdPacket(&Socks5Cmd, SOCKS5_CMD_BIND, UseIPadrs, CurSockAddr.sin_addr.s_addr, DomainName, CurSockAddr.sin_port);

				Socks5Reply.Result = -1;
				if((SocksSendCmd(listen_skt, &Socks5Cmd, Len, CancelCheckWork) != SUCCESS) ||
				   (Socks5GetCmdReply(listen_skt, &Socks5Reply) != SUCCESS) || 
				   (Socks5Reply.Result != SOCKS5_RES_OK))
				{
					SetTaskMsg(MSGJPN029, Socks5Reply.Result);
					DoClose(listen_skt);
					listen_skt = INVALID_SOCKET;
				}

				if(Socks5Reply.AdrsInt == 0)
					Socks5Reply.AdrsInt = SocksSockAddr.sin_addr.s_addr;

				a = (char *)&Socks5Reply.AdrsInt;
				p = (char *)&Socks5Reply.Port;
			}
		}
		else
		{
			/*===== SOCKS���g��Ȃ� =====*/
			DoPrintf("Use normal BIND");
			saCtrlAddr.sin_port = htons(0);
			saCtrlAddr.sin_family = AF_INET;
			saCtrlAddr.sin_addr.s_addr = 0;

			if(bind(listen_skt, (struct sockaddr *)&saCtrlAddr, sizeof(struct sockaddr)) != SOCKET_ERROR)
			{
				iLength = sizeof(saCtrlAddr);
				if(getsockname(listen_skt, (struct sockaddr *)&saCtrlAddr, &iLength) != SOCKET_ERROR)
				{
					if(do_listen(listen_skt, 1) == 0)
					{
						iLength = sizeof(saTmpAddr);
						if(getsockname(ctrl_skt, (struct sockaddr *)&saTmpAddr, &iLength) == SOCKET_ERROR)
							ReportWSError("getsockname", WSAGetLastError());

						a = (char *)&saTmpAddr.sin_addr;
						p = (char *)&saCtrlAddr.sin_port;
					}
					else
					{
						ReportWSError("listen", WSAGetLastError());
						do_closesocket(listen_skt);
						listen_skt = INVALID_SOCKET;
					}
				}
				else
				{
					ReportWSError("getsockname", WSAGetLastError());
					do_closesocket(listen_skt);
					listen_skt = INVALID_SOCKET;
				}
			}
			else
			{
				ReportWSError("bind", WSAGetLastError());
				do_closesocket(listen_skt);
				listen_skt = INVALID_SOCKET;
			}

			if(listen_skt == INVALID_SOCKET)
				SetTaskMsg(MSGJPN030);
		}
	}
	else
		ReportWSError("socket create", WSAGetLastError());

	if(listen_skt != INVALID_SOCKET)
	{
#define  UC(b)  (((int)b)&0xff)
		if((command(ctrl_skt,NULL, &CancelFlg, "PORT %d,%d,%d,%d,%d,%d",
				UC(a[0]), UC(a[1]), UC(a[2]), UC(a[3]),
				UC(p[0]), UC(p[1])) / 100) != FTP_COMPLETE)
		{
			SetTaskMsg(MSGJPN031);
			do_closesocket(listen_skt);
			listen_skt = INVALID_SOCKET;
		}
//		else
//			DoPrintf("Skt=%u : listener %s port %u",listen_skt,inet_ntoa(saCtrlAddr.sin_addr),ntohs(saCtrlAddr.sin_port));
	}

	return(listen_skt);
}


/*----- �z�X�g�֐ڑ����������ǂ�����Ԃ�---------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X
*			YES/NO
*----------------------------------------------------------------------------*/

int AskTryingConnect(void)
{
	return(TryConnect);
}


#if 0
///*----- �u���b�L���O�R�[���̃t�b�N�R�[���o�b�N --------------------------------
//*
//*	Parameter
//*		�Ȃ�
//*
//*	Return Value
//*		BOOL FALSE
//*----------------------------------------------------------------------------*/
//
//static BOOL CALLBACK BlkHookFnc(void)
//{
//	BackgrndMessageProc();
//
//	if(CancelFlg == YES)
//	{
//		SetTaskMsg(MSGJPN032);
//		WSACancelBlockingCall();
//		CancelFlg = NO;
//	}
//	return(FALSE);
//}
#endif



/*----- SOCKS5�̃R�}���h�p�P�b�g���쐬���� ------------------------------------
*
*	Parameter
*		SOCKS5REQUEST *Packet : �p�P�b�g���쐬���郏�[�N
*		char Cmd : �R�}���h
*		int ValidIP : IP�A�h���X���g�����ǂ���(YES/NO)
*		ulong IP : IP�A�h���X
*		char *Host : �z�X�g��
*		ushort Port : �|�[�g
*
*	Return Value
*		int �R�}���h�p�P�b�g�̒���
*----------------------------------------------------------------------------*/

static int Socks5MakeCmdPacket(SOCKS5REQUEST *Packet, char Cmd, int ValidIP, ulong IP, char *Host, ushort Port)
{
	uchar *Pos;
	int Len;
	int TotalLen;

	Pos = (uchar *)Packet;
	Pos += SOCKS5REQUEST_SIZE;
	TotalLen = SOCKS5REQUEST_SIZE + 2;	/* +2�̓|�[�g�̕� */

	Packet->Ver = SOCKS5_VER;
	Packet->Cmd = Cmd;
	Packet->Rsv = 0;
	if(ValidIP == YES)
	{
		/* IP�A�h���X���w�� */
		Packet->Type = SOCKS5_ADRS_IPV4;
		*((ulong *)Pos) = IP;
		Pos += 4;
		TotalLen += 4;
	}
	else
	{
		/* �z�X�g�����w�� */
		Packet->Type = SOCKS5_ADRS_NAME;
		Len = strlen(Host);
		*Pos++ = Len;
		strcpy(Pos, Host);
		Pos += Len;
		TotalLen += Len + 1;
	}
	*((ushort *)Pos) = Port;

	return(TotalLen);
}


/*----- SOCKS�̃R�}���h�𑗂� -------------------------------------------------
*
*	Parameter
*		SOCKET Socket : �\�P�b�g
*		void *Data : ����f�[�^
*		int Size : �T�C�Y
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL)
*----------------------------------------------------------------------------*/

static int SocksSendCmd(SOCKET Socket, void *Data, int Size, int *CancelCheckWork)
{
	int Ret;

	Ret = SendData(Socket, (char *)Data, Size, 0, CancelCheckWork);

	if(Ret != SUCCESS)
		SetTaskMsg(MSGJPN033, *((short *)Data));

	return(Ret);
}


/*----- SOCKS5�̃R�}���h�ɑ΂��郊�v���C�p�P�b�g����M���� --------------------
*
*	Parameter
*		SOCKET Socket : �\�P�b�g
*		SOCKS5REPLY *Packet : �p�P�b�g
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL)
*----------------------------------------------------------------------------*/

static int Socks5GetCmdReply(SOCKET Socket, SOCKS5REPLY *Packet)
{
	uchar *Pos;
	int Len;
	int Ret;

	Pos = (uchar *)Packet;
	Pos += SOCKS5REPLY_SIZE;

	if((Ret = ReadNchar(Socket, (char *)Packet, SOCKS5REPLY_SIZE, &CancelFlg)) == SUCCESS)
	{
		if(Packet->Type == SOCKS5_ADRS_IPV4)
			Len = 4 + 2;
		else if(Packet->Type == SOCKS5_ADRS_IPV6)
			Len = 6 + 2;
		else
		{
			if((Ret = ReadNchar(Socket, (char *)Pos, 1, &CancelFlg)) == SUCCESS)
			{
				Len = *Pos + 2;
				Pos++;
			}
		}

		if(Ret == SUCCESS)
			Ret = ReadNchar(Socket, (char *)Pos, Len, &CancelFlg);
	}

	if(Ret != SUCCESS)
		SetTaskMsg(MSGJPN034);

	return(Ret);
}


/*----- SOCKS4�̃R�}���h�ɑ΂��郊�v���C�p�P�b�g����M���� --------------------
*
*	Parameter
*		SOCKET Socket : �\�P�b�g
*		SOCKS5REPLY *Packet : �p�P�b�g
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL)
*----------------------------------------------------------------------------*/

static int Socks4GetCmdReply(SOCKET Socket, SOCKS4REPLY *Packet)
{
	int Ret;

	Ret = ReadNchar(Socket, (char *)Packet, SOCKS4REPLY_SIZE, &CancelFlg);

	if(Ret != SUCCESS)
		DoPrintf(MSGJPN035);

	return(Ret);
}


/*----- SOCKS5�̔F�؂��s�� ----------------------------------------------------
*
*	Parameter
*		SOCKET Socket : �\�P�b�g
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL)
*----------------------------------------------------------------------------*/

static int Socks5SelMethod(SOCKET Socket, int *CancelCheckWork)
{
	int Ret;
	SOCKS5METHODREQUEST Socks5Method;
	SOCKS5METHODREPLY Socks5MethodReply;
	SOCKS5USERPASSSTATUS Socks5Status;
	char Buf[USER_NAME_LEN + PASSWORD_LEN + 4];
	int Len;
	int Len2;

	Ret = SUCCESS;
	Socks5Method.Ver = SOCKS5_VER;
	Socks5Method.Num = 1;
	if(FwallType == FWALL_SOCKS5_NOAUTH)
		Socks5Method.Methods[0] = SOCKS5_AUTH_NONE;
	else
		Socks5Method.Methods[0] = SOCKS5_AUTH_USER;

	if((SocksSendCmd(Socket, &Socks5Method, SOCKS5METHODREQUEST_SIZE, CancelCheckWork) != SUCCESS) ||
	   (ReadNchar(Socket, (char *)&Socks5MethodReply, SOCKS5METHODREPLY_SIZE, &CancelFlg) != SUCCESS) ||
	   (Socks5MethodReply.Method == (uchar)0xFF))
	{
		SetTaskMsg(MSGJPN036);
		Ret = FAIL;
	}
	else if(Socks5MethodReply.Method == SOCKS5_AUTH_USER)
	{
		DoPrintf("SOCKS5 User/Pass Authentication");
		Buf[0] = SOCKS5_USERAUTH_VER;
		Len = strlen(FwallUser);
		Len2 = strlen(FwallPass);
		Buf[1] = Len;
		strcpy(Buf+2, FwallUser);
		Buf[2 + Len] = Len2;
		strcpy(Buf+3+Len, FwallPass);

		if((SocksSendCmd(Socket, &Buf, Len+Len2+3, CancelCheckWork) != SUCCESS) ||
		   (ReadNchar(Socket, (char *)&Socks5Status, SOCKS5USERPASSSTATUS_SIZE, &CancelFlg) != SUCCESS) ||
		   (Socks5Status.Status != 0))
		{
			SetTaskMsg(MSGJPN037);
			Ret = FAIL;
		}
	}
	else
		DoPrintf("SOCKS5 No Authentication");

	return(Ret);
}


/*----- SOCKS��BIND�̑�Q���v���C���b�Z�[�W���󂯎�� -------------------------
*
*	Parameter
*		SOCKET Socket : �\�P�b�g
*		SOCKET *Data : �f�[�^�\�P�b�g��Ԃ����[�N
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL)
*----------------------------------------------------------------------------*/

int SocksGet2ndBindReply(SOCKET Socket, SOCKET *Data)
{
	int Ret;
	char Buf[300];

	Ret = FAIL;
	if((AskHostFireWall() == YES) && (FwallType == FWALL_SOCKS4))
	{
		Socks4GetCmdReply(Socket, (SOCKS4REPLY *)Buf);
		*Data = Socket;
		Ret = SUCCESS;
	}
	else if((AskHostFireWall() == YES) &&
			((FwallType == FWALL_SOCKS5_NOAUTH) || (FwallType == FWALL_SOCKS5_USER)))
	{
		Socks5GetCmdReply(Socket, (SOCKS5REPLY *)Buf);
		*Data = Socket;
		Ret = SUCCESS;
	}
	return(Ret);
}



