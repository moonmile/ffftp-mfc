/*=============================================================================
*
*								�t�@�C���ꗗ
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

#define _WIN32_WINNT	0x400

#define	STRICT
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <mbstring.h>
#include <malloc.h>
#include <windowsx.h>
#include <commctrl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

#include "common.h"
#include "resource.h"

#include <htmlhelp.h>
#include "helpid.h"

#include <shlobj.h>
#include "OleDragDrop.h"
#include "common.h"

#define BUF_SIZE		256
#define CF_CNT 2
#define WM_DRAGDROP		(WM_APP + 100)
#define WM_GETDATA		(WM_APP + 101)
#define WM_DRAGOVER		(WM_APP + 102)


/*===== �t�@�C�����X�g�̃��X�g�p�X�g���N�`�� =====*/

typedef struct {
	FILELIST *Top;			/* �t�@�C�����X�g�̐擪 */
	int Files;				/* �t�@�C���̐� */
} FLISTANCHOR;

/*===== �v���g�^�C�v =====*/

static LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK RemoteWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT FileListCommonWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void AddDispFileList(FLISTANCHOR *Anchor, char *Name, FILETIME *Time, LONGLONG Size, int Attr, int Type, int Link, char *Owner, int InfoExist, int Win);
static void EraseDispFileList(FLISTANCHOR *Anchor);
static void DispFileList2View(HWND hWnd, FLISTANCHOR *Anchor);
static void AddListView(HWND hWnd, int Pos, char *Name, int Type, LONGLONG Size, FILETIME *Time, int Attr, char *Owner, int Link, int InfoExist);
static BOOL CALLBACK SelectDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static void DispListList(FILELIST *Pos, char *Title);
static void MakeRemoteTree1(char *Path, char *Cur, FILELIST **Base, int *CancelCheckWork);
static void MakeRemoteTree2(char *Path, char *Cur, FILELIST **Base, int *CancelCheckWork);
static void CopyTmpListToFileList(FILELIST **Base, FILELIST *List);
static int GetListOneLine(char *Buf, int Max, FILE *Fd);
static int MakeDirPath(char *Str, int ListType, char *Path, char *Dir);
static void MakeLocalTree(char *Path, FILELIST **Base);
static void AddFileList(FILELIST *Pkt, FILELIST **Base);
static int AnalizeFileInfo(char *Str);
static int CheckUnixType(char *Str, char *Tmp, int Add1, int Add2, int Day);
static int CheckHHMMformat(char *Str);
static int CheckYYMMDDformat(char *Str, char Sym, int Dig3);
static int CheckYYYYMMDDformat(char *Str, char Sym);
static int ResolvFileInfo(char *Str, int ListType, char *Fname, LONGLONG *Size, FILETIME *Time, int *Attr, char *Owner, int *Link, int *InfoExist);
static int FindField(char *Str, char *Buf, int Num, int ToLast);
static void GetMonth(char *Str, WORD *Month, WORD *Day);
static int GetYearMonthDay(char *Str, WORD *Year, WORD *Month, WORD *Day);
static int GetHourAndMinute(char *Str, WORD *Hour, WORD *Minute);
static int GetVMSdate(char *Str, WORD *Year, WORD *Month, WORD *Day);
static int CheckSpecialDirName(char *Fname);
static int AskFilterStr(char *Fname, int Type);
static BOOL CALLBACK FilterWndProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static int atoi_n(const char *Str, int Len);

/*===== �O���Q�� =====*/

extern int SepaWidth;
extern int RemoteWidth;
extern int ListHeight;
extern char FilterStr[FILTER_EXT_LEN+1];
extern HWND hHelpWin;

/* �ݒ�l */
extern int LocalWidth;
extern int LocalTabWidth[4];
extern int RemoteTabWidth[6];
extern char UserMailAdrs[USER_MAIL_LEN+1];
extern HFONT ListFont;
extern int ListType;
extern int FindMode;
extern int DotFile;
extern int DispIgnoreHide;
extern int DispDrives;
extern int MoveMode;

/*===== ���[�J���ȃ��[�N =====*/

static HWND hWndListLocal = NULL;
static HWND hWndListRemote = NULL;

static WNDPROC LocalProcPtr;
static WNDPROC RemoteProcPtr;

static HIMAGELIST ListImg = NULL;

static char FindStr[40+1] = { "*" };		/* ���������� */
static int IgnoreNew = NO;
static int IgnoreOld = NO;
static int IgnoreExist = NO;

static int Dragging = NO;

static int StratusMode;			/* 0=�t�@�C��, 1=�f�B���N�g��, 2=�����N */


// �����[�g�t�@�C�����X�g (2007.9.3 yutaka)
static FILELIST *remoteFileListBase;
static FILELIST *remoteFileListBaseNoExpand;
static char remoteFileDir[FMAX_PATH + 1];


/*----- �t�@�C�����X�g�E�C���h�E���쐬���� ------------------------------------
*
*	Parameter
*		HWND hWnd : �e�E�C���h�E�̃E�C���h�E�n���h��
*		HINSTANCE hInst : �C���X�^���X�n���h��
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int MakeListWin(HWND hWnd, HINSTANCE hInst)
{
	int Sts;
	LV_COLUMN LvCol;
	long Tmp;

	/*===== ���[�J�����̃��X�g�r���[ =====*/

	hWndListLocal = CreateWindowEx(/*WS_EX_STATICEDGE*/WS_EX_CLIENTEDGE,
			WC_LISTVIEWA, NULL,
			WS_CHILD | /*WS_BORDER | */LVS_REPORT | LVS_SHOWSELALWAYS,
			0, TOOLWIN_HEIGHT*2, LocalWidth, ListHeight,
			GetMainHwnd(), (HMENU)1500, hInst, NULL);

	if(hWndListLocal != NULL)
	{
		LocalProcPtr = (WNDPROC)SetWindowLong(hWndListLocal, GWL_WNDPROC, (LONG)LocalWndProc);

	    Tmp = SendMessage(hWndListLocal, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	    Tmp |= LVS_EX_FULLROWSELECT;
	    SendMessage(hWndListLocal, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)Tmp);

		if(ListFont != NULL)
			SendMessage(hWndListLocal, WM_SETFONT, (WPARAM)ListFont, MAKELPARAM(TRUE, 0));

		ListImg = ImageList_LoadBitmap(hInst, MAKEINTRESOURCE(dirattr_bmp), 16, 9, RGB(255,0,0));
		SendMessage(hWndListLocal, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)ListImg);
		ShowWindow(hWndListLocal, SW_SHOW);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		LvCol.cx = LocalTabWidth[0];
		LvCol.pszText = MSGJPN038;
		LvCol.iSubItem = 0;
		SendMessage(hWndListLocal, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		LvCol.cx = LocalTabWidth[1];
		LvCol.pszText = MSGJPN039;
		LvCol.iSubItem = 1;
		SendMessage(hWndListLocal, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
		LvCol.fmt = LVCFMT_RIGHT;
		LvCol.cx = LocalTabWidth[2];
		LvCol.pszText = MSGJPN040;
		LvCol.iSubItem = 2;
		SendMessage(hWndListLocal, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		LvCol.cx = LocalTabWidth[3];
		LvCol.pszText = MSGJPN041;
		LvCol.iSubItem = 3;
		SendMessage(hWndListLocal, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);
	}

	/*===== �z�X�g���̃��X�g�r���[ =====*/

	hWndListRemote = CreateWindowEx(/*WS_EX_STATICEDGE*/WS_EX_CLIENTEDGE,
			WC_LISTVIEWA, NULL,
			WS_CHILD | /*WS_BORDER | */LVS_REPORT | LVS_SHOWSELALWAYS,
			LocalWidth + SepaWidth, TOOLWIN_HEIGHT*2, RemoteWidth, ListHeight,
			GetMainHwnd(), (HMENU)1500, hInst, NULL);

	if(hWndListRemote != NULL)
	{
		RemoteProcPtr = (WNDPROC)SetWindowLong(hWndListRemote, GWL_WNDPROC, (LONG)RemoteWndProc);

	    Tmp = SendMessage(hWndListRemote, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	    Tmp |= LVS_EX_FULLROWSELECT;
	    SendMessage(hWndListRemote, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)Tmp);

		if(ListFont != NULL)
			SendMessage(hWndListRemote, WM_SETFONT, (WPARAM)ListFont, MAKELPARAM(TRUE, 0));

		SendMessage(hWndListRemote, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)ListImg);
		ShowWindow(hWndListRemote, SW_SHOW);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		LvCol.cx = RemoteTabWidth[0];
		LvCol.pszText = MSGJPN042;
		LvCol.iSubItem = 0;
		SendMessage(hWndListRemote, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		LvCol.cx = RemoteTabWidth[1];
		LvCol.pszText = MSGJPN043;
		LvCol.iSubItem = 1;
		SendMessage(hWndListRemote, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
		LvCol.fmt = LVCFMT_RIGHT;
		LvCol.cx = RemoteTabWidth[2];
		LvCol.pszText = MSGJPN044;
		LvCol.iSubItem = 2;
		SendMessage(hWndListRemote, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		LvCol.cx = RemoteTabWidth[3];
		LvCol.pszText = MSGJPN045;
		LvCol.iSubItem = 3;
		SendMessage(hWndListRemote, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		LvCol.cx = RemoteTabWidth[4];
		LvCol.pszText = MSGJPN046;
		LvCol.iSubItem = 4;
		SendMessage(hWndListRemote, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol);

		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		LvCol.cx = RemoteTabWidth[5];
		LvCol.pszText = MSGJPN047;
		LvCol.iSubItem = 5;
		SendMessage(hWndListRemote, LVM_INSERTCOLUMN, 5, (LPARAM)&LvCol);
	}

	Sts = SUCCESS;
	if((hWndListLocal == NULL) ||
	   (hWndListRemote == NULL))
	{
		Sts = FAIL;
	}
	return(Sts);
}


/*----- �t�@�C�����X�g�E�C���h�E���폜 ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DeleteListWin(void)
{
//	if(ListImg != NULL)
//		ImageList_Destroy(ListImg);
	if(hWndListLocal != NULL)
		DestroyWindow(hWndListLocal);
	if(hWndListRemote != NULL)
		DestroyWindow(hWndListRemote);
	return;
}


/*----- ���[�J�����̃t�@�C�����X�g�̃E�C���h�E�n���h����Ԃ� ------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetLocalHwnd(void)
{
	return(hWndListLocal);
}


/*----- �z�X�g���̃t�@�C�����X�g�̃E�C���h�E�n���h����Ԃ� --------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetRemoteHwnd(void)
{
	return(hWndListRemote);
}


/*----- ���[�J�����̃t�@�C�����X�g�E�C���h�E�̃��b�Z�[�W���� ------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		UINT message  : ���b�Z�[�W�ԍ�
*		WPARAM wParam : ���b�Z�[�W�� WPARAM ����
*		LPARAM lParam : ���b�Z�[�W�� LPARAM ����
*
*	Return Value
*		���b�Z�[�W�ɑΉ�����߂�l
*----------------------------------------------------------------------------*/

static LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return(FileListCommonWndProc(hWnd, message, wParam, lParam));
}


/*----- �z�X�g���̃t�@�C�����X�g�E�C���h�E�̃��b�Z�[�W���� --------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		UINT message  : ���b�Z�[�W�ԍ�
*		WPARAM wParam : ���b�Z�[�W�� WPARAM ����
*		LPARAM lParam : ���b�Z�[�W�� LPARAM ����
*
*	Return Value
*		���b�Z�[�W�ɑΉ�����߂�l
*----------------------------------------------------------------------------*/

static LRESULT CALLBACK RemoteWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return(FileListCommonWndProc(hWnd, message, wParam, lParam));
}


// �_�C�A���O�v���V�[�W��
static BOOL CALLBACK doOleDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
#define TIMER_ID     (100)      // �쐬����^�C�}�̎���ID
#define TIMER_ELAPSE (100)       // WM_TIMER�̔����Ԋu
	MSG message;

	switch( msg ){
	case WM_INITDIALOG:  // �_�C�A���O�{�b�N�X���쐬���ꂽ�Ƃ�
		SetTimer( hDlg, TIMER_ID, 0, NULL);
		return TRUE;

	case WM_TIMER:
		ShowWindow(hDlg, SW_HIDE);  // �_�C�A���O�͉B��

		if (wp != TIMER_ID)
			break;

		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&message);
				DispatchMessage(&message);

		} else {
			if (AskTransferNow() == NO) {
				EndDialog( hDlg, 0 );
				return TRUE;
			}
		}

		SetTimer( hDlg, TIMER_ID, TIMER_ELAPSE, NULL );
		return TRUE;

	case WM_COMMAND:     // �_�C�A���O�{�b�N�X���̉������I�����ꂽ�Ƃ�
		switch( LOWORD( wp ) ){
//		case IDOK:       // �uOK�v�{�^�����I�����ꂽ
		case IDCANCEL:   // �u�L�����Z���v�{�^�����I�����ꂽ
			// �_�C�A���O�{�b�N�X������
			EndDialog( hDlg, 0 );
			break;
		}
		return TRUE;
	}

	return FALSE;  // DefWindowProc()�ł͂Ȃ��AFALSE��Ԃ����ƁI
#undef TIMER_ID     
#undef TIMER_ELAPSE 
}


static void doTransferRemoteFile(void)
{
	FILELIST *FileListBase, *FileListBaseNoExpand, *pf;
	int CancelFlg = NO;
	char LocDir[FMAX_PATH+1];
	char TmpDir[FMAX_PATH+1];
	char buf[32];
	int i;
	DWORD pid;

	// ���łɃ����[�g����]���ς݂Ȃ牽�����Ȃ��B(2007.9.3 yutaka)
	if (remoteFileListBase != NULL)
		return;

	FileListBase = NULL;
	MakeSelectedFileList(WIN_REMOTE, YES, NO, &FileListBase, &CancelFlg);
	FileListBaseNoExpand = NULL;
	MakeSelectedFileList(WIN_REMOTE, NO, NO, &FileListBaseNoExpand, &CancelFlg);

	// set temporary folder
	AskLocalCurDir(LocDir, FMAX_PATH);

	// �A�v���𑽏d�N�����Ă��R���t���N�g���Ȃ��悤�ɁA�e���|�����t�H���_���Ƀv���Z�XID
	// ��t������B(2007.9.13 yutaka)
	GetTempPath(sizeof(TmpDir), TmpDir);
	pid = GetCurrentProcessId();
	_snprintf_s(buf, sizeof(buf), _TRUNCATE, "ffftp%d", pid);
	strncat_s(TmpDir, sizeof(TmpDir), buf, _TRUNCATE);
	_mkdir(TmpDir);
#if 0
	if (TmpDir[strlen(TmpDir) - 1] == '\\') {
		TmpDir[strlen(TmpDir) - 1] = '\0';
	}
#endif

	// �����̃t�@�C�����폜����
	for (pf = FileListBase ; pf ; pf = pf->Next) {
		char fn[FMAX_PATH+1];

		strncpy_s(fn, sizeof(fn), TmpDir, _TRUNCATE);
		strncat_s(fn, sizeof(fn), "\\", _TRUNCATE);
		strncat_s(fn, sizeof(fn), pf->File, _TRUNCATE);

		remove(fn);
	}

	// �_�E�����[�h����e���|�����ɐݒ�
	SetLocalDirHist(TmpDir);

	// FFFTP�Ƀ_�E�����[�h�v�����o���A�_�E�����[�h�̊�����҂B
	PostMessage(GetMainHwnd(), WM_COMMAND, MAKEWPARAM(MENU_DOWNLOAD, 0), 0);

	for (i = 0 ; i < 10 ; i++) {
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		} else {
			// �]���X���b�h�������o�����甲����B
			if (AskTransferNow() == YES)
				break;
		}

		Sleep(10);
	}

	// OLE D&D���Ƀ��C���E�B���h�E�����[�U�ɑ��삳����ƁA���������Ȃ�̂ŁA
	// �B�����[�_���_�C�A���O�����B
	// (2007.9.11 yutaka)
	DialogBox(GetFtpInst(), MAKEINTRESOURCE(IDD_OLEDRAG), GetMainHwnd(), (DLGPROC)doOleDlgProc);

	// �_�E�����[�h������ɖ߂�
	SetLocalDirHist(LocDir);
	SetCurrentDirAsDirHist();

	remoteFileListBase = FileListBase;  // ���ƂŃt���[���邱��
	remoteFileListBaseNoExpand = FileListBaseNoExpand;  // ���ƂŃt���[���邱��
	strncpy_s(remoteFileDir, sizeof(remoteFileDir), TmpDir, _TRUNCATE);

#if 0
	// add temporary list
	if (remoteFileListBase != NULL) {
		FILELIST *pf = remoteFileListBase;
		char fn[FMAX_PATH + 1];
		while (pf) {
			strncpy_s(fn, sizeof(fn), remoteFileDir, _TRUNCATE);
			strncat_s(fn, sizeof(fn), "\\", _TRUNCATE);
			strncat_s(fn, sizeof(fn), pf->File, _TRUNCATE);
			AddTempFileList(fn);
			pf = pf->Next;
		}
	}
#endif
}


int isDirectory(char *fn)
{
	struct _stat buf;

	if (_stat(fn, &buf) == 0) {
		if (buf.st_mode & _S_IFDIR) { // is directory
			return 1;
		}
	}
	return 0;
}

// �e���|�����̃t�@�C������уt�H���_���폜����B
void doDeleteRemoteFile(void)
{
	if (remoteFileListBase != NULL) {
#if 0
		int dirs = 0;
		int i, count;
		FILELIST *pf = remoteFileListBase;
		char fn[FMAX_PATH + 1];
		while (pf) {
			strncpy_s(fn, sizeof(fn), remoteFileDir, _TRUNCATE);
			strncat_s(fn, sizeof(fn), "\\", _TRUNCATE);
			strncat_s(fn, sizeof(fn), pf->File, _TRUNCATE);
			if (isDirectory(fn)) {
				dirs++;
			} else {
				remove(fn);
			}
			pf = pf->Next;
		}

		count = 0;
		for (i = 0 ; i < 1000 ; i++) {
			pf = remoteFileListBase;
			while (pf) {
				strncpy_s(fn, sizeof(fn), remoteFileDir, _TRUNCATE);
				strncat_s(fn, sizeof(fn), "\\", _TRUNCATE);
				strncat_s(fn, sizeof(fn), pf->File, _TRUNCATE);
				if (isDirectory(fn)) {
					if (_rmdir(fn) == 0) { // �f�B���N�g������������J�E���g�A�b�v
						count++;
						if (count >= dirs)  // ���ׂď�������I���
							goto skip;
					}
				}
				pf = pf->Next;
			}
		}
skip:
		_rmdir(remoteFileDir);  // �����ō�����f�B���N�g��������
#else
		SHFILEOPSTRUCT FileOp = { NULL, FO_DELETE, remoteFileDir, NULL, 
			FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI, 
			FALSE, NULL, NULL };	
		SHFileOperation(&FileOp);
#endif

		DeleteFileList(&remoteFileListBase);
		remoteFileListBase = NULL;
	}

	if (remoteFileListBaseNoExpand != NULL)	{
		DeleteFileList(&remoteFileListBaseNoExpand);
		remoteFileListBaseNoExpand = NULL;
	}
}


