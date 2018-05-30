/*=============================================================================
*
*								�c�[���o�[�ƃ��j���[
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

#define  STRICT
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbstring.h>
#include <malloc.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdarg.h>
#include <winsock.h>

#include "common.h"
#include "resource.h"


/*===== �v���g�^�C�v =====*/

static void AddOpenMenu(HMENU hMenu, UINT Flg);
static LRESULT CALLBACK HistEditBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/* 2007/09/21 sunasunamix  �������� *********************/
static LRESULT CALLBACK CountermeasureTbarMainProc(HWND hWnd,UINT uMessage,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK CountermeasureTbarLocalProc(HWND hWnd,UINT uMessage,WPARAM wParam,LPARAM lParam);
static LRESULT CALLBACK CountermeasureTbarRemoteProc(HWND hWnd,UINT uMessage,WPARAM wParam,LPARAM lParam);
/********************************************* �����܂� */


/*===== �O���Q�� =====*/

extern int SepaWidth;
extern int RemoteWidth;

extern int CancelFlg;

/* �ݒ�l */
extern int DotFile;
extern char AsciiExt[ASCII_EXT_LEN+1];
extern int TransMode;
extern int ListType;
extern int LocalWidth;
extern char ViewerName[VIEWERS][FMAX_PATH+1];
extern int TransMode;
extern int SortSave;

/*===== ���[�J���ȃ��[�N =====*/

static HWND hWndTbarMain = NULL;
static HWND hWndTbarLocal = NULL;
static HWND hWndTbarRemote = NULL;
static HWND hWndDirLocal = NULL;
static HWND hWndDirRemote = NULL;
static HWND hWndDirLocalEdit = NULL;
static HWND hWndDirRemoteEdit = NULL;

static WNDPROC HistEditBoxProcPtr;

static HFONT DlgFont = NULL;

static int TmpTransMode;
static int TmpHostKanjiCode;
static int TmpHostKanaCnv;

static int TmpLocalFileSort;
static int TmpLocalDirSort;
static int TmpRemoteFileSort;
static int TmpRemoteDirSort;

static int SyncMove = NO;

static int HideUI = NO;


/* 2007/09/21 sunasunamix  �������� *********************/
static WNDPROC pOldTbarMainProc   = NULL;
static WNDPROC pOldTbarLocalProc  = NULL;
static WNDPROC pOldTbarRemoteProc = NULL;
/********************************************* �����܂� */


/* �ȑO�A�R���{�{�b�N�X�ɃJ�����g�t�H���_�����������Ă�������� */
/* ���̃t�@�C���ŃJ�����g�t�H���_������������ */
static char LocalCurDir[FMAX_PATH+1];
static char RemoteCurDir[FMAX_PATH+1];


/* ���C���̃c�[���o�[ */
static TBBUTTON TbarDataMain[] = {
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 0,  MENU_CONNECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 16, MENU_QUICK, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 1,  MENU_DISCONNECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 2,  MENU_DOWNLOAD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 3,  MENU_UPLOAD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 24, MENU_MIRROR_UPLOAD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 4,  MENU_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 5,  MENU_RENAME, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 6,  MENU_MKDIR, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 7,  MENU_TEXT, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0 },
	{ 8,  MENU_BINARY, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0 },
	{ 17, MENU_AUTO, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 20, MENU_KNJ_EUC, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0 },
	{ 21, MENU_KNJ_JIS, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0 },
	{ 22, MENU_KNJ_NONE, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 23, MENU_KANACNV, TBSTATE_ENABLED, TBSTYLE_CHECK, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 15, MENU_REFRESH, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 18, MENU_LIST, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0 },
	{ 19, MENU_REPORT, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 25, MENU_SYNC, TBSTATE_ENABLED, TBSTYLE_CHECK, 0, 0 },
	{ 0,  0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 26, MENU_ABORT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
};

/* ���[�J�����̃c�[���o�[ */
static TBBUTTON TbarDataLocal[] = {
	{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 0, MENU_LOCAL_UPDIR, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 1, MENU_LOCAL_CHDIR, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 }
};

/* �z�X�g���̃c�[���o�[ */
static TBBUTTON TbarDataRemote[] = {
	{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ 0, MENU_REMOTE_UPDIR, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 1, MENU_REMOTE_CHDIR, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 }
};

/* �S�{�^���^���j���[���� */
static const int HideMenus[] = {
	MENU_CONNECT,		MENU_QUICK,			MENU_DISCONNECT,
	MENU_SET_CONNECT,	MENU_IMPORT_WS,		MENU_EXIT,
	MENU_DOWNLOAD,		MENU_UPLOAD,		MENU_DOWNLOAD_AS,	MENU_MIRROR_UPLOAD,
	MENU_UPLOAD_AS,		MENU_DOWNLOAD_NAME,	MENU_MIRROR_UPLOAD,
	MENU_FILESIZE,		MENU_DELETE,		MENU_RENAME,
	MENU_CHMOD,			MENU_MKDIR,			MENU_SOMECMD,
	MENU_SYNC,
	MENU_BMARK_ADD,		MENU_BMARK_ADD_LOCAL, MENU_BMARK_ADD_BOTH,
	MENU_BMARK_EDIT,
	MENU_FILTER,		MENU_FIND,			MENU_FINDNEXT,		MENU_SELECT,
	MENU_SELECT_ALL,	MENU_LIST,			MENU_REPORT,
	MENU_SORT,			MENU_DOTFILE,
	MENU_DIRINFO,		MENU_TASKINFO,		MENU_REFRESH,
	MENU_OPTION,
	MENU_OTPCALC,
	MENU_HELP,			MENU_HELP_TROUBLE,	MENU_ABOUT,
	MENU_REGINIT,
	MENU_TEXT,			MENU_BINARY,		MENU_AUTO,
	MENU_KNJ_EUC,		MENU_KNJ_JIS,		MENU_KNJ_NONE,
	MENU_KANACNV,
	MENU_LOCAL_UPDIR,	MENU_LOCAL_CHDIR,
	MENU_REMOTE_UPDIR,	MENU_REMOTE_CHDIR,
	MENU_HIST_1,		MENU_HIST_2,		MENU_HIST_3,		MENU_HIST_4,
	MENU_HIST_5,		MENU_HIST_6,		MENU_HIST_7,		MENU_HIST_8,
	MENU_HIST_9,		MENU_HIST_10,		MENU_HIST_11,		MENU_HIST_12,
	MENU_HIST_13,		MENU_HIST_14,		MENU_HIST_15,		MENU_HIST_16,
	MENU_HIST_17,		MENU_HIST_18,		MENU_HIST_19,		MENU_HIST_20
};



/*----- �c�[���o�[���쐬���� --------------------------------------------------
*
*	Parameter
*		HWND hWnd : �e�E�C���h�E�̃E�C���h�E�n���h��
*		HINSTANCE hInst : �C���X�^���X�n���h��
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int MakeToolBarWindow(HWND hWnd, HINSTANCE hInst)
{
	int Sts;
	RECT Rect1;
	char Tmp[FMAX_PATH+1];
	char *Pos;
	int Tmp2;
	DWORD NoDrives;

	/*===== ���C���̃c�[���o�[ =====*/

	hWndTbarMain = CreateToolbarEx(
				hWnd,
				WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | CCS_TOP,
				1,
				27,
				hInst,
				main_toolbar_bmp,
				TbarDataMain,
				sizeof(TbarDataMain)/sizeof(TBBUTTON),
				16,16,
				16,16,
				sizeof(TBBUTTON));

	if(hWndTbarMain != NULL)
	{
		/* 2007/09/21 sunasunamix  �������� *********************/
		pOldTbarMainProc = (WNDPROC)SetWindowLong(hWndTbarMain, GWL_WNDPROC, (DWORD)CountermeasureTbarMainProc);
		/********************************************* �����܂� */

		GetClientRect(hWnd, &Rect1);
		MoveWindow(hWndTbarMain, 0, 0, Rect1.right, TOOLWIN_HEIGHT, FALSE);
	}

	/*===== ���[�J���̃c�[���o�[ =====*/

	hWndTbarLocal = CreateToolbarEx(
				hWnd,
				WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | CCS_NORESIZE,
				2,
				2,
				hInst,
				remote_toolbar_bmp,
				TbarDataLocal,
				sizeof(TbarDataLocal)/sizeof(TBBUTTON),
				16,16,
				16,16,
				sizeof(TBBUTTON));

	if(hWndTbarLocal != NULL)
	{
		/* 2007/09/21 sunasunamix  �������� *********************/
		pOldTbarLocalProc = (WNDPROC)SetWindowLong(hWndTbarLocal, GWL_WNDPROC, (DWORD)CountermeasureTbarLocalProc);
		/********************************************* �����܂� */

		MoveWindow(hWndTbarLocal, 0, TOOLWIN_HEIGHT, LocalWidth, TOOLWIN_HEIGHT, FALSE);

		/*===== ���[�J���̃f�B���N�g�����E�C���h�E =====*/

		SendMessage(hWndTbarLocal, TB_GETITEMRECT, 3, (LPARAM)&Rect1);
#ifndef ENGLISH
		DlgFont = CreateFont(Rect1.bottom-Rect1.top-8, 0, 0, 0, 0, FALSE,FALSE,FALSE,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,MSGJPN254);
#else
		DlgFont = CreateFont(Rect1.bottom-Rect1.top-8, 0, 0, 0, 0, FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,MSGJPN254);
#endif

		hWndDirLocal = CreateWindowEx(WS_EX_CLIENTEDGE,
					"COMBOBOX", "",
					WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWN | CBS_SORT | CBS_AUTOHSCROLL,
					Rect1.right, Rect1.top, LocalWidth - Rect1.right, 200,
					hWndTbarLocal, (HMENU)COMBO_LOCAL, hInst, NULL);

		if(hWndDirLocal != NULL)
		{
			/* �G�f�B�b�g�R���g���[����T�� */
			hWndDirLocalEdit = GetWindow(hWndDirLocal, GW_CHILD);
			if(hWndDirLocalEdit != NULL)
				HistEditBoxProcPtr = (WNDPROC)SetWindowLong(hWndDirLocalEdit, GWL_WNDPROC, (LONG)HistEditBoxWndProc);

			SendMessage(hWndDirLocal, WM_SETFONT, (WPARAM)DlgFont, MAKELPARAM(TRUE, 0));
			SendMessage(hWndDirLocal, CB_LIMITTEXT, FMAX_PATH, 0);

			/* �h���C�u�����Z�b�g���Ă��� */
			GetLogicalDriveStrings(FMAX_PATH, Tmp);
			NoDrives = LoadHideDriveListRegistory();
			Pos = Tmp;
			while(*Pos != NUL)
			{
				Tmp2 = toupper(*Pos) - 'A';
				if((NoDrives & (0x00000001 << Tmp2)) == 0)
					SetLocalDirHist(Pos);
				Pos = strchr(Pos, NUL) + 1;
			}
			SendMessage(hWndDirLocal, CB_SETCURSEL, 0, 0);
		}
	}

	/*===== �z�X�g�̃c�[���o�[ =====*/

	hWndTbarRemote = CreateToolbarEx(
				hWnd,
				WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS | CCS_NORESIZE,
				3,
				2,
				hInst,
				remote_toolbar_bmp,
				TbarDataRemote,
				sizeof(TbarDataRemote)/sizeof(TBBUTTON),
				16,16,
				16,16,
				sizeof(TBBUTTON));

	if(hWndTbarRemote != NULL)
	{
		/* 2007/09/21 sunasunamix  �������� *********************/
		pOldTbarRemoteProc = (WNDPROC)SetWindowLong(hWndTbarRemote, GWL_WNDPROC, (DWORD)CountermeasureTbarRemoteProc);
		/********************************************* �����܂� */

		MoveWindow(hWndTbarRemote, LocalWidth + SepaWidth, TOOLWIN_HEIGHT, RemoteWidth, TOOLWIN_HEIGHT, FALSE);

		/*===== �z�X�g�̃f�B���N�g�����E�C���h�E =====*/

		SendMessage(hWndTbarRemote, TB_GETITEMRECT, 3, (LPARAM)&Rect1);
		hWndDirRemote = CreateWindowEx(WS_EX_CLIENTEDGE,
					"COMBOBOX", "",
					WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWN | CBS_AUTOHSCROLL,
					Rect1.right, Rect1.top, RemoteWidth - Rect1.right, 200,
					hWndTbarRemote, (HMENU)COMBO_REMOTE, hInst, NULL);

		if(hWndDirRemote != NULL)
		{
			/* �G�f�B�b�g�R���g���[����T�� */
			hWndDirRemoteEdit = GetWindow(hWndDirRemote, GW_CHILD);
			if(hWndDirRemoteEdit != NULL)
				HistEditBoxProcPtr = (WNDPROC)SetWindowLong(hWndDirRemoteEdit, GWL_WNDPROC, (LONG)HistEditBoxWndProc);

			SendMessage(hWndDirRemote, WM_SETFONT, (WPARAM)DlgFont, MAKELPARAM(TRUE, 0));
			SendMessage(hWndDirRemote, CB_LIMITTEXT, FMAX_PATH, 0);
			SendMessage(hWndDirRemote, CB_SETCURSEL, 0, 0);
		}
	}

	Sts = SUCCESS;
	if((hWndTbarMain == NULL) ||
	   (hWndTbarLocal == NULL) ||
	   (hWndTbarRemote == NULL) ||
	   (hWndDirLocal == NULL) ||
	   (hWndDirRemote == NULL))
	{
		Sts = FAIL;
	}
	return(Sts);
}





