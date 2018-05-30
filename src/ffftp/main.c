/*=============================================================================
*
*									�e�e�e�s�o
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
#include "aes.h"

#include <htmlhelp.h>
#include "helpid.h"


#define RESIZE_OFF		0		/* �E�C���h�E�̋�؂�ʒu�ύX���Ă��Ȃ� */
#define RESIZE_ON		1		/* �E�C���h�E�̋�؂�ʒu�ύX�� */
#define RESIZE_PREPARE	2		/* �E�C���h�E�̋�؂�ʒu�ύX�̏��� */

#define RESIZE_HPOS		0		/* ���[�J���|�z�X�g�Ԃ̋�؂�ʒu�ύX */
#define RESIZE_VPOS		1		/* ���X�g�|�^�X�N�Ԃ̋�؂�ʒu�̕ύX */


/*===== �v���g�^�C�v =====*/

static int InitApp(LPSTR lpszCmdLine, int cmdShow);
static int MakeAllWindows(int cmdShow);
static void DeleteAllObject(void);
static LRESULT CALLBACK FtpWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void StartupProc(char *Cmd);
static int AnalyzeComLine(char *Str, int *AutoConnect, int *CmdOption, char *unc, int Max);
static int CheckIniFileName(char *Str, char *Ini);
static int CheckMasterPassword(char *Str, char *Ini);
static int GetTokenAfterOption(char *Str, char *Result, const char* Opt1, const char* Opt2 );
static char *GetToken(char *Str, char *Buf);
static void ExitProc(HWND hWnd);
static void ChangeDir(int Win, char *Path);
static void ResizeWindowProc(void);
static void CalcWinSize(void);
// static void AskWindowPos(HWND hWnd);
static void CheckResizeFrame(WPARAM Keys, int x, int y);
static void DispDirInfo(void);
static void DeleteAlltempFile(void);
static BOOL CALLBACK AboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static int EnterMasterPasswordAndSet( int Res, HWND hWnd );

/*===== ���[�J���ȃ��[�N =====*/

static const char FtpClassStr[] = "FFFTPWin";

static HINSTANCE hInstFtp;
static HWND hWndFtp = NULL;
static HWND hWndCurFocus = NULL;

static HACCEL Accel;
static HBRUSH RootColorBrush = NULL;

static int Resizing = RESIZE_OFF;
static int ResizePos;
static HCURSOR hCursor;

int ClientWidth;
static int ClientHeight;
int SepaWidth;
int RemoteWidth;
int ListHeight;

static TEMPFILELIST *TempFiles = NULL;

static int SaveExit = YES;
static int AutoExit = NO;

static char HelpPath[FMAX_PATH+1];
static char IniPath[FMAX_PATH+1];
static int ForceIni = NO;

TRANSPACKET MainTransPkt;		/* �t�@�C���]���p�p�P�b�g */
								/* ������g���ē]�����s���ƁA�c�[���o�[�̓]�� */
								/* ���~�{�^���Œ��~�ł��� */

char TitleHostName[HOST_ADRS_LEN+1];
char FilterStr[FILTER_EXT_LEN+1] = { "*" };

int CancelFlg;

static int SuppressRefresh = 0;

static DWORD dwCookie;


/*===== �O���[�o���ȃ��[�N =====*/

HWND hHelpWin = NULL;

/* �ݒ�l */
int WinPosX = CW_USEDEFAULT;
int WinPosY = 0;
int WinWidth = 630;
int WinHeight = 393;
int LocalWidth = 309;
int TaskHeight = 50;
int LocalTabWidth[4] = { 120, 90, 60, 37 };
int RemoteTabWidth[6] = { 120, 90, 60, 37, 60, 60 };
char UserMailAdrs[USER_MAIL_LEN+1] = { "who@example.com" };
char ViewerName[VIEWERS][FMAX_PATH+1] = { { "notepad" }, { "" }, { "" } };
HFONT ListFont = NULL;
LOGFONT ListLogFont;
int LocalFileSort = SORT_NAME;
int LocalDirSort = SORT_NAME;
int RemoteFileSort = SORT_NAME;
int RemoteDirSort = SORT_NAME;
int TransMode = TYPE_X;
int ConnectOnStart = YES;
int DebugConsole = NO;
int SaveWinPos = NO;
char AsciiExt[ASCII_EXT_LEN+1] = { "*.txt\0*.html\0*.htm\0*.cgi\0*.pl\0" };
int RecvMode = TRANS_DLG;
int SendMode = TRANS_DLG;
int MoveMode = MOVE_DLG;
int ListType = LVS_REPORT;
int CacheEntry = 10;
int CacheSave = NO;
char DefaultLocalPath[FMAX_PATH+1] = { "" };
int SaveTimeStamp = YES;
int FindMode = 0;
int DotFile = YES;
int DclickOpen = YES;
int ConnectAndSet = YES;
SOUNDFILE Sound[SOUND_TYPES] = { { NO, "" }, { NO, "" }, { NO, "" } };
int FnameCnv = FNAME_NOCNV;
int TimeOut = 90;
int RmEOF = NO;
int RegType = REGTYPE_REG;
char FwallHost[HOST_ADRS_LEN+1] = { "" };
char FwallUser[USER_NAME_LEN+1] = { "" };
char FwallPass[PASSWORD_LEN+1] = { "" };
int FwallPort = PORT_NOR;
int FwallType = 1;
int FwallDefault = NO;
int FwallSecurity = SECURITY_AUTO;
int FwallResolv = NO;
int FwallLower = NO;
int FwallDelimiter = '@';
int PasvDefault = NO;
char MirrorNoTrn[MIRROR_LEN+1] = { "*.bak\0" };
char MirrorNoDel[MIRROR_LEN+1] = { "" };
int MirrorFnameCnv = NO;
int SplitVertical = YES;
int RasClose = NO;
int RasCloseNotify = YES;
int FileHist = 5;
char DefAttrList[DEFATTRLIST_LEN+1] = { "" };
SIZE HostDlgSize = { -1, -1 };
SIZE BmarkDlgSize = { -1, -1 };
SIZE MirrorDlgSize = { -1, -1 };
int Sizing = SW_RESTORE;
int SortSave = NO;
char TmpPath[FMAX_PATH+1];
int QuickAnonymous = YES;
int PassToHist = YES;
int VaxSemicolon = NO;
int SendQuit = NO;
int NoRasControl = NO;
int SuppressSave = NO;
int DispIgnoreHide = NO;
int DispDrives = NO;
int MirUpDelNotify = YES; 
int MirDownDelNotify = YES; 
int FolderAttr = NO;
int FolderAttrNum = 777;




/*----- ���C�����[�`�� --------------------------------------------------------
*
*	Parameter
*		HINSTANCE hInstance : ���̃A�v���P�[�V�����̂��̃C���X�^���X�̃n���h��
*		HINSTANCE hPrevInstance : ���̃A�v���P�[�V�����̒��O�̃C���X�^���X�̃n���h��
*		LPSTR lpszCmdLine : �A�v���P�[�V�������N�������Ƃ��̃R�}���h���C�������������O�|�C���^
*		int cmdShow : �ŏ��ɕ\������E�C���h�E�̌`���B
*
*	Return Value
*		int �Ō�̃��b�Z�[�W��wParam
*----------------------------------------------------------------------------*/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int cmdShow)
{
    MSG Msg;
	int Ret;
	BOOL Sts;

	// yutaka
	if(OleInitialize(NULL) != S_OK){
		MessageBox(NULL, MSGJPN298, "FFFTP", MB_OK | MB_ICONERROR);
		return 0;
	}

	InitCommonControls();

	Ret = FALSE;
	hWndFtp = NULL;
	hInstFtp = hInstance;
	if(InitApp(lpszCmdLine, cmdShow) == SUCCESS)
	{
		for(;;)
		{
			Sts = GetMessage(&Msg, NULL, 0, 0);
			if((Sts == 0) || (Sts == -1))
				break;

			if(!HtmlHelp(NULL, NULL, HH_PRETRANSLATEMESSAGE, (DWORD)&Msg))
			{ 
				/* �f�B���N�g�����̕\���R���{�{�b�N�X��BS��RET�������悤�� */
				/* �R���{�{�b�N�X���ł̓A�N�Z�����[�^�𖳌��ɂ��� */
				if((Msg.hwnd == GetLocalHistEditHwnd()) ||
				   (Msg.hwnd == GetRemoteHistEditHwnd()) ||
				   ((hHelpWin != NULL) && (GetAncestor(Msg.hwnd, GA_ROOT) == hHelpWin)) ||
				   GetHideUI() == YES ||
				   (TranslateAccelerator(hWndFtp, Accel, &Msg) == 0))
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
			}
		}
		Ret = Msg.wParam;
	}
    UnregisterClass(FtpClassStr, hInstFtp);
	OleUninitialize();
	return(Ret);
}