// yutaka
// cf. http://www.nakka.com/lib/
/* �h���b�v�t�@�C���̍쐬 */
static HDROP APIPRIVATE CreateDropFileMem(char **FileName,int cnt,BOOL fWide)
{
	HDROP hDrop;
	LPDROPFILES lpDropFile;
	wchar_t wbuf[BUF_SIZE];
	int flen = 0;
	int i;
	
	if(fWide == TRUE){
		/* ���C�h�L���� */
		for(i = 0;i < cnt;i++){
			MultiByteToWideChar(CP_ACP,0,FileName[i],-1,wbuf,BUF_SIZE);
			flen += (wcslen(wbuf) + 1) * sizeof(wchar_t);
		}
		flen++;
	}else{
		/* �}���`�o�C�g */
		for(i = 0;i < cnt;i++){
			flen += lstrlen(FileName[i]) + 1;
		}
	}

	hDrop = (HDROP)GlobalAlloc(GHND,sizeof(DROPFILES) + flen + 1);
	if (hDrop == NULL){
		return NULL;
	}

	lpDropFile = (LPDROPFILES) GlobalLock(hDrop);
	lpDropFile->pFiles = sizeof(DROPFILES);		/* �t�@�C�����̃��X�g�܂ł̃I�t�Z�b�g */
	lpDropFile->pt.x = 0;
	lpDropFile->pt.y = 0;
	lpDropFile->fNC = FALSE;
	lpDropFile->fWide = fWide;					/* ���C�h�L�����̏ꍇ�� TRUE */

	/* �\���̂̌��Ƀt�@�C�����̃��X�g���R�s�[����B(�t�@�C����\0�t�@�C����\0�t�@�C����\0\0) */
	if(fWide == TRUE){
		/* ���C�h�L���� */
		wchar_t *buf;

		buf = (wchar_t *)(&lpDropFile[1]);
		for(i = 0;i < cnt;i++){
			MultiByteToWideChar(CP_ACP,0,FileName[i],-1,wbuf,BUF_SIZE);
			wcscpy(buf,wbuf);
			buf += wcslen(wbuf) + 1;
		}
	}else{
		/* �}���`�o�C�g */
		char *buf;

		buf = (char *)(&lpDropFile[1]);
		for(i = 0;i < cnt;i++){
			lstrcpy(buf,FileName[i]);
			buf += lstrlen(FileName[i]) + 1;
		}
	}

	GlobalUnlock(hDrop);
	return(hDrop);
}


// OLE D&D���J�n���� 
// (2007.8.30 yutaka)
static void doDragDrop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT cf[CF_CNT];
	POINT pt;
	int ret;

	// �e���|���������ꂢ�ɂ��� (2007.9.3 yutaka)
	doDeleteRemoteFile();

	/* �h���b�O&�h���b�v�̊J�n */
	cf[0] = CF_HDROP;
	cf[1] = CF_HDROP;	/* �t�@�C�� */
	if((ret = OLE_IDropSource_Start(hWnd,WM_GETDATA, WM_DRAGOVER, cf,1,DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK)) == DROPEFFECT_MOVE){
	}

	// �h���b�v��̃A�v���� WM_LBUTTONUP ���΂��B
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	PostMessage(hWnd,WM_LBUTTONUP,0,MAKELPARAM(pt.x,pt.y));
}



/*----- �t�@�C���ꗗ�E�C���h�E�̋��ʃ��b�Z�[�W���� ----------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		UINT message  : ���b�Z�[�W�ԍ�
*		WPARAM wParam : ���b�Z�[�W�� WPARAM ����
*		LPARAM lParam : ���b�Z�[�W�� LPARAM ����
*
*	Return Value
*		���b�Z�[�W�ɑΉ�����߂�l
*----------------------------------------------------------------------------*/

static LRESULT FileListCommonWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT Point;
	HWND hWndPnt;
	HWND hWndParent;
	static HCURSOR hCsrDrg;
	static HCURSOR hCsrNoDrg;
	static POINT DragPoint;
	static HWND hWndDragStart;
	static int RemoteDropFileIndex = -1;
	int Win;
	HWND hWndDst;
	WNDPROC ProcPtr;
	HWND hWndHistEdit;

	Win = WIN_LOCAL;
	hWndDst = hWndListRemote;
	ProcPtr = LocalProcPtr;
	hWndHistEdit = GetLocalHistEditHwnd();
	if(hWnd == hWndListRemote)
	{
		Win = WIN_REMOTE;
		hWndDst = hWndListLocal;
		ProcPtr = RemoteProcPtr;
		hWndHistEdit = GetRemoteHistEditHwnd();
	}

	switch (message)
	{
        case WM_SYSKEYDOWN:
			if (wParam == 'D') {	// Alt+D
				SetFocus(hWndHistEdit);
				break;
			}
			EraseListViewTips();
			return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));

        case WM_KEYDOWN:
			if(wParam == 0x09)
			{
				SetFocus(hWndDst);
				break;
			}
			EraseListViewTips();
			return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));

		case WM_SETFOCUS :
			SetFocusHwnd(hWnd);
			MakeButtonsFocus();
			DispCurrentWindow(Win);
			DispSelectedSpace();
			return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));

		case WM_KILLFOCUS :
			EraseListViewTips();
			MakeButtonsFocus();
			DispCurrentWindow(-1);
			return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));

		case WM_DROPFILES :
			// �h���b�O���͏������Ȃ��B�h���b�O���WM_LBUTTONDOWN�����ł��邽�߁A�����ŏ�������B
			if (Dragging == YES) 
				return (FALSE);

			if(hWnd == hWndListRemote)
			{
				if(AskConnecting() == YES)
					UpLoadDragProc(wParam);
			}
			else if(hWnd == hWndListLocal)
			{
				ChangeDirDropFileProc(wParam);
			}
			break;

		case WM_LBUTTONDOWN :
			EraseListViewTips();
			SetFocus(hWnd);
			DragPoint.x = LOWORD(lParam);
			DragPoint.y = HIWORD(lParam);
			hWndDragStart = hWnd;
			return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));
			break;

		case WM_LBUTTONUP :
			if(Dragging == YES)
			{
				Dragging = NO;
				ReleaseCapture();
				hCsrDrg = LoadCursor(NULL, IDC_ARROW);
				SetCursor(hCsrDrg);

				Point.x = (long)(short)LOWORD(lParam);
				Point.y = (long)(short)HIWORD(lParam);
				ClientToScreen(hWnd, &Point);
				hWndPnt = WindowFromPoint(Point);
				if(hWndPnt == hWndDst)  // local <-> remote 
				{
					if(hWndPnt == hWndListRemote) {
						PostMessage(GetMainHwnd(), WM_COMMAND, MAKEWPARAM(MENU_UPLOAD, 0), 0);
					} else if(hWndPnt == hWndListLocal) {
						PostMessage(GetMainHwnd(), WM_COMMAND, MAKEWPARAM(MENU_DOWNLOAD, 0), 0);
					}
				} else { // ����E�B���h�E���̏ꍇ (yutaka)
					if (hWndDragStart == hWndListRemote && hWndPnt == hWndListRemote) {
						// remote <-> remote�̏ꍇ�́A�T�[�o�ł̃t�@�C���̈ړ����s���B(2007.9.5 yutaka)
						if (RemoteDropFileIndex != -1) {
							ListView_SetItemState(hWnd, RemoteDropFileIndex, 0, LVIS_DROPHILITED);
							MoveRemoteFileProc(RemoteDropFileIndex);
						}

					}

				}
			}
			return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));

 		case WM_DRAGDROP:  
 			// OLE D&D���J�n���� (yutaka)
 			doDragDrop(hWnd, message, wParam, lParam);
 			return (TRUE);
 			break;
 
 		case WM_GETDATA:  // �t�@�C���̃p�X��D&D��̃A�v���֕Ԃ� (yutaka)
 			switch(wParam)
 			{
 			case CF_HDROP:		/* �t�@�C�� */
 				{
 					OSVERSIONINFO os_info;
 					BOOL NTFlag = FALSE;
 					char **FileNameList;
 					int filelen;
 					int i, j, filenum = 0;
 
 					FILELIST *FileListBase, *FileListBaseNoExpand, *pf;
 					int CancelFlg = NO;
 					char LocDir[FMAX_PATH+1];
 					char *PathDir;
 
 					// ���[�J�����őI�΂�Ă���t�@�C����FileListBase�ɓo�^
 					if (hWndDragStart == hWndListLocal) {
 						AskLocalCurDir(LocDir, FMAX_PATH);
 						PathDir = LocDir;
 
 						FileListBase = NULL;
 						MakeSelectedFileList(WIN_LOCAL, YES, NO, &FileListBase, &CancelFlg);			
 
 					} else if (hWndDragStart == hWndListRemote) {
 						GetCursorPos(&Point);
 						hWndPnt = WindowFromPoint(Point);
						hWndParent = GetParent(hWndPnt);
 						if (hWndPnt == hWndListRemote || hWndPnt == hWndListLocal ||
							hWndParent == hWndListRemote || hWndParent == hWndListLocal) {
 							FileListBase = NULL;
 
 						} else {
 							// �I������Ă��郊���[�g�t�@�C���̃��X�g�A�b�v
 							// ���̃^�C�~���O�Ń����[�g���烍�[�J���̈ꎞ�t�H���_�փ_�E�����[�h����
 							// (2007.8.31 yutaka)
 							doTransferRemoteFile();
 							PathDir = remoteFileDir;
 							FileListBase = remoteFileListBase;
 							FileListBaseNoExpand = remoteFileListBaseNoExpand;
 						}
 
 					} 
 
 					pf = FileListBaseNoExpand;
 					for (filenum = 0; pf ; filenum++) {
 						pf = pf->Next;
 					}
 					// �t�@�C�������I���̏ꍇ�͉������Ȃ��B(yutaka)
 					if (filenum <= 0) {
 						*((HANDLE *)lParam) = NULL;
 						return (FALSE);
 					}
 					
 					/* �t�@�C�����̔z����쐬���� */
 					FileNameList = (char **)GlobalAlloc(GPTR,sizeof(char *) * filenum);
 					if(FileNameList == NULL){
 						abort();
 					}
 					pf = FileListBaseNoExpand;
 					for (j = 0; pf ; j++) {
 						filelen = strlen(PathDir) + 1 + strlen(pf->File) + 1;
 						FileNameList[j] = (char *)GlobalAlloc(GPTR, filelen);
 						strncpy_s(FileNameList[j], filelen, PathDir, _TRUNCATE);
 						strncat_s(FileNameList[j], filelen, "\\", _TRUNCATE);
 						strncat_s(FileNameList[j], filelen, pf->File, _TRUNCATE);
 						pf = pf->Next;
#if 0
						if (FileListBase->Node == NODE_DIR) { 
 							// �t�H���_��͂񂾏ꍇ�͂���ȍ~�W�J���Ȃ�
 							filenum = 1;
 							break;
 						}
#endif
 					}
 					
 					os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
 					GetVersionEx(&os_info);
 					if(os_info.dwPlatformId == VER_PLATFORM_WIN32_NT){
 						NTFlag = TRUE;
 					}
 
 					/* �h���b�v�t�@�C�����X�g�̍쐬 */
 					/* NT�̏ꍇ��UNICODE�ɂȂ�悤�ɂ��� */
 					*((HANDLE *)lParam) = CreateDropFileMem(FileNameList, filenum, NTFlag);
 
 					/* �t�@�C�����̔z���������� */
 					for (i = 0; i < filenum ; i++) {
 						GlobalFree(FileNameList[i]);
 					}
 					GlobalFree(FileNameList);
 
 					if (hWndDragStart == hWndListLocal) {
 						DeleteFileList(&FileListBase);
 					} else {
 						// ���ƂŃt�@�C���폜���ăt���[����
 					}
 
 					return (TRUE);
 				}
 				break;
 
 			default:
 				*((HANDLE *)lParam) = NULL;
 				break;
 			}
 
			break;

		case WM_DRAGOVER:
			{
				LVHITTESTINFO hi;
				int Node, index;
				static int prev_index = -1;

				// ����E�B���h�E���ł�D&D�̓����[�g���̂�
				if (Win != WIN_REMOTE)
					break;

				if(MoveMode == MOVE_DISABLE)
					break;

				memset(&hi, 0, sizeof(hi));

				GetCursorPos(&Point);
				hWndPnt = WindowFromPoint(Point);
				ScreenToClient(hWnd, &Point);

				hi.pt = Point;

				// �ȑO�̑I��������
				ListView_SetItemState(hWnd, prev_index, 0, LVIS_DROPHILITED);
				RemoteDropFileIndex = -1;

				if ((hWndPnt == hWndListRemote) && (ListView_HitTest(hWnd, &hi) != -1)) {
					if (hi.flags == LVHT_ONITEMLABEL) { // The position is over a list-view item's text.
					
						index = hi.iItem;
						prev_index = index;
						Node = GetNodeType(Win, index);
						if (Node == NODE_DIR) {
							ListView_SetItemState(hWnd, index, LVIS_DROPHILITED, LVIS_DROPHILITED);
							RemoteDropFileIndex = index;
						}
					}
				} 

			}
			break;

		case WM_RBUTTONDOWN :
			/* �����Ńt�@�C����I�� */
			CallWindowProc(ProcPtr, hWnd, message, wParam, lParam);

			EraseListViewTips();
			SetFocus(hWnd);
			if(hWnd == hWndListRemote)
				RemoteRbuttonMenu(0);
			else if(hWnd == hWndListLocal)
				LocalRbuttonMenu(0);
			break;

		case WM_LBUTTONDBLCLK :
			DoubleClickProc(Win, NO, -1);
			break;

		case WM_MOUSEMOVE :
			if(wParam == MK_LBUTTON)
			{
				if((Dragging == NO) && 
				   (hWnd == hWndDragStart) &&
				   (AskConnecting() == YES) &&
				   (SendMessage(hWnd, LVM_GETSELECTEDCOUNT, 0, 0) > 0) &&
				   ((abs((short)LOWORD(lParam) - DragPoint.x) > 5) ||
					(abs((short)HIWORD(lParam) - DragPoint.y) > 5)))
				{
					SetCapture(hWnd);
					Dragging = YES;
					hCsrDrg = LoadCursor(GetFtpInst(), MAKEINTRESOURCE(drag_csr));
					hCsrNoDrg = LoadCursor(GetFtpInst(), MAKEINTRESOURCE(nodrop_csr));
					SetCursor(hCsrDrg);
				}
				else if(Dragging == YES)
				{
					Point.x = (long)(short)LOWORD(lParam);
					Point.y = (long)(short)HIWORD(lParam);
					ClientToScreen(hWnd, &Point);
					hWndPnt = WindowFromPoint(Point);
					if((hWndPnt == hWndListRemote) || (hWndPnt == hWndListLocal))
						SetCursor(hCsrDrg);
					else {
						// �}�E�X�|�C���^�́~�\������߂� (yutaka)
#if 0
						SetCursor(hCsrNoDrg);
#endif
					}

					// OLE D&D�̊J�n���w������
					PostMessage(hWnd, WM_DRAGDROP, MAKEWPARAM(wParam, lParam), 0);

				}
				else
					return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));
			}
			else
			{
				CheckTipsDisplay(hWnd, lParam);
				return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));
			}
			break;

		case WM_MOUSEWHEEL :
			if(Dragging == NO)
			{
                short zDelta = (short)HIWORD(wParam);

				EraseListViewTips();
				Point.x = (short)LOWORD(lParam);
				Point.y = (short)HIWORD(lParam);
				hWndPnt = WindowFromPoint(Point);

				if((wParam & MAKEWPARAM(MK_SHIFT, 0)) && 
				   ((hWndPnt == hWndListRemote) ||
					(hWndPnt == hWndListLocal) || 
					(hWndPnt == GetTaskWnd())))
				{
					PostMessage(hWndPnt, WM_VSCROLL, zDelta > 0 ? MAKEWPARAM(SB_PAGEUP, 0) : MAKEWPARAM(SB_PAGEDOWN, 0), 0);
//					PostMessage(hWndPnt, WM_VSCROLL, MAKEWPARAM(SB_ENDSCROLL, 0), 0);
				}
				else if(hWndPnt == hWnd)
					return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));
				else if((hWndPnt == hWndDst) || (hWndPnt == GetTaskWnd()))
					PostMessage(hWndPnt, message, wParam, lParam);
			}
			break;

		default :
			return(CallWindowProc(ProcPtr, hWnd, message, wParam, lParam));
	}
    return(0L);
}


/*----- �t�@�C�����X�g�̃^�u�����擾���� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void GetListTabWidth(void)
{
	LV_COLUMN LvCol;
	int i;

	for(i = 0; i <= 3; i++)
	{
		LvCol.mask = LVCF_WIDTH;
		if(SendMessage(hWndListLocal, LVM_GETCOLUMN, i, (LPARAM)&LvCol) == TRUE)
			LocalTabWidth[i] = LvCol.cx;
	}

	for(i = 0; i <= 5; i++)
	{
		LvCol.mask = LVCF_WIDTH;
		if(SendMessage(hWndListRemote, LVM_GETCOLUMN, i, (LPARAM)&LvCol) == TRUE)
			RemoteTabWidth[i] = LvCol.cx;
	}
	return;
}


/*----- �t�@�C���ꗗ���@�ɂ��������ă��X�g�r���[��ݒ肷�� --------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetListViewType(void)
{
	long lStyle;

	switch(ListType)
	{
		case LVS_LIST :
			lStyle = GetWindowLong(GetLocalHwnd(), GWL_STYLE);
			lStyle &= ~(LVS_REPORT | LVS_LIST);
			lStyle |= LVS_LIST;
			SetWindowLong(GetLocalHwnd(), GWL_STYLE, lStyle);

			lStyle = GetWindowLong(GetRemoteHwnd(), GWL_STYLE);
			lStyle &= ~(LVS_REPORT | LVS_LIST);
			lStyle |= LVS_LIST;
			SetWindowLong(GetRemoteHwnd(), GWL_STYLE, lStyle);
			break;

		default :
			lStyle = GetWindowLong(GetLocalHwnd(), GWL_STYLE);
			lStyle &= ~(LVS_REPORT | LVS_LIST);
			lStyle |= LVS_REPORT;
			SetWindowLong(GetLocalHwnd(), GWL_STYLE, lStyle);

			lStyle = GetWindowLong(GetRemoteHwnd(), GWL_STYLE);
			lStyle &= ~(LVS_REPORT | LVS_LIST);
			lStyle |= LVS_REPORT;
			SetWindowLong(GetRemoteHwnd(), GWL_STYLE, lStyle);
			break;
	}
	return;
}


/*----- �z�X�g���̃t�@�C���ꗗ�E�C���h�E�Ƀt�@�C�������Z�b�g ------------------
*
*	Parameter
*		int Mode : �L���b�V�����[�h (CACHE_xxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void GetRemoteDirForWnd(int Mode, int *CancelCheckWork)
{
	FILE *fd;
	LONGLONG Size;
	char Str[FMAX_PATH+1];
	char Buf[FMAX_PATH+1];
	FILETIME Time;
	int Attr;
	int Type;
	int ListType;
	int Num;
	FLISTANCHOR Anchor;
	char Owner[OWNER_NAME_LEN+1];
	int Link;
	int InfoExist;

//#pragma aaa
//DoPrintf("===== GetRemoteDirForWnd");

	Anchor.Top = NULL;
	Anchor.Files = 0;

	if(AskConnecting() == YES)
	{
//		SetCursor(LoadCursor(NULL, IDC_WAIT));
		DisableUserOpe();

		AskRemoteCurDir(Buf, FMAX_PATH);
		SetRemoteDirHist(Buf);

		Type = FTP_COMPLETE;
		if(Mode != CACHE_LASTREAD)
		{

			if((Num = AskCached(Buf)) == -1)
			{
				Num = AskFreeCache();
				Mode = CACHE_REFRESH;
			}

			if(Mode == CACHE_REFRESH)
			{
				if((Type = DoDirListCmdSkt("", "", Num, CancelCheckWork)) == FTP_COMPLETE)
					SetCache(Num, Buf);
				else
					ClearCache(Num);
			}
		}
		else
			Num = AskCurrentFileListNum();

		if(Type == FTP_COMPLETE)
		{
			SetCurrentFileListNum(Num);
			MakeCacheFileName(Num, Buf);
			if((fd = fopen(Buf, "rb"))!=NULL)
			{
				ListType = LIST_UNKNOWN;

				while(GetListOneLine(Str, FMAX_PATH, fd) == SUCCESS)
				{
					if((ListType = AnalizeFileInfo(Str)) != LIST_UNKNOWN)
					{
						if((Type = ResolvFileInfo(Str, ListType, Buf, &Size, &Time, &Attr, Owner, &Link, &InfoExist)) != NODE_NONE)
						{
							if(AskFilterStr(Buf, Type) == YES)
							{
								if((DotFile == YES) || (Buf[0] != '.'))
								{
									AddDispFileList(&Anchor, Buf, &Time, Size, Attr, Type, Link, Owner, InfoExist, WIN_REMOTE);
								}
							}
						}
					}
				}
				fclose(fd);

				DispFileList2View(GetRemoteHwnd(), &Anchor);
				EraseDispFileList(&Anchor);

				// �擪�̃A�C�e����I��
				ListView_SetItemState(GetRemoteHwnd(), 0, LVIS_FOCUSED, LVIS_FOCUSED);
			}
			else
			{
				SetTaskMsg(MSGJPN048);
				SendMessage(GetRemoteHwnd(), LVM_DELETEALLITEMS, 0, 0);
			}
		}
		else
		{
#if defined(HAVE_OPENVMS)
			/* OpenVMS�̏ꍇ��f�B���N�g���ړ��̎��ɏo��̂ŁA���b�Z�[�W�����o���Ȃ�
			 * �悤�ɂ���(VIEW�̓N���A���ėǂ�) */
			if (AskHostType() != HTYPE_VMS)