static LRESULT CALLBACK HistEditBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char Tmp[FMAX_PATH+1];

	switch (message)
	{
		case WM_CHAR :
			if(wParam == 0x0D)		/* ���^�[���L�[�������ꂽ */
			{
				if(hWnd == hWndDirLocalEdit)
				{
					SendMessage(hWndDirLocalEdit, WM_GETTEXT, FMAX_PATH+1, (LPARAM)Tmp);
					DoLocalCWD(Tmp);
					GetLocalDirForWnd();
				}
				else
				{
					SendMessage(hWndDirRemoteEdit, WM_GETTEXT, FMAX_PATH+1, (LPARAM)Tmp);
					if(CheckClosedAndReconnect() == SUCCESS)
					{
						if(DoCWD(Tmp, YES, NO, YES) < FTP_RETRY)
							GetRemoteDirForWnd(CACHE_NORMAL, &CancelFlg);
					}
				}
			}
			else if(wParam == 0x09)		/* TAB�L�[�������ꂽ */
			{
				if(hWnd == hWndDirLocalEdit)
				{
					SetFocus(GetLocalHwnd());
				}
				else
				{
					SetFocus(GetRemoteHwnd());
				}
			}
			else
				return(CallWindowProc(HistEditBoxProcPtr, hWnd, message, wParam, lParam));
			break;

		default :
			return(CallWindowProc(HistEditBoxProcPtr, hWnd, message, wParam, lParam));
	}
    return(0L);
}