/*----- �A�v���P�[�V�����̏����ݒ� --------------------------------------------
*
*	Parameter
*		HINSTANCE hInstance : ���̃A�v���P�[�V�����̂��̃C���X�^���X�̃n���h��
*		HINSTANCE hPrevInstance : ���̃A�v���P�[�V�����̒��O�̃C���X�^���X�̃n���h��
*		LPSTR lpszCmdLine : �A�v���P�[�V�������N�������Ƃ��̃R�}���h���C�������������O�|�C���^
*		int cmdShow : �ŏ��ɕ\������E�C���h�E�̌`���B
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

static int InitApp(LPSTR lpszCmdLine, int cmdShow)
{
	int sts;
	int Err;
	WSADATA WSAData;
	char PwdBuf[FMAX_PATH+1];
	int useDefautPassword = 0; /* �x�����\���p */
	int masterpass;

	sts = FAIL;

	aes_init();
	srand(GetTickCount());
	
	HtmlHelp(NULL, NULL, HH_INITIALIZE, (DWORD)&dwCookie);

	SaveUpdateBellInfo();

	if((Err = WSAStartup((WORD)0x0202, &WSAData)) != 0)
		MessageBox(NULL, ReturnWSError(Err), "FFFTP - Startup", MB_OK);
	else
	{
		Accel = LoadAccelerators(hInstFtp, MAKEINTRESOURCE(ffftp_accel));

		GetTempPath(FMAX_PATH, TmpPath);

		GetModuleFileName(NULL, HelpPath, FMAX_PATH);
		strcpy(GetFileName(HelpPath), "ffftp.chm");

		if(CheckIniFileName(lpszCmdLine, IniPath) == 0)
		{
			GetModuleFileName(NULL, IniPath, FMAX_PATH);
			strcpy(GetFileName(IniPath), "ffftp.ini");
		}
		else
		{
			ForceIni = YES;
			RegType = REGTYPE_INI;
		}

//		AllocConsole();

		/* 2010.02.01 genta �}�X�^�[�p�X���[�h����͂�����
		  -z �I�v�V����������Ƃ��͍ŏ������X�L�b�v
		  -z �I�v�V�������Ȃ��Ƃ��́C�f�t�H���g�p�X���[�h���܂�����
		  LoadRegistory()����
		  �p�X���[�h���s��v�Ȃ�ē��͂��邩�q�˂�D
		  (�j�����Ă����ꍇ�͂����Ȃ�)
		*/
		if( CheckMasterPassword(lpszCmdLine, PwdBuf))
		{
			SetMasterPassword( PwdBuf );
			useDefautPassword = 0;
		}
		else {
			/* �p�X���[�h�w�薳�� */
			SetMasterPassword( NULL );
			/* ���̏�ł͕\���ł��Ȃ��̂Ńt���O�������ĂĂ���*/
			useDefautPassword = 2;
		}

		/* �p�X���[�h�`�F�b�N�̂ݎ��{ */
		masterpass = 1;
		while( ValidateMasterPassword() == YES &&
				GetMasterPasswordStatus() == PASSWORD_UNMATCH ){
			
			if( useDefautPassword != 2 ){
				/* �ăg���C���邩�m�F */
				if( MessageBox(NULL, MSGJPN304, "FFFTP", MB_YESNO | MB_ICONEXCLAMATION) == IDNO ){
					useDefautPassword = 0; /* �s��v�Ȃ̂ŁC���͂�f�t�H���g���ǂ����͕�����Ȃ� */
					break;
				}
			}
			
			/* �ē��͂�����*/
			masterpass = EnterMasterPasswordAndSet(masterpasswd_dlg, NULL);
			if( masterpass == 2 ){
				useDefautPassword = 1;
			}
			else if( masterpass == 0 ){
				SaveExit = NO;
				break;
			}
			else {
				useDefautPassword = 0;
			}
		}
		
		if(masterpass != 0)
		{
			LoadRegistory();

			LoadJre();
			if(NoRasControl == NO)
				LoadRasLib();
			LoadKernelLib();

			//�^�C�}�̐��x�����P
			timeBeginPeriod(1);

			CountPrevFfftpWindows();

			if(MakeAllWindows(cmdShow) == SUCCESS)
			{
				hWndCurFocus = GetLocalHwnd();

				if(strlen(DefaultLocalPath) > 0)
					SetCurrentDirectory(DefaultLocalPath);

				SetSortTypeImm(LocalFileSort, LocalDirSort, RemoteFileSort, RemoteDirSort);
				SetTransferTypeImm(TransMode);
				DispTransferType();
				SetHostKanaCnvImm(YES);
				SetHostKanjiCodeImm(KANJI_NOCNV);
				DispListType();
				DispDotFileMode();
				DispSyncMoveMode();

				MakeCacheBuf(CacheEntry);
				if(CacheSave == YES)
					LoadCache();

				if(MakeTransferThread() == SUCCESS)
				{
					DoPrintf("DEBUG MESSAGE ON ! ##");

					DispWindowTitle();
					SetTaskMsg("FFFTP Ver." VER_STR " Copyright(C) 1997-2010 Sota & cooperators.");

					if(ForceIni)
						SetTaskMsg("%s%s", MSGJPN283, IniPath);

					if(IsFolderExist(TmpPath) == NO)
					{
						SetTaskMsg(MSGJPN152, TmpPath);
						GetTempPath(FMAX_PATH, TmpPath);
						SetTaskMsg(MSGJPN153, TmpPath);
					}

					DoPrintf("Tmp =%s", TmpPath);
					DoPrintf("Help=%s", HelpPath);

					DragAcceptFiles(GetRemoteHwnd(), TRUE);
					DragAcceptFiles(GetLocalHwnd(), TRUE);

					SetAllHistoryToMenu();
					GetLocalDirForWnd();
					MakeButtonsFocus();
					DispTransferFiles();

					StartupProc(lpszCmdLine);
					sts = SUCCESS;

					/* �Z�L�����e�B�x�����̕\�� */
					if( useDefautPassword ){
						SetTaskMsg(MSGJPN300);
					}
					
					/* �p�X���[�h�s��v�x�����̕\�� */
					switch( GetMasterPasswordStatus() ){
					case PASSWORD_UNMATCH:
						SetTaskMsg(MSGJPN301);
						break;
					case BAD_PASSWORD_HASH:
						SetTaskMsg(MSGJPN302);
						break;
					default:
						break;
					}
				}
			}
		}
	}

	if(sts == FAIL)
		DeleteAllObject();

	return(sts);
}


/*----- �E�C���h�E���쐬���� --------------------------------------------------
*
*	Parameter
*		int cmdShow : �ŏ��ɕ\������E�C���h�E�̌`���B
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

static int MakeAllWindows(int cmdShow)
{
	RECT Rect1;
	RECT Rect2;
	WNDCLASSEX wClass;
	int Sts;
	int StsTask;
	int StsSbar;
	int StsTbar;
	int StsList;
	int StsLvtips;
	int StsSocket;

	/*===== ���C���E�C���h�E =====*/

	RootColorBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.style         = 0;
	wClass.lpfnWndProc   = FtpWndProc;
	wClass.cbClsExtra    = 0;
	wClass.cbWndExtra    = 0;
	wClass.hInstance     = hInstFtp;
	wClass.hIcon         = LoadIcon(hInstFtp, MAKEINTRESOURCE(ffftp));
	wClass.hCursor       = NULL;
	wClass.hbrBackground = RootColorBrush;
	wClass.lpszMenuName  = (LPSTR)MAKEINTRESOURCE(main_menu);
	wClass.lpszClassName = FtpClassStr;
	wClass.hIconSm       = NULL;
	RegisterClassEx(&wClass);

	if(SaveWinPos == NO)
	{
		WinPosX = CW_USEDEFAULT;
		WinPosY = 0;
	}
	hWndFtp = CreateWindow(FtpClassStr, "FFFTP",
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				WinPosX, WinPosY, WinWidth, WinHeight,
				HWND_DESKTOP, 0, hInstFtp, NULL);

	if(hWndFtp != NULL)
	{
		SystemParametersInfo(SPI_GETWORKAREA, 0, &Rect1, 0);
		GetWindowRect(hWndFtp, &Rect2);
		if(Rect2.bottom > Rect1.bottom)
		{
			Rect2.top = max1(0, Rect2.top - (Rect2.bottom - Rect1.bottom));
			MoveWindow(hWndFtp, Rect2.left, Rect2.top, WinWidth, WinHeight, FALSE);
		}

		/*===== �X�e�C�^�X�o�[ =====*/

		StsSbar = MakeStatusBarWindow(hWndFtp, hInstFtp);

		CalcWinSize();

		/*===== �c�[���o�[ =====*/

		StsTbar = MakeToolBarWindow(hWndFtp, hInstFtp);

		/*===== �t�@�C�����X�g�E�C���h�E =====*/

		StsList = MakeListWin(hWndFtp, hInstFtp);

		/*==== �^�X�N�E�C���h�E ====*/

		StsTask = MakeTaskWindow(hWndFtp, hInstFtp);

		if((cmdShow != SW_MINIMIZE) && (cmdShow != SW_SHOWMINIMIZED) && (cmdShow != SW_SHOWMINNOACTIVE) &&
		   (Sizing == SW_MAXIMIZE))
			cmdShow = SW_MAXIMIZE;

		ShowWindow(hWndFtp, cmdShow);

		/*==== �\�P�b�g�E�C���h�E ====*/

		StsSocket = MakeSocketWin(hWndFtp, hInstFtp);

		StsLvtips = InitListViewTips(hWndFtp, hInstFtp);
	}

	Sts = SUCCESS;
	if((hWndFtp == NULL) ||
	   (StsTbar == FAIL) ||
	   (StsList == FAIL) ||
	   (StsSbar == FAIL) ||
	   (StsTask == FAIL) ||
	   (StsLvtips == FAIL) ||
	   (StsSocket == FAIL))
	{
		Sts = FAIL;
	}

	if(Sts == SUCCESS)
		SetListViewType();

	return(Sts);
}


