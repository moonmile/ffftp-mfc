/*=============================================================================
*
*							�X�e�[�^�X�E�C���h�E
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


/*===== ���[�J���ȃ��[�N =====*/

static HWND hWndSbar = NULL;
static int SbarColWidth[5] = { 70, 230, 410, 570, -1 };



/*----- �X�e�[�^�X�E�C���h�E���쐬���� ----------------------------------------
*
*	Parameter
*		HWND hWnd : �e�E�C���h�E�̃E�C���h�E�n���h��
*		HINSTANCE hInst : �C���X�^���X�n���h��
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int MakeStatusBarWindow(HWND hWnd, HINSTANCE hInst)
{
	int Sts;

	Sts = FAIL;
	hWndSbar = CreateWindowEx(0,
			STATUSCLASSNAME, NULL,
			WS_CHILD | SBS_SIZEGRIP | WS_CLIPSIBLINGS | SBT_NOBORDERS,
			0, 0, 0, 0,
			hWnd, (HMENU)1500, hInst, NULL);

	if(hWndSbar != NULL)
	{
		SendMessage(hWndSbar, SB_SETPARTS, sizeof(SbarColWidth)/sizeof(int), (LPARAM)SbarColWidth);
		ShowWindow(hWndSbar, SW_SHOW);
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �X�e�[�^�X�E�C���h�E���폜 --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DeleteStatusBarWindow(void)
{
	if(hWndSbar != NULL)
		DestroyWindow(hWndSbar);
	return;
}


/*----- �X�e�[�^�X�E�C���h�E�̃E�C���h�E�n���h����Ԃ� ------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetSbarWnd(void)
{
	return(hWndSbar);
}


/*----- �J�����g�E�C���h�E��\�� ----------------------------------------------
*
*	Parameter
*		int Win : �E�C���h�E�ԍ� (WIN_xxx : -1=�Ȃ�)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispCurrentWindow(int Win)
{
	if(Win == WIN_LOCAL)
		SendMessage(GetSbarWnd(), SB_SETTEXT, 0 | 0, (LPARAM)MSGJPN245);
	else if(Win == WIN_REMOTE)
		SendMessage(GetSbarWnd(), SB_SETTEXT, 0 | 0, (LPARAM)MSGJPN246);
	else
		SendMessage(GetSbarWnd(), SB_SETTEXT, 0 | 0, (LPARAM)"");
	return;
}


/*----- �I������Ă���t�@�C�����ƃT�C�Y��\�� --------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispSelectedSpace(void)
{
	char Buf1[50];
	char Buf2[50];
	int Win;

	Win = WIN_LOCAL;
	if(GetFocus() == GetRemoteHwnd())
		Win = WIN_REMOTE;

	MakeSizeString(GetSelectedTotalSize(Win), Buf1);
	sprintf(Buf2, MSGJPN247, GetSelectedCount(Win), Buf1);
	SendMessage(GetSbarWnd(), SB_SETTEXT, 1 | 0, (LPARAM)Buf2);
	return;
}


/*----- ���[�J�����̋󂫗e�ʂ�\�� --------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispLocalFreeSpace(char *Path)
{
	char Buf[40];

	sprintf(Buf, MSGJPN248, AskLocalFreeSpace(Path));
	SendMessage(GetSbarWnd(), SB_SETTEXT, 2 | 0, (LPARAM)Buf);
	return;
}


/*----- �]������t�@�C���̐���\�� --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispTransferFiles(void)
{
	char Buf[50];

	sprintf(Buf, MSGJPN249, AskTransferFileNum());
	SendMessage(GetSbarWnd(), SB_SETTEXT, 3 | 0, (LPARAM)Buf);
	return;
}


/*----- ��M���̃o�C�g����\�� ------------------------------------------------
*
*	Parameter
*		LONGLONG Size : �o�C�g�� (-1=�\��������)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispDownloadSize(LONGLONG Size)
{
	char Buf[50];
	char Tmp[50];

	strcpy(Buf, "");
	if(Size >= 0)
	{
		MakeSizeString((double)Size, Tmp);
		sprintf(Buf, MSGJPN250, Tmp);
	}
	SendMessage(GetSbarWnd(), SB_SETTEXT, 4 | 0, (LPARAM)Buf);
	return;
}