/*----- �c�[���o�[���폜 ------------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DeleteToolBarWindow(void)
{
//	if(DlgFont != NULL)
//		DeleteObject(DlgFont);

	if(hWndTbarMain != NULL)
		DestroyWindow(hWndTbarMain);
	if(hWndTbarLocal != NULL)
		DestroyWindow(hWndTbarLocal);
	if(hWndTbarRemote != NULL)
		DestroyWindow(hWndTbarRemote);
	if(hWndDirLocal != NULL)
		DestroyWindow(hWndDirLocal);
	if(hWndDirRemote != NULL)
		DestroyWindow(hWndDirRemote);
	return;
}


/*----- ���C���̃c�[���o�[�̃E�C���h�E�n���h����Ԃ� --------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetMainTbarWnd(void)
{
	return(hWndTbarMain);
}


/*----- ���[�J�����̃q�X�g���E�C���h�E�̃E�C���h�E�n���h����Ԃ� --------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetLocalHistHwnd(void)
{
	return(hWndDirLocal);
}


/*----- �z�X�g���̃q�X�g���E�C���h�E�̃E�C���h�E�n���h����Ԃ� ----------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetRemoteHistHwnd(void)
{
	return(hWndDirRemote);
}


/*----- ���[�J�����̃q�X�g���G�f�B�b�g�̃E�C���h�E�n���h����Ԃ� --------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetLocalHistEditHwnd(void)
{
	return(hWndDirLocalEdit);
}


/*----- �z�X�g���̃q�X�g���G�f�B�b�g�̃E�C���h�E�n���h����Ԃ� ----------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetRemoteHistEditHwnd(void)
{
	return(hWndDirRemoteEdit);
}


/*----- ���[�J�����̃c�[���o�[�̃E�C���h�E�n���h����Ԃ� ----------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetLocalTbarWnd(void)
{
	return(hWndTbarLocal);
}


/*----- �z�X�g���̃c�[���o�[�̃E�C���h�E�n���h����Ԃ� ------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetRemoteTbarWnd(void)
{
	return(hWndTbarRemote);
}


/*----- HideUI �̏�Ԃ�Ԃ� ---------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int HideUI �̏��
*----------------------------------------------------------------------------*/

int GetHideUI(void)
{
	return(HideUI);
}