/*----- �E�C���h�E�̃^�C�g����\������ ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispWindowTitle(void)
{
	char Tmp[HOST_ADRS_LEN+FILTER_EXT_LEN+20];

	if(AskConnecting() == YES)
		sprintf(Tmp, "%s (%s) - FFFTP", TitleHostName, FilterStr);
	else
		sprintf(Tmp, "FFFTP (%s)", FilterStr);

	SetWindowText(GetMainHwnd(), Tmp);
	return;
}


/*----- �S�ẴI�u�W�F�N�g���폜 ----------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DeleteAllObject(void)
{
	DeleteCacheBuf();

//move to WM_DESTROY
	WSACleanup();

//test �V�X�e���C��
//	if(ListFont != NULL)
//		DeleteObject(ListFont);
//	if(RootColorBrush != NULL)
//		DeleteObject(RootColorBrush);

//test �V�X�e���C��
//	DeleteListViewTips();
//	DeleteListWin();
//	DeleteStatusBarWindow();
//	DeleteTaskWindow();
//	DeleteToolBarWindow();
//	DeleteSocketWin();

//move to WM_DESTROY
	if(hWndFtp != NULL)
		DestroyWindow(hWndFtp);

	ReleaseJre();
	ReleaseRasLib();
	ReleaseKernelLib();

	return;
}


/*----- ���C���E�C���h�E�̃E�C���h�E�n���h����Ԃ� ----------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetMainHwnd(void)
{
	return(hWndFtp);
}


/*----- ���݃t�H�[�J�X������E�C���h�E�̃E�C���h�E�n���h����Ԃ� --------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetFocusHwnd(void)
{
	return(hWndCurFocus);
}


/*----- ���݃t�H�[�J�X������E�C���h�E�̂��Z�b�g���� --------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetFocusHwnd(HWND hWnd)
{
	hWndCurFocus = hWnd;
	return;
}


/*----- �v���O�����̃C���X�^���X��Ԃ� ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HINSTANCE �C���X�^���X
*----------------------------------------------------------------------------*/

HINSTANCE GetFtpInst(void)
{
	return(hInstFtp);
}


/*----- ���C���E�C���h�E�̃��b�Z�[�W���� --------------------------------------
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

static LRESULT CALLBACK FtpWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	LPTOOLTIPTEXT lpttt;
	RECT Rect;

	int TmpTransType;

	switch (message)
	{
		case WM_COMMAND :
			switch(LOWORD(wParam))
			{
				case MENU_CONNECT :
					ConnectProc(DLG_TYPE_CON, -1);
					break;

				case MENU_CONNECT_NUM :
					ConnectProc(DLG_TYPE_CON, (int)lParam);
					if(AskConnecting() == YES)
					{
						if(HIWORD(wParam) & OPT_MIRROR)
						{
							if(HIWORD(wParam) & OPT_FORCE)
								MirrorUploadProc(NO);
							else
								MirrorUploadProc(YES);
						}
						else if(HIWORD(wParam) & OPT_MIRRORDOWN)
						{
							if(HIWORD(wParam) & OPT_FORCE)
								MirrorDownloadProc(NO);
							else
								MirrorDownloadProc(YES);
						}
					}
					break;

				case MENU_SET_CONNECT :
					ConnectProc(DLG_TYPE_SET, -1);
					break;

				case MENU_QUICK :
					QuickConnectProc();
					break;

				case MENU_DISCONNECT :
					if(AskTryingConnect() == YES)
						CancelFlg = YES;
					else if(AskConnecting() == YES)
					{
						SaveBookMark();
						SaveCurrentSetToHost();
						DisconnectProc();
					}
					break;

				case MENU_HIST_1 :
				case MENU_HIST_2 :
				case MENU_HIST_3 :
				case MENU_HIST_4 :
				case MENU_HIST_5 :
				case MENU_HIST_6 :
				case MENU_HIST_7 :
				case MENU_HIST_8 :
				case MENU_HIST_9 :
				case MENU_HIST_10 :
				case MENU_HIST_11 :
				case MENU_HIST_12 :
				case MENU_HIST_13 :
				case MENU_HIST_14 :
				case MENU_HIST_15 :
				case MENU_HIST_16 :
				case MENU_HIST_17 :
				case MENU_HIST_18 :
				case MENU_HIST_19 :
				case MENU_HIST_20 :
					HistoryConnectProc(LOWORD(wParam));
					break;

				case MENU_UPDIR :
					if(hWndCurFocus == GetLocalHwnd())
						PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(MENU_LOCAL_UPDIR, 0), 0);
					else
						PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(MENU_REMOTE_UPDIR, 0), 0);
					break;

				case MENU_DCLICK :
					if(hWndCurFocus == GetLocalHwnd())
						DoubleClickProc(WIN_LOCAL, YES, -1);
					else
					{
						SuppressRefresh = 1;
						DoubleClickProc(WIN_REMOTE, YES, -1);
						SuppressRefresh = 0;
					}
					break;

				case MENU_OPEN1 :
					if(hWndCurFocus == GetLocalHwnd())
						DoubleClickProc(WIN_LOCAL, YES, 0);
					else
					{
						SuppressRefresh = 1;
						DoubleClickProc(WIN_REMOTE, YES, 0);
						SuppressRefresh = 0;
					}
					break;

				case MENU_OPEN2 :
					if(hWndCurFocus == GetLocalHwnd())
						DoubleClickProc(WIN_LOCAL, YES, 1);
					else
					{
						SuppressRefresh = 1;
						DoubleClickProc(WIN_REMOTE, YES, 1);
						SuppressRefresh = 0;
					}
					break;

				case MENU_OPEN3 :
					if(hWndCurFocus == GetLocalHwnd())
						DoubleClickProc(WIN_LOCAL, YES, 2);
					else
					{
						SuppressRefresh = 1;
						DoubleClickProc(WIN_REMOTE, YES, 2);
						SuppressRefresh = 0;
					}
					break;

				case MENU_REMOTE_UPDIR :
					SuppressRefresh = 1;
					SetCurrentDirAsDirHist();
					ChangeDir(WIN_REMOTE, "..");
					SuppressRefresh = 0;
					break;

				case MENU_LOCAL_UPDIR :
					SetCurrentDirAsDirHist();
					ChangeDir(WIN_LOCAL, "..");
					break;

				case MENU_REMOTE_CHDIR :
					SuppressRefresh = 1;
					SetCurrentDirAsDirHist();
					ChangeDirDirectProc(WIN_REMOTE);
					SuppressRefresh = 0;
					break;

				case MENU_LOCAL_CHDIR :
					SetCurrentDirAsDirHist();
					ChangeDirDirectProc(WIN_LOCAL);
					break;

				case MENU_DOWNLOAD :
					SetCurrentDirAsDirHist();
					DownLoadProc(NO, NO, NO);
					break;

				case MENU_DOWNLOAD_AS :
					SetCurrentDirAsDirHist();
					DownLoadProc(YES, NO, NO);
					break;

				case MENU_DOWNLOAD_AS_FILE :
					SetCurrentDirAsDirHist();
					DownLoadProc(NO, YES, NO);
					break;

				case MENU_DOWNLOAD_ALL :
					SetCurrentDirAsDirHist();
					DownLoadProc(NO, NO, YES);
					break;

				case MENU_DOWNLOAD_NAME :
					SetCurrentDirAsDirHist();
					InputDownLoadProc();
					break;

				case MENU_UPLOAD :
					SetCurrentDirAsDirHist();
					UpLoadListProc(NO, NO);
					break;

				case MENU_UPLOAD_AS :
					SetCurrentDirAsDirHist();
					UpLoadListProc(YES, NO);
					break;

				case MENU_UPLOAD_ALL :
					SetCurrentDirAsDirHist();
					UpLoadListProc(NO, YES);
					break;

				case MENU_MIRROR_UPLOAD :
					SetCurrentDirAsDirHist();
					MirrorUploadProc(YES);
					break;

				case MENU_MIRROR_DOWNLOAD :
					SetCurrentDirAsDirHist();
					MirrorDownloadProc(YES);
					break;

				case MENU_FILESIZE :
					SetCurrentDirAsDirHist();
					CalcFileSizeProc();
					break;

				case MENU_DELETE :
					SuppressRefresh = 1;
					SetCurrentDirAsDirHist();
					DeleteProc();
					SuppressRefresh = 0;
					break;

				case MENU_RENAME :
					SuppressRefresh = 1;
					SetCurrentDirAsDirHist();
					RenameProc();
					SuppressRefresh = 0;
					break;

				case MENU_MKDIR :
					SuppressRefresh = 1;
					SetCurrentDirAsDirHist();
					MkdirProc();
					SuppressRefresh = 0;
					break;

				case MENU_CHMOD :
					SuppressRefresh = 1;
					ChmodProc();
					SuppressRefresh = 0;
					break;

				case MENU_SOMECMD :
					SuppressRefresh = 1;
					SomeCmdProc();
					SuppressRefresh = 0;
					break;

				case MENU_OPTION :
					SetOption(0);
					if(ListFont != NULL)
					{
						SendMessage(GetLocalHwnd(), WM_SETFONT, (WPARAM)ListFont, MAKELPARAM(TRUE, 0));
						SendMessage(GetRemoteHwnd(), WM_SETFONT, (WPARAM)ListFont, MAKELPARAM(TRUE, 0));
						SendMessage(GetTaskWnd(), WM_SETFONT, (WPARAM)ListFont, MAKELPARAM(TRUE, 0));
					}
					GetLocalDirForWnd();
					DispTransferType();
					CheckHistoryNum(0);
					SetAllHistoryToMenu();
					break;

				case MENU_FILTER :
					SetFilter(&CancelFlg);
					break;

				case MENU_SORT :
					if(SortSetting() == YES)
					{
						LocalFileSort = AskSortType(ITEM_LFILE);
						LocalDirSort = AskSortType(ITEM_LDIR);
						RemoteFileSort = AskSortType(ITEM_RFILE);
						RemoteDirSort = AskSortType(ITEM_RDIR);
						ReSortDispList(WIN_LOCAL, &CancelFlg);
						ReSortDispList(WIN_REMOTE, &CancelFlg);
					}
					break;

				case MENU_EXIT :
					PostMessage(hWnd, WM_CLOSE, 0, 0L);
					break;

				case MENU_AUTO_EXIT :
					if(AutoExit == YES)
						PostMessage(hWnd, WM_CLOSE, 0, 0L);
					break;

				case MENU_ABOUT :
					DialogBox(hInstFtp, MAKEINTRESOURCE(about_dlg), hWnd, AboutDialogProc);
					break;

				case MENU_TEXT :
				case MENU_BINARY :
				case MENU_AUTO :
					SetTransferType(LOWORD(wParam));
					DispTransferType();
					break;

				case MENU_XFRMODE :
					switch(AskTransferType())
					{
						case TYPE_A :
							TmpTransType = MENU_BINARY;
							break;

						case TYPE_I :
							TmpTransType = MENU_AUTO;
							break;

						default :
							TmpTransType = MENU_TEXT;
							break;
					}
					SetTransferType(TmpTransType);
					DispTransferType();
					break;

				case MENU_KNJ_EUC :
				case MENU_KNJ_JIS :
				case MENU_KNJ_NONE :
					SetHostKanjiCode(LOWORD(wParam));
					break;

				case MENU_KANACNV :
					SetHostKanaCnv();
					break;

				case MENU_REFRESH :
					SuppressRefresh = 1;
					GetLocalDirForWnd();
					if(CheckClosedAndReconnect() == SUCCESS)
						GetRemoteDirForWnd(CACHE_REFRESH, &CancelFlg);
					SuppressRefresh = 0;
					break;

				case MENU_LIST :
					ListType = LVS_LIST;
					DispListType();
					SetListViewType();
					break;

				case MENU_REPORT :
					ListType = LVS_REPORT;
					DispListType();
					SetListViewType();
					break;

				case REFRESH_LOCAL :
					GetLocalDirForWnd();
					break;

				case REFRESH_REMOTE :
					SuppressRefresh = 1;
					if(CheckClosedAndReconnect() == SUCCESS)
						GetRemoteDirForWnd(CACHE_REFRESH, &CancelFlg);
					SuppressRefresh = 0;
					break;

				case COMBO_LOCAL :
				case COMBO_REMOTE :
					SuppressRefresh = 1;
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						SetCurrentDirAsDirHist();
						ChangeDirComboProc((HWND)lParam);
					}
					else if(HIWORD(wParam) != CBN_CLOSEUP)
					{
						MakeButtonsFocus();
						SuppressRefresh = 0;
						return(0);
					}
					SuppressRefresh = 0;
					break;

				case MENU_HELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000001);
					break;

				case MENU_HELP_TROUBLE :
					ShellExecute(NULL, "open", MYWEB_URL, NULL, ".", SW_SHOW);
					break;

				case MENU_BMARK_ADD :
					AddCurDirToBookMark(WIN_REMOTE);
					break;

				case MENU_BMARK_ADD_LOCAL :
					AddCurDirToBookMark(WIN_LOCAL);
					break;

				case MENU_BMARK_ADD_BOTH :
					AddCurDirToBookMark(WIN_BOTH);
					break;

				case MENU_BMARK_EDIT :
					EditBookMark();
					break;

				case MENU_SELECT_ALL :
					SelectFileInList(hWndCurFocus, SELECT_ALL);
					break;

				case MENU_SELECT :
					SelectFileInList(hWndCurFocus, SELECT_REGEXP);
					break;

				case MENU_FIND :
					FindFileInList(hWndCurFocus, FIND_FIRST);
					break;

				case MENU_FINDNEXT :
					FindFileInList(hWndCurFocus, FIND_NEXT);
					break;

				case MENU_DOTFILE :
					DotFile ^= 1;
					DispDotFileMode();
					GetLocalDirForWnd();
					GetRemoteDirForWnd(CACHE_LASTREAD, &CancelFlg);
					break;

				case MENU_SYNC :
					ToggleSyncMoveMode();
					break;

				case MENU_IMPORT_WS :
					ImportFromWSFTP();
					break;

				case MENU_REGSAVE :
					GetListTabWidth();
					SaveRegistory();
					SaveSettingsToFile();
					break;

				case MENU_REGLOAD :
					if(LoadSettingsFromFile() == YES)
					{
						MessageBox(hWnd, MSGJPN292, "FFFTP", MB_OK);
						SaveExit = NO;
						PostMessage(hWnd, WM_CLOSE, 0, 0L);
					}
					break;

				case MENU_REGINIT :
					if(DialogBox(hInstFtp, MAKEINTRESOURCE(reginit_dlg), hWnd, ExeEscDialogProc) == YES)
					{
						ClearRegistory();
						SaveExit = NO;
						PostMessage(hWnd, WM_CLOSE, 0, 0L);
					}
					break;
				case MENU_CHANGEPASSWD:	/* 2010.01.31 genta */
					if( GetMasterPasswordStatus() != PASSWORD_OK )
					{
						/* �����I�ɐݒ肷�邩�m�F */
						if( DialogBox(hInstFtp, MAKEINTRESOURCE(forcepasschange_dlg), hWnd, ExeEscDialogProc) != YES){
							break;
						}
					}
					if( EnterMasterPasswordAndSet( newmasterpasswd_dlg, hWnd ) != 0 ){
						SetTaskMsg( MSGJPN303 );
					}
					break;

				case MENU_DIRINFO :
					DispDirInfo();
					break;

				case MENU_TASKINFO :
					DispTaskMsg();
					break;

				case MENU_ABORT :
					CancelFlg = YES;
					if(AskTryingConnect() == NO)
						MainTransPkt.Abort = ABORT_USER;
					break;

				case MENU_OTPCALC :
					OtpCalcTool();
					break;

				case MENU_URL_COPY :
					CopyURLtoClipBoard();
					break;

				case MENU_APPKEY :
					EraseListViewTips();
					if(hWndCurFocus == GetRemoteHwnd())
						RemoteRbuttonMenu(1);
					else if(hWndCurFocus == GetLocalHwnd())
						LocalRbuttonMenu(1);
					break;

				default :
					if((LOWORD(wParam) >= MENU_BMARK_TOP) &&
					   (LOWORD(wParam) < MENU_BMARK_TOP+100))
					{
						ChangeDirBmarkProc(LOWORD(wParam));
					}
					break;
			}