#endif
			SetTaskMsg(MSGJPN049);
			SendMessage(GetRemoteHwnd(), LVM_DELETEALLITEMS, 0, 0);
		}

//		SetCursor(LoadCursor(NULL, IDC_ARROW));
		EnableUserOpe();

	}

//#pragma aaa
//DoPrintf("===== GetRemoteDirForWnd Done");

	return;
}


/*----- ���[�J�����̃t�@�C���ꗗ�E�C���h�E�Ƀt�@�C�������Z�b�g ----------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void GetLocalDirForWnd(void)
{
	HANDLE fHnd;
	WIN32_FIND_DATA Find;
	char Scan[FMAX_PATH+1];
	char *Pos;
	char Buf[10];
	FILETIME Time;
	FLISTANCHOR Anchor;
	DWORD NoDrives;
	int Tmp;

	Anchor.Top = NULL;
	Anchor.Files = 0;

	DoLocalPWD(Scan);
	SetLocalDirHist(Scan);
	DispLocalFreeSpace(Scan);

	/* �f�B���N�g���^�t�@�C�� */

	SetYenTail(Scan);
	strcat(Scan, "*");
	if((fHnd = FindFirstFileAttr(Scan, &Find, DispIgnoreHide)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((strcmp(Find.cFileName, ".") != 0) &&
			   (strcmp(Find.cFileName, "..") != 0))
			{
				if((DotFile == YES) || (Find.cFileName[0] != '.'))
				{
					if(Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						AddDispFileList(&Anchor, Find.cFileName, &Find.ftLastWriteTime, MakeLongLong(Find.nFileSizeHigh, Find.nFileSizeLow), 0, NODE_DIR, NO, "", FINFO_ALL, WIN_LOCAL);
					else
					{
						if(AskFilterStr(Find.cFileName, NODE_FILE) == YES)
						{
							AddDispFileList(&Anchor, Find.cFileName, &Find.ftLastWriteTime, MakeLongLong(Find.nFileSizeHigh, Find.nFileSizeLow), 0, NODE_FILE, NO, "", FINFO_ALL, WIN_LOCAL);
						}
					}
				}
			}
		}
		while(FindNextFileAttr(fHnd, &Find, DispIgnoreHide) == TRUE);
		FindClose(fHnd);
	}

	/* �h���C�u */
	if(DispDrives)
	{
		GetLogicalDriveStrings(FMAX_PATH, Scan);
		NoDrives = LoadHideDriveListRegistory();

		Pos = Scan;
		while(*Pos != NUL)
		{
			Tmp = toupper(*Pos) - 'A';
			if((NoDrives & (0x00000001 << Tmp)) == 0)
			{
				sprintf(Buf, "%s", Pos);
				memset(&Time, 0, sizeof(FILETIME));
				AddDispFileList(&Anchor, Buf, &Time, 0, 0, NODE_DRIVE, NO, "", FINFO_ALL, WIN_LOCAL);
			}
			Pos = strchr(Pos, NUL) + 1;
		}
	}

	DispFileList2View(GetLocalHwnd(), &Anchor);
	EraseDispFileList(&Anchor);

	// �擪�̃A�C�e����I��
	ListView_SetItemState(GetLocalHwnd(), 0, LVIS_FOCUSED, LVIS_FOCUSED);

	return;
}


/*----- �t�@�C�������t�@�C���ꗗ�p���X�g�ɓo�^���� --------------------------
*
*	Parameter
*		FLISTANCHOR *Anchor : �t�@�C�����X�g�̐擪
*		char *Name : �t�@�C����
*		FILETIME *Time : ���t
*		LONGLONG Size : �T�C�Y
*		int Attr : ����
*		int Type : �^�C�v (NODE_xxxx)
*		int Link : �����N���ǂ��� (YES/NO)
*		char *Owner : �I�[�i��
*		int InfoExist : ��񂪂��邩�ǂ��� (FINFO_xxx)
*		int Win : �E�B���h�E�ԍ� (WIN_xxxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void AddDispFileList(FLISTANCHOR *Anchor, char *Name, FILETIME *Time, LONGLONG Size, int Attr, int Type, int Link, char *Owner, int InfoExist, int Win)
{
	int i;
	FILELIST *Pos;
	FILELIST *Prev;
	FILELIST *New;
	int FileSort;
	int DirSort;
	int Sort;
	LONGLONG Cmp;

	FileSort = AskSortType(ITEM_LFILE);
	DirSort = AskSortType(ITEM_LDIR);
	if(Win == WIN_REMOTE)
	{
		FileSort = AskSortType(ITEM_RFILE);
		DirSort = AskSortType(ITEM_RDIR);
	}

	Pos = Anchor->Top;
	for(i = 0; i < Anchor->Files; i++)
	{
		if((Type == NODE_DIR) && (Pos->Node == NODE_FILE))
			break;
		if((Type == NODE_FILE) && (Pos->Node == NODE_DRIVE))
			break;

		if(Type == Pos->Node)
		{
			if(Type == NODE_DIR)
				Sort = DirSort;
			else
				Sort = FileSort;

			if((Sort & SORT_GET_ORD) == SORT_ASCENT)
			{
				if((((Sort & SORT_MASK_ORD) == SORT_EXT) &&
					((Cmp = _mbsicmp(GetFileExt(Name), GetFileExt(Pos->File))) < 0)) ||
				   (((Sort & SORT_MASK_ORD) == SORT_SIZE) &&
					((Cmp = Size - Pos->Size) < 0)) ||
				   (((Sort & SORT_MASK_ORD) == SORT_DATE) &&
					((Cmp = CompareFileTime(Time, &Pos->Time)) < 0)))
				{
					break;
				}

				if(((Sort & SORT_MASK_ORD) == SORT_NAME) || (Cmp == 0))
				{
					if(_mbsicmp(Name, Pos->File) < 0)
						break;
				}
			}
			else
			{
				if((((Sort & SORT_MASK_ORD) == SORT_EXT) &&
					((Cmp = _mbsicmp(GetFileExt(Name), GetFileExt(Pos->File))) > 0)) ||
				   (((Sort & SORT_MASK_ORD) == SORT_SIZE) &&
					((Cmp = Size - Pos->Size) > 0)) ||
				   (((Sort & SORT_MASK_ORD) == SORT_DATE) &&
					((Cmp = CompareFileTime(Time, &Pos->Time)) > 0)))
				{
					break;
				}

				if(((Sort & SORT_MASK_ORD) == SORT_NAME) || (Cmp == 0))
				{
					if(_mbsicmp(Name, Pos->File) > 0)
						break;
				}
			}
		}
		Prev = Pos;
		Pos = Pos->Next;
	}

	if((New = malloc(sizeof(FILELIST))) != NULL)
	{
		strcpy(New->File, Name);
		New->Node = Type;
		New->Link = Link;
		New->Size = Size;
		New->Attr = Attr;
		New->Time = *Time;
		strcpy(New->Owner, Owner);
		New->InfoExist = InfoExist;

		if(Pos == Anchor->Top)
		{
			New->Next = Anchor->Top;
			Anchor->Top = New;
		}
		else
		{
			New->Next = Prev->Next;
			Prev->Next = New;
		}
		Anchor->Files += 1;
	}
	return;
}


/*----- �t�@�C���ꗗ�p���X�g���N���A���� --------------------------------------
*
*	Parameter
*		FLISTANCHOR *Anchor : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void EraseDispFileList(FLISTANCHOR *Anchor)
{
	FILELIST *Pos;
	FILELIST *Next;
	int i;

	Pos = Anchor->Top;
	for(i = 0; i < Anchor->Files; i++)
	{
		Next = Pos->Next;
		free(Pos);
		Pos = Next;
	}
	Anchor->Files = 0;
	Anchor->Top = NULL;
	return;
}


/*----- �t�@�C���ꗗ�p���X�g�̓��e���t�@�C���ꗗ�E�C���h�E�ɃZ�b�g ------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		FLISTANCHOR *Anchor : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DispFileList2View(HWND hWnd, FLISTANCHOR *Anchor)
{
	int i;
	FILELIST *Pos;

	SendMessage(hWnd, WM_SETREDRAW, (WPARAM)FALSE, 0);
	SendMessage(hWnd, LVM_DELETEALLITEMS, 0, 0);

	Pos = Anchor->Top;
	for(i = 0; i < Anchor->Files; i++)
	{
		AddListView(hWnd, -1, Pos->File, Pos->Node, Pos->Size, &Pos->Time, Pos->Attr, Pos->Owner, Pos->Link, Pos->InfoExist);
		Pos = Pos->Next;
	}

	SendMessage(hWnd, WM_SETREDRAW, (WPARAM)TRUE, 0);
	UpdateWindow(hWnd);

	DispSelectedSpace();
	return;
}


/*----- �t�@�C���ꗗ�E�C���h�E�i���X�g�r���[�j�ɒǉ� --------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		int Pos : �}���ʒu
*		char *Name : ���O
*		int Type : �^�C�v (NIDE_xxxx)
*		LONGLONG Size : �T�C�Y
*		FILETIME *Time : ���t
*		int Attr : ����
*		char Owner : �I�[�i��
*		int Link : �����N���ǂ���
*		int InfoExist : ��񂪂��邩�ǂ��� (FINFO_xxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void AddListView(HWND hWnd, int Pos, char *Name, int Type, LONGLONG Size, FILETIME *Time, int Attr, char *Owner, int Link, int InfoExist)
{
	LV_ITEM LvItem;
	char Tmp[20];

	if(Pos == -1)
		Pos = SendMessage(hWnd, LVM_GETITEMCOUNT, 0, 0);

	/* �A�C�R��/�t�@�C���� */
	LvItem.mask = LVIF_TEXT | LVIF_IMAGE;
	LvItem.iItem = Pos;
	LvItem.iSubItem = 0;
	LvItem.pszText = Name;
	if((Type == NODE_FILE) && (AskTransferTypeAssoc(Name, TYPE_X) == TYPE_I))
		Type = 3;
	if(Link == NO)
		LvItem.iImage = Type;
	else
		LvItem.iImage = 4;
	LvItem.iItem = SendMessage(hWnd, LVM_INSERTITEM, 0, (LPARAM)&LvItem);

	/* ���t/���� */
	FileTime2TimeString(Time, Tmp, DISPFORM_LEGACY, InfoExist);
	LvItem.mask = LVIF_TEXT;
	LvItem.iItem = Pos;
	LvItem.iSubItem = 1;
	LvItem.pszText = Tmp;
	LvItem.iItem = SendMessage(hWnd, LVM_SETITEM, 0, (LPARAM)&LvItem);

	/* �T�C�Y */
	if(Type == NODE_DIR)
		strcpy(Tmp, "<DIR>");
	else if(Type == NODE_DRIVE)
		strcpy(Tmp, "<DRIVE>");
	else if(Size >= 0)
		MakeNumString(Size, Tmp, TRUE);
	else
		strcpy(Tmp, "");
	LvItem.mask = LVIF_TEXT;
	LvItem.iItem = Pos;
	LvItem.iSubItem = 2;
	LvItem.pszText = Tmp;
	LvItem.iItem = SendMessage(hWnd, LVM_SETITEM, 0, (LPARAM)&LvItem);

	/* �g���q */
	LvItem.mask = LVIF_TEXT;
	LvItem.iItem = Pos;
	LvItem.iSubItem = 3;
	LvItem.pszText = GetFileExt(Name);
	LvItem.iItem = SendMessage(hWnd, LVM_SETITEM, 0, (LPARAM)&LvItem);

	if(hWnd == GetRemoteHwnd())
	{
		/* ���� */
		strcpy(Tmp, "");
		if(InfoExist & FINFO_ATTR)
			AttrValue2String(Attr, Tmp);
		LvItem.mask = LVIF_TEXT;
		LvItem.iItem = Pos;
		LvItem.iSubItem = 4;
		LvItem.pszText = Tmp;
		LvItem.iItem = SendMessage(hWnd, LVM_SETITEM, 0, (LPARAM)&LvItem);

		/* �I�[�i�� */
		LvItem.mask = LVIF_TEXT;
		LvItem.iItem = Pos;
		LvItem.iSubItem = 5;
		LvItem.pszText = Owner;
		LvItem.iItem = SendMessage(hWnd, LVM_SETITEM, 0, (LPARAM)&LvItem);
	}
	return;
}


/*----- �t�@�C�����ꗗ�E�C���h�E���\�[�g������ --------------------------------
*
*	Parameter
*		int Win : �E�B���h�E�ԍ� (WIN_xxxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ReSortDispList(int Win, int *CancelCheckWork)
{
	if(Win == WIN_REMOTE)
		GetRemoteDirForWnd(CACHE_LASTREAD, CancelCheckWork);
	else
		GetLocalDirForWnd();
	return;
}


/*----- �t�@�C���ꗗ�E�C���h�E�̃t�@�C����I������ ----------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		int Type : �I����@ (SELECT_xxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SelectFileInList(HWND hWnd, int Type)
{
	int Win;
	int WinDst;
	int i;
	int Num;
	char RegExp[FMAX_PATH+1];
	char Name[FMAX_PATH+1];
	LV_ITEM LvItem;
	int CsrPos;
	FILETIME Time1;
	FILETIME Time2;
	int Find;

	Win = WIN_LOCAL;
	WinDst = WIN_REMOTE;
	if(hWnd == GetRemoteHwnd())
	{
		Win = WIN_REMOTE;
		WinDst = WIN_LOCAL;
	}

	Num = GetItemCount(Win);
	switch(Type)
	{
		case SELECT_ALL :
			LvItem.state = 0;
			if(GetSelectedCount(Win) <= 1)
				LvItem.state = LVIS_SELECTED;
			for(i = 0; i < Num; i++)
			{
				if(GetNodeType(Win, i) != NODE_DRIVE)
				{
					LvItem.mask = LVIF_STATE;
					LvItem.iItem = i;
					LvItem.stateMask = LVIS_SELECTED;
					LvItem.iSubItem = 0;
					SendMessage(hWnd, LVM_SETITEMSTATE, i, (LPARAM)&LvItem);
				}
			}
			break;

		case SELECT_REGEXP :
			if(((Win == WIN_LOCAL) &&
				(DialogBox(GetFtpInst(), MAKEINTRESOURCE(sel_local_dlg), hWnd, SelectDialogCallBack) == YES)) ||
			   ((Win == WIN_REMOTE) &&
				(DialogBox(GetFtpInst(), MAKEINTRESOURCE(sel_remote_dlg), hWnd, SelectDialogCallBack) == YES)))
			{
				strcpy(RegExp, FindStr);
//				if(FindMode == 0)
//					WildCard2RegExp(RegExp);

				_mbslwr(RegExp);
				if((FindMode == 0) || (JreCompileStr(RegExp) == TRUE))
				{
					CsrPos = -1;
					for(i = 0; i < Num; i++)
					{
						GetNodeName(Win, i, Name, FMAX_PATH);
						Find = FindNameNode(WinDst, Name);

						_mbslwr(Name);
						LvItem.state = 0;
						if(GetNodeType(Win, i) != NODE_DRIVE)
						{
							if(((FindMode == 0) && (CheckFname(Name, RegExp) == SUCCESS)) ||
							   ((FindMode != 0) && (JreGetStrMatchInfo(Name, 0) != NULL)))
							{
								LvItem.state = LVIS_SELECTED;

								if(Find >= 0)
								{
									if(IgnoreExist == YES)
										LvItem.state = 0;

									if((LvItem.state != 0) && (IgnoreNew == YES))
									{
										GetNodeTime(Win, i, &Time1);
										GetNodeTime(WinDst, Find, &Time2);
										if(CompareFileTime(&Time1, &Time2) > 0)
											LvItem.state = 0;
									}

									if((LvItem.state != 0) && (IgnoreOld == YES))
									{
										GetNodeTime(Win, i, &Time1);
										GetNodeTime(WinDst, Find, &Time2);
										if(CompareFileTime(&Time1, &Time2) < 0)
											LvItem.state = 0;
									}
								}
							}
						}

						if((LvItem.state != 0) && (CsrPos == -1))
							CsrPos = i;

						LvItem.mask = LVIF_STATE;
						LvItem.iItem = i;
						LvItem.stateMask = LVIS_SELECTED;
						LvItem.iSubItem = 0;
						SendMessage(hWnd, LVM_SETITEMSTATE, i, (LPARAM)&LvItem);
					}
					if(CsrPos != -1)
					{
						LvItem.mask = LVIF_STATE;
						LvItem.iItem = CsrPos;
						LvItem.state = LVIS_FOCUSED;
						LvItem.stateMask = LVIS_FOCUSED;
						LvItem.iSubItem = 0;
						SendMessage(hWnd, LVM_SETITEMSTATE, CsrPos, (LPARAM)&LvItem);
						SendMessage(hWnd, LVM_ENSUREVISIBLE, CsrPos, (LPARAM)TRUE);
					}
				}
			}
			break;
	}
	return;
}


/*----- �I���_�C�A���O�̃R�[���o�b�N ------------------------------------------
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

static BOOL CALLBACK SelectDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, SEL_FNAME, EM_LIMITTEXT, 40, 0);
			SendDlgItemMessage(hDlg, SEL_FNAME, WM_SETTEXT, 0, (LPARAM)FindStr);
			SendDlgItemMessage(hDlg, SEL_REGEXP, BM_SETCHECK, FindMode, 0);
			SendDlgItemMessage(hDlg, SEL_NOOLD, BM_SETCHECK, IgnoreOld, 0);
			SendDlgItemMessage(hDlg, SEL_NONEW, BM_SETCHECK, IgnoreNew, 0);
			SendDlgItemMessage(hDlg, SEL_NOEXIST, BM_SETCHECK, IgnoreExist, 0);
			return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
					SendDlgItemMessage(hDlg, SEL_FNAME, WM_GETTEXT, 40+1, (LPARAM)FindStr);
					FindMode = SendDlgItemMessage(hDlg, SEL_REGEXP, BM_GETCHECK, 0, 0);
					IgnoreOld = SendDlgItemMessage(hDlg, SEL_NOOLD, BM_GETCHECK, 0, 0);
					IgnoreNew = SendDlgItemMessage(hDlg, SEL_NONEW, BM_GETCHECK, 0, 0);
					IgnoreExist = SendDlgItemMessage(hDlg, SEL_NOEXIST, BM_GETCHECK, 0, 0);
					EndDialog(hDlg, YES);
					break;

				case IDCANCEL :
					EndDialog(hDlg, NO);
					break;

				case IDHELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000061);
					break;
			}
            return(TRUE);
	}
	return(FALSE);
}


/*----- �t�@�C���ꗗ�E�C���h�E�̃t�@�C������������ ----------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		int Type : �������@ (FIND_xxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void FindFileInList(HWND hWnd, int Type)
{
	int Win;
	int i;
	int Num;
	static char RegExp[FMAX_PATH+1] = { "" };
	char Name[FMAX_PATH+1];
	LV_ITEM LvItem;
	char *Title;

	Win = WIN_LOCAL;
	Title = MSGJPN050;
	if(hWnd == GetRemoteHwnd())
	{
		Win = WIN_REMOTE;
		Title = MSGJPN051;
	}

	Num = GetItemCount(Win);
	switch(Type)
	{
		case FIND_FIRST :
			if(InputDialogBox(find_dlg, hWnd, Title, FindStr, 40+1, &FindMode, IDH_HELP_TOPIC_0000001) == YES)
			{
				strcpy(RegExp, FindStr);
//				if(FindMode == 0)
//					WildCard2RegExp(RegExp);

				_mbslwr(RegExp);
				if((FindMode == 0) || (JreCompileStr(RegExp) == TRUE))
				{
					for(i = GetCurrentItem(Win)+1; i < Num; i++)
					{
						GetNodeName(Win, i, Name, FMAX_PATH);
						_mbslwr(Name);

						LvItem.state = 0;
						if(((FindMode == 0) && (CheckFname(Name, RegExp) == SUCCESS)) ||
						   ((FindMode != 0) && (JreGetStrMatchInfo(Name, 0) != NULL)))
						{
							LvItem.mask = LVIF_STATE;
							LvItem.iItem = i;
							LvItem.state = LVIS_FOCUSED;
							LvItem.stateMask = LVIS_FOCUSED;
							LvItem.iSubItem = 0;
							SendMessage(hWnd, LVM_SETITEMSTATE, i, (LPARAM)&LvItem);
							SendMessage(hWnd, LVM_ENSUREVISIBLE, i, (LPARAM)TRUE);
							break;
						}
					}
				}
			}
			break;

		case FIND_NEXT :
			for(i = GetCurrentItem(Win)+1; i < Num; i++)
			{
				GetNodeName(Win, i, Name, FMAX_PATH);
				_mbslwr(Name);

				LvItem.state = 0;
				if(((FindMode == 0) && (CheckFname(Name, RegExp) == SUCCESS)) ||
				   ((FindMode != 0) && (JreGetStrMatchInfo(Name, 0) != NULL)))
				{
					LvItem.mask = LVIF_STATE;
					LvItem.iItem = i;
					LvItem.state = LVIS_FOCUSED;
					LvItem.stateMask = LVIS_FOCUSED;
					LvItem.iSubItem = 0;
					SendMessage(hWnd, LVM_SETITEMSTATE, i, (LPARAM)&LvItem);
					SendMessage(hWnd, LVM_ENSUREVISIBLE, i, (LPARAM)TRUE);
					break;
				}
			}
			break;
	}
	return;
}


#if 0
/*----- ���C���h�J�[�h�𐳋K�\���ɕϊ����� ------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void WildCard2RegExp(char *Str)
{
	char Tmp[FMAX_PATH+1];
	char *Org;
	char *Pos;
	UINT Ch;

	Org = Str;
	Pos = Tmp;

	*Pos++ = '^';
	*Pos++ = '(';
	while(*Str != NUL)
	{
		if(Pos >= Tmp + FMAX_PATH - 3)
			break;

		Ch = _mbsnextc(Str);
		Str = _mbsinc(Str);

		if(Ch <= 0x7F)
		{
			if(strchr("[]()^$.+", Ch) != NULL)
			{
				*Pos++ = '\\';
				*Pos++ = Ch;
			}
			else if(Ch == '*')
			{
				*Pos++ = '.';
				*Pos++ = '*';
			}
			else if(Ch == '?')
				*Pos++ = '.';
			else if(Ch == '|')
			{
				*Pos++ = '|';
			}
			else
				*Pos++ = Ch;
		}
		else
		{
			_mbsnset(Pos, Ch, 1);
			Pos = _mbsinc(Pos);
		}
	}
	*Pos++ = ')';
	*Pos++ = '$';
	*Pos = NUL;
	strcpy(Org, Tmp);

	return;
}
#endif


/*----- �J�[�\���ʒu�̃A�C�e���ԍ���Ԃ� --------------------------------------
*
*	Parameter
*		int Win : �E�B���h�E�ԍ� (WIN_xxxx)
*
*	Return Value
*		int �A�C�e���ԍ�
*----------------------------------------------------------------------------*/