/*----- �c�[���{�^���^���j���[�̃n�C�h���� ------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void MakeButtonsFocus(void)
{
	HWND hWndFocus;
	HWND hWndMain;
	int Count;
	int Win;

	if(HideUI == NO)
	{
		hWndMain = GetMainHwnd();
		hWndFocus = GetFocus();
		Win = WIN_LOCAL;
		if(hWndFocus == GetRemoteHwnd())
			Win = WIN_REMOTE;

		Count = GetSelectedCount(Win);

		if(AskConnecting() == YES)
		{
			EnableMenuItem(GetMenu(hWndMain), MENU_BMARK_ADD, MF_ENABLED);
			EnableMenuItem(GetMenu(hWndMain), MENU_BMARK_ADD_LOCAL, MF_ENABLED);
			EnableMenuItem(GetMenu(hWndMain), MENU_BMARK_ADD_BOTH, MF_ENABLED);
			EnableMenuItem(GetMenu(hWndMain), MENU_BMARK_EDIT, MF_ENABLED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DIRINFO, MF_ENABLED);
			EnableMenuItem(GetMenu(hWndMain), MENU_MIRROR_UPLOAD, MF_ENABLED);
			EnableMenuItem(GetMenu(hWndMain), MENU_MIRROR_DOWNLOAD, MF_ENABLED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_NAME, MF_ENABLED);
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_MIRROR_UPLOAD, MAKELONG(TRUE, 0));
		}
		else
		{
			EnableMenuItem(GetMenu(hWndMain), MENU_BMARK_ADD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_BMARK_ADD_LOCAL, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_BMARK_ADD_BOTH, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_BMARK_EDIT, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DIRINFO, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_MIRROR_UPLOAD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_MIRROR_DOWNLOAD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_NAME, MF_GRAYED);
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_MIRROR_UPLOAD, MAKELONG(FALSE, 0));
		}

		if(hWndFocus == GetLocalHwnd())
		{
			if((AskConnecting() == YES) && (Count > 0))
			{
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_UPLOAD, MAKELONG(TRUE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_UPLOAD, MF_ENABLED);
				EnableMenuItem(GetMenu(hWndMain), MENU_UPLOAD_AS, MF_ENABLED);
			}
			else
			{
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_UPLOAD, MAKELONG(FALSE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_UPLOAD, MF_GRAYED);
				EnableMenuItem(GetMenu(hWndMain), MENU_UPLOAD_AS, MF_GRAYED);
			}
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_DOWNLOAD, MAKELONG(FALSE, 0));
			EnableMenuItem(GetMenu(hWndMain), MENU_SOMECMD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_AS, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_AS_FILE, MF_GRAYED);
		}

		if(hWndFocus == GetRemoteHwnd())
		{
			if(AskConnecting() == YES)
			{
				EnableMenuItem(GetMenu(hWndMain), MENU_SOMECMD, MF_ENABLED);
			}
			else
			{
				EnableMenuItem(GetMenu(hWndMain), MENU_SOMECMD, MF_GRAYED);
			}

			if((AskConnecting() == YES) && (Count > 0))
			{
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_DOWNLOAD, MAKELONG(TRUE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD, MF_ENABLED);
				EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_AS, MF_ENABLED);
				EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_AS_FILE, MF_ENABLED);
			}
			else
			{
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_DOWNLOAD, MAKELONG(FALSE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD, MF_GRAYED);
				EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_AS, MF_GRAYED);
				EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_AS_FILE, MF_GRAYED);
			}
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_UPLOAD, MAKELONG(FALSE, 0));
			EnableMenuItem(GetMenu(hWndMain), MENU_UPLOAD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_UPLOAD_AS, MF_GRAYED);
		}

		if((hWndFocus == GetLocalHwnd()) || (hWndFocus == GetRemoteHwnd()))
		{
			if(Count > 0)
			{
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_DELETE, MAKELONG(TRUE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_DELETE, MF_ENABLED);
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_RENAME, MAKELONG(TRUE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_RENAME, MF_ENABLED);

				EnableMenuItem(GetMenu(hWndMain), MENU_CHMOD, MF_ENABLED);

			}
			else
			{
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_DELETE, MAKELONG(FALSE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_DELETE, MF_GRAYED);
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_RENAME, MAKELONG(FALSE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_RENAME, MF_GRAYED);

				EnableMenuItem(GetMenu(hWndMain), MENU_CHMOD, MF_GRAYED);
			}

			if((hWndFocus == GetLocalHwnd()) || (AskConnecting() == YES))
			{
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_MKDIR, MAKELONG(TRUE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_MKDIR, MF_ENABLED);
				EnableMenuItem(GetMenu(hWndMain), MENU_FILESIZE, MF_ENABLED);
			}
			else
			{
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_MKDIR, MAKELONG(FALSE, 0));
				EnableMenuItem(GetMenu(hWndMain), MENU_MKDIR, MF_GRAYED);
				EnableMenuItem(GetMenu(hWndMain), MENU_FILESIZE, MF_GRAYED);
			}
		}
		else
		{
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_UPLOAD, MAKELONG(FALSE, 0));
			EnableMenuItem(GetMenu(hWndMain), MENU_UPLOAD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_UPLOAD_AS, MF_GRAYED);
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_DOWNLOAD, MAKELONG(FALSE, 0));
			EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_AS, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_DOWNLOAD_AS_FILE, MF_GRAYED);
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_DELETE, MAKELONG(FALSE, 0));
			EnableMenuItem(GetMenu(hWndMain), MENU_DELETE, MF_GRAYED);
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_RENAME, MAKELONG(FALSE, 0));
			EnableMenuItem(GetMenu(hWndMain), MENU_RENAME, MF_GRAYED);
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_MKDIR, MAKELONG(FALSE, 0));
			EnableMenuItem(GetMenu(hWndMain), MENU_MKDIR, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_CHMOD, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_FILESIZE, MF_GRAYED);
			EnableMenuItem(GetMenu(hWndMain), MENU_SOMECMD, MF_GRAYED);
		}
	}
	return;
}


/*----- ���[�U�̑�����֎~���� ------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DisableUserOpe(void)
{
	int i;

	HideUI = YES;

	for(i = 0; i < sizeof(HideMenus) / sizeof(int); i++)
	{
		EnableMenuItem(GetMenu(GetMainHwnd()), HideMenus[i], MF_GRAYED);
		SendMessage(hWndTbarMain, TB_ENABLEBUTTON, HideMenus[i], MAKELONG(FALSE, 0));
		SendMessage(hWndTbarLocal, TB_ENABLEBUTTON, HideMenus[i], MAKELONG(FALSE, 0));
		SendMessage(hWndTbarRemote, TB_ENABLEBUTTON, HideMenus[i], MAKELONG(FALSE, 0));
	}

	EnableWindow(hWndDirLocal, FALSE);
	EnableWindow(hWndDirRemote, FALSE);

	return;
}


/*----- ���[�U�̑���������� ------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void EnableUserOpe(void)
{
	int i;

	if(HideUI == YES)
	{
		for(i = 0; i < sizeof(HideMenus) / sizeof(int); i++)
		{
			EnableMenuItem(GetMenu(GetMainHwnd()), HideMenus[i], MF_ENABLED);
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, HideMenus[i], MAKELONG(TRUE, 0));
			SendMessage(hWndTbarLocal, TB_ENABLEBUTTON, HideMenus[i], MAKELONG(TRUE, 0));
			SendMessage(hWndTbarRemote, TB_ENABLEBUTTON, HideMenus[i], MAKELONG(TRUE, 0));
		}
		EnableWindow(hWndDirLocal, TRUE);
		EnableWindow(hWndDirRemote, TRUE);

		HideUI = NO;

		MakeButtonsFocus();
	}
	return;
}


/*----- ���[�U�̑��삪�֎~����Ă��邩�ǂ�����Ԃ� ----------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X
*			YES=�֎~����Ă���/NO
*----------------------------------------------------------------------------*/