// ��Ƀz�X�g�����[�J���փt�H�[�J�X���ړ�
//			SetFocus(hWndCurFocus);
			MakeButtonsFocus();
			break;

		case WM_NOTIFY :
			switch(((LPNMHDR)lParam)->code)
			{
				/* �c�[���`�b�v�R���g���[�����b�Z�[�W�̏��� */
				case TTN_NEEDTEXT:
					lpttt = (LPTOOLTIPTEXT)lParam;
					lpttt->hinst = hInstFtp;
					switch(lpttt->hdr.idFrom)
					{
						case MENU_CONNECT :
							lpttt->lpszText = MSGJPN154;
							break;

						case MENU_QUICK :
							lpttt->lpszText = MSGJPN155;
							break;

						case MENU_DISCONNECT :
							lpttt->lpszText = MSGJPN156;
							break;

						case MENU_DOWNLOAD :
							lpttt->lpszText = MSGJPN157;
							break;

						case MENU_UPLOAD :
							lpttt->lpszText = MSGJPN158;
							break;

						case MENU_MIRROR_UPLOAD :
							lpttt->lpszText = MSGJPN159;
							break;

						case MENU_DELETE :
							lpttt->lpszText = MSGJPN160;
							break;

						case MENU_RENAME :
							lpttt->lpszText = MSGJPN161;
							break;

						case MENU_MKDIR :
							lpttt->lpszText = MSGJPN162;
							break;

						case MENU_LOCAL_UPDIR :
						case MENU_REMOTE_UPDIR :
							lpttt->lpszText = MSGJPN163;
							break;

						case MENU_LOCAL_CHDIR :
						case MENU_REMOTE_CHDIR :
							lpttt->lpszText = MSGJPN164;
							break;

						case MENU_TEXT :
							lpttt->lpszText = MSGJPN165;
							break;

						case MENU_BINARY :
							lpttt->lpszText = MSGJPN166;
							break;

						case MENU_AUTO :
							lpttt->lpszText = MSGJPN167;
							break;

						case MENU_REFRESH :
							lpttt->lpszText = MSGJPN168;
							break;

						case MENU_LIST :
							lpttt->lpszText = MSGJPN169;
							break;

						case MENU_REPORT :
							lpttt->lpszText = MSGJPN170;
							break;

						case MENU_KNJ_EUC :
							lpttt->lpszText = MSGJPN171;
							break;

						case MENU_KNJ_JIS :
							lpttt->lpszText = MSGJPN172;
							break;

						case MENU_KNJ_NONE :
							lpttt->lpszText = MSGJPN173;
							break;

						case MENU_KANACNV :
							lpttt->lpszText = MSGJPN174;
							break;

						case MENU_SYNC :
							lpttt->lpszText = MSGJPN175;
							break;

						case MENU_ABORT :
							lpttt->lpszText = MSGJPN176;
							break;
					}
					break;

				case LVN_COLUMNCLICK :
					if(((NMHDR *)lParam)->hwndFrom == GetLocalHwnd())
					{
						SetSortTypeByColumn(WIN_LOCAL, ((NM_LISTVIEW *)lParam)->iSubItem);
						ReSortDispList(WIN_LOCAL, &CancelFlg);
					}
					else if(((NMHDR *)lParam)->hwndFrom == GetRemoteHwnd())
					{
						if(((NM_LISTVIEW *)lParam)->iSubItem != 4)
						{
							SetSortTypeByColumn(WIN_REMOTE, ((NM_LISTVIEW *)lParam)->iSubItem);
							ReSortDispList(WIN_REMOTE, &CancelFlg);
						}
					}
					SetFocus(hWndCurFocus);
					break;

				case LVN_ITEMCHANGED :
					DispSelectedSpace();
					MakeButtonsFocus();
					break;
			}
			break;

		case WM_SIZE :
			Sizing = SW_RESTORE;
			if(wParam == SIZE_RESTORED)
			{
				ResizeWindowProc();
				GetWindowRect(hWnd, &Rect);
				WinPosX = Rect.left;
				WinPosY = Rect.top;
			}
			else if(wParam == SIZE_MAXIMIZED)
			{
				Sizing = SW_MAXIMIZE;
				ResizeWindowProc();
			}
			else
				return(DefWindowProc(hWnd, message, wParam, lParam));
			break;

		case WM_MOVING :
			WinPosX = ((RECT *)lParam)->left;
			WinPosY = ((RECT *)lParam)->top;
			return(DefWindowProc(hWnd, message, wParam, lParam));

		case WM_SETFOCUS :
			SetFocus(hWndCurFocus);
			break;

		case WM_LBUTTONDOWN :
		case WM_LBUTTONUP :
		case WM_MOUSEMOVE :
			CheckResizeFrame(wParam, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CHANGE_COND :
			DispTransferFiles();
			break;

		case WM_REFRESH_LOCAL_FLG :
			PostMessage(hWnd,  WM_COMMAND, MAKEWPARAM(REFRESH_LOCAL, 0), 0);
			break;

		case WM_REFRESH_REMOTE_FLG :
			if(SuppressRefresh == 0)
				PostMessage(hWnd,  WM_COMMAND, MAKEWPARAM(REFRESH_REMOTE, 0), 0);
			break;

		case WM_PAINT :
		    BeginPaint(hWnd, (LPPAINTSTRUCT) &ps);
		    EndPaint(hWnd, (LPPAINTSTRUCT) &ps);
			break;

		case WM_DESTROY :
//			WSACleanup();
//			DestroyWindow(hWndFtp);
			PostQuitMessage(0);
			break;

		case WM_QUERYENDSESSION :
			ExitProc(hWnd);
			return(TRUE);

		case WM_CLOSE :
			if((AskTransferNow() == NO) ||
			   (DialogBox(hInstFtp, MAKEINTRESOURCE(exit_dlg), hWnd, ExeEscDialogProc) == YES))
			{
				ExitProc(hWnd);
				return(DefWindowProc(hWnd, message, wParam, lParam));
			}
			break;

		default :
			return(DefWindowProc(hWnd, message, wParam, lParam));
	}
    return(0L);
}