int GetCurrentItem(int Win)
{
	HWND hWnd;
	int Ret;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	if((Ret = SendMessage(hWnd, LVM_GETNEXTITEM, -1, MAKELPARAM(LVNI_ALL | LVNI_FOCUSED, 0))) == -1)
		Ret = 0;

	return(Ret);
}


/*----- �A�C�e������Ԃ� ------------------------------------------------------
*
*	Parameter
*		int Win : �E�B���h�E�ԍ� (WIN_xxxx)
*
*	Return Value
*		int �A�C�e����
*----------------------------------------------------------------------------*/

int GetItemCount(int Win)
{
	HWND hWnd;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	return(SendMessage(hWnd, LVM_GETITEMCOUNT, 0, 0));
}


/*----- �I������Ă���A�C�e������Ԃ� ----------------------------------------
*
*	Parameter
*		int Win : �E�B���h�E�ԍ� (WIN_xxxx)
*
*	Return Value
*		int �I������Ă���A�C�e����
*----------------------------------------------------------------------------*/

int GetSelectedCount(int Win)
{
	HWND hWnd;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	return(SendMessage(hWnd, LVM_GETSELECTEDCOUNT, 0, 0));
}


/*----- �I������Ă���ŏ��̃A�C�e���ԍ���Ԃ� --------------------------------
*
*	Parameter
*		int Win : �E�B���h�E�ԍ� (WIN_xxxx)
*		int All : �I�΂�Ă��Ȃ����̂��܂߂�
*
*	Return Value
*		int �A�C�e���ԍ�
*			-1 = �I������Ă��Ȃ�
*----------------------------------------------------------------------------*/

int GetFirstSelected(int Win, int All)
{
	HWND hWnd;
	int Ope;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	Ope = LVNI_SELECTED;
	if(All == YES)
		Ope = LVNI_ALL;

	return(SendMessage(hWnd, LVM_GETNEXTITEM, (WPARAM)-1, (LPARAM)MAKELPARAM(Ope, 0)));
}


/*----- �I������Ă��鎟�̃A�C�e���ԍ���Ԃ� ----------------------------------
*
*	Parameter
*		int Win : �E�B���h�E�ԍ� (WIN_xxxx)
*		int Pos : ���̃A�C�e���ԍ�
*		int All : �I�΂�Ă��Ȃ����̂��܂߂�
*
*	Return Value
*		int �A�C�e���ԍ�
*			-1 = �I������Ă��Ȃ�
*----------------------------------------------------------------------------*/

int GetNextSelected(int Win, int Pos, int All)
{
	HWND hWnd;
	int Ope;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	Ope = LVNI_SELECTED;
	if(All == YES)
		Ope = LVNI_ALL;

	return(SendMessage(hWnd, LVM_GETNEXTITEM, (WPARAM)Pos, (LPARAM)MAKELPARAM(Ope, 0)));
}


/*----- �w�肳�ꂽ���O�̃A�C�e����T�� ----------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		char *Name : ���O
*
*	Return Value
*		int �A�C�e���ԍ�
*			-1=������Ȃ�����
*----------------------------------------------------------------------------*/

int FindNameNode(int Win, char *Name)
{
	LV_FINDINFO FindInfo;
	HWND hWnd;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = Name;
	return(SendMessage(hWnd, LVM_FINDITEM, -1, (LPARAM)&FindInfo));
}


/*----- �w��ʒu�̃A�C�e���̖��O��Ԃ� ----------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		int Pos : �ʒu
*		char *Buf : ���O��Ԃ��o�b�t�@
*		int Max : �o�b�t�@�̃T�C�Y
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void GetNodeName(int Win, int Pos, char *Buf, int Max)
{
	HWND hWnd;
	LV_ITEM LvItem;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	LvItem.mask = LVIF_TEXT;
	LvItem.iItem = Pos;
	LvItem.iSubItem = 0;
	LvItem.pszText = Buf;
	LvItem.cchTextMax = Max;
	SendMessage(hWnd, LVM_GETITEM, 0, (LPARAM)&LvItem);
	return;
}


/*----- �w��ʒu�̃A�C�e���̓��t��Ԃ� ----------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		int Pos : �ʒu
*		FILETIME *Buf : ���t��Ԃ��o�b�t�@
*
*	Return Value
*		int �X�e�[�^�X
*			YES/NO=���t��񂪂Ȃ�����
*----------------------------------------------------------------------------*/

int GetNodeTime(int Win, int Pos, FILETIME *Buf)
{
	HWND hWnd;
	LV_ITEM LvItem;
	char Tmp[20];
	int Ret;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	LvItem.mask = LVIF_TEXT;
	LvItem.iItem = Pos;
	LvItem.iSubItem = 1;
	LvItem.pszText = Tmp;
	LvItem.cchTextMax = 20;
	SendMessage(hWnd, LVM_GETITEM, 0, (LPARAM)&LvItem);
	Ret = TimeString2FileTime(Tmp, Buf);
	return(Ret);
}


/*----- �w��ʒu�̃A�C�e���̃T�C�Y��Ԃ� --------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		int Pos : �ʒu
*		int *Buf : �T�C�Y��Ԃ����[�N
*
*	Return Value
*		int �X�e�[�^�X
*			YES/NO=�T�C�Y��񂪂Ȃ�����
*----------------------------------------------------------------------------*/

int GetNodeSize(int Win, int Pos, LONGLONG *Buf)
{
	HWND hWnd;
	LV_ITEM LvItem;
	char Tmp[40];
	int Ret;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	LvItem.mask = LVIF_TEXT;
	LvItem.iItem = Pos;
	LvItem.iSubItem = 2;
	LvItem.pszText = Tmp;
	LvItem.cchTextMax = 20;
	SendMessage(hWnd, LVM_GETITEM, 0, (LPARAM)&LvItem);
	*Buf = -1;
	Ret = NO;
	if(strlen(Tmp) > 0)
	{
		RemoveComma(Tmp);
		*Buf = _atoi64(Tmp);
		Ret = YES;
	}
	return(Ret);
}


/*----- �w��ʒu�̃A�C�e���̑�����Ԃ� ----------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		int Pos : �ʒu
*		int *Buf : ������Ԃ����[�N
*
*	Return Value
*		int �X�e�[�^�X
*			YES/NO=�T�C�Y��񂪂Ȃ�����
*----------------------------------------------------------------------------*/

int GetNodeAttr(int Win, int Pos, int *Buf)
{
	LV_ITEM LvItem;
	char Tmp[20];
	int Ret;

	*Buf = 0;
	Ret = NO;
	if(Win == WIN_REMOTE)
	{
		LvItem.mask = LVIF_TEXT;
		LvItem.iItem = Pos;
		LvItem.iSubItem = 4;
		LvItem.pszText = Tmp;
		LvItem.cchTextMax = 20;
		SendMessage(GetRemoteHwnd(), LVM_GETITEM, 0, (LPARAM)&LvItem);
		if(strlen(Tmp) > 0)
		{
			*Buf = AttrString2Value(Tmp);
			Ret = YES;
		}
	}
	return(Ret);
}


/*----- �w��ʒu�̃A�C�e���̃^�C�v��Ԃ� --------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		int Pos : �ʒu
*
*	Return Value
*		int �^�C�v (NODE_xxx)
*----------------------------------------------------------------------------*/

int GetNodeType(int Win, int Pos)
{
	char Tmp[20];
	LV_ITEM LvItem;
	int Ret;
	HWND hWnd;

	hWnd = GetLocalHwnd();
	if(Win == WIN_REMOTE)
		hWnd = GetRemoteHwnd();

	LvItem.mask = LVIF_TEXT;
	LvItem.iItem = Pos;
	LvItem.iSubItem = 2;
	LvItem.pszText = Tmp;
	LvItem.cchTextMax = 20;
	SendMessage(hWnd, LVM_GETITEM, 0, (LPARAM)&LvItem);

	if(strcmp(Tmp, "<DIR>") == 0)
		Ret = NODE_DIR;
	else if(strcmp(Tmp, "<DRIVE>") == 0)
		Ret = NODE_DRIVE;
	else
		Ret = NODE_FILE;

	return(Ret);
}


/*----- �w��ʒu�̃A�C�e���̃I�[�i����Ԃ� ------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		int Pos : �ʒu
*		char *Buf : �I�[�i����Ԃ��o�b�t�@
*		int Max : �o�b�t�@�̃T�C�Y
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void GetNodeOwner(int Win, int Pos, char *Buf, int Max)
{
	LV_ITEM LvItem;

	strcpy(Buf, "");
	if(Win == WIN_REMOTE)
	{
		LvItem.mask = LVIF_TEXT;
		LvItem.iItem = Pos;
		LvItem.iSubItem = 5;
		LvItem.pszText = Buf;
		LvItem.cchTextMax = Max;
		SendMessage(GetRemoteHwnd(), LVM_GETITEM, 0, (LPARAM)&LvItem);
	}
	return;
}


/*----- �z�X�g���̃t�@�C���ꗗ�E�C���h�E���N���A ------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void EraseRemoteDirForWnd(void)
{
	SendMessage(GetRemoteHwnd(), LVM_DELETEALLITEMS, 0, 0);
	SendMessage(GetRemoteHistHwnd(), CB_RESETCONTENT, 0, 0);
	return;
}


/*----- �I������Ă���t�@�C���̑��T�C�Y��Ԃ� --------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*
*	Return Value
*		double �T�C�Y
*----------------------------------------------------------------------------*/

double GetSelectedTotalSize(int Win)
{
	double Ret;
	LONGLONG Size;
	int Pos;

	Ret = 0;
	if(GetSelectedCount(Win) > 0)
	{
		Pos = GetFirstSelected(Win, NO);
		while(Pos != -1)
		{
			GetNodeSize(Win, Pos, &Size);
			if(Size >= 0)
				Ret += Size;
			Pos = GetNextSelected(Win, Pos, NO);
		}
	}
	return(Ret);
}



/*===================================================================

===================================================================*/



/*----- �t�@�C���ꗗ�őI�΂�Ă���t�@�C�������X�g�ɓo�^���� ------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		int Expand : �T�u�f�B���N�g����W�J���� (YES/NO)
*		int All : �I�΂�Ă��Ȃ����̂����ׂēo�^���� (YES/NO)
*		FILELIST **Base : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void MakeSelectedFileList(int Win, int Expand, int All, FILELIST **Base, int *CancelCheckWork)
{
	int Pos;
	char Name[FMAX_PATH+1];
	char Cur[FMAX_PATH+1];
	FILELIST Pkt;
	int Node;
	DWORD Attr;
	int Ignore;

	if((All == YES) || (GetSelectedCount(Win) > 0))
	{
		/*===== �J�����g�f�B���N�g���̃t�@�C�� =====*/

		Pos = GetFirstSelected(Win, All);
		while(Pos != -1)
		{
			Node = GetNodeType(Win, Pos);
			if((Node == NODE_FILE) ||
			   ((Expand == NO) && (Node == NODE_DIR)))
			{
				Pkt.InfoExist = 0;
				GetNodeName(Win, Pos, Pkt.File, FMAX_PATH);
				if(GetNodeSize(Win, Pos, &Pkt.Size) == YES)
					Pkt.InfoExist |= FINFO_SIZE;
				if(GetNodeAttr(Win, Pos, &Pkt.Attr) == YES)
					Pkt.InfoExist |= FINFO_ATTR;
				if(GetNodeTime(Win, Pos, &Pkt.Time) == YES)
					Pkt.InfoExist |= (FINFO_TIME | FINFO_DATE);
				Pkt.Node = Node;

				Ignore = NO;
				if((DispIgnoreHide == YES) && (Win == WIN_LOCAL))
				{
					AskLocalCurDir(Cur, FMAX_PATH);
					SetYenTail(Cur);
					strcat(Cur, Pkt.File);
					Attr = GetFileAttributes(Cur);
					if((Attr != 0xFFFFFFFF) && (Attr & FILE_ATTRIBUTE_HIDDEN))
						Ignore = YES;
				}

				if(Ignore == NO)
					AddFileList(&Pkt, Base);
			}
			Pos = GetNextSelected(Win, Pos, All);
		}

		if(Expand == YES)
		{
			/*===== �f�B���N�g���c���[ =====*/

			Pos = GetFirstSelected(Win, All);
			while(Pos != -1)
			{
				if(GetNodeType(Win, Pos) == NODE_DIR)
				{
					GetNodeName(Win, Pos, Name, FMAX_PATH);
					strcpy(Pkt.File, Name);
					ReplaceAll(Pkt.File, '\\', '/');
//8/26

					Ignore = NO;
					if((DispIgnoreHide == YES) && (Win == WIN_LOCAL))
					{
						AskLocalCurDir(Cur, FMAX_PATH);
						SetYenTail(Cur);
						strcat(Cur, Pkt.File);
						ReplaceAll(Cur, '/', '\\');
						Attr = GetFileAttributes(Cur);
						if((Attr != 0xFFFFFFFF) && (Attr & FILE_ATTRIBUTE_HIDDEN))
							Ignore = YES;
					}

					if(Ignore == NO)
					{
						Pkt.Node = NODE_DIR;
						Pkt.Attr = 0;
						Pkt.Size = 0;
						memset(&Pkt.Time, 0, sizeof(FILETIME));
						AddFileList(&Pkt, Base);

						if(Win == WIN_LOCAL)
							MakeLocalTree(Name, Base);
						else
						{
							AskRemoteCurDir(Cur, FMAX_PATH);

							if((AskListCmdMode() == NO) &&
							   (AskUseNLST_R() == YES))
								MakeRemoteTree1(Name, Cur, Base, CancelCheckWork);
							else
								MakeRemoteTree2(Name, Cur, Base, CancelCheckWork);

//DispListList(*Base, "LIST");

						}
					}
				}
				Pos = GetNextSelected(Win, Pos, All);
			}
		}
	}
	return;
}


/* �f�o�b�O�p */
/* �t�@�C�����X�g�̓��e��\�� */
static void DispListList(FILELIST *Pos, char *Title)
{
	DoPrintf("############ %s ############", Title);
	while(Pos != NULL)
	{
		DoPrintf("%d %s", Pos->Node, Pos->File);
		Pos = Pos->Next;
	}
	DoPrintf("############ END ############");
	return;
}