int AskUserOpeDisabled(void)
{
	return(HideUI);
}


/*===================================================
*			�]�����[�h
*===================================================*/

/*----- �]�����[�h��ݒ肷�� --------------------------------------------------
*
*	Parameter
*		int Mode : �]�����[�h (TYPE_xx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetTransferTypeImm(int Mode)
{
	TmpTransMode = Mode;
	HideHostKanjiButton();
	return;
}


/*----- ���j���[�ɂ��]�����[�h��ݒ肷�� ------------------------------------
*
*	Parameter
*		int Type : �]�����[�h (MENU_xxxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetTransferType(int Type)
{
	switch(Type)
	{
		case MENU_TEXT :
			TmpTransMode = TYPE_A;
			break;

		case MENU_BINARY :
			TmpTransMode = TYPE_I;
			break;

		default :
			TmpTransMode = TYPE_X;
			break;
	}
	HideHostKanjiButton();
	return;
}


/*----- �]�����[�h�ɂ��������ă{�^����\������ --------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispTransferType(void)
{
	switch(TmpTransMode)
	{
		case TYPE_A :
			SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_TEXT, MAKELONG(TRUE, 0));
			break;

		case TYPE_I :
			SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_BINARY, MAKELONG(TRUE, 0));
			break;

		default :
			SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_AUTO, MAKELONG(TRUE, 0));
			break;
	}
	return;
}


/*----- �ݒ��̓]�����[�h��Ԃ� ----------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �]�����[�h (TYPE_xx)
*----------------------------------------------------------------------------*/

int AskTransferType(void)
{
	return(TmpTransMode);
}


/*----- ���ۂ̓]�����[�h��Ԃ� ------------------------------------------------
*
*	Parameter
*		char Fname : �t�@�C����
*		int Type : �ݒ��̓]�����[�h (TYPE_xx)
*
*	Return Value
*		int �]�����[�h (TYPE_xx)
*----------------------------------------------------------------------------*/

int AskTransferTypeAssoc(char *Fname, int Type)
{
	int Ret;
	char *Pos;
	char *Name;

	Ret = Type;
	if(Ret == TYPE_X)
	{
		Ret = TYPE_I;
		if(StrMultiLen(AsciiExt) > 0)
		{
			Name = GetFileName(Fname);
			Pos = AsciiExt;
			while(*Pos != NUL)
			{
				if(CheckFname(Name, Pos) == SUCCESS)
				{
					Ret = TYPE_A;
					break;
				}
				Pos += strlen(Pos) + 1;
			}
		}
	}
	return(Ret);
}


/*----- �]�����[�h��ۑ����� --------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*
*	Note
*		���݂̓]�����[�h�����W�X�g���ɕۑ������
*----------------------------------------------------------------------------*/

void SaveTransferType(void)
{
	TransMode = TmpTransMode;
	return;
}


/*===================================================
*			�������[�h
*===================================================*/

/*----- �z�X�g�̊������[�h���Z�b�g���� ----------------------------------------
*
*	Parameter
*		int Mode : �������[�h (KANJI_xxxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetHostKanjiCodeImm(int Mode)
{
	TmpHostKanjiCode = Mode;
	DispHostKanjiCode();
	HideHostKanjiButton();
	return;
}


/*----- ���j���[�ɂ��z�X�g�̊������[�h��ݒ肷�� -----------------------------
*
*	Parameter
*		int Type : �������[�h (MENU_xxxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetHostKanjiCode(int Type)
{
	switch(Type)
	{
		case MENU_KNJ_EUC :
			TmpHostKanjiCode = KANJI_EUC;
			break;

		case MENU_KNJ_JIS :
			TmpHostKanjiCode = KANJI_JIS;
			break;

		default :
			TmpHostKanjiCode = KANJI_NOCNV;
			break;
	}
	DispHostKanjiCode();
	HideHostKanjiButton();
	return;
}


/*----- �z�X�g�̊������[�h�ɂ��������ă{�^����\������ ------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispHostKanjiCode(void)
{
	switch(TmpHostKanjiCode)
	{
		case KANJI_EUC :
			SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_KNJ_EUC, MAKELONG(TRUE, 0));
			break;

		case KANJI_JIS :
			SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_KNJ_JIS, MAKELONG(TRUE, 0));
			break;

		default :
			SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_KNJ_NONE, MAKELONG(TRUE, 0));
			break;
	}
	return;
}


/*----- �z�X�g�̊������[�h��Ԃ� ----------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �������[�h (KANJI_xxxx)
*----------------------------------------------------------------------------*/

int AskHostKanjiCode(void)
{
	return(TmpHostKanjiCode);
}


/*----- �������[�h�{�^���̃n�C�h�������s�� ------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void HideHostKanjiButton(void)
{
	switch(TmpTransMode)
	{
		case TYPE_I : 
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KNJ_EUC, MAKELONG(FALSE, 0));
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KNJ_JIS, MAKELONG(FALSE, 0));
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KNJ_NONE, MAKELONG(FALSE, 0));
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KANACNV, MAKELONG(FALSE, 0));
			break;

		default :
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KNJ_EUC, MAKELONG(TRUE, 0));
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KNJ_JIS, MAKELONG(TRUE, 0));
			SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KNJ_NONE, MAKELONG(TRUE, 0));
			if(TmpHostKanjiCode != KANJI_NOCNV)
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KANACNV, MAKELONG(TRUE, 0));
			else
				SendMessage(hWndTbarMain, TB_ENABLEBUTTON, MENU_KANACNV, MAKELONG(FALSE, 0));
			break;
	}
	return;
}


/*===================================================
*			���p�ϊ����[�h
*===================================================*/