/*----- �v���O�����J�n���̏��� ------------------------------------------------
*
*	Parameter
*		char *Cmd : �R�}���h���C��������
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void StartupProc(char *Cmd)
{
	int Sts;
	int AutoConnect;
	int CmdOption;
	int Kanji;
	int Kana;
	int FnameKanji;
	int TrMode;
	char unc[FMAX_PATH+1];

	Sts = AnalyzeComLine(Cmd, &AutoConnect, &CmdOption, unc, FMAX_PATH);

	TrMode = TYPE_DEFAULT;
	Kanji = KANJI_NOCNV;
	FnameKanji = KANJI_NOCNV;
	Kana = YES;
	if(CmdOption & OPT_ASCII)
		TrMode = TYPE_A;
	if(CmdOption & OPT_BINARY)
		TrMode = TYPE_I;
	if(CmdOption & OPT_EUC)
		Kanji = KANJI_EUC;
	if(CmdOption & OPT_JIS)
		Kanji = KANJI_JIS;
	if(CmdOption & OPT_EUC_NAME)
		FnameKanji = KANJI_EUC;
	if(CmdOption & OPT_JIS_NAME)
		FnameKanji = KANJI_JIS;
	if(CmdOption & OPT_KANA)
		Kana = NO;

	if(CmdOption & OPT_QUIT)
		AutoExit = YES;

	if(CmdOption & OPT_SAVEOFF)
		SuppressSave = YES;
	if(CmdOption & OPT_SAVEON)
		SuppressSave = NO;

	if(Sts == 0)
	{
		if(ConnectOnStart == YES)
			PostMessage(hWndFtp, WM_COMMAND, MAKEWPARAM(MENU_CONNECT, 0), 0);
	}
	else if(Sts == 1)
	{
		DirectConnectProc(unc, Kanji, Kana, FnameKanji, TrMode);
	}
	else if(Sts == 2)
	{
		PostMessage(hWndFtp, WM_COMMAND, MAKEWPARAM(MENU_CONNECT_NUM, CmdOption), (LPARAM)AutoConnect);
	}
	return;
}


/*----- �R�}���h���C������� --------------------------------------------------
*
*	Parameter
*		char *Str : �R�}���h���C��������
*		int *AutoConnect : �ڑ��z�X�g�ԍ���Ԃ����[�N
*		int *CmdOption : �I�v�V������Ԃ����[�N
*		char *unc : unc��Ԃ����[�N
*		int Max : unc�̍ő咷
*
*	Return Value
*		int �X�e�[�^�X
*			0=�w��Ȃ��A1=URL�w��A2=�ݒ薼�w��A-1=�G���[
*
*	Note
*		-m	--mirror
*		-d	--mirrordown
*		-f	--force
*		-q	--quit
*		-s	--set
*		-h	--help
*		-e	--euc
*		-j	--jis
*		-a	--ascii
*		-b	--binary
*		-x	--auto
*		-k	--kana
*		-u	--eucname
*		-i	--jisname
*		-n  --ini		(CheckIniFileName�Ō���)
*			--saveoff
*			--saveon
*		-z	--mpasswd	(CheckMasterPassword�Ō���)	2010.01.30 genta �ǉ�
*----------------------------------------------------------------------------*/

static int AnalyzeComLine(char *Str, int *AutoConnect, int *CmdOption, char *unc, int Max)
{
	int Ret;
	char Tmp[FMAX_PATH+1];

	*AutoConnect = -1;
	*CmdOption = 0;

	Ret = 0;
	memset(unc, NUL, Max+1);

	while((Ret != -1) && ((Str = GetToken(Str, Tmp)) != NULL))
	{
		if(Tmp[0] == '-')
		{
			_strlwr(Tmp);
			if((strcmp(&Tmp[1], "m") == 0) || (strcmp(&Tmp[1], "-mirror") == 0))
				*CmdOption |= OPT_MIRROR;
			else if((strcmp(&Tmp[1], "d") == 0) || (strcmp(&Tmp[1], "-mirrordown") == 0))
				*CmdOption |= OPT_MIRRORDOWN;
			else if((strcmp(&Tmp[1], "e") == 0) || (strcmp(&Tmp[1], "-euc") == 0))
				*CmdOption |= OPT_EUC;
			else if((strcmp(&Tmp[1], "j") == 0) || (strcmp(&Tmp[1], "-jis") == 0))
				*CmdOption |= OPT_JIS;
			else if((strcmp(&Tmp[1], "a") == 0) || (strcmp(&Tmp[1], "-ascii") == 0))
				*CmdOption |= OPT_ASCII;
			else if((strcmp(&Tmp[1], "b") == 0) || (strcmp(&Tmp[1], "-binary") == 0))
				*CmdOption |= OPT_BINARY;
			else if((strcmp(&Tmp[1], "x") == 0) || (strcmp(&Tmp[1], "-auto") == 0))
				*CmdOption |= OPT_AUTO;
			else if((strcmp(&Tmp[1], "f") == 0) || (strcmp(&Tmp[1], "-force") == 0))
				*CmdOption |= OPT_FORCE;
			else if((strcmp(&Tmp[1], "q") == 0) || (strcmp(&Tmp[1], "-quit") == 0))
				*CmdOption |= OPT_QUIT;
			else if((strcmp(&Tmp[1], "k") == 0) || (strcmp(&Tmp[1], "-kana") == 0))
				*CmdOption |= OPT_KANA;
			else if((strcmp(&Tmp[1], "u") == 0) || (strcmp(&Tmp[1], "-eucname") == 0))
				*CmdOption |= OPT_EUC_NAME;
			else if((strcmp(&Tmp[1], "i") == 0) || (strcmp(&Tmp[1], "-jisname") == 0))
				*CmdOption |= OPT_JIS_NAME;
			else if((strcmp(&Tmp[1], "n") == 0) || (strcmp(&Tmp[1], "-ini") == 0))
			{
				if((Str = GetToken(Str, Tmp)) == NULL)
				{
					SetTaskMsg(MSGJPN282);
					Ret = -1;
				}
			}
			else if(strcmp(&Tmp[1], "-saveoff") == 0)
				*CmdOption |= OPT_SAVEOFF;
			else if(strcmp(&Tmp[1], "-saveon") == 0)
				*CmdOption |= OPT_SAVEON;
			else if((strcmp(&Tmp[1], "z") == 0) || (strcmp(&Tmp[1], "-mpasswd") == 0))
			{	/* 2010.01.30 genta : Add master password option */
				if((Str = GetToken(Str, Tmp)) == NULL)
				{
					SetTaskMsg(MSGJPN299);
					Ret = -1;
				}
			}
			else if((strcmp(&Tmp[1], "s") == 0) || (strcmp(&Tmp[1], "-set") == 0))
			{
				if(Ret == 0)
				{
					if((Str = GetToken(Str, Tmp)) != NULL)
					{
						if((*AutoConnect = SearchHostName(Tmp)) != -1)
							Ret = 2;
						else
						{
							SetTaskMsg(MSGJPN177, Tmp);
							Ret = -1;
						}
					}
					else
					{
						SetTaskMsg(MSGJPN178);
						Ret = -1;
					}
				}
				else
				{
					SetTaskMsg(MSGJPN179);
					Ret = -1;
				}
			}
			else if((strcmp(&Tmp[1], "h") == 0) || (strcmp(&Tmp[1], "-help") == 0))
			{
				hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000024);
			}
			else
			{
				SetTaskMsg(MSGJPN180, Tmp);
				Ret = -1;
			}
		}
		else
		{
			if(Ret == 0)
			{
				strncpy(unc, Tmp, Max);
				Ret = 1;
			}
			else
			{
				SetTaskMsg(MSGJPN181);
				Ret = -1;
			}
		}
	}
	return(Ret);
}