/*----- Drag&Drop���ꂽ�t�@�C�������X�g�ɓo�^���� -----------------------------
*
*	Parameter
*		WPARAM wParam : �h���b�v���ꂽ�t�@�C���̏��
*		char *Cur : �J�����g�f�B���N�g����Ԃ��o�b�t�@
*		FILELIST **Base : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void MakeDroppedFileList(WPARAM wParam, char *Cur, FILELIST **Base)
{
	int Max;
	int i;
	char Name[FMAX_PATH+1];
	char Tmp[FMAX_PATH+1];
	FILELIST Pkt;
	HANDLE fHnd;
	WIN32_FIND_DATA Find;

	Max = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);

	DragQueryFile((HDROP)wParam, 0, Cur, FMAX_PATH);
	GetUpperDir(Cur);

	for(i = 0; i < Max; i++)
	{
		DragQueryFile((HDROP)wParam, i, Name, FMAX_PATH);

		if((GetFileAttributes(Name) & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			Pkt.Node = NODE_FILE;
			strcpy(Pkt.File, GetFileName(Name));

			memset(&Pkt.Time, 0, sizeof(FILETIME));
			if((fHnd = FindFirstFile(Name, &Find)) != INVALID_HANDLE_VALUE)
			{
				FindClose(fHnd);
				Pkt.Time = Find.ftLastWriteTime;
			}
			AddFileList(&Pkt, Base);
		}
	}

	GetCurrentDirectory(FMAX_PATH, Tmp);
	SetCurrentDirectory(Cur);
	for(i = 0; i < Max; i++)
	{
		DragQueryFile((HDROP)wParam, i, Name, FMAX_PATH);

		if(GetFileAttributes(Name) & FILE_ATTRIBUTE_DIRECTORY)
		{
			Pkt.Node = NODE_DIR;
			strcpy(Pkt.File, GetFileName(Name));
			AddFileList(&Pkt, Base);

			MakeLocalTree(Pkt.File, Base);
		}
	}
	SetCurrentDirectory(Tmp);

	DragFinish((HDROP)wParam);

	return;
}


/*----- Drag&Drop���ꂽ�t�@�C��������t�H���_���擾���� -----------------------
*
*	Parameter
*		WPARAM wParam : �h���b�v���ꂽ�t�@�C���̏��
*		char *Cur : �J�����g�f�B���N�g����Ԃ��o�b�t�@
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void MakeDroppedDir(WPARAM wParam, char *Cur)
{
	int Max;

	Max = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
	DragQueryFile((HDROP)wParam, 0, Cur, FMAX_PATH);
	GetUpperDir(Cur);
	DragFinish((HDROP)wParam);

	return;
}


/*----- �z�X�g���̃T�u�f�B���N�g���ȉ��̃t�@�C�������X�g�ɓo�^����i�P�j-------
*
*	Parameter
*		char *Path : �p�X��
*		char *Cur : �J�����g�f�B���N�g��
*		FILELIST **Base : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*
*	Note
*		NLST -alLR ���g��
*----------------------------------------------------------------------------*/

static void MakeRemoteTree1(char *Path, char *Cur, FILELIST **Base, int *CancelCheckWork)
{
	int Sts;

	if(DoCWD(Path, NO, NO, NO) == FTP_COMPLETE)
	{
		/* �T�u�t�H���_���܂߂����X�g���擾 */
		Sts = DoDirListCmdSkt("R", "", 999, CancelCheckWork);	/* NLST -alLR*/
		DoCWD(Cur, NO, NO, NO);

		if(Sts == FTP_COMPLETE)
			AddRemoteTreeToFileList(999, Path, RDIR_NLST, Base);
	}
	return;
}


/*----- �z�X�g���̃T�u�f�B���N�g���ȉ��̃t�@�C�������X�g�ɓo�^����i�Q�j-------
*
*	Parameter
*		char *Path : �p�X��
*		char *Cur : �J�����g�f�B���N�g��
*		FILELIST **Base : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�e�t�H���_�Ɉړ����ă��X�g���擾
*----------------------------------------------------------------------------*/

static void MakeRemoteTree2(char *Path, char *Cur, FILELIST **Base, int *CancelCheckWork)
{
	int Sts;
	FILELIST *CurList;
	FILELIST *Pos;
	FILELIST Pkt;

	/* VAX VMS �� CWD xxx/yyy �Ƃ����w�肪�ł��Ȃ��̂�	*/
	/* CWD xxx, Cwd yyy �ƕ����ɕ�����					*/
	if(AskHostType() != HTYPE_VMS)
		Sts = DoCWD(Path, NO, NO, NO);
	else
	{
#if defined(HAVE_OPENVMS)
		/* OpenVMS�̏ꍇ�A�f�B���N�g���ړ�����"HOGE.DIR;1"��"HOGE"�ɂ��� */
		ReformVMSDirName(Path, TRUE);
#endif
		Sts = DoCWDStepByStep(Path, Cur);
	}

	if(Sts == FTP_COMPLETE)
	{
		Sts = DoDirListCmdSkt("", "", 999, CancelCheckWork);		/* NLST -alL*/
		DoCWD(Cur, NO, NO, NO);

		if(Sts == FTP_COMPLETE)
		{
			CurList = NULL;
			AddRemoteTreeToFileList(999, Path, RDIR_CWD, &CurList);
			CopyTmpListToFileList(Base, CurList);

			Pos = CurList;
			while(Pos != NULL)
			{
				if(Pos->Node == NODE_DIR)
				{
					/* �܂��f�B���N�g�������Z�b�g */
					strcpy(Pkt.File, Pos->File);
					Pkt.Node = NODE_DIR;
					Pkt.Size = 0;
					Pkt.Attr = 0;
					memset(&Pkt.Time, 0, sizeof(FILETIME));
					AddFileList(&Pkt, Base);

					/* ���̃f�B���N�g���̒������� */
					MakeRemoteTree2(Pos->File, Cur, Base, CancelCheckWork);
				}
				Pos = Pos->Next;
			}
			DeleteFileList(&CurList);
		}
	}
	return;
}


/*----- �t�@�C�����X�g�̓��e��ʂ̃t�@�C�����X�g�ɃR�s�[ ----------------------
*
*	Parameter
*		FILELIST **Base : �R�s�[��
*		FILELIST *List : �R�s�[��
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�R�s�[����̂̓t�@�C���̏�񂾂�
*		�f�B���N�g���̏��̓R�s�[���Ȃ�
*----------------------------------------------------------------------------*/

static void CopyTmpListToFileList(FILELIST **Base, FILELIST *List)
{
	while(List != NULL)
	{
		if(List->Node == NODE_FILE)
			AddFileList(List, Base);

		List = List->Next;
	}
	return;
}


/*----- �z�X�g���̃t�@�C�������t�@�C�����X�g�ɓo�^ --------------------------
*
*	Parameter
*		int Num : �e���|�����t�@�C���̃t�@�C�����ԍ� (_ffftp.???)
*		char *Path : �p�X��
*		int IncDir : �ċA�����̕��@ (RDIR_xxx)
*		FILELIST **Base : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void AddRemoteTreeToFileList(int Num, char *Path, int IncDir, FILELIST **Base)
{
	char Str[FMAX_PATH+1];
	char Dir[FMAX_PATH+1];
	char Name[FMAX_PATH+1];
	LONGLONG Size;
	FILETIME Time;
	int Attr;
	FILELIST Pkt;
	FILE *fd;
	int Node;
	int ListType;
	char Owner[OWNER_NAME_LEN+1];
	int Link;
	int InfoExist;

	MakeCacheFileName(Num, Str);
	if((fd = fopen(Str, "rb")) != NULL)
	{
		strcpy(Dir, Path);

		ListType = LIST_UNKNOWN;

		while(GetListOneLine(Str, FMAX_PATH, fd) == SUCCESS)
		{
			if((ListType = AnalizeFileInfo(Str)) == LIST_UNKNOWN)
			{
				if(MakeDirPath(Str, ListType, Path, Dir) == SUCCESS)
				{
					if(IncDir == RDIR_NLST)
					{
						strcpy(Pkt.File, Dir);
						Pkt.Node = NODE_DIR;
						Pkt.Size = 0;
						Pkt.Attr = 0;
						memset(&Pkt.Time, 0, sizeof(FILETIME));
						AddFileList(&Pkt, Base);
					}
				}
			}
			else
			{
				Node = ResolvFileInfo(Str, ListType, Name, &Size, &Time, &Attr, Owner, &Link, &InfoExist);

				if(AskFilterStr(Name, Node) == YES)
				{
					if((Node == NODE_FILE) ||
					   ((IncDir == RDIR_CWD) && (Node == NODE_DIR)))
					{
						strcpy(Pkt.File, Dir);
						if(strlen(Pkt.File) > 0)
							SetSlashTail(Pkt.File);
						strcat(Pkt.File, Name);
						Pkt.Node = Node;
						Pkt.Link = Link;
						Pkt.Size = Size;
						Pkt.Attr = Attr;
						Pkt.Time = Time;
						Pkt.InfoExist = InfoExist;
						AddFileList(&Pkt, Base);
					}
				}
			}
		}
		fclose(fd);
	}
	return;
}


/*----- �t�@�C���ꗗ���̂P�s���擾 ------------------------------------------
*
*	Parameter
*		char *Buf : �P�s�̏����Z�b�g����o�b�t�@
*		int Max : �ő啶����
*		FILE *Fd : �X�g���[��
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL)
*
*	Note
*		VAX VMS�ȊO�̎��� fgets(Buf, Max, Fd) �Ɠ���
*		Vax VMS�̎��́A�����s�̃t�@�C�������P�s�ɂ܂Ƃ߂�
*----------------------------------------------------------------------------*/

static int GetListOneLine(char *Buf, int Max, FILE *Fd)
{
	char Tmp[FMAX_PATH+1];
	int Sts;

	Sts = FAIL;
	while((Sts == FAIL) && (fgets(Buf, Max, Fd) != NULL))
	{
		Sts = SUCCESS;
		RemoveReturnCode(Buf);
		ReplaceAll(Buf, '\x08', ' ');

		/* VAX VMS�ł̓t�@�C����񂪕����s�ɂ킩��Ă���	*/
		/* ������P�s�ɂ܂Ƃ߂�								*/
		if(AskHostType() == HTYPE_VMS)
		{
			if(strchr(Buf, ';') == NULL)	/* �t�@�C�����ȊO�̍s */
				Sts = FAIL;
			else
			{
				Max -= strlen(Buf);
				while(strchr(Buf, ')') == NULL)
				{
					if(fgets(Tmp, FMAX_PATH, Fd) != NULL)
					{
						RemoveReturnCode(Tmp);
						ReplaceAll(Buf, '\x08', ' ');
						if((int)strlen(Tmp) > Max)
							Tmp[Max] = NUL;
						Max -= strlen(Tmp);
						strcat(Buf, Tmp);
					}
					else
						break;
				}
			}
		}
	}

//	DoPrintf("List : %s", Buf);

	return(Sts);
}


/*----- �T�u�f�B���N�g�����̉�� --------------------------------------------
*
*	Parameter
*		char *Str : �t�@�C�����i�P�s�j
*		int ListType : ���X�g�̃^�C�v
*		char *Path : �擪����̃p�X��
*		char *Dir : �f�B���N�g����
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL=�f�B���N�g�����łȂ�
*----------------------------------------------------------------------------*/

static int MakeDirPath(char *Str, int ListType, char *Path, char *Dir)
{
	int Sts;

	Sts = FAIL;
	switch(ListType)
	{
		case LIST_ACOS :
		case LIST_ACOS_4 :
			break;

		default:
			if(*(Str + strlen(Str) - 1) == ':')		/* �Ōオ : �Ȃ�T�u�f�B���N�g�� */
			{
				if(strcmp(Str, ".:") != 0)
				{
					if((strncmp(Str, "./", 2) == 0) ||
					   (strncmp(Str, ".\\", 2) == 0))
					{
						Str += 2;
					}

					if(strlen(Str) > 1)
					{
						strcpy(Dir, Path);
						SetSlashTail(Dir);
						strcat(Dir, Str);
						*(Dir + strlen(Dir) - 1) = NUL;

						ChangeFnameRemote2Local(Dir, FMAX_PATH);

						ReplaceAll(Dir, '\\', '/');
					}
				}
				Sts = SUCCESS;
			}
			break;
	}
	return(Sts);
}


/*----- ���[�J�����̃T�u�f�B���N�g���ȉ��̃t�@�C�������X�g�ɓo�^���� ----------
*
*	Parameter
*		char *Path : �p�X��
*		FILELIST **Base : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void MakeLocalTree(char *Path, FILELIST **Base)
{
	char Src[FMAX_PATH+1];
	HANDLE fHnd;
	WIN32_FIND_DATA FindBuf;
	FILELIST Pkt;
	SYSTEMTIME TmpStime;

	strcpy(Src, Path);
	SetYenTail(Src);
	strcat(Src, "*");
	ReplaceAll(Src, '/', '\\');

	if((fHnd = FindFirstFileAttr(Src, &FindBuf, DispIgnoreHide)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((FindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				if(AskFilterStr(FindBuf.cFileName, NODE_FILE) == YES)
				{
					strcpy(Pkt.File, Path);
					SetSlashTail(Pkt.File);
					strcat(Pkt.File, FindBuf.cFileName);
					ReplaceAll(Pkt.File, '\\', '/');
					Pkt.Node = NODE_FILE;
					Pkt.Size = MakeLongLong(FindBuf.nFileSizeHigh, FindBuf.nFileSizeLow);
					Pkt.Attr = 0;
					Pkt.Time = FindBuf.ftLastWriteTime;
					FileTimeToSystemTime(&Pkt.Time, &TmpStime);
					TmpStime.wSecond = 0;
					TmpStime.wMilliseconds = 0;
					SystemTimeToFileTime(&TmpStime, &Pkt.Time);
					AddFileList(&Pkt, Base);
				}
			}
		}
		while(FindNextFileAttr(fHnd, &FindBuf, DispIgnoreHide) == TRUE);
		FindClose(fHnd);
	}

	if((fHnd = FindFirstFileAttr(Src, &FindBuf, DispIgnoreHide)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if((FindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			   (strcmp(FindBuf.cFileName, ".") != 0) &&
			   (strcmp(FindBuf.cFileName, "..") != 0))
			{
				strcpy(Src, Path);
				SetYenTail(Src);
				strcat(Src, FindBuf.cFileName);
				strcpy(Pkt.File, Src);
				ReplaceAll(Pkt.File, '\\', '/');
				Pkt.Node = NODE_DIR;
				Pkt.Size = 0;
				Pkt.Attr = 0;
				memset(&Pkt.Time, 0, sizeof(FILETIME));
				AddFileList(&Pkt, Base);

				MakeLocalTree(Src, Base);
			}
		}
		while(FindNextFileAttr(fHnd, &FindBuf, DispIgnoreHide) == TRUE);
		FindClose(fHnd);
	}
	return;
}


/*----- �t�@�C�����X�g�ɏ���o�^���� ----------------------------------------
*
*	Parameter
*		FILELIST *Pkt : �o�^����t�@�C�����
*		FILELIST **Base : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void AddFileList(FILELIST *Pkt, FILELIST **Base)
{
	FILELIST *Pos;
	FILELIST *Prev;

	DoPrintf("FileList : NODE=%d : %s", Pkt->Node, Pkt->File);

	/* ���X�g�̏d������菜�� */
	Pos = *Base;
	while(Pos != NULL)
	{
		if(strcmp(Pkt->File, Pos->File) == 0)
		{
			DoPrintf(" --> Duplicate!!");
			break;
		}
		Prev = Pos;
		Pos = Pos->Next;
	}

	if(Pos == NULL)		/* �d�����Ă��Ȃ��̂œo�^���� */
	{
		if((Pos = malloc(sizeof(FILELIST))) != NULL)
		{
			memcpy(Pos, Pkt, sizeof(FILELIST));
			Pos->Next = NULL;

			if(*Base == NULL)
				*Base = Pos;
			else
				Prev->Next = Pos;
		}
	}
	return;
}


/*----- �t�@�C�����X�g���N���A���� --------------------------------------------
*
*	Parameter
*		FILELIST **Base : �t�@�C�����X�g�̐擪
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DeleteFileList(FILELIST **Base)
{
	FILELIST *New;
	FILELIST *Next;

	New = *Base;
	while(New != NULL)
	{
		Next = New->Next;
		free(New);
		New = Next;
	}
	*Base = NULL;
	return;
}


/*----- �t�@�C�����X�g�Ɏw��̃t�@�C�������邩�`�F�b�N ------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*		FILELIST *Base : �t�@�C�����X�g�̐擪
*		int Caps : �啶��/�������̋�ʃ��[�h (COMP_xxx)
*
*	Return Value
*		FILELIST *���������t�@�C�����X�g�̃f�[�^
*			NULL=������Ȃ�
*----------------------------------------------------------------------------*/

FILELIST *SearchFileList(char *Fname, FILELIST *Base, int Caps)
{
	char Tmp[FMAX_PATH+1];

	while(Base != NULL)
	{
		if(Caps == COMP_STRICT)
		{
			if(_mbscmp(Fname, Base->File) == 0)
				break;
		}
		else
		{
			if(_mbsicmp(Fname, Base->File) == 0)
			{
				if(Caps == COMP_IGNORE)
					break;
				else
				{
					strcpy(Tmp, Base->File);
					_mbslwr(Tmp);
					if(_mbscmp(Tmp, Base->File) == 0)
						break;
				}
			}
		}
		Base = Base->Next;
	}
	return(Base);
}


/*----- �t�@�C����񂩂烊�X�g�^�C�v�����߂� ----------------------------------
*
*	Parameter
*		char *Str : �t�@�C�����i�P�s�j
*
*	Return Value
*		int ���X�g�^�C�v (LIST_xxx)
*----------------------------------------------------------------------------*/