/*----- �z�X�g�̔��p�ϊ����[�h��ݒ肷�� --------------------------------------
*
*	Parameter
*		int Mode : ���p�ϊ����[�h(YES/NO)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetHostKanaCnvImm(int Mode)
{
	TmpHostKanaCnv = Mode;
	DispHostKanaCnv();
	return;
}


/*----- �z�X�g�̔��p�ϊ����[�h�𔽓]���� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetHostKanaCnv(void)
{
	TmpHostKanaCnv ^= 1;
	DispHostKanaCnv();
	return;
}


/*----- �z�X�g�̔��p�ϊ����[�h�ɂ��������ă{�^����\������ --------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispHostKanaCnv(void)
{
	if(TmpHostKanaCnv != 0)
		SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_KANACNV, MAKELONG(TRUE, 0));
	else
		SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_KANACNV, MAKELONG(FALSE, 0));
	return;
}


/*----- �z�X�g�̔��p�ϊ����[�h��Ԃ� ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int ���p�ϊ����[�h
*----------------------------------------------------------------------------*/

int AskHostKanaCnv(void)
{
	return(TmpHostKanaCnv);
}


/*===================================================
*			�\�[�g���@
*===================================================*/

/*----- �\�[�g���@���Z�b�g���� ------------------------------------------------
*
*	Parameter
*		int LFsort : ���[�J�����̃t�@�C���̃\�[�g���@ (SORT_xxx)
*		int LDsort : ���[�J�����̃f�B���N�g���̃\�[�g���@ (SORT_xxx)
*		int RFsort : �z�X�g���̃t�@�C���̃\�[�g���@ (SORT_xxx)
*		int RDsort : �z�X�g���̃f�B���N�g���̃\�[�g���@ (SORT_xxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetSortTypeImm(int LFsort, int LDsort, int RFsort, int RDsort)
{
	TmpLocalFileSort = LFsort;
	TmpLocalDirSort = LDsort;
	TmpRemoteFileSort = RFsort;
	TmpRemoteDirSort = RDsort;
	return;
}


/*----- ���X�g�r���[�̃^�u�N���b�N�ɂ��\�[�g���@�̃Z�b�g --------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ�
*		int Tab : �J�����ԍ�
*
*	Return Value
*		int �\�[�g���@ (SORT_xxx)
*----------------------------------------------------------------------------*/

void SetSortTypeByColumn(int Win, int Tab)
{
	if(Win == WIN_LOCAL)
	{
		if((TmpLocalFileSort & SORT_MASK_ORD) == Tab)
			TmpLocalFileSort ^= SORT_GET_ORD;
		else
			TmpLocalFileSort = Tab;

		if((Tab == SORT_NAME) || (Tab == SORT_DATE))
			TmpLocalDirSort = TmpLocalFileSort;
		else
			TmpLocalDirSort = SORT_NAME;
	}
	else
	{
		if(Tab != 4)
		{
			if((TmpRemoteFileSort & SORT_MASK_ORD) == Tab)
				TmpRemoteFileSort ^= SORT_GET_ORD;
			else
				TmpRemoteFileSort = Tab;

			if((Tab == SORT_NAME) || (Tab == SORT_DATE))
				TmpRemoteDirSort = TmpRemoteFileSort;
			else
				TmpRemoteDirSort = SORT_NAME;
		}
	}
	return;
}


/*----- �\�[�g���@��Ԃ� ------------------------------------------------------
*
*	Parameter
*		int Name : �ǂ̕����� (ITEM_xxx)
*
*	Return Value
*		int �\�[�g���@ (SORT_xxx)
*----------------------------------------------------------------------------*/

int AskSortType(int Name)
{
	int Ret;

	switch(Name)
	{
		case ITEM_LFILE :
			Ret = TmpLocalFileSort;
			break;

		case ITEM_LDIR :
			Ret = TmpLocalDirSort;
			break;

		case ITEM_RFILE :
			Ret = TmpRemoteFileSort;
			break;

		case ITEM_RDIR :
			Ret = TmpRemoteDirSort;
			break;
	}
	return(Ret);
}


/*----- �z�X�g���ƂɃ\�[�g��ۑ����邩�ǂ������Z�b�g����-----------------------
*
*	Parameter
*		int Sw : �X�C�b�` (YES/NO)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetSaveSortToHost(int Sw)
{
	SortSave = Sw;
	return;
}


/*----- �z�X�g���ƂɃ\�[�g��ۑ����邩�ǂ�����Ԃ� ----------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�C�b�` (YES/NO)
*----------------------------------------------------------------------------*/

int AskSaveSortToHost(void)
{
	return(SortSave);
}



/*===================================================
*			���X�g���[�h
*===================================================*/

/*----- ���X�g���[�h�ɂ��������ă{�^����\������ ------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispListType(void)
{
	HWND hWndMain;

	hWndMain = GetMainHwnd();
	switch(ListType)
	{
		case LVS_LIST :
			SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_LIST, MAKELONG(TRUE, 0));
			CheckMenuItem(GetMenu(hWndMain), MENU_LIST, MF_CHECKED);
			CheckMenuItem(GetMenu(hWndMain), MENU_REPORT, MF_UNCHECKED);
			break;

		default :
			SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_REPORT, MAKELONG(TRUE, 0));
			CheckMenuItem(GetMenu(hWndMain), MENU_REPORT, MF_CHECKED);
			CheckMenuItem(GetMenu(hWndMain), MENU_LIST, MF_UNCHECKED);
			break;
	}
	return;
}


/*===================================================
*			�t�H���_�����ړ����[�h
*===================================================*/