/*----- INI�t�@�C���̃p�X���̎w����`�F�b�N ------------------------------------
*
*	Parameter
*		char *Str : �R�}���h���C��������
*		char *Ini : ini�t�@�C������Ԃ����[�N
*
*	Return Value
*		int �X�e�[�^�X
*			0=�w��Ȃ��A1=����
*
*	Note
*		-n  --ini
*----------------------------------------------------------------------------*/

static int CheckIniFileName(char *Str, char *Ini)
{
	return GetTokenAfterOption( Str, Ini, "n", "-ini" );
}

/* �}�X�^�[�p�X���[�h�̎w����`�F�b�N */
static int CheckMasterPassword(char *Str, char *Ini)
{
	return GetTokenAfterOption( Str, Ini, "z", "-mpasswd" );
}

/*----- �I�v�V�����̌��̃g�[�N�������o�� ------------------------------------
*
*	Parameter
*		char *Str : �R�}���h���C��������
*		char *Result : ���o������������i�[���郏�[�N
*		const char* Opt1, *Opt2: �I�v�V����������(2��)
*
*	Return Value
*		int �X�e�[�^�X
*			0=�w��Ȃ��A1=����
*
*	Note
*		2010.01.30 genta �}�X�^�[�p�X���[�h���o���̂��ߋ��ʉ�
*----------------------------------------------------------------------------*/
static int GetTokenAfterOption(char *Str, char *Result, const char* Opt1, const char* Opt2 )
{
	int Ret = 0;
	char Tmp[FMAX_PATH+1];

	Result[0] = NUL;
	while((Str = GetToken(Str, Tmp)) != NULL)
	{
		if(Tmp[0] == '-')
		{
			_strlwr(Tmp);
			if((strcmp(&Tmp[1], Opt1) == 0) || (strcmp(&Tmp[1], Opt2) == 0))
			{
				if((Str = GetToken(Str, Result)) != NULL)
					Ret = 1;
				break;
			}
		}
	}
	return(Ret);
}

/*----- �g�[�N����Ԃ� --------------------------------------------------------
*
*	Parameter
*		char *Str : ������
*		char *Buf : �������Ԃ��o�b�t�@
*
*	Return Value
*		char *�Ԃ����g�[�N���̖���
*			NULL=�I���
*----------------------------------------------------------------------------*/

static char *GetToken(char *Str, char *Buf)
{
	int InQuote;

	while(*Str != NUL)
	{
		if((*Str != ' ') && (*Str != '\t'))
			break;
		Str++;
	}

	if(*Str != NUL)
	{
		InQuote = 0;
		while(*Str != NUL)
		{
			if(*Str == 0x22)
				InQuote = !InQuote;
			else
			{
				if(((*Str == ' ') || (*Str == '\t')) &&
				   (InQuote == 0))
				{
					break;
				}
				*Buf++ = *Str;
			}
			Str++;
		}
	}
	else
		Str = NULL;

	*Buf = NUL;

	return(Str);
}


/*----- �v���O�����I�����̏��� ------------------------------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void ExitProc(HWND hWnd)
{
	CancelFlg = YES;

	CloseTransferThread();

	if(SaveExit == YES)
	{
		SaveBookMark();
		SaveCurrentSetToHost();
	}
	DeleteAlltempFile();

//	WSACancelBlockingCall();
	DisconnectProc();
//	CloseTransferThread();

	if(SaveExit == YES)
	{
		GetListTabWidth();
		SaveRegistory();

		if((CacheEntry > 0) && (CacheSave == YES))
			SaveCache();
		else
			DeleteCache();
	}
	else
		DeleteCache();

	if(RasClose == YES)
	{
		DisconnectRas(RasCloseNotify);
	}
	DeleteAllObject();
	HtmlHelp(NULL, NULL, HH_UNINITIALIZE, dwCookie); 
	return;
}


/*----- �t�@�C�������_�u���N���b�N�����Ƃ��̏��� ------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		int Mode : ��Ɂu�J���v��������邩�ǂ��� (YES/NO)
*		int App : �A�v���P�[�V�����ԍ��i-1=�֘A�Â��D��j
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DoubleClickProc(int Win, int Mode, int App)
{
	int Pos;
	int Type;
	char Local[FMAX_PATH+1];
	char Remote[FMAX_PATH+1];
	char Tmp[FMAX_PATH+1];
	int Sts;
	int UseDiffViewer;

	if(AskUserOpeDisabled() == NO)
	{
		SetCurrentDirAsDirHist();
		if(GetSelectedCount(Win) == 1)
		{
			if((Pos = GetFirstSelected(Win, NO)) != -1)
			{
				GetNodeName(Win, Pos, Tmp, FMAX_PATH);
				Type = GetNodeType(Win, Pos);

				if(Win == WIN_LOCAL)
				{
					if((App != -1) || (Type == NODE_FILE))
					{
						if((DclickOpen == YES) || (Mode == YES))
						{
							AskLocalCurDir(Local, FMAX_PATH);
							ReplaceAll(Local, '/', '\\');
							SetYenTail(Local);
							strcat(Local, Tmp);
							ExecViewer(Local, App);
						}
						else
							PostMessage(hWndFtp, WM_COMMAND, MAKEWPARAM(MENU_UPLOAD, 0), 0);
					}
					else
						ChangeDir(WIN_LOCAL, Tmp);
				}
				else if(CheckClosedAndReconnect() == SUCCESS)
				{
					if((App != -1) || (Type == NODE_FILE))
					{
						if((DclickOpen == YES) || (Mode == YES))
						{
							// �r���[���Q�A�R�̃p�X�� "d " �Ŏn�܂��Ă����獷���r���[�A�g�p
							if ((App == 1 || App == 2) && strncmp(ViewerName[App], "d ", 2) == 0)
								UseDiffViewer = YES;
							else
								UseDiffViewer = NO;

							strcpy(Remote, TmpPath);
							SetYenTail(Remote);
							if (UseDiffViewer == YES) {
								strcat(Remote, "remote.");
							}
							strcat(Remote, Tmp);

							if(AskTransferNow() == YES)
								SktShareProh();

	//						MainTransPkt.ctrl_skt = AskCmdCtrlSkt();
							strcpy(MainTransPkt.Cmd, "RETR ");
							if(AskHostType() == HTYPE_ACOS)
							{
								strcpy(MainTransPkt.RemoteFile, "'");
								strcat(MainTransPkt.RemoteFile, AskHostLsName());
								strcat(MainTransPkt.RemoteFile, "(");
								strcat(MainTransPkt.RemoteFile, Tmp);
								strcat(MainTransPkt.RemoteFile, ")");
								strcat(MainTransPkt.RemoteFile, "'");
							}
							else if(AskHostType() == HTYPE_ACOS_4)
							{
								strcpy(MainTransPkt.RemoteFile, Tmp);
							}
							else
							{
								strcpy(MainTransPkt.RemoteFile, Tmp);
							}
							strcpy(MainTransPkt.LocalFile, Remote);
							MainTransPkt.Type = AskTransferTypeAssoc(MainTransPkt.RemoteFile, AskTransferType());
							MainTransPkt.Size = 1;
							MainTransPkt.KanjiCode = AskHostKanjiCode();
							MainTransPkt.KanaCnv = AskHostKanaCnv();
							MainTransPkt.Mode = EXIST_OVW;
							MainTransPkt.ExistSize = 0;
							MainTransPkt.hWndTrans = NULL;
							MainTransPkt.Next = NULL;

							DisableUserOpe();

							/* �s���ȃp�X�����o */
							if(CheckPathViolation(&MainTransPkt) == NO)
							{
//								if((Sts = DoDownLoad(AskCmdCtrlSkt(), &MainTransPkt, NO)) == 429)
//								{
//									ReConnectCmdSkt();
									Sts = DoDownLoad(AskCmdCtrlSkt(), &MainTransPkt, NO, &CancelFlg);
//								}
							}

							EnableUserOpe();

							AddTempFileList(Remote);
							if(Sts/100 == FTP_COMPLETE) {
								if (UseDiffViewer == YES) {
									AskLocalCurDir(Local, FMAX_PATH);
									ReplaceAll(Local, '/', '\\');
									SetYenTail(Local);
									strcat(Local, Tmp);
									ExecViewer2(Local, Remote, App);
								} else {
									ExecViewer(Remote, App);
								}
							}
						}
						else
							PostMessage(hWndFtp, WM_COMMAND, MAKEWPARAM(MENU_DOWNLOAD, 0), 0);
					}
					else
						ChangeDir(WIN_REMOTE, Tmp);
				}
			}
		}
		MakeButtonsFocus();
	}
	return;
}


/*----- �t�H���_�̈ړ� --------------------------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx)
*		char *Path : �ړ���̃p�X��
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�t�H���_�����ړ��̏������s��
*----------------------------------------------------------------------------*/

static void ChangeDir(int Win, char *Path)
{
	int Sync;
	char Local[FMAX_PATH+1];
	char Remote[FMAX_PATH+1];

	Sync = AskSyncMoveMode();
	if(Sync == YES)
	{
		if(strcmp(Path, "..") == 0)
		{
			AskLocalCurDir(Local, FMAX_PATH);
			AskRemoteCurDir(Remote, FMAX_PATH);
			if(strcmp(GetFileName(Local), GetFileName(Remote)) != 0)
				Sync = NO;
		}
	}

	if((Win == WIN_LOCAL) || (Sync == YES))
	{
		if(DoLocalCWD(Path) == SUCCESS)
			GetLocalDirForWnd();
	}

	if((Win == WIN_REMOTE) || (Sync == YES))
	{
		if(CheckClosedAndReconnect() == SUCCESS)
		{
#if defined(HAVE_OPENVMS)
			/* OpenVMS�̏ꍇ�A".DIR;?"����� */
			if (AskHostType() == HTYPE_VMS)
				ReformVMSDirName(Path, TRUE);
#endif
			if(DoCWD(Path, YES, NO, YES) < FTP_RETRY)
				GetRemoteDirForWnd(CACHE_NORMAL, &CancelFlg);
		}
	}
	return;
}