static int AnalizeFileInfo(char *Str)
{
	int Ret;
	char Tmp[FMAX_PATH+1];
	int Add1;
	int TmpInt;
	int Flag1;
	WORD Month;
	WORD Day;

//DoPrintf("LIST : %s", Str);

	Ret = LIST_UNKNOWN;
	Flag1 = AskHostType();
	if(Flag1 == HTYPE_ACOS)
		Ret = LIST_ACOS;
	else if(Flag1 == HTYPE_ACOS_4)
		Ret = LIST_ACOS_4;
	else if(Flag1 == HTYPE_VMS)
		Ret = LIST_VMS;
	else if(Flag1 == HTYPE_IRMX)
		Ret = LIST_IRMX;
	else if(Flag1 == HTYPE_STRATUS)
		Ret = LIST_STRATUS;
	else if(Flag1 == HTYPE_AGILENT)
		Ret = LIST_AGILENT;
	else if(Flag1 == HTYPE_SHIBASOKU)
		Ret = LIST_SHIBASOKU;
	else
	{
		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_UNIX_10, LIST_UNIX_20, LIST_UNIX_12, LIST_UNIX_22, LIST_UNIX_50, LIST_UNIX_60 */
		/* MELCOM80 */

		if(FindField(Str, Tmp, 0, NO) == SUCCESS)
		{
			/* MELCOM80�� "d rwxrwxrwx" �̂悤�ɃX�y�[�X���󂢂Ă��� */
			Flag1 = NO;
			if((strlen(Tmp) == 1) && (strchr("-dDlL", Tmp[0]) != NULL))
			{
				if(FindField(Str, Tmp, 1, NO) == SUCCESS)
				{
					if((strlen(Tmp) == 9) ||
					   ((strlen(Tmp) > 9) && (IsDigit(Tmp[9]) != 0)))
					{
						memmove(Str+1, Str+2, strlen(Str+2)+1);
						FindField(Str, Tmp, 0, NO);
						Flag1 = YES;
					}
				}
			}

			if(strlen(Tmp) >= 10)
			{
				Add1 = 0;
				if((strlen(Tmp) > 10) && (IsDigit(Tmp[10]) != 0))
				{
					/* ���������� */
					/*   drwxr-xr-x1234  owner group  1024  Nov 6 14:21 Linux/    */
					Add1 = -1;
				}

////////////
// LIST_UNIX_60 support
				if(FindField(Str, Tmp, 7+Add1, NO) == SUCCESS)
				{
					GetMonth(Tmp, &Month, &Day);
					if(Month != 0)
					{
						Ret = CheckUnixType(Str, Tmp, Add1, 2, Day);
					}
				}
///////////

////////////
// LIST_UNIX_12 support
				if((Ret == LIST_UNKNOWN) &&
				   (FindField(Str, Tmp, 6+Add1, NO) == SUCCESS))
				{
					GetMonth(Tmp, &Month, &Day);
					if(Month != 0)
					{
						Ret = CheckUnixType(Str, Tmp, Add1, 0, Day);
					}
				}
//////////////////

////////////
// LIST_UNIX_70 support
				if((Ret == LIST_UNKNOWN) &&
				   (FindField(Str, Tmp, 6+Add1, NO) == SUCCESS))
				{
					GetMonth(Tmp, &Month, &Day);
					if(Month != 0)
					{
						Ret = CheckUnixType(Str, Tmp, Add1, 1, Day);
					}
				}
///////////

				if((Ret == LIST_UNKNOWN) &&
				   (FindField(Str, Tmp, 5+Add1, NO) == SUCCESS))
				{
					GetMonth(Tmp, &Month, &Day);
					if(Month != 0)
					{
						Ret = CheckUnixType(Str, Tmp, Add1, 0, Day);
					}
				}

				if((Ret == LIST_UNKNOWN) &&
				   (FindField(Str, Tmp, 4+Add1, NO) == SUCCESS))
				{
					GetMonth(Tmp, &Month, &Day);
					if(Month != 0)
					{
						Ret = CheckUnixType(Str, Tmp, Add1, -1, Day);
					}
				}

				if((Ret == LIST_UNKNOWN) &&
				   (FindField(Str, Tmp, 3+Add1, NO) == SUCCESS))
				{
					GetMonth(Tmp, &Month, &Day);
					if(Month != 0)
					{
						Ret = CheckUnixType(Str, Tmp, Add1, -2, Day);
					}
				}

				if((Ret != LIST_UNKNOWN) && (Flag1 == YES))
					Ret |= LIST_MELCOM;
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_AS400 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
			   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
			{
				if((FindField(Str, Tmp, 3, NO) == SUCCESS) &&
				   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
				{
					if((FindField(Str, Tmp, 1, NO) == SUCCESS) &&
					   (IsDigit(Tmp[0]) != 0))
					{
						if(FindField(Str, Tmp, 5, NO) == SUCCESS)
						{
							Ret = LIST_AS400;
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_M1800 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 5, NO) == SUCCESS) &&
			   (CheckYYMMDDformat(Tmp, '*', NO) != 0))
			{
				if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
				   ((IsDigit(Tmp[0]) != 0) || (StrAllSameChar(Tmp, '*') == YES)))
				{
					if((FindField(Str, Tmp, 3, NO) == SUCCESS) &&
					   ((IsDigit(Tmp[0]) != 0) || (StrAllSameChar(Tmp, '*') == YES)))
					{
						if((FindField(Str, Tmp, 0, NO) == SUCCESS) &&
						   (strlen(Tmp) == 4))
						{
							if(FindField(Str, Tmp, 6, NO) == SUCCESS)
							{
								Ret = LIST_M1800;
							}
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_GP6000 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 1, NO) == SUCCESS) &&
			   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
			{
				if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
				   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
				{
					if((FindField(Str, Tmp, 5, NO) == SUCCESS) &&
					   (IsDigit(Tmp[0]) != 0))
					{
						if(FindField(Str, Tmp, 6, NO) == SUCCESS)
						{
							Ret = LIST_GP6000;
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_DOS_1, LIST_DOS_2 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 1, NO) == SUCCESS) &&
			   (CheckHHMMformat(Tmp) == YES))
			{
				if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
				   ((Tmp[0] == '<') || (IsDigit(Tmp[0]) != 0)))
				{
					if(FindField(Str, Tmp, 3, NO) == SUCCESS)
					{
						if((FindField(Str, Tmp, 0, NO) == SUCCESS) &&
						   (CheckYYMMDDformat(Tmp, NUL, YES) != 0))
						{
							TmpInt = atoi(Tmp);
							if((TmpInt >= 1) && (TmpInt <= 12))
								Ret = LIST_DOS_2;
							else
								Ret = LIST_DOS_1;
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_DOS_3 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 3, NO) == SUCCESS) &&
			   (CheckHHMMformat(Tmp) == YES))
			{
				if((FindField(Str, Tmp, 1, NO) == SUCCESS) &&
				   ((Tmp[0] == '<') || (IsDigit(Tmp[0]) != 0)))
				{
					if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
					   (CheckYYMMDDformat(Tmp, NUL, YES) != 0))
					{
						Ret = LIST_DOS_3;
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_DOS_4 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 0, NO) == SUCCESS) &&
			   (CheckYYYYMMDDformat(Tmp, NUL) == YES))
			{
				if((FindField(Str, Tmp, 1, NO) == SUCCESS) &&
				   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
				{
					if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
					   ((Tmp[0] == '<') || (IsDigit(Tmp[0]) != 0)))
					{
						if(FindField(Str, Tmp, 3, NO) == SUCCESS)
						{
							Ret = LIST_DOS_4;
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_CHAMELEON */

		if(Ret == LIST_UNKNOWN)
		{
			if(FindField(Str, Tmp, 2, NO) == SUCCESS)
			{
				GetMonth(Tmp, &Month, &Day);
				if((Month != 0) && (Day == 0))
				{
					if((FindField(Str, Tmp, 1, NO) == SUCCESS) &&
					   ((Tmp[0] == '<') || (IsDigit(Tmp[0]) != 0)))
					{
						if((FindField(Str, Tmp, 5, NO) == SUCCESS) &&
						   (CheckHHMMformat(Tmp) == YES))
						{
							Ret = LIST_CHAMELEON;
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_OS2 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 3, NO) == SUCCESS) &&
			   (CheckHHMMformat(Tmp) == YES))
			{
				if((FindField(Str, Tmp, 0, NO) == SUCCESS) &&
				   (IsDigit(Tmp[0]) != 0))
				{
					if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
					   (CheckYYMMDDformat(Tmp, NUL, YES) != 0))
					{
						if(FindField(Str, Tmp, 4, NO) == SUCCESS)
						{
							Ret = LIST_OS2;
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_OS7 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 0, NO) == SUCCESS) &&
			   (strlen(Tmp) == 10))
			{
				if((FindField(Str, Tmp, 3, NO) == SUCCESS) &&
				   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
				{
					if((FindField(Str, Tmp, 4, NO) == SUCCESS) &&
					   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
					{
						if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
						   (IsDigit(Tmp[0]) != 0))
						{
							if(FindField(Str, Tmp, 5, NO) == SUCCESS)
							{
								Ret = LIST_OS7_2;
							}
						}
					}
				}
				else if((FindField(Str, Tmp, 1, NO) == SUCCESS) &&
						(CheckYYMMDDformat(Tmp, NUL, NO) != 0))
				{
					if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
					   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
					{
						if(FindField(Str, Tmp, 3, NO) == SUCCESS)
						{
							Ret = LIST_OS7_1;
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_ALLIED */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 0, NO) == SUCCESS) &&
			   ((Tmp[0] == '<') || (IsDigit(Tmp[0]) != 0)))
			{
				if((FindField(Str, Tmp, 5, NO) == SUCCESS) &&
				   (CheckHHMMformat(Tmp) == YES))
				{
					if(FindField(Str, Tmp, 3, NO) == SUCCESS)
					{
						GetMonth(Tmp, &Month, &Day);
						if(Month != 0)
						{
							if((FindField(Str, Tmp, 6, NO) == SUCCESS) &&
							   (IsDigit(Tmp[0]) != 0))
							{
								Ret = LIST_ALLIED;
							}
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_OS9 */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 1, NO) == SUCCESS) &&
			   (CheckYYMMDDformat(Tmp, NUL, NO) != 0))
			{
				if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
				   (IsDigit(Tmp[0]) != 0) && (strlen(Tmp) == 4))
				{
					if((FindField(Str, Tmp, 5, NO) == SUCCESS) &&
					   (IsDigit(Tmp[0]) != 0))
					{
						if(FindField(Str, Tmp, 6, NO) == SUCCESS)
						{
							Ret = LIST_OS9;
						}
					}
				}
			}
		}

		/* �ȉ��̃t�H�[�}�b�g���`�F�b�N */
		/* LIST_IBM */

		if(Ret == LIST_UNKNOWN)
		{
			if((FindField(Str, Tmp, 2, NO) == SUCCESS) &&
			   (CheckYYYYMMDDformat(Tmp, NUL) == YES))
			{
				if((FindField(Str, Tmp, 1, NO) == SUCCESS) && IsDigit(Tmp[0]))
				{
					if((FindField(Str, Tmp, 7, NO) == SUCCESS) && IsDigit(Tmp[0]))
					{
						if(FindField(Str, Tmp, 9, NO) == SUCCESS)
						{
							Ret = LIST_IBM;
						}
					}
				}
			}
		}

	}

DoPrintf("ListType=%d", Ret);

	return(Ret);
}


/*----- UNIX�n���X�g�^�C�v�̃`�F�b�N���s�Ȃ� ----------------------------------
*
*	Parameter
*		char *Str : �t�@�C�����i�P�s�j
*		char *Tmp : �ꎞ���[�N
*		int Add1 : ���Z�p�����[�^1
*		int Add2 : ���Z�p�����[�^2
*		int Day : �� (0=�����Ŏ擾����)
*
*	Return Value
*		int ���X�g�^�C�v (LIST_xxx)
*----------------------------------------------------------------------------*/

static int CheckUnixType(char *Str, char *Tmp, int Add1, int Add2, int Day)
{
	int Ret;
	int Add3;
	WORD Hour;
	WORD Minute;
	int Flag;

	Flag = 0;
	Ret = LIST_UNKNOWN;

//Day�ɂ����Add3��ς���

	Add3 = 0;
	if(Day != 0)
		Add3 = -1;

	// unix�n�`�F�b�N
	if((Day != 0) ||
	   ((FindField(Str, Tmp, 6+Add1+Add2+Add3, NO) == SUCCESS) &&
		((atoi(Tmp) >= 1) && (atoi(Tmp) <= 31))))
	{
		if((FindField(Str, Tmp, 7+Add1+Add2+Add3, NO) == SUCCESS) &&
		   ((atoi(Tmp) >= 1900) || (GetHourAndMinute(Tmp, &Hour, &Minute) == SUCCESS)))
		{
			if(FindField(Str, Tmp, 8+Add1+Add2+Add3, NO) == SUCCESS)
			{
				Flag = 1;
			}
		}
	}

	// ������Solaris��p
	if(Flag == 0)
	{
	   if((FindField(Str, Tmp, 7+Add1+Add2+Add3, NO) == SUCCESS) &&
		  ((atoi(Tmp) >= 1) && (atoi(Tmp) <= 31)))
		{
			if((FindField(Str, Tmp, 5+Add1+Add2+Add3, NO) == SUCCESS) &&
			   (atoi(Tmp) >= 1900))
			{
				if((FindField(Str, Tmp, 6+Add1+Add2+Add3, NO) == SUCCESS) &&
				   (((atoi(Tmp) >= 1) && (atoi(Tmp) <= 9) && 
					 ((unsigned char)Tmp[1] == 0xD4) &&
					 ((unsigned char)Tmp[2] == 0xC2)) ||
				    ((atoi(Tmp) >= 10) && (atoi(Tmp) <= 12) && 
					 ((unsigned char)Tmp[2] == 0xD4) && 
					 ((unsigned char)Tmp[3] == 0xC2))))
				{
					if(FindField(Str, Tmp, 8+Add1+Add2+Add3, NO) == SUCCESS)
					{
						Flag = 2;
					}
				}
			}
		}
	}

	if(Flag != 0)
	{
		if(Add2 == 2)
		{
			Ret = LIST_UNIX_60;
			if(Flag == 2)
				Ret = LIST_UNIX_64;
			if(Day != 0)
				Ret = LIST_UNIX_61;

			if(Add1 == -1)
			{
				Ret = LIST_UNIX_62;
				if(Flag == 2)
					Ret = LIST_UNIX_65;
				if(Day != 0)
					Ret = LIST_UNIX_63;
			}
		}
		else if(Add2 == 1)
		{
			Ret = LIST_UNIX_70;
			if(Flag == 2)
				Ret = LIST_UNIX_74;
			if(Day != 0)
				Ret = LIST_UNIX_71;

			if(Add1 == -1)
			{
				Ret = LIST_UNIX_72;
				if(Flag == 2)
					Ret = LIST_UNIX_75;
				if(Day != 0)
					Ret = LIST_UNIX_73;
			}
		}
		else if(Add2 == 0)
		{
			Ret = LIST_UNIX_10;
			if(Flag == 2)
				Ret = LIST_UNIX_14;
			if(Day != 0)
				Ret = LIST_UNIX_11;

			if(Add1 == -1)
			{
				Ret = LIST_UNIX_12;
				if(Flag == 2)
					Ret = LIST_UNIX_15;
				if(Day != 0)
					Ret = LIST_UNIX_13;
			}
		}
		else if(Add2 == -1)
		{
			Ret = LIST_UNIX_20;
			if(Flag == 2)
				Ret = LIST_UNIX_24;
			if(Day != 0)
				Ret = LIST_UNIX_21;

			if(Add1 == -1)
			{
				Ret = LIST_UNIX_22;
				if(Flag == 2)
					Ret = LIST_UNIX_25;
				if(Day != 0)
					Ret = LIST_UNIX_23;
			}
		}
		else
		{
			Ret = LIST_UNIX_50;
			if(Flag == 2)
				Ret = LIST_UNIX_54;
			if(Day != 0)
				Ret = LIST_UNIX_51;
		}
	}
	return(Ret);
}


/*----- HH:MM �`���̕����񂩂ǂ������`�F�b�N ----------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		int �X�e�[�^�X (YES/NO)
*
*	Note
*		��؂蕶���͉��ł��悢
*		�����łȂ��Ă��悢
*		���ɗ]���ȕ������t���Ă��Ă��悢
*----------------------------------------------------------------------------*/

static int CheckHHMMformat(char *Str)
{
	int Ret;

	Ret = NO;
	if((strlen(Str) >= 3) &&
	   (IsDigit(Str[0]) != 0))
	{
		if((Str = strchr(Str, ':')) != NULL)
		{
			if(IsDigit(*(Str+1)) != 0)
				Ret = YES;
		}
	}
	return(Ret);
}


/*----- YY/MM/DD �`���̕����񂩂ǂ������`�F�b�N -------------------------------
*
*	Parameter
*		char *Str : ������
*		char Sym : �����̑���Ɏg����L�� (NUL=�����ȊO�g���Ȃ�)
*		int Dig3 : 3���̔N������
*
*	Return Value
*		int �X�e�[�^�X
*			0 = �Y�����Ȃ�
*			1 = ??/??/??, ??/??/???
*			2 = ???/??/??
*
*	Note
*		��؂蕶���͉��ł��悢
*		�N�����łȂ��Ă��悢
*----------------------------------------------------------------------------*/

static int CheckYYMMDDformat(char *Str, char Sym, int Dig3)
{
	int Ret;

	Ret = 0;
	if((strlen(Str) == 8) &&
	   (IsDigitSym(Str[0], Sym) != 0) && (IsDigitSym(Str[1], Sym) != 0) &&
	   (IsDigit(Str[2]) == 0) &&
	   (IsDigitSym(Str[3], Sym) != 0) && (IsDigitSym(Str[4], Sym) != 0) &&
	   (IsDigit(Str[5]) == 0) &&
	   (IsDigitSym(Str[6], Sym) != 0) && (IsDigitSym(Str[7], Sym) != 0))
	{
		Ret = 1; 
	}
	if(Dig3 == YES)
	{
		if((strlen(Str) == 9) &&
		   (IsDigitSym(Str[0], Sym) != 0) && (IsDigitSym(Str[1], Sym) != 0) && (IsDigitSym(Str[2], Sym) != 0) &&
		   (IsDigit(Str[3]) == 0) &&
		   (IsDigitSym(Str[4], Sym) != 0) && (IsDigitSym(Str[5], Sym) != 0) &&
		   (IsDigit(Str[6]) == 0) &&
		   (IsDigitSym(Str[7], Sym) != 0) && (IsDigitSym(Str[8], Sym) != 0))
		{
			Ret = 2; 
		}
		else if((strlen(Str) == 9) &&
				(IsDigitSym(Str[0], Sym) != 0) && (IsDigitSym(Str[1], Sym) != 0) &&
				(IsDigit(Str[2]) == 0) &&
				(IsDigitSym(Str[3], Sym) != 0) && (IsDigitSym(Str[4], Sym) != 0) &&
				(IsDigit(Str[5]) == 0) &&
				(IsDigitSym(Str[6], Sym) != 0) && (IsDigitSym(Str[7], Sym) != 0) && (IsDigitSym(Str[8], Sym) != 0))
		{
			Ret = 1; 
		}
	}
	return(Ret);
}


/*----- YYYY/MM/DD �`���̕����񂩂ǂ������`�F�b�N -----------------------------
*
*	Parameter
*		char *Str : ������
*		char Sym : �����̑���Ɏg����L�� (NUL=�����ȊO�g���Ȃ�)
*
*	Return Value
*		int �X�e�[�^�X (YES/NO)
*
*	Note
*		��؂蕶���͉��ł��悢
*		�N�����łȂ��Ă��悢
*----------------------------------------------------------------------------*/

static int CheckYYYYMMDDformat(char *Str, char Sym)
{
	int Ret;

	Ret = NO;
	if((strlen(Str) == 10) &&
	   (IsDigitSym(Str[0], Sym) != 0) && (IsDigitSym(Str[1], Sym) != 0) &&
	   (IsDigitSym(Str[2], Sym) != 0) && (IsDigitSym(Str[3], Sym) != 0) &&
	   (IsDigit(Str[4]) == 0) &&
	   (IsDigitSym(Str[5], Sym) != 0) && (IsDigitSym(Str[6], Sym) != 0) &&
	   (IsDigit(Str[7]) == 0) &&
	   (IsDigitSym(Str[8], Sym) != 0) && (IsDigitSym(Str[9], Sym) != 0))
	{
		Ret = YES; 
	}
	return(Ret);
}


/*----- �t�@�C����񂩂�t�@�C�����A�T�C�Y�Ȃǂ����o�� ----------------------
*
*	Parameter
*		char *Str : �t�@�C�����i�P�s�j
*		int ListType : ���X�g�̃^�C�v
*		char *Name : �t�@�C�����̃R�s�[��
*		LONGLONG *Size : �T�C�Y�̃R�s�[��
*		FILETIME *Time : ���t�̃R�s�[��
*		int *Attr : �����̃R�s�[��
*		char *Owner : �I�[�i��
*		int *Link : �����N���ǂ��� (YES/NO)
*		int *InfoExist : �����̏�񂪂��������ǂ��� (YES/NO)
*
*	Return Value
*		int ��� (NODE_xxxx)
*----------------------------------------------------------------------------*/

static int ResolvFileInfo(char *Str, int ListType, char *Fname, LONGLONG *Size, FILETIME *Time, int *Attr, char *Owner, int *Link, int *InfoExist)
{
	SYSTEMTIME sTime;
	SYSTEMTIME sTimeNow;
	char Buf[FMAX_PATH+1];
	char *Pos;
	char Flag;
	int Ret;
	int offs;
	int offs2;
	int offs3;
	int OrgListType;
	int err;
	int Flag2;

	static const int DosPos[3][4] = { { 1, 0, 2, 3 }, { 1, 0, 2, 3 }, { 3, 2, 1, 0 } };
	static const int DosDate[3][3][2] = { { {0, 0}, {3, 4}, {6, 7} }, { {6, 7}, {0, 0}, {3, 4} }, { {6, 7}, {0, 0}, {3, 4} } };
	static const int DosLongFname[3] = { YES, YES, NO };

	/* �܂��N���A���Ă��� */
	Ret = NODE_NONE;
	*Size = -1;
	*Attr = 0;
	*Link = NO;
	memset(Owner, NUL, OWNER_NAME_LEN+1);
	Time->dwLowDateTime = 0;
	Time->dwHighDateTime = 0;
	*InfoExist = 0;
	offs = 0;
	offs2 = 0;
	offs3 = 0;

	OrgListType = ListType;
	ListType &= LIST_MASKFLG;
	switch(ListType)
	{
		case LIST_DOS_1 :
		case LIST_DOS_2 :
		case LIST_DOS_3 :
			if(ListType == LIST_DOS_1)
				offs = 0;
			else if(ListType == LIST_DOS_2)
				offs = 1;
			else
				offs = 2;

			*InfoExist |= (FINFO_DATE | FINFO_SIZE);

			/* ���� */
			FindField(Str, Buf, DosPos[offs][0], NO);
			if((Pos = strchr(Buf, ':')) != NULL)
			{
				*InfoExist |= FINFO_TIME;
				sTime.wHour = atoi(Buf);
				sTime.wMinute = atoi(Pos+1);
				sTime.wSecond = 0;
				sTime.wMilliseconds = 0;

				if(strlen(Pos) >= 4)
				{
					if(tolower(Pos[3]) == 'a')
					{
						if(sTime.wHour == 12)
							sTime.wHour = 0;
					}
					else if(tolower(Pos[3]) == 'p')
					{
						if(sTime.wHour != 12)
							sTime.wHour += 12;
					}
				}
			}

			/* ���t */
			FindField(Str, Buf, DosPos[offs][1], NO);
			if((offs2 = CheckYYMMDDformat(Buf, NUL, YES)) == 0)
				break;
			offs2--;
			sTime.wYear = Assume1900or2000(atoi(Buf + DosDate[offs][0][offs2]));
			sTime.wMonth = atoi(Buf + DosDate[offs][1][offs2]);
			sTime.wDay = atoi(Buf + DosDate[offs][2][offs2]);
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �T�C�Y */
			FindField(Str, Buf, DosPos[offs][2], NO);
			*Size = _atoi64(Buf);

			/* ���O */
			if(FindField(Str, Fname, DosPos[offs][3], DosLongFname[offs]) == SUCCESS)
			{
				Ret = NODE_FILE;
				if(Buf[0] == '<')
					Ret = NODE_DIR;
			}
			break;

		case LIST_DOS_4 :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_SIZE);

			/* ���t */
			FindField(Str, Buf, 0, NO);
			sTime.wYear = atoi(Buf);
			sTime.wMonth = atoi(Buf+5);
			sTime.wDay = atoi(Buf+8);

			/* ���� */
			*InfoExist |= FINFO_TIME;
			FindField(Str, Buf, 1, NO);
			sTime.wHour = atoi(Buf);
			sTime.wMinute = atoi(Buf+3);
			sTime.wSecond = 0;				// atoi(Buf+6);
			sTime.wMilliseconds = 0;
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �T�C�Y */
			FindField(Str, Buf, 2, NO);
			*Size = _atoi64(Buf);

			/* ���O */
			if(FindField(Str, Fname, 3, YES) == SUCCESS)
			{
				Ret = NODE_FILE;
				if(Buf[0] == '<')
					Ret = NODE_DIR;
			}
			break;

		case LIST_OS2 :
			*InfoExist |= (FINFO_DATE | FINFO_SIZE);

			/* ���� */
			FindField(Str, Buf, 3, NO);
			if((Pos = strchr(Buf, ':')) != NULL)
			{
				*InfoExist |= FINFO_TIME;
				sTime.wHour = atoi(Buf);
				sTime.wMinute = atoi(Pos+1);
				sTime.wSecond = 0;
				sTime.wMilliseconds = 0;
			}

			/* ���t */
			FindField(Str, Buf, 2, NO);
			sTime.wYear = Assume1900or2000(atoi(Buf+6));
			sTime.wMonth = atoi(Buf+0);
			sTime.wDay = atoi(Buf+3);
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �T�C�Y */
			FindField(Str, Buf, 0, NO);
			*Size = _atoi64(Buf);

			/* ���O */
			if(FindField(Str, Fname, 4, YES) == SUCCESS)
			{
				FindField(Str, Buf, 1, NO);
				Ret = NODE_FILE;
				if(strstr(Buf, "DIR") != NULL)
					Ret = NODE_DIR;
			}
			break;

		case LIST_CHAMELEON :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_SIZE | FINFO_ATTR);

			/* ���� */
			FindField(Str, Buf, 6, NO);
			strcat(Buf, "------");
			*Attr = AttrString2Value(Buf+1);

			/* ���t */
			FindField(Str, Buf, 2, NO);
			GetMonth(Buf, &sTime.wMonth, &sTime.wDay);	/* wDay�͏��0 */
			FindField(Str, Buf, 3, NO);
			sTime.wDay = atoi(Buf);
			FindField(Str, Buf, 4, NO);
			sTime.wYear = atoi(Buf);

			/* ���� */
			FindField(Str, Buf, 5, NO);
			sTime.wHour = atoi(Buf);
			sTime.wMinute = atoi(Buf+3);
			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �T�C�Y */
			FindField(Str, Buf, 1, NO);
			*Size = _atoi64(Buf);

			/* ���O */
			if(FindField(Str, Fname, 0, NO) == SUCCESS)
			{
				Ret = NODE_FILE;
				if(Buf[0] == '<')
					Ret = NODE_DIR;
			}
			break;

		case LIST_AS400 :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_SIZE);

			/* �I�[�i�� */
			FindField(Str, Buf, 0, NO);
			strncpy(Owner, Buf, OWNER_NAME_LEN);

			/* ���� */
			FindField(Str, Buf, 3, NO);
			sTime.wHour = atoi(Buf);
			sTime.wMinute = atoi(Buf+3);
			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;

			/* ���t */
			FindField(Str, Buf, 2, NO);
			sTime.wYear = Assume1900or2000(atoi(Buf));
			sTime.wMonth = atoi(Buf + 3);
			sTime.wDay = atoi(Buf + 6);
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �T�C�Y */
			FindField(Str, Buf, 1, NO);
			*Size = _atoi64(Buf);

			/* ���O */
			if(FindField(Str, Fname, 5, YES) == SUCCESS)
			{
				Ret = NODE_FILE;
				if((Pos = strchr(Fname, '/')) != NULL)
				{
					Ret = NODE_DIR;
					*Pos = NUL;
				}
			}
			break;

		case LIST_M1800 :
			*InfoExist |= FINFO_ATTR;

			/* ���� */
			FindField(Str, Buf, 0, NO);
			strcat(Buf, "------");
			*Attr = AttrString2Value(Buf+1);

			/* ���t */
			Time->dwLowDateTime = 0;
			Time->dwHighDateTime = 0;
			FindField(Str, Buf, 5, NO);
			if(Buf[0] != '*')
			{
				*InfoExist |= FINFO_DATE;
				sTime.wHour = 0;
				sTime.wMinute = 0;
				sTime.wSecond = 0;
				sTime.wMilliseconds = 0;

				sTime.wYear = Assume1900or2000(atoi(Buf));
				sTime.wMonth = atoi(Buf + 3);
				sTime.wDay = atoi(Buf + 6);
				SystemTimeToFileTime(&sTime, Time);
				SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());
			}

			/* ���O */
			if(FindField(Str, Fname, 6, YES) == SUCCESS)
			{
				RemoveTailingSpaces(Fname);
				Ret = NODE_FILE;
				if((Pos = strchr(Fname, '/')) != NULL)
				{
					Ret = NODE_DIR;
					*Pos = NUL;
				}
			}
			break;

		case LIST_GP6000 :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_SIZE | FINFO_ATTR);

			/* �I�[�i�� */
			FindField(Str, Buf, 3, NO);
			strncpy(Owner, Buf, OWNER_NAME_LEN);

			/* ���� */
			FindField(Str, Buf, 2, NO);
			sTime.wHour = atoi(Buf);
			sTime.wMinute = atoi(Buf+3);
			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;

			/* ���t */
			FindField(Str, Buf, 1, NO);
			sTime.wYear = Assume1900or2000(atoi(Buf));
			sTime.wMonth = atoi(Buf + 3);
			sTime.wDay = atoi(Buf + 6);
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �T�C�Y */
			FindField(Str, Buf, 5, NO);
			*Size = _atoi64(Buf);

			/* ���� */
			FindField(Str, Buf, 0, NO);
			*Attr = AttrString2Value(Buf+1);

			/* ���O */
			if(FindField(Str, Fname, 6, YES) == SUCCESS)
			{
				Ret = NODE_FILE;
				if(strchr("dl", Buf[0]) != NULL)
					Ret = NODE_DIR;
			}
			break;

		case LIST_ACOS :
		case LIST_ACOS_4 :
			/* ���O */
			FindField(Str, Fname, 0, NO);
			Ret = NODE_FILE;
			break;

		case LIST_VMS :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_SIZE);

			/* �T�C�Y */
			FindField(Str, Buf, 1, NO);
			*Size = _atoi64(Buf) * BLOCK_SIZE;

			/* �����^���t */
			FindField(Str, Buf, 2, NO);
			GetVMSdate(Buf, &sTime.wYear, &sTime.wMonth, &sTime.wDay);

			FindField(Str, Buf, 3, NO);
			GetHourAndMinute(Buf, &sTime.wHour, &sTime.wMinute);

			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* ���O */
			FindField(Str, Fname, 0, NO);

			Ret = NODE_FILE;
			if((Pos = strchr(Fname, '.')) != NULL)
			{
				if(_strnicmp(Pos, ".DIR;", 5) == 0)
				{
					/* OpenVMS�̏ꍇ�A�t�@�C��/�f�B���N�g���폜���ɂ�".DIR;?"�܂łȂ���
					 * �폜�ł��Ȃ��̂ŁA�����ł͂Ԃ��Ȃ� */
#if !defined(HAVE_OPENVMS)
					*Pos = NUL;
#endif
					Ret = NODE_DIR;
				}
			}
			break;

		case LIST_OS7_2 :
			*InfoExist |= FINFO_SIZE;
			offs = 2;

			/* �T�C�Y */
			FindField(Str, Buf, 2, NO);
			*Size = _atoi64(Buf);
			/* ������break�͂Ȃ� */

		case LIST_OS7_1 :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_ATTR);

			/* ���t */
			FindField(Str, Buf, 1+offs, NO);
			sTime.wYear = Assume1900or2000(atoi(Buf));
			sTime.wMonth = atoi(Buf + 3);
			sTime.wDay = atoi(Buf + 6);

			/* ���� */
			FindField(Str, Buf, 2+offs, NO);
			sTime.wHour = atoi(Buf);
			sTime.wMinute = atoi(Buf+3);
			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* ���� */
			FindField(Str, Buf, 0, NO);
			*Attr = AttrString2Value(Buf+1);

			/* ���O */
			if(FindField(Str, Fname, 3+offs, YES) == SUCCESS)
			{
				RemoveTailingSpaces(Fname);
				Ret = NODE_FILE;
				if(strchr("dl", Buf[0]) != NULL)
					Ret = NODE_DIR;
			}
			break;

		case LIST_STRATUS :
			if(FindField(Str, Buf, 0, NO) != SUCCESS)
				break;
			if(_strnicmp(Buf, "Files:", 6) == 0)
				StratusMode = 0;
			else if(_strnicmp(Buf, "Dirs:", 5) == 0)
				StratusMode = 1;
			else if(_strnicmp(Buf, "Links:", 6) == 0)
				StratusMode = 2;
			else
			{
				if(StratusMode == 0)
					offs = 1;
				else if(StratusMode == 1)
					offs = 0;
				else
					break;

				*InfoExist |= (FINFO_TIME | FINFO_DATE);

				/* ���t */
				if(FindField(Str, Buf, 2+offs, NO) != SUCCESS)
					break;
				sTime.wYear = Assume1900or2000(atoi(Buf));
				sTime.wMonth = atoi(Buf + 3);
				sTime.wDay = atoi(Buf + 6);

				/* ���� */
				if(FindField(Str, Buf, 3+offs, NO) != SUCCESS)
					break;
				sTime.wHour = atoi(Buf);
				sTime.wMinute = atoi(Buf+3);
				sTime.wSecond = 0;
				sTime.wMilliseconds = 0;
				SystemTimeToFileTime(&sTime, Time);
				SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

				/* ���O */
				if(FindField(Str, Fname, 4+offs, YES) != SUCCESS)
					break;

				if(StratusMode == 0)
				{
					*InfoExist |= FINFO_SIZE;

					/* �T�C�Y */
					if(FindField(Str, Buf, 1, NO) != SUCCESS)
						break;
					*Size = _atoi64(Buf) * 4096;

					/* ��ށi�I�[�i���̃t�B�[���h�ɂ����j */
					if(FindField(Str, Buf, 2, NO) != SUCCESS)
						break;
					strncpy(Owner, Buf, OWNER_NAME_LEN);

					Ret = NODE_FILE;
				}
				else
					Ret = NODE_DIR;
			}
			break;

		case LIST_IRMX :
			*InfoExist |= (FINFO_DATE | FINFO_SIZE);

			/* ���t */
			for(offs = 11; offs > 7; offs--)
			{
				if((err = FindField(Str, Buf, offs, NO)) == SUCCESS)
					break;
			}
			if(err != SUCCESS)
				break;
			if(IsDigit(*Buf) == 0)
				break;
			sTime.wYear = Assume1900or2000(atoi(Buf));
			if(FindField(Str, Buf, --offs, NO) != SUCCESS)
				break;
			GetMonth(Buf, &sTime.wMonth, &sTime.wDay);
			if(FindField(Str, Buf, --offs, NO) != SUCCESS)
				break;
			if(IsDigit(*Buf) == 0)
				break;
			sTime.wDay = atoi(Buf);
			sTime.wHour = 0;
			sTime.wMinute = 0;
			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �I�[�i�� */
			if(FindField(Str, Buf, --offs, NO) != SUCCESS)
				break;
			strncpy(Owner, Buf, OWNER_NAME_LEN);

			/* �T�C�Y */
			do
			{
				if((err = FindField(Str, Buf, --offs, NO)) != SUCCESS)
					break;
			}
			while(IsDigit(*Buf) == 0);
			--offs;
			if((err = FindField(Str, Buf, --offs, NO)) != SUCCESS)
				break;
			RemoveComma(Buf);
			*Size = _atoi64(Buf);
			if((err = FindField(Str, Buf, --offs, NO)) != SUCCESS)
				break;
			if(IsDigit(*Buf) == 0)
				break;
			/* ���O */
			if(FindField(Str, Fname, 0, NO) != SUCCESS)
				break;
			/* ��� */
			if(offs == 0)
				Ret = NODE_FILE;
			else
			{
				if((FindField(Str, Buf, 1, NO) == SUCCESS) &&
				   (strcmp(Buf, "DR") == 0))
					Ret = NODE_DIR;
				else
					Ret = NODE_FILE;
			}
			break;

		case LIST_ALLIED :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_SIZE);

			/* ���t */
			FindField(Str, Buf, 3, NO);
			GetMonth(Buf, &sTime.wMonth, &sTime.wDay);	/* wDay�͏��0 */
			FindField(Str, Buf, 4, NO);
			sTime.wDay = atoi(Buf);
			FindField(Str, Buf, 6, NO);
			sTime.wYear = atoi(Buf);

			/* ���� */
			FindField(Str, Buf, 5, NO);
			sTime.wHour = atoi(Buf);
			sTime.wMinute = atoi(Buf+3);
			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �T�C�Y */
			FindField(Str, Buf, 0, NO);
			*Size = _atoi64(Buf);

			/* ���O */
			if(FindField(Str, Fname, 1, NO) == SUCCESS)
			{
				Ret = NODE_FILE;
				if(Buf[0] == '<')
					Ret = NODE_DIR;
			}
			break;

		case LIST_OS9 :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_SIZE);

			/* ���t */
			FindField(Str, Buf, 1, NO);
			sTime.wYear = Assume1900or2000(atoi(Buf));
			sTime.wMonth = atoi(Buf + 3);
			sTime.wDay = atoi(Buf + 6);
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* ���� */
			FindField(Str, Buf, 2, NO);
			sTime.wHour = atoi_n(Buf, 2);
			sTime.wMinute = atoi(Buf+2);
			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* �T�C�Y */
			FindField(Str, Buf, 5, NO);
			*Size = _atoi64(Buf);

			/* �I�[�i�� */
			FindField(Str, Buf, 0, NO);
			strncpy(Owner, Buf, OWNER_NAME_LEN);

			/* �I�[�i�� */
			FindField(Str, Buf, 3, NO);

			/* ���O */
			if(FindField(Str, Fname, 6, NO) == SUCCESS)
			{
				if((Buf[0] == 'd') || (Buf[0] == 'D'))
					Ret = NODE_DIR;
				else
					Ret = NODE_FILE;
			}
			break;

		case LIST_IBM :
			*InfoExist |= FINFO_DATE;


			/* ���t */
			FindField(Str, Buf, 2, NO);
			sTime.wYear = atoi(Buf);
			sTime.wMonth = atoi(Buf + 5);
			sTime.wDay = atoi(Buf + 8);
			sTime.wHour = 0;
			sTime.wMinute = 0;
			sTime.wSecond = 0;
			sTime.wMilliseconds = 0;
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* ���O */
			FindField(Str, Buf, 8, NO);
			if(FindField(Str, Fname, 9, NO) == SUCCESS)
			{
				if(strcmp(Buf, "PO") == 0)
					Ret = NODE_DIR;
				else if(strcmp(Buf, "PS") == 0)
					Ret = NODE_FILE;
			}
			break;

		case LIST_AGILENT :
			*InfoExist |= (FINFO_SIZE | FINFO_ATTR);

			/* �I�[�i�� */
			FindField(Str, Buf, 2, NO);
			strncpy(Owner, Buf, OWNER_NAME_LEN);

			/* �T�C�Y */
			FindField(Str, Buf, 4, NO);
			*Size = _atoi64(Buf);

			/* ���� */
			FindField(Str, Buf, 0, NO);
			*Attr = AttrString2Value(Buf+1);

			/* ���O */
			if(FindField(Str, Fname, 5, YES) == SUCCESS)
			{
				Ret = NODE_FILE;
				if(strchr("dl", Buf[0]) != NULL)
					Ret = NODE_DIR;
			}
			break;

		case LIST_SHIBASOKU :
			*InfoExist |= (FINFO_TIME | FINFO_DATE | FINFO_SIZE);

			/* �T�C�Y */
			FindField(Str, Buf, 0, NO);
			if(IsDigit(Buf[0]))
			{
				*Size = _atoi64(Buf);

				/* ���t */
				FindField(Str, Buf, 1, NO);
				Buf[3] = '\0';
				GetMonth(Buf, &sTime.wMonth, &sTime.wDay);
				sTime.wDay = atoi(Buf+4);
				sTime.wYear = atoi(Buf+7);

				/* ���� */
				FindField(Str, Buf, 2, NO);
				sTime.wHour = atoi(Buf);
				sTime.wMinute = atoi(Buf+3);
				sTime.wSecond = 0;
				sTime.wMilliseconds = 0;
				SystemTimeToFileTime(&sTime, Time);
				SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

				/* ���O */
				FindField(Str, Fname, 3, NO);

				/* ��� */
				Ret = NODE_FILE;
				if(FindField(Str, Buf, 4, NO) == SUCCESS)
				{
					if(strcmp(Buf, "<DIR>") == 0)
						Ret = NODE_DIR;
				}
			}
			break;

		case LIST_UNIX_10 :
		case LIST_UNIX_11 :
		case LIST_UNIX_12 :
		case LIST_UNIX_13 :
		case LIST_UNIX_14 :
		case LIST_UNIX_15 :
		case LIST_UNIX_20 :
		case LIST_UNIX_21 :
		case LIST_UNIX_22 :
		case LIST_UNIX_23 :
		case LIST_UNIX_24 :
		case LIST_UNIX_25 :
		case LIST_UNIX_50 :
		case LIST_UNIX_51 :
		case LIST_UNIX_54 :
		case LIST_UNIX_60 :
		case LIST_UNIX_61 :
		case LIST_UNIX_62 :
		case LIST_UNIX_63 :
		case LIST_UNIX_64 :
		case LIST_UNIX_65 :
		case LIST_UNIX_70 :
		case LIST_UNIX_71 :
		case LIST_UNIX_72 :
		case LIST_UNIX_73 :
		case LIST_UNIX_74 :
		case LIST_UNIX_75 :
// MELCOM�̓r�b�g�t���O�ɂȂ��Ă���
//		case LIST_MELCOM :
		default:
			/* offs�̓T�C�Y�̈ʒu, offs=0�̓J����4 */
			offs = 0;
			if((ListType == LIST_UNIX_12) ||
			   (ListType == LIST_UNIX_13) ||
			   (ListType == LIST_UNIX_15) ||
			   (ListType == LIST_UNIX_20) ||
			   (ListType == LIST_UNIX_21) ||
			   (ListType == LIST_UNIX_24))
				offs = -1;

			if((ListType == LIST_UNIX_22) ||
			   (ListType == LIST_UNIX_23) ||
			   (ListType == LIST_UNIX_25) ||
			   (ListType == LIST_UNIX_50) ||
			   (ListType == LIST_UNIX_51) ||
			   (ListType == LIST_UNIX_54))
				offs = -2;

			if((ListType == LIST_UNIX_60) ||
			   (ListType == LIST_UNIX_61) ||
			   (ListType == LIST_UNIX_64))
				offs = 2;

			if((ListType == LIST_UNIX_62) ||
			   (ListType == LIST_UNIX_63) ||
			   (ListType == LIST_UNIX_65) ||
			   (ListType == LIST_UNIX_70) ||
			   (ListType == LIST_UNIX_71) ||
			   (ListType == LIST_UNIX_74))
				offs = 1;

			/* offs2�͎���(�������͔N)�̈ʒu */
			offs2 = 0;
			if((ListType == LIST_UNIX_11) ||
			   (ListType == LIST_UNIX_13) ||
			   (ListType == LIST_UNIX_21) ||
			   (ListType == LIST_UNIX_23) ||
			   (ListType == LIST_UNIX_51) ||
			   (ListType == LIST_UNIX_61) ||
			   (ListType == LIST_UNIX_63) ||
			   (ListType == LIST_UNIX_71) ||
			   (ListType == LIST_UNIX_73))
				offs2 = -1;

			/* offs3�̓I�[�i���̈ʒu */
			offs3 = 0;
			if((ListType == LIST_UNIX_12) ||
			   (ListType == LIST_UNIX_13) ||
			   (ListType == LIST_UNIX_15) ||
			   (ListType == LIST_UNIX_22) ||
			   (ListType == LIST_UNIX_23) ||
			   (ListType == LIST_UNIX_25) ||
			   (ListType == LIST_UNIX_50) ||
			   (ListType == LIST_UNIX_51) ||
			   (ListType == LIST_UNIX_62) ||
			   (ListType == LIST_UNIX_63) ||
			   (ListType == LIST_UNIX_65) ||
			   (ListType == LIST_UNIX_72) ||
			   (ListType == LIST_UNIX_73) ||
			   (ListType == LIST_UNIX_75))
				offs3 = -1;

			Flag2 = 0;
			if((ListType == LIST_UNIX_14) ||
			   (ListType == LIST_UNIX_15) ||
			   (ListType == LIST_UNIX_24) ||
			   (ListType == LIST_UNIX_25) ||
			   (ListType == LIST_UNIX_54) ||
			   (ListType == LIST_UNIX_64) ||
			   (ListType == LIST_UNIX_65) ||
			   (ListType == LIST_UNIX_74) ||
			   (ListType == LIST_UNIX_75))
				Flag2 = 1;

			*InfoExist |= (FINFO_DATE | FINFO_SIZE | FINFO_ATTR);

			/* ���� */
			FindField(Str, Buf, 0, NO);
			*Attr = AttrString2Value(Buf+1);

			/* �I�[�i�� */
			FindField(Str, Buf, 2+offs3, NO);
			strncpy(Owner, Buf, OWNER_NAME_LEN);

			/* �T�C�Y */
			FindField(Str, Buf, 4+offs, NO);
			Pos = Buf;
			if((*Pos != NUL) && (IsDigit(*Pos) == 0))
			{
				Pos = strchr(Pos, NUL) - 1;
				for(; Pos > Buf; Pos--)
				{
					if(IsDigit(*Pos) == 0)
					{
						Pos++;
						break;
					}
				}
			}
			*Size = _atoi64(Pos);

			if(Flag2 == 0)
			{
				/* �����^���t */
				GetLocalTime(&sTime);
				memcpy(&sTimeNow, &sTime, sizeof(SYSTEMTIME));
				sTime.wSecond = 0;
				sTime.wMilliseconds = 0;

				FindField(Str, Buf, 5+offs, NO);
				/* ���t�� yy/mm/dd �̏ꍇ�ɑΉ� */
				if(GetYearMonthDay(Buf, &sTime.wYear, &sTime.wMonth, &sTime.wDay) == SUCCESS)
				{
					sTime.wYear = Assume1900or2000(sTime.wYear);

					FindField(Str, Buf, 7+offs+offs2, NO);
					if(GetHourAndMinute(Buf, &sTime.wHour, &sTime.wMinute) == SUCCESS)
						*InfoExist |= FINFO_TIME;
				}
				else
				{
					GetMonth(Buf, &sTime.wMonth, &sTime.wDay);
					if(offs2 == 0)
					{
						FindField(Str, Buf, 6+offs, NO);
						sTime.wDay = atoi(Buf);
					}

					FindField(Str, Buf, 7+offs+offs2, NO);
					if(GetHourAndMinute(Buf, &sTime.wHour, &sTime.wMinute) == FAIL)
					{
						sTime.wYear = atoi(Buf);
					}
					else
					{
						*InfoExist |= FINFO_TIME;

						/* �N���Ȃ� */
						/* ���݂̓��t���琄�� */
						if((sTimeNow.wMonth == 12) && (sTime.wMonth == 1))
							sTime.wYear++;
						else if(sTimeNow.wMonth+1 == sTime.wMonth)
							/* nothing */;
						else if(sTimeNow.wMonth < sTime.wMonth)
							sTime.wYear--;


//#################
						/* ���N�̍����ȍ~�̃t�@�C���́A���͋��N�̃t�@�C�� */
						if((sTime.wYear == sTimeNow.wYear) &&
						   ((sTime.wMonth > sTimeNow.wMonth) ||
							((sTime.wMonth == sTimeNow.wMonth) && (sTime.wDay > sTimeNow.wDay))))
						{
							sTime.wYear--;
						}
					}
				}
			}
			else
			{
				/* LIST_UNIX_?4, LIST_UNIX_?5 �̎� */
				FindField(Str, Buf, 5+offs, NO);
				sTime.wYear = atoi(Buf);
				FindField(Str, Buf, 6+offs, NO);
				sTime.wMonth = atoi(Buf);
				FindField(Str, Buf, 7+offs, NO);
				sTime.wDay = atoi(Buf);
				sTime.wHour = 0;
				sTime.wMinute = 0;
				sTime.wSecond = 0;
				sTime.wMilliseconds = 0;
			}
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

			/* ���O */
			if(FindField(Str, Fname, 8+offs+offs2, YES) == SUCCESS)
			{
				Flag = 'B';
				if(OrgListType & LIST_MELCOM)
				{
					Flag = Fname[14];
					Fname[14] = NUL;
					RemoveTailingSpaces(Fname);
				}
				else
				{
					if((Pos = strstr(Fname, " -> ")) != NULL)
						*Pos = NUL;
				}

				if(strchr("dl", *Str) != NULL)
				{
					if((_mbscmp(_mbsninc(Fname, _mbslen(Fname) - 1), "/") == 0) ||
					   (_mbscmp(_mbsninc(Fname, _mbslen(Fname) - 1), "\\") == 0))
					{
						*(Fname + strlen(Fname) - 1) = NUL;
					}
					Ret = NODE_DIR;
					if(*Str == 'l')
						*Link = YES;
				}
				else if(strchr("-+f", *Str) != NULL)
					Ret = NODE_FILE;

				if((Ret == NODE_FILE) && (Flag != 'B'))
					Ret = NODE_NONE;
			}
			break;
	}

	if((Ret != NODE_NONE) && (strlen(Fname) > 0))
	{
		if(CheckSpecialDirName(Fname) == YES)
			Ret = NODE_NONE;
		else
			ChangeFnameRemote2Local(Fname, FMAX_PATH);
	}
	return(Ret);
}


/*----- �w��̔ԍ��̃t�B�[���h�����߂� ----------------------------------------
*
*	Parameter
*		char *Str : ������
*		char *Buf : ������̃R�s�[��
*		int Num : �t�B�[���h�ԍ�
*		int ToLast : ������̍Ō�܂ŃR�s�[ (YES/NO)
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

static int FindField(char *Str, char *Buf, int Num, int ToLast)
{
	char *Pos;
	int Sts;

	Sts = FAIL;
	*Buf = NUL;
	if(Num >= 0)
	{
		while(*Str == ' ')
			Str++;

		for(; Num > 0; Num--)
		{
			if((Str = strchr(Str, ' ')) != NULL)
			{
				while(*Str == ' ')
				{
					if(*Str == NUL)
					{
						Str = NULL;
						break;
					}
					Str++;
				}
			}
			else
				break;
		}
	}

	if(Str != NULL)
	{
		if((ToLast == YES) || ((Pos = strchr(Str, ' ')) == NULL))
			strcpy(Buf, Str);
		else
		{
			strncpy(Buf, Str, Pos - Str);
			*(Buf + (Pos - Str)) = NUL;
		}
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �����񂩂猎�����߂� --------------------------------------------------
*
*	Parameter
*		char *Str : ������
*		WORD *Month : �� (0=����\��������ł͂Ȃ�)
*		WORD *Day : �� (0=���͊܂܂�Ă��Ȃ�)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void GetMonth(char *Str, WORD *Month, WORD *Day)
{
	static const char DateStr[] = { "JanFebMarAprMayJunJulAugSepOctNovDec" };
	char *Pos;

	*Month = 0;
	*Day = 0;

	if(IsDigit(*Str) == 0)
	{
		_strlwr(Str);
		*Str = toupper(*Str);
		if((Pos = strstr(DateStr, Str)) != NULL)
			*Month = ((Pos - DateStr) / 3) + 1;
	}
	else
	{
		/* �u10���v�̂悤�ȓ��t��Ԃ����̂����� */
		/* ������JIS�̎��̂�SJIS�ɕϊ� */
		ConvAutoToSJIS(Str, KANJI_NOCNV);

		Pos = Str;
		while(*Pos != NUL)
		{
			if(!IsDigit(*Pos))
			{
				if((_mbsncmp(Pos, "��", 1) == 0) ||
				   (memcmp(Pos, "\xB7\xEE", 2) == 0) ||	/* EUC�́u���v */
				   (memcmp(Pos, "\xD4\xC2", 2) == 0))	/* GB�R�[�h�́u���v */
				{
					Pos += 2;
					*Month = atoi(Str);
					if((*Month < 1) || (*Month > 12))
						*Month = 0;
					else
					{
						/* �u10��11���v�̂悤�ɓ����������Ă��鎖������ */
						if(*Pos != NUL)
						{
							*Day = atoi(Pos);
							if((*Day < 1) || (*Day > 31))
								*Day = 0;
						}
					}
				}
				else if(_mbsncmp(Pos, "/", 1) == 0)
				{
					/* �u10/�v�̂悤�ȓ��t��Ԃ����̂����� */
					Pos += 1;
					*Month = atoi(Str);
					if((*Month < 1) || (*Month > 12))
						*Month = 0;
					else
					{
						/* �u10/11�v�̂悤�ɓ����������Ă��鎖������ */
						if(*Pos != NUL)
						{
							*Day = atoi(Pos);
							if((*Day < 1) || (*Day > 31))
								*Day = 0;
						}
					}
				}
				break;
			}
			Pos++;
		}
	}
	return;
}


/*----- �����񂩂�N���������߂� ----------------------------------------------
*
*	Parameter
*		char *Str : ������
*		WORD *Year : �N
*		WORD *Month : ��
*		WORD *Day : ��
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL=���t��\�������ł͂Ȃ�)
*
*	Note
*		�ȉ��̌`�����T�|�[�g
*			01/07/25
*		FAIL��Ԃ����� *Year = 0; *Month = 0; *Day = 0
*----------------------------------------------------------------------------*/
static int GetYearMonthDay(char *Str, WORD *Year, WORD *Month, WORD *Day)
{
	int Sts;

	Sts = FAIL;
	if(strlen(Str) == 8)
	{
		if(IsDigit(Str[0]) && IsDigit(Str[1]) && !IsDigit(Str[2]) &&
		   IsDigit(Str[3]) && IsDigit(Str[4]) && !IsDigit(Str[5]) &&
		   IsDigit(Str[6]) && IsDigit(Str[7]))
		{
			*Year = atoi(&Str[0]);
			*Month = atoi(&Str[3]);
			*Day = atoi(&Str[6]);
			Sts = SUCCESS;
		}
	}
	return(Sts);
}


/*----- �����񂩂玞�������o�� ----------------------------------------------
*
*	Parameter
*		char *Str : ������
*		WORD *Hour : ��
*		WORD *Minute : ��
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL=������\�������ł͂Ȃ�)
*
*	Note
*		�ȉ��̌`�����T�|�[�g
*			HH:MM
*			HH��MM��
*		FAIL��Ԃ����� *Hour = 0; *Minute = 0
*----------------------------------------------------------------------------*/

static int GetHourAndMinute(char *Str, WORD *Hour, WORD *Minute)
{
	int Ret;
	char *Pos;

	Ret = FAIL;
	if((_mbslen(Str) >= 3) && (isdigit(Str[0]) != 0))
	{
		*Hour = atoi(Str);
		if(*Hour <= 24)
		{
			if((Pos = _mbschr(Str, ':')) != NULL)
			{
				Pos++;
				if(IsDigit(*Pos) != 0)
				{
					*Minute = atoi(Pos);
					if(*Minute < 60)
						Ret = SUCCESS;
				}
			}
			else
			{
				/* ������JIS�̎��̂�SJIS�ɕϊ� */
				ConvAutoToSJIS(Str, KANJI_NOCNV);

				Pos = Str;
				while(*Pos != NUL)
				{
					if(IsDigit(*Pos) == 0)
					{
						if((_mbsncmp(Pos, "��", 1) == 0) ||
						   (memcmp(Pos, "\xBB\xFE", 2) == 0))	/* EUC�́u���v */
						{
							Pos += 2;
							if(*Pos != NUL)
							{
								*Minute = atoi(Pos);
								if(*Minute < 60)
									Ret = SUCCESS;
							}
						}
						break;
					}
					Pos++;
				}
			}
		}
	}
	else if((_stricmp(Str, "a:m") == 0) || (_stricmp(Str, "p:m") == 0))
	{
		*Hour = 0;
		*Minute = 0;
		Ret = SUCCESS;
	}

	if(Ret == FAIL)
	{
		*Hour = 0;
		*Minute = 0;
	}
	return(Ret);
}


/*----- VAX VMS�̓��t�����񂩂���t�����o�� ---------------------------------
*
*	Parameter
*		char *Str : ������
*		WORD *Year : �N
*		WORD *Month : ��
*		WORD *Day : ��
*
*	Return Value
*		int �X�e�[�^�X (SUCCESS/FAIL=���t��\�������ł͂Ȃ�)
*
*	Note
*		�ȉ��̌`�����T�|�[�g
*			18-SEP-1998
*		FAIL��Ԃ����� *Year = 0; *Month = 0; *Day = 0
*----------------------------------------------------------------------------*/

static int GetVMSdate(char *Str, WORD *Year, WORD *Month, WORD *Day)
{
	char *Pos;
	int Ret;
	WORD Tmp;
	char Buf[4];

	Ret = FAIL;
	*Day = atoi(Str);
	if((Pos = strchr(Str, '-')) != NULL)
	{
		Pos++;
		strncpy(Buf, Pos, 3);
		Buf[3] = NUL;
		GetMonth(Buf, Month, &Tmp);
		if((Pos = strchr(Pos, '-')) != NULL)
		{
			Pos++;
			*Year = atoi(Pos);
			Ret = SUCCESS;
		}
	}

	if(Ret == FAIL)
	{
		*Year = 0;
		*Month = 0;
		*Day = 0;
	}
	return(Ret);
}


/*----- 1900�N�ォ2000�N�ォ�����߂� ------------------------------------------
*
*	Parameter
*		int Year : �N�i�Q���j
*
*	Return Value
*		int �N
*----------------------------------------------------------------------------*/

int Assume1900or2000(int Year)
{
	if(Year >= 60)
		Year += 1900;
	else
		Year += 2000;
	return(Year);
}



/*----- "."��".."���ǂ�����Ԃ� -----------------------------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*
*	Return Value
*		int �X�e�[�^�X (YES="."��".."�̂ǂ��炩/NO)
*----------------------------------------------------------------------------*/

static int CheckSpecialDirName(char *Fname)
{
	int Sts;

	Sts = NO;
	if((strcmp(Fname, ".") == 0) || (strcmp(Fname, "..") == 0))
		Sts = YES;

	return(Sts);
}


/*----- �t�B���^�Ɏw�肳�ꂽ�t�@�C�������ǂ�����Ԃ� --------------------------
*
*	Parameter
*		char Fname : �t�@�C����
*		int Type : �t�@�C���̃^�C�v (NODE_xxx)
*
*	Return Value
*		int �X�e�[�^�X
*			YES/NO=�\�����Ȃ�
*
*	Note
*		�t�B���^������͈ȉ��̌`��
*			*.txt;*.log
*----------------------------------------------------------------------------*/

static int AskFilterStr(char *Fname, int Type)
{
	int Ret;
	char *Tbl;
	char *Pos;
	char Tmp[FILTER_EXT_LEN+1];

	Tbl = FilterStr;
	Ret = YES;
	if((strlen(Tbl) > 0) && (Type == NODE_FILE))
	{
		Ret = NO;
		while((Tbl != NULL) && (*Tbl != NUL))
		{
			while(*Tbl == ';')
				Tbl++;
			if(*Tbl == NUL)
				break;

			strcpy(Tmp, Tbl);
			if((Pos = strchr(Tmp, ';')) != NULL)
				*Pos = NUL;

			if(CheckFname(Fname, Tmp) == SUCCESS)
			{
				Ret = YES;
				break;
			}

			Tbl = strchr(Tbl, ';');
		}
	}
	return(Ret);
}


/*----- �t�B���^��ݒ肷�� ----------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetFilter(int *CancelCheckWork)
{
 	if(DialogBox(GetFtpInst(), MAKEINTRESOURCE(filter_dlg), GetMainHwnd(), FilterWndProc) == YES)
	{
		DispWindowTitle();
		GetLocalDirForWnd();
		GetRemoteDirForWnd(CACHE_LASTREAD, CancelCheckWork);
	}
	return;
}


/*----- �t�B���^���̓_�C�A���O�̃R�[���o�b�N ----------------------------------
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

static BOOL CALLBACK FilterWndProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, FILTER_STR, EM_LIMITTEXT, FILTER_EXT_LEN+1, 0);
			SendDlgItemMessage(hDlg, FILTER_STR, WM_SETTEXT, 0, (LPARAM)FilterStr);
			return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
					SendDlgItemMessage(hDlg, FILTER_STR, WM_GETTEXT, FILTER_EXT_LEN, (LPARAM)FilterStr);
					EndDialog(hDlg, YES);
					break;

				case IDCANCEL :
					EndDialog(hDlg, NO);
					break;

				case FILTER_NOR :
					strcpy(FilterStr, "*");
					EndDialog(hDlg, YES);
					break;

				case IDHELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000021);
					break;
			}
            return(TRUE);
	}
	return(FALSE);
}





static int atoi_n(const char *Str, int Len)
{
	char *Tmp;
	int Ret;

	Ret = 0;
	if((Tmp = malloc(Len+1)) != NULL)
	{
		memset(Tmp, 0, Len+1);
		strncpy(Tmp, Str, Len);
		Ret = atoi(Tmp);
		free(Tmp);
	}
	return(Ret);
}