/*----- �]�����[�h��ݒ肷�� --------------------------------------------------
*
*	Parameter
*		int Mode : �]�����[�h (TYPE_xx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetSyncMoveMode(int Mode)
{
	SyncMove = Mode;
	DispSyncMoveMode();
	return;
}


/*----- �t�H���_�����ړ����[�h��؂�ւ��� ------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ToggleSyncMoveMode(void)
{
	SyncMove ^= 1;
	DispSyncMoveMode();
	return;
}


/*----- �t�H���_�����ړ����s�����ǂ������ɂ���ă��j���[�^�{�^����\�� --------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispSyncMoveMode(void)
{
	if(SyncMove != 0)
	{
		SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_SYNC, MAKELONG(TRUE, 0));
		CheckMenuItem(GetMenu(GetMainHwnd()), MENU_SYNC, MF_CHECKED);
	}
	else
	{
		SendMessage(hWndTbarMain, TB_CHECKBUTTON, MENU_SYNC, MAKELONG(FALSE, 0));
		CheckMenuItem(GetMenu(GetMainHwnd()), MENU_SYNC, MF_UNCHECKED);
	}
	return;
}


/*----- �t�H���_�����ړ����[�h��Ԃ� ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int ���p�ϊ����[�h
*----------------------------------------------------------------------------*/

int AskSyncMoveMode(void)
{
	return(SyncMove);
}


/*===================================================
*			�f�B���N�g���q�X�g��
*===================================================*/

/*----- �z�X�g���̃q�X�g���ꗗ�E�C���h�E�ɓo�^ --------------------------------
*
*	Parameter
*		char *Path : �p�X
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetRemoteDirHist(char *Path)
{
	int i;

	if((i = SendMessage(hWndDirRemote, CB_FINDSTRINGEXACT, 0, (LPARAM)Path)) != CB_ERR)
		SendMessage(hWndDirRemote, CB_DELETESTRING, i, 0);

	SendMessage(hWndDirRemote, CB_ADDSTRING, 0, (LPARAM)Path);
	i = SendMessage(hWndDirRemote, CB_GETCOUNT, 0, 0);
	SendMessage(hWndDirRemote, CB_SETCURSEL, i-1, 0);

	strcpy(RemoteCurDir, Path);
	return;
}


/*----- ���[�J�����̃q�X�g���ꗗ�E�C���h�E�ɓo�^ -------------------------------
*
*	Parameter
*		char *Path : �p�X
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetLocalDirHist(char *Path)
{
	int i;

	if((i = SendMessage(hWndDirLocal, CB_FINDSTRINGEXACT, 0, (LPARAM)Path)) == CB_ERR)
		SendMessage(hWndDirLocal, CB_ADDSTRING, 0, (LPARAM)Path);
	i = SendMessage(hWndDirLocal, CB_FINDSTRINGEXACT, 0, (LPARAM)Path);
	SendMessage(hWndDirLocal, CB_SETCURSEL, i, 0);

	strcpy(LocalCurDir, Path);
	return;
}


/*----- ���[�J���̃J�����g�f�B���N�g����Ԃ� ----------------------------------
*
*	Parameter
*		char *Buf : �J�����g�f�B���N�g������Ԃ��o�b�t�@
*		int Max : �o�b�t�@�̃T�C�Y
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void AskLocalCurDir(char *Buf, int Max)
{
	memset(Buf, 0, Max);
	strncpy(Buf, LocalCurDir, Max-1);
	return;
}


/*----- �z�X�g�̃J�����g�f�B���N�g����Ԃ� ------------------------------------
*
*	Parameter
*		char *Buf : �J�����g�f�B���N�g������Ԃ��o�b�t�@
*		int Max : �o�b�t�@�̃T�C�Y
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void AskRemoteCurDir(char *Buf, int Max)
{
	memset(Buf, 0, Max);
	strncpy(Buf, RemoteCurDir, Max-1);
	return;
}


/*----- �J�����g�f�B���N�g����ݒ肷�� ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetCurrentDirAsDirHist(void)
{
	SetCurrentDirectory(LocalCurDir);
	return;
}


/*===================================================
*			���j���[
*===================================================*/