/*----- �E�C���h�E�̃T�C�Y�ύX�̏��� ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void ResizeWindowProc(void)
{
#if 0
	RECT Rect;
	int RemotePosX;

	GetClientRect(hWndFtp, &Rect);
	SendMessage(GetSbarWnd(), WM_SIZE, SIZE_RESTORED, MAKELPARAM(Rect.right, Rect.bottom));

	CalcWinSize();
	SetWindowPos(GetMainTbarWnd(), 0, 0, 0, WinWidth, TOOLWIN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);

	SetWindowPos(GetLocalTbarWnd(), 0, 0, TOOLWIN_HEIGHT, LocalWidth, TOOLWIN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	SendMessage(GetLocalTbarWnd(), TB_GETITEMRECT, 3, (LPARAM)&Rect);
	SetWindowPos(GetLocalHistHwnd(), 0, Rect.right, Rect.top, LocalWidth - Rect.right, 200, SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(GetLocalHwnd(), 0, 0, TOOLWIN_HEIGHT*2, LocalWidth, ListHeight, SWP_NOACTIVATE | SWP_NOZORDER);

	RemotePosX = LocalWidth + SepaWidth;
	if(SplitVertical == YES)
		RemotePosX = 0;

	SetWindowPos(GetRemoteTbarWnd(), 0, RemotePosX, TOOLWIN_HEIGHT, RemoteWidth, TOOLWIN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	SendMessage(GetRemoteTbarWnd(), TB_GETITEMRECT, 3, (LPARAM)&Rect);
	SetWindowPos(GetRemoteHistHwnd(), 0, Rect.right, Rect.top, RemoteWidth - Rect.right, 200, SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(GetRemoteHwnd(), 0, RemotePosX, TOOLWIN_HEIGHT*2, RemoteWidth, ListHeight, SWP_NOACTIVATE | SWP_NOZORDER);

	SetWindowPos(GetTaskWnd(), 0, 0, TOOLWIN_HEIGHT*2+ListHeight+SepaWidth, ClientWidth, TaskHeight, SWP_NOACTIVATE | SWP_NOZORDER);
#else
	RECT Rect;

	GetClientRect(hWndFtp, &Rect);
	SendMessage(GetSbarWnd(), WM_SIZE, SIZE_RESTORED, MAKELPARAM(Rect.right, Rect.bottom));

	CalcWinSize();
	SetWindowPos(GetMainTbarWnd(), 0, 0, 0, Rect.right, TOOLWIN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(GetLocalTbarWnd(), 0, 0, TOOLWIN_HEIGHT, LocalWidth, TOOLWIN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(GetRemoteTbarWnd(), 0, LocalWidth + SepaWidth, TOOLWIN_HEIGHT, RemoteWidth, TOOLWIN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	SendMessage(GetLocalTbarWnd(), TB_GETITEMRECT, 3, (LPARAM)&Rect);
	SetWindowPos(GetLocalHistHwnd(), 0, Rect.right, Rect.top, LocalWidth - Rect.right, 200, SWP_NOACTIVATE | SWP_NOZORDER);
	SendMessage(GetRemoteTbarWnd(), TB_GETITEMRECT, 3, (LPARAM)&Rect);
	SetWindowPos(GetRemoteHistHwnd(), 0, Rect.right, Rect.top, RemoteWidth - Rect.right, 200, SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(GetLocalHwnd(), 0, 0, TOOLWIN_HEIGHT*2, LocalWidth, ListHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(GetRemoteHwnd(), 0, LocalWidth + SepaWidth, TOOLWIN_HEIGHT*2, RemoteWidth, ListHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(GetTaskWnd(), 0, 0, TOOLWIN_HEIGHT*2+ListHeight+SepaWidth, ClientWidth, TaskHeight, SWP_NOACTIVATE | SWP_NOZORDER);
#endif

	return;
}


/*----- �E�C���h�E�̊e�����̃T�C�Y���v�Z���� ----------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void CalcWinSize(void)
{
	RECT Rect;

	GetWindowRect(hWndFtp, &Rect);

	if(Sizing != SW_MAXIMIZE)
	{
		WinWidth = Rect.right - Rect.left;
		WinHeight = Rect.bottom - Rect.top;
	}

	GetClientRect(hWndFtp, &Rect);

	ClientWidth = Rect.right;
	ClientHeight = Rect.bottom;

	SepaWidth = 4;
	LocalWidth = max1(0, min1(LocalWidth, ClientWidth - SepaWidth));
	RemoteWidth = max1(0, ClientWidth - LocalWidth - SepaWidth);
//	TaskHeight = min1(TaskHeight, max1(0, ClientHeight - TOOLWIN_HEIGHT * 2 - SepaWidth));

	GetClientRect(GetSbarWnd(), &Rect);

	ListHeight = max1(0, ClientHeight - TOOLWIN_HEIGHT * 2 - TaskHeight - SepaWidth - Rect.bottom);

	return;
}


#if 0
/*----- �E�C���h�E�̕\���ʒu���擾���� ----------------------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void AskWindowPos(HWND hWnd)
{
	WINDOWPLACEMENT WinPlace;

	WinPlace.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd, &WinPlace);
	WinPosX = WinPlace.rcNormalPosition.left;
	WinPosY = WinPlace.rcNormalPosition.top;

	return;
}
#endif


/*----- �f�B���N�g�����X�g�ƃt�@�C�����X�g�̋��E�ύX���� ----------------------
*
*	Parameter
*		WPARAM Keys : WM_MOUSEMOVE�Ȃǂ�WPARAM�̒l
*		int x : �}�E�X�J�[�\���̂w���W
*		int y : �}�E�X�J�[�\���̂x���W
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void CheckResizeFrame(WPARAM Keys, int x, int y)
{
	RECT Rect;
	RECT Rect1;

	if((Resizing == RESIZE_OFF) && (Keys == 0))
	{
		if((x >= LocalWidth) && (x <= LocalWidth + SepaWidth) &&
		   (y > TOOLWIN_HEIGHT) && (y < (TOOLWIN_HEIGHT * 2 + ListHeight)))
		{
			/* ���E�ʒu�ύX�p�J�[�\���ɕύX */
			SetCapture(hWndFtp);
			hCursor = LoadCursor(hInstFtp, MAKEINTRESOURCE(resize_lr_csr));
			SetCursor(hCursor);
			Resizing = RESIZE_PREPARE;
			ResizePos = RESIZE_HPOS;
		}
		else if((y >= TOOLWIN_HEIGHT*2+ListHeight) && (y <= TOOLWIN_HEIGHT*2+ListHeight+SepaWidth))
		{
			/* ���E�ʒu�ύX�p�J�[�\���ɕύX */
			SetCapture(hWndFtp);
			hCursor = LoadCursor(hInstFtp, MAKEINTRESOURCE(resize_ud_csr));
			SetCursor(hCursor);
			Resizing = RESIZE_PREPARE;
			ResizePos = RESIZE_VPOS;
		}
	}
	else if(Resizing == RESIZE_PREPARE)
	{
		if(Keys & MK_LBUTTON)
		{
			/* ���E�ʒu�ύX�J�n */
			Resizing = RESIZE_ON;
			GetWindowRect(hWndFtp, &Rect);
			GetClientRect(GetSbarWnd(), &Rect1);
			Rect.left += GetSystemMetrics(SM_CXFRAME);
			Rect.right -= GetSystemMetrics(SM_CXFRAME);
			Rect.top += TOOLWIN_HEIGHT*2 + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
			Rect.bottom -= GetSystemMetrics(SM_CYFRAME) + Rect1.bottom;
			ClipCursor(&Rect);
		}
		else
		{
			if(((ResizePos == RESIZE_HPOS) &&
				((x < LocalWidth) || (x > LocalWidth + SepaWidth) ||
				 (y <= TOOLWIN_HEIGHT) || (y >= (TOOLWIN_HEIGHT * 2 + ListHeight)))) ||
			   ((ResizePos == RESIZE_VPOS) &&
				((y < TOOLWIN_HEIGHT*2+ListHeight) || (y > TOOLWIN_HEIGHT*2+ListHeight+SepaWidth))))
			{
				/* ���̃J�[�\���ɖ߂� */
				ReleaseCapture();
				hCursor = LoadCursor(NULL, IDC_ARROW);
				SetCursor(hCursor);
				Resizing = RESIZE_OFF;
			}
		}
	}
	else if(Resizing == RESIZE_ON)
	{
		if(ResizePos == RESIZE_HPOS)
			LocalWidth = x;
		else
		{
			GetClientRect(hWndFtp, &Rect);
			GetClientRect(GetSbarWnd(), &Rect1);
			TaskHeight = max1(0, Rect.bottom - y - Rect1.bottom);
		}
		ResizeWindowProc();

		if((Keys & MK_LBUTTON) == 0)
		{
			/* ���E�ʒu�ύX�I�� */
			ReleaseCapture();
			ClipCursor(NULL);
			hCursor = LoadCursor(NULL, IDC_ARROW);
			SetCursor(hCursor);
			Resizing = RESIZE_OFF;
		}
	}
	return;
}