/*----- �h�b�g�t�@�C����\�����邩�ǂ��������j���[�ɕ\������ ------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispDotFileMode(void)
{
	CheckMenuItem(GetMenu(GetMainHwnd()), MENU_DOTFILE, MF_UNCHECKED);
	if(DotFile == YES)
		CheckMenuItem(GetMenu(GetMainHwnd()), MENU_DOTFILE, MF_CHECKED);
	return;
}


/*----- ���[�J�����̉E�{�^�����j���[��\�� ------------------------------------------------
*
*	Parameter
*		int Pos : ���j���[�̈ʒu
*					0=�}�E�X�J�[�\���̈ʒu
*					1=�E�C���h�E�̍���
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void LocalRbuttonMenu(int Pos)
{
	HMENU hMenu;
	POINT point;
	RECT Rect;
	UINT Flg1;
	UINT Flg2;
	UINT Flg3;
	int Count;

	if(HideUI == NO)
	{
		Flg1 = 0;
		if(AskConnecting() == NO)
			Flg1 = MF_GRAYED;

		Count = GetSelectedCount(WIN_LOCAL);
		Flg2 = 0;
		if(Count == 0)
			Flg2 = MF_GRAYED;

		Flg3 = 0;
		if(Count != 1)
			Flg3 = MF_GRAYED;

		hMenu = CreatePopupMenu();
		AddOpenMenu(hMenu, Flg3);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_UPLOAD, MSGJPN255);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_UPLOAD_AS, MSGJPN256);
		AppendMenu(hMenu, MF_STRING | Flg1, MENU_UPLOAD_ALL, MSGJPN257);
		AppendMenu(hMenu, MF_STRING | Flg2, MENU_DELETE, MSGJPN258);
		AppendMenu(hMenu, MF_STRING | Flg2, MENU_RENAME, MSGJPN259);
		AppendMenu(hMenu, MF_STRING , MENU_MKDIR, MSGJPN260);
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING , MENU_FILESIZE, MSGJPN261);
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING, REFRESH_LOCAL, MSGJPN262);

		if(Pos == 0)
			GetCursorPos(&point);
		else
		{
			GetWindowRect(GetLocalHwnd(), &Rect);
			point.x = Rect.left + 20;
			point.y = Rect.top + 20;
		}
		TrackPopupMenu(hMenu, TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, 0, GetMainHwnd(), NULL);

		DestroyMenu(hMenu);
	}
	return;
}


/*----- �z�X�g���̉E�{�^�����j���[��\�� --------------------------------------
*
*	Parameter
*		int Pos : ���j���[�̈ʒu
*					0=�}�E�X�J�[�\���̈ʒu
*					1=�E�C���h�E�̍���
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void RemoteRbuttonMenu(int Pos)
{
	HMENU hMenu;
	POINT point;
	RECT Rect;
	UINT Flg1;
	UINT Flg2;
	UINT Flg3;
	int Count;

	if(HideUI == NO)
	{
		Flg1 = 0;
		if(AskConnecting() == NO)
			Flg1 = MF_GRAYED;

		Count = GetSelectedCount(WIN_REMOTE);
		Flg2 = 0;
		if(Count == 0)
			Flg2 = MF_GRAYED;

		Flg3 = 0;
		if(Count != 1)
			Flg3 = MF_GRAYED;

		hMenu = CreatePopupMenu();
		AddOpenMenu(hMenu, Flg1 | Flg3);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_DOWNLOAD, MSGJPN263);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_DOWNLOAD_AS, MSGJPN264);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_DOWNLOAD_AS_FILE, MSGJPN265);
		AppendMenu(hMenu, MF_STRING | Flg1, MENU_DOWNLOAD_ALL, MSGJPN266);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_DELETE, MSGJPN267);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_RENAME, MSGJPN268);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_CHMOD, MSGJPN269);
		AppendMenu(hMenu, MF_STRING | Flg1, MENU_MKDIR, MSGJPN270);
		AppendMenu(hMenu, MF_STRING | Flg1 | Flg2, MENU_URL_COPY, MSGJPN271);
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING | Flg1, MENU_FILESIZE, MSGJPN272);
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING | Flg1, REFRESH_REMOTE, MSGJPN273);

		if(Pos == 0)
			GetCursorPos(&point);
		else
		{
			GetWindowRect(GetRemoteHwnd(), &Rect);
			point.x = Rect.left + 20;
			point.y = Rect.top + 20;
		}
		if(TrackPopupMenu(hMenu, TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, 0, GetMainHwnd(), NULL) == 0)
			Count = GetLastError();

		DestroyMenu(hMenu);
	}
	return;
}


/*----- �E�{�^�����j���[�Ɂu�J���v��ǉ�  -------------------------------------
*
*	Parameter
*		HMENU hMenu : ���j���[�n���h��
*		UINT Flg : �t���O
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void AddOpenMenu(HMENU hMenu, UINT Flg)
{
	static const UINT MenuID[VIEWERS] = { MENU_OPEN1, MENU_OPEN2, MENU_OPEN3 };
	char Tmp[FMAX_PATH+1];
	int i;

	AppendMenu(hMenu, MF_STRING | Flg, MENU_DCLICK, MSGJPN274);
	for(i = 0; i < VIEWERS; i++)
	{
		if(strlen(ViewerName[i]) != 0)
		{
			sprintf(Tmp, MSGJPN275, GetToolName(ViewerName[i]), i+1);
			AppendMenu(hMenu, MF_STRING | Flg, MenuID[i], Tmp);
		}
	}
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	return;
}

/* 2007/09/21 sunasunamix  �������� *********************/

/*----- CreateToolbarEx �̃}�E�X�N���b�N�֘A�𖳎�����(TbarMain�p) -----------
*       (�T�u�N���X�����s�����߂̃E�C���h�E�v���V�[�W��)
*----------------------------------------------------------------------------*/
static LRESULT CALLBACK CountermeasureTbarMainProc(HWND hWnd,UINT uMessage,WPARAM wParam,LPARAM lParam)
{
	switch (uMessage) {
	case WM_DESTROY :
		SetWindowLong(hWnd,GWL_WNDPROC,(DWORD)pOldTbarMainProc);
		break;
	case WM_RBUTTONDBLCLK :
	case WM_RBUTTONDOWN :
	case WM_RBUTTONUP :
		return TRUE;
	}
	return CallWindowProc(pOldTbarMainProc, hWnd, uMessage, wParam, lParam);
}

/*----- CreateToolbarEx �̃}�E�X�N���b�N�֘A�𖳎�����(TbarLocal�p) ----------
*       (�T�u�N���X�����s�����߂̃E�C���h�E�v���V�[�W��)
*----------------------------------------------------------------------------*/
static LRESULT CALLBACK CountermeasureTbarLocalProc(HWND hWnd,UINT uMessage,WPARAM wParam,LPARAM lParam)
{
	switch (uMessage) {
	case WM_DESTROY :
		SetWindowLong(hWnd,GWL_WNDPROC,(DWORD)pOldTbarLocalProc);
		break;
	case WM_RBUTTONDBLCLK :
	case WM_RBUTTONDOWN :
	case WM_RBUTTONUP :
		return TRUE;
	}
	return CallWindowProc(pOldTbarLocalProc, hWnd, uMessage, wParam, lParam);
}

/*----- CreateToolbarEx �̃}�E�X�N���b�N�֘A�𖳎�����(TbarRemote�p) ---------
*       (�T�u�N���X�����s�����߂̃E�C���h�E�v���V�[�W��)
*----------------------------------------------------------------------------*/
static LRESULT CALLBACK CountermeasureTbarRemoteProc(HWND hWnd,UINT uMessage,WPARAM wParam,LPARAM lParam)
{
	switch (uMessage) {
	case WM_DESTROY :
		SetWindowLong(hWnd,GWL_WNDPROC,(DWORD)pOldTbarRemoteProc);
		break;
	case WM_RBUTTONDBLCLK :
	case WM_RBUTTONDOWN :
	case WM_RBUTTONUP :
		return TRUE;
	}
	return CallWindowProc(pOldTbarRemoteProc, hWnd, uMessage, wParam, lParam);
}
/********************************************* �����܂� */