/*----- �t�@�C���ꗗ�����r���[���ŕ\�� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DispDirInfo(void)
{
	char Buf[FMAX_PATH+1];

	MakeCacheFileName(AskCurrentFileListNum(), Buf);
	ExecViewer(Buf, 0);
	return;
}



/*----- �r���[�����N�� --------------------------------------------------------
*
*	Parameter
*		char Fname : �t�@�C����
*		int App : �A�v���P�[�V�����ԍ��i-1=�֘A�Â��D��j
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ExecViewer(char *Fname, int App)
{
	PROCESS_INFORMATION Info;
	STARTUPINFO Startup;
	char AssocProg[FMAX_PATH+1];
	char ComLine[FMAX_PATH*2+3+1];
	char CurDir[FMAX_PATH+1];

	/* FindExecutable()�͊֘A�t����ꂽ�v���O�����̃p�X���ɃX�y�[�X��	*/
	/* �܂܂�Ă��鎞�A�Ԉ�����p�X����Ԃ���������B					*/
	/* �����ŁA�֘A�t����ꂽ�v���O�����̋N����ShellExecute()���g���B	*/

	AskLocalCurDir(CurDir, FMAX_PATH);

	if((App == -1) && (FindExecutable(Fname, NULL, AssocProg) > (HINSTANCE)32))
	{
		DoPrintf("ShellExecute - %s", Fname);
		ShellExecute(NULL, "open", Fname, NULL, CurDir, SW_SHOW);
	}
	else
	{
		App = max1(0, App);
		strcpy(AssocProg, ViewerName[App]);

		if(strchr(Fname, ' ') == NULL)
			sprintf(ComLine, "%s %s", AssocProg, Fname);
		else
			sprintf(ComLine, "%s \"%s\"", AssocProg, Fname);

		DoPrintf("FindExecutable - %s", ComLine);

		memset(&Startup, NUL, sizeof(STARTUPINFO));
		Startup.cb = sizeof(STARTUPINFO);
		Startup.wShowWindow = SW_SHOW;
		if(CreateProcess(NULL, ComLine, NULL, NULL, FALSE, 0, NULL, NULL, &Startup, &Info) == FALSE)
		{
			SetTaskMsg(MSGJPN182, GetLastError());
			SetTaskMsg(">>%s", ComLine);
		}
	}
	return;
}


/*----- �����\���r���[�����N�� ------------------------------------------------
*
*	Parameter
*		char Fname1 : �t�@�C����
*		char Fname2 : �t�@�C����2
*		int App : �A�v���P�[�V�����ԍ��i2 or 3�j
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ExecViewer2(char *Fname1, char *Fname2, int App)
{
	PROCESS_INFORMATION Info;
	STARTUPINFO Startup;
	char AssocProg[FMAX_PATH+1];
	char ComLine[FMAX_PATH*2+3+1];
	char CurDir[FMAX_PATH+1];

	/* FindExecutable()�͊֘A�t����ꂽ�v���O�����̃p�X���ɃX�y�[�X��	*/
	/* �܂܂�Ă��鎞�A�Ԉ�����p�X����Ԃ���������B					*/
	/* �����ŁA�֘A�t����ꂽ�v���O�����̋N����ShellExecute()���g���B	*/

	AskLocalCurDir(CurDir, FMAX_PATH);

	strcpy(AssocProg, ViewerName[App] + 2);	/* �擪�� "d " �͓ǂݔ�΂� */

	if(strchr(Fname1, ' ') == NULL && strchr(Fname2, ' ') == NULL)
		sprintf(ComLine, "%s %s %s", AssocProg, Fname1, Fname2);
	else
		sprintf(ComLine, "%s \"%s\" \"%s\"", AssocProg, Fname1, Fname2);

	DoPrintf("FindExecutable - %s", ComLine);

	memset(&Startup, NUL, sizeof(STARTUPINFO));
	Startup.cb = sizeof(STARTUPINFO);
	Startup.wShowWindow = SW_SHOW;
	if(CreateProcess(NULL, ComLine, NULL, NULL, FALSE, 0, NULL, NULL, &Startup, &Info) == FALSE)
	{
		SetTaskMsg(MSGJPN182, GetLastError());
		SetTaskMsg(">>%s", ComLine);
	}

	return;
}


/*----- �e���|�����t�@�C�������e���|�����t�@�C�����X�g�ɒǉ� ------------------
*
*	Parameter
*		char *Fname : �e���|�����t�@�C����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void AddTempFileList(char *Fname)
{
	TEMPFILELIST *New;

	if((New = malloc(sizeof(TEMPFILELIST))) != NULL)
	{
		if((New->Fname = malloc(strlen(Fname)+1)) != NULL)
		{
			strcpy(New->Fname, Fname);
			if(TempFiles == NULL)
				New->Next = NULL;
			else
				New->Next = TempFiles;
			TempFiles = New;
		}
		else
			free(New);
	}
	return;
}


/*----- �e���|�����t�@�C�����X�g�ɓo�^����Ă���t�@�C����S�č폜 ------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void DeleteAlltempFile(void)
{
	TEMPFILELIST *Pos;
	TEMPFILELIST *Next;

	Pos = TempFiles;
	while(Pos != NULL)
	{
		DeleteFile(Pos->Fname);

		Next = Pos->Next;
		free(Pos->Fname);
		free(Pos);
		Pos = Next;
	}

	// OLE D&D�̃e���|�������폜���� (2007.9.11 yutaka)
	doDeleteRemoteFile();

	return;
}


/*----- �`���������_�C�A���O�{�b�N�X�̃R�[���o�b�N�֐� ------------------------
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

static BOOL CALLBACK AboutDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char Tmp[80];
	int Ver;

	switch (message)
	{
		case WM_INITDIALOG :
			Ver = GetJreVersion();
			if(Ver == -1)
				sprintf(Tmp, MSGJPN183);
			else
				sprintf(Tmp, MSGJPN184, Ver / 0x100, Ver % 0x100);
			SendDlgItemMessage(hDlg, ABOUT_JRE, WM_SETTEXT, 0, (LPARAM)Tmp);
			SendDlgItemMessage(hDlg, ABOUT_URL, EM_LIMITTEXT, 256, 0);
			SendDlgItemMessage(hDlg, ABOUT_URL, WM_SETTEXT, 0, (LPARAM)MSGJPN284);
			return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
				case IDCANCEL :
					EndDialog(hDlg, YES);
					break;
			}
			return(TRUE);
	}
    return(FALSE);
}


/*----- �T�E���h��炷 ------------------------------------------------------
*
*	Parameter
*		Int num : �T�E���h�̎�� (SND_xxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SoundPlay(int Num)
{
	if(Sound[Num].On == YES)
		sndPlaySound(Sound[Num].Fname, SND_ASYNC | SND_NODEFAULT);

	return;
}


/*----- �w���v�t�@�C���̃p�X����Ԃ� ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		char *�p�X��
*----------------------------------------------------------------------------*/

char *AskHelpFilePath(void)
{
	return(HelpPath);
}


/*----- �e���|�����t�@�C���̃p�X����Ԃ� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		char *�p�X��
*----------------------------------------------------------------------------*/

char *AskTmpFilePath(void)
{
	return(TmpPath);
}


/*----- INI�t�@�C���̃p�X����Ԃ� ---------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		char *�p�X��
*----------------------------------------------------------------------------*/

char *AskIniFilePath(void)
{
	return(IniPath);
}

/*----- INI�t�@�C���݂̂��g�����ǂ�����Ԃ� -----------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X : YES/NO
*----------------------------------------------------------------------------*/

int AskForceIni(void)
{
	return(ForceIni);
}




/*----- ���b�Z�[�W���� --------------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �I���t���O (YES=WM_CLOSE������/NO)
*----------------------------------------------------------------------------*/

int BackgrndMessageProc(void)
{
    MSG Msg;
	int Ret;

	Ret = NO;
	while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
	{
		if(!HtmlHelp(NULL, NULL, HH_PRETRANSLATEMESSAGE, (DWORD)&Msg))
		{
	 		/* �f�B���N�g�����̕\���R���{�{�b�N�X��BS��RET�������悤�� */
			/* �R���{�{�b�N�X���ł̓A�N�Z�����[�^�𖳌��ɂ��� */
			if((Msg.hwnd == GetLocalHistEditHwnd()) ||
			   (Msg.hwnd == GetRemoteHistEditHwnd()) ||
			   ((hHelpWin != NULL) && (Msg.hwnd == hHelpWin)) ||
			   GetHideUI() == YES ||
			   (TranslateAccelerator(GetMainHwnd(), Accel, &Msg) == 0))
			{
				if(Msg.message == WM_QUIT)
				{
					Ret = YES;
					PostQuitMessage(0);
					break;
				}
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
	}
	return(Ret);
}


/*----- �����I���t���O���N���A���� --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ResetAutoExitFlg(void)
{
	AutoExit = NO;
	return;
}


/*----- �����I���t���O��Ԃ� --------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �t���O (YES/NO)
*----------------------------------------------------------------------------*/

int AskAutoExit(void)
{
	return(AutoExit);
}

/*----- ���[�U�Ƀp�X���[�h����͂����C�����ݒ肷�� -----------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int : 0/���[�U�L�����Z��, 1/�ݒ肵��, 2/�f�t�H���g�ݒ�
*----------------------------------------------------------------------------*/
int EnterMasterPasswordAndSet( int Res, HWND hWnd )
{
	char buf[MAX_PASSWORD_LEN + 1];
	char *p;
	int Flag;

	buf[0] = NUL;
	if( InputDialogBox(Res, hWnd, NULL, buf, MAX_PASSWORD_LEN + 1,
		&Flag, IDH_HELP_TOPIC_0000064) == YES){
		/* �����̋󔒂��폜 */
		RemoveTailingSpaces(buf);
		/* �擪�̋󔒂��폜 */
		for( p = buf; *p == ' '; p++ )
			;
		
		if( p[0] != NUL ){
			SetMasterPassword( p );
			return 1;
		}
		else {
			/* ��̏ꍇ�̓f�t�H���g�l��ݒ� */
			SetMasterPassword( NULL );
			return 2;
		}
	}
	return 0;
}
