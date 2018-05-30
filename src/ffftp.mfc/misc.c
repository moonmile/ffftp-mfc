/*=============================================================================
*
*							���̑��̔ėp�T�u���[�`��
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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <mbstring.h>
#include <winsock.h>
#include <windowsx.h>
#include <commctrl.h>
#include <shlobj.h>
#include <locale.h>

#include "common.h"
#include "resource.h"

#include <htmlhelp.h>
#include "helpid.h"



/*===== ���̓_�C�A���O�f�[�^�̃X�g���N�`�� =====*/

typedef struct {
	char Title[80];			/* �_�C�A���O�̃^�C�g�� */
	char Str[FMAX_PATH+1];	/* �f�t�H���g������^���͂��ꂽ������(Output) */
	int MaxLen;				/* ������̍Œ� */
	int Anonymous;			/* Anonymous�t���O(Output) */
} DIALOGDATA;

/*===== �v���g�^�C�v =====*/

static BOOL CALLBACK InputDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

/*===== �O���Q�� =====*/

extern HWND hHelpWin;

/*===== ���[�J���ȃ��[�N =====*/

static DIALOGDATA *DialogData;		/* ���̓_�C�A���O�f�[�^ */
static int HelpPage;



/*----- ���̓_�C�A���O��\�� --------------------------------------------------
*
*	Parameter
*		int Res : �_�C�A���O�{�b�N�X��ID
*		HWND hWnd : �e�E�C���h�E�̃E�C���h�E�n���h��
*		char *Title : �E�C���h�E�^�C�g�� (NULL=�ݒ肵�Ȃ�)
*		char *Buf : �G�f�B�b�g�{�b�N�X�̏���������^���͕������Ԃ��o�b�t�@
*		int Max : �o�b�t�@�̃T�C�Y (FMAX_PATH+1�ȉ��ł��邱��)
*		int *Flg : �t���O�̏����l�^�t���O��Ԃ����[�N
*		int Help : �w���v�̃R���e�L�X�g�ԍ�
*
*	Return Value
*		int �X�e�[�^�X (YES/NO=������)
*
*	Note
*		�_�C�A���O�͂P��EditBox�ƂP��Button�������̂��g��
*----------------------------------------------------------------------------*/

int InputDialogBox(int Res, HWND hWnd, char *Title, char *Buf, int Max, int *Flg, int Help)
{
	int Ret;
	DIALOGDATA dData;

	dData.MaxLen = Max;
	memset(dData.Str, NUL, FMAX_PATH+1);
	strncpy(dData.Str, Buf, FMAX_PATH);
	strcpy(dData.Title, "");
	if(Title != NULL)
		strcpy(dData.Title, Title);
	dData.Anonymous = *Flg;
	DialogData = &dData;
	HelpPage = Help;

	Ret = DialogBox(GetFtpInst(), MAKEINTRESOURCE(Res), hWnd, InputDialogCallBack);

	if(Ret == YES)
	{
		memset(Buf, NUL, Max);
		strncpy(Buf, dData.Str, Max-1);
		*Flg = dData.Anonymous;
	}
	return(Ret);
}


/*----- ���̓_�C�A���O�̃R�[���o�b�N ------------------------------------------
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

static BOOL CALLBACK InputDialogCallBack(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	char Tmp[FMAX_PATH+1];

	switch (iMessage)
	{
		case WM_INITDIALOG :
			if(strlen(DialogData->Title) != 0)
				SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM)DialogData->Title);
			SendDlgItemMessage(hDlg, INP_INPSTR, EM_LIMITTEXT, DialogData->MaxLen-1, 0);
			SendDlgItemMessage(hDlg, INP_INPSTR, WM_SETTEXT, 0, (LPARAM)DialogData->Str);
			SendDlgItemMessage(hDlg, INP_ANONYMOUS, BM_SETCHECK, DialogData->Anonymous, 0);
			return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
					SendDlgItemMessage(hDlg, INP_INPSTR, WM_GETTEXT, DialogData->MaxLen, (LPARAM)DialogData->Str);
					DialogData->Anonymous = SendDlgItemMessage(hDlg, INP_ANONYMOUS, BM_GETCHECK, 0, 0);
					EndDialog(hDlg, YES);
					break;

				case IDCANCEL :
					EndDialog(hDlg, NO);
					break;

				case IDHELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, HelpPage);
					break;

				case INP_BROWSE :
					if(SelectDir(hDlg, Tmp, FMAX_PATH) == TRUE)
						SendDlgItemMessage(hDlg, INP_INPSTR, WM_SETTEXT, 0, (LPARAM)Tmp);
					break;
			}
            return(TRUE);
	}
	return(FALSE);
}


/*----- �m���s�n�Ɓm����n�����̃_�C�A���O�̋��ʃR�[���o�b�N�֐� --------------
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

BOOL CALLBACK ExeEscDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
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
			}
			return(TRUE);
	}
    return(FALSE);
}


/*----- �m���s�n�Ɓm����n�����̃_�C�A���O�̋��ʃR�[���o�b�N�֐�(�e�L�X�g�\����)
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

BOOL CALLBACK ExeEscTextDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, COMMON_TEXT, WM_SETTEXT, 0, lParam);
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


/*----- ������̍Ō�� "\" ��t���� -------------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�I���W�i���̕����� char *Str ���ύX����܂��B
*----------------------------------------------------------------------------*/

void SetYenTail(char *Str)
{
	if(_mbscmp(_mbsninc(Str, _mbslen(Str) - 1), "\\") != 0)
		strcat(Str, "\\");

	return;;
}


/*----- ������̍Ō�� "\" ����菜�� -----------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�I���W�i���̕����� char *Str ���ύX����܂��B
*----------------------------------------------------------------------------*/

void RemoveYenTail(char *Str)
{
	char *Pos;

	if(strlen(Str) > 0)
	{
		Pos = _mbsninc(Str, _mbslen(Str) - 1);
		if(_mbscmp(Pos, "\\") == 0)
			*Pos = NUL;
	}
	return;;
}


/*----- ������̍Ō�� "/" ��t���� -------------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�I���W�i���̕����� char *Str ���ύX����܂��B
*----------------------------------------------------------------------------*/

void SetSlashTail(char *Str)
{
	if(_mbscmp(_mbsninc(Str, _mbslen(Str) - 1), "/") != 0)
		strcat(Str, "/");

	return;
}


/*----- �����񂩂���s�R�[�h����菜�� ----------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�I���W�i���̕����� char *Str ���ύX����܂��B
*----------------------------------------------------------------------------*/

void RemoveReturnCode(char *Str)
{
	char *Pos;

	if((Pos = strchr(Str, 0x0D)) != NULL)
		*Pos = NUL;
	if((Pos = strchr(Str, 0x0A)) != NULL)
		*Pos = NUL;
	return;
}


/*----- ��������̓���̕�����S�Ēu�������� ----------------------------------
*
*	Parameter
*		char *Str : ������
*		char Src : ��������
*		char Dst : �u������
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ReplaceAll(char *Str, char Src, char Dst)
{
	char *Pos;

	while((Pos = _mbschr(Str, Src)) != NULL)
		*Pos = Dst;
	return;
}


/*----- �����������͓���̂P�������`�F�b�N ------------------------------------
*
*	Parameter
*		int Ch : �`�F�b�N���镶��
*		int Sym : �L��
*
*	Return Value
*		int �X�e�[�^�X
*			0=�����ł�����̋L���ł��Ȃ�
*----------------------------------------------------------------------------*/

int IsDigitSym(int Ch, int Sym)
{
	int Ret;

	if((Ret = IsDigit(Ch)) == 0)
	{
		if((Sym != NUL) && (Sym == Ch))
			Ret = 1;
	}
	return(Ret);
}


/*----- �����񂪑S�ē����������`�F�b�N ----------------------------------------
*
*	Parameter
*		char *Str : ������
*		int Ch : ����
*
*	Return Value
*		int �X�e�[�^�X
*			YES/NO
*----------------------------------------------------------------------------*/

int StrAllSameChar(char *Str, char Ch)
{
	int Ret;

	Ret = YES;
	while(*Str != NUL)
	{
		if(*Str != Ch)
		{
			Ret = NO;
			break;
		}
		Str++;
	}
	return(Ret);
}


/*----- ������̖����̃X�y�[�X���폜 ------------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void RemoveTailingSpaces(char *Str)
{
	char *Pos;

	Pos = Str + strlen(Str);
	while(--Pos > Str)
	{
		if(*Pos != ' ')
			break;
		*Pos = NUL;
	}
	return;
}


/*----- �啶���^����������ʂ��Ȃ�strstr --------------------------------------
*
*	Parameter
*		char *s1 : ������P
*		char *s2 : ������Q
*
*	Return Value
*		char *������P���ŕ�����Q�����������ʒu
*			NULL=������Ȃ�����
*----------------------------------------------------------------------------*/

char *stristr(char *s1, char *s2)
{
	char *Ret;

	Ret = NULL;
	while(*s1 != NUL)
	{
		if((tolower(*s1) == tolower(*s2)) &&
		   (_strnicmp(s1, s2, strlen(s2)) == 0))
		{
			Ret = s1;
			break;
		}
		s1++;
	}
	return(Ret);
}


/*----- �����񒆂̃X�y�[�X�ŋ�؂�ꂽ���̃t�B�[���h��Ԃ� --------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		char *���̃t�B�[���h
*			NULL=������Ȃ�����
*----------------------------------------------------------------------------*/

char *GetNextField(char *Str)
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
	return(Str);
}


/*----- ���݂̃t�B�[���h�̕�������R�s�[���� ----------------------------------
*
*	Parameter
*		char *Str : ������
*		char *Buf : �R�s�[��
*		int Max : �ő啶����
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL=��������������
*----------------------------------------------------------------------------*/

int GetOneField(char *Str, char *Buf, int Max)
{
	int Sts;
	char *Pos;

	Sts = FAIL;
	if((Pos = strchr(Str, ' ')) == NULL)
	{
		if((int)strlen(Str) <= Max)
		{
			strcpy(Buf, Str);
			Sts = SUCCESS;
		}
	}
	else
	{
		if(Pos - Str <= Max)
		{
			strncpy(Buf, Str, Pos - Str);
			*(Buf + (Pos - Str)) = NUL;
			Sts = SUCCESS;
		}
	}
	return(Sts);
}


/*----- �J���}����菜�� ------------------------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void RemoveComma(char *Str)
{
	char *Put;

	Put = Str;
	while(*Str != NUL)
	{
		if(*Str != ',')
		{
			*Put = *Str;
			Put++;
		}
		Str++;
	}
	*Put = NUL;
	return;
}


/*----- �p�X���̒��̃t�@�C�����̐擪��Ԃ� ------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		char *�t�@�C�����̐擪
*
*	Note
*		�f�B���N�g���̋�؂�L���� "\" �� "/" �̗������L��
*----------------------------------------------------------------------------*/

char *GetFileName(char *Path)
{
	char *Pos;

	if((Pos = _mbschr(Path, ':')) != NULL)
		Path = Pos + 1;

	if((Pos = _mbsrchr(Path, '\\')) != NULL)
		Path = Pos + 1;

	if((Pos = _mbsrchr(Path, '/')) != NULL)
		Path = Pos + 1;

	return(Path);
}


/*----- �c�[���̕\������Ԃ� --------------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		char * : �\����
*----------------------------------------------------------------------------*/

char *GetToolName(char *Path)
{
	char *Pos;

	if((Pos = _mbschr(Path, ':')) != NULL)
		Path = Pos + 1;

	if((Pos = _mbsrchr(Path, '\\')) != NULL)
		Path = Pos + 1;

	return(Path);
}


/*----- �p�X���̒��̊g���q�̐擪��Ԃ� ----------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		char *�g���q�̐擪
*----------------------------------------------------------------------------*/

char *GetFileExt(char *Path)
{
	char *Ret;

	Ret = _mbschr(Path, NUL);
	if((_mbscmp(Path, ".") != 0) &&
	   (_mbscmp(Path, "..") != 0))
	{
		while((Path = _mbschr(Path, '.')) != NULL)
		{
			Path++;
			Ret = Path;
		}
	}
	return(Ret);
}


/*----- �p�X������t�@�C��������菜�� ----------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*		char *Buf : �t�@�C�������������p�X���̃R�s�[��
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�f�B���N�g���̋�؂�L���� "\" �� "/" �̗������L��
*----------------------------------------------------------------------------*/

void RemoveFileName(char *Path, char *Buf)
{
	char *Pos;

	strcpy(Buf, Path);

	if((Pos = _mbsrchr(Buf, '/')) != NULL)
		*Pos = NUL;
	else if((Pos = _mbsrchr(Buf, '\\')) != NULL)
	{
		if((Pos == Buf) || 
		   ((Pos != Buf) && (*(Pos - 1) != ':')))
			*Pos = NUL;
	}
	return;
}


/*----- ��ʃf�B���N�g���̃p�X�����擾 ----------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�f�B���N�g���̋�؂�L���� "\" �� "/" �̗������L��
*		�ŏ��� "\"��"/"�͎c��
*			��j "/pub"   --> "/"
*			��j "C:\DOS" --> "C:\"
*----------------------------------------------------------------------------*/

void GetUpperDir(char *Path)
{
	char *Top;
	char *Pos;

	if(((Top = _mbschr(Path, '/')) != NULL) ||
	   ((Top = _mbschr(Path, '\\')) != NULL))
	{
		Top++;
		if(((Pos = _mbsrchr(Top, '/')) != NULL) ||
		   ((Pos = _mbsrchr(Top, '\\')) != NULL))
			*Pos = NUL;
		else
			*Top = NUL;
	}
	return;
}


/*----- ��ʃf�B���N�g���̃p�X�����擾 ----------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�f�B���N�g���̋�؂�L���� "\" �� "/" �̗������L��
*		�ŏ��� "\"��"/"������
*			��j "/pub"   --> ""
*			��j "C:\DOS" --> "C:"
*----------------------------------------------------------------------------*/

void GetUpperDirEraseTopSlash(char *Path)
{
	char *Pos;

	if(((Pos = _mbsrchr(Path, '/')) != NULL) ||
	   ((Pos = _mbsrchr(Path, '\\')) != NULL))
		*Pos = NUL;
	else
		*Path = NUL;

	return;
}


/*----- �f�B���N�g���̊K�w����Ԃ� --------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�P�� '\' �� '/'�̐���Ԃ�����
*----------------------------------------------------------------------------*/

int AskDirLevel(char *Path)
{
	char *Pos;
	int Level;

	Level = 0;
	while(((Pos = _mbschr(Path, '/')) != NULL) ||
		  ((Pos = _mbschr(Path, '\\')) != NULL))
	{
		Path = Pos + 1;
		Level++;
	}
	return(Level);
}


/*----- �t�@�C���T�C�Y�𕶎���ɕϊ����� --------------------------------------
*
*	Parameter
*		double Size : �t�@�C���T�C�Y
*		char *Buf : �������Ԃ��o�b�t�@
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void MakeSizeString(double Size, char *Buf)
{
	if(Size >= (1024*1024))
	{
		Size /= (1024*1024);
		sprintf(Buf, "%.2fM Bytes", Size);
	}
	else if (Size >= 1024)
	{
		Size /= 1024;
		sprintf(Buf, "%.2fK Bytes", Size);
	}
	else
		sprintf(Buf, "%.0f Bytes", Size);

	return;
}


/*----- StaticText�̗̈�Ɏ��܂�悤�Ƀp�X���𐮌`���ĕ\�� --------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		char *Str : ������ (������FMAX_PATH�ȉ�)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispStaticText(HWND hWnd, char *Str)
{
	char Buf[FMAX_PATH+1];
	char *Pos;
	char *Tmp;
	RECT Rect;
	SIZE fSize;
	HDC hDC;
	int Force;

	GetClientRect(hWnd, &Rect);
	Rect.right -= Rect.left;

	hDC = GetDC(hWnd);
	strcpy(Buf, Str);
	Pos = Buf;
	Force = NO;
	while(Force == NO)
	{
		GetTextExtentPoint32(hDC, Pos, strlen(Pos), &fSize);

		if(fSize.cx <= Rect.right)
			break;

		if(_mbslen(Pos) <= 4)
			Force = YES;
		else
		{
			Pos = _mbsninc(Pos, 4);
			if((Tmp = _mbschr(Pos, '\\')) == NULL)
				Tmp = _mbschr(Pos, '/');

			if(Tmp == NULL)
				Tmp = _mbsninc(Pos, 4);

			Pos = Tmp - 3;
			memset(Pos, '.', 3);
		}
	}
	ReleaseDC(hWnd, hDC);

	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)Pos);
	return;
}


/*----- ������A���C�̒��������߂� --------------------------------------------
*
*	Parameter
*		char *Str : ������A���C (������NUL�Q��)
*
*	Return Value
*		int ����
*
*	Note
*		�I�[��2��NUL�̂����̍Ō�̕��͐����Ȃ�
*			StrMultiLen("") = 0
*			StrMultiLen("abc\0xyz\0") = 8
*			StrMultiLen("abc") = �I�[���Q��NUL�łȂ��̂ŋ��߂��Ȃ�
*----------------------------------------------------------------------------*/

int StrMultiLen(char *Str)
{
	int Len;
	int Tmp;

	Len = 0;
	while(*Str != NUL)
	{
		Tmp = strlen(Str) + 1;
		Str += Tmp;
		Len += Tmp;
	}
	return(Len);
}


/*----- RECT���N���C�A���g���W����X�N���[�����W�ɕϊ� ------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		RECT *Rect : RECT
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void RectClientToScreen(HWND hWnd, RECT *Rect)
{
	POINT Tmp;

	Tmp.x = Rect->left;
	Tmp.y = Rect->top;
	ClientToScreen(hWnd, &Tmp);
	Rect->left = Tmp.x;
	Rect->top = Tmp.y;

	Tmp.x = Rect->right;
	Tmp.y = Rect->bottom;
	ClientToScreen(hWnd, &Tmp);
	Rect->right = Tmp.x;
	Rect->bottom = Tmp.y;

	return;
}


/*----- 16�i�������o�C�i���ɕϊ� ----------------------------------------------
*
*	Parameter
*		char Ch : 16�i����
*
*	Return Value
*		int �o�C�i���l
*----------------------------------------------------------------------------*/

int hex2bin(char Ch)
{
	int Ret;

	if((Ch >= '0') && (Ch <= '9'))
		Ret = Ch - '0';
	else if((Ch >= 'A') && (Ch <= 'F'))
		Ret = Ch - 'A' + 10;
	else if((Ch >= 'a') && (Ch <= 'f'))
		Ret = Ch - 'a' + 10;

	return(Ret);
}


/*----- �t�m�b������𕪉����� ------------------------------------------------
*
*	Parameter
*		char *unc : UNC������
*		char *Host : �z�X�g�����R�s�[����o�b�t�@ (�T�C�Y�� HOST_ADRS_LEN+1)
*		char *Path : �p�X�����R�s�[����o�b�t�@ (�T�C�Y�� FMAX_PATH+1)
*		char *File : �t�@�C�������R�s�[����o�b�t�@ (�T�C�Y�� FMAX_PATH+1)
*		char *User : ���[�U�����R�s�[����o�b�t�@ (�T�C�Y�� USER_NAME_LEN+1)
*		char *Pass : �p�X���[�h���R�s�[����o�b�t�@ (�T�C�Y�� PASSWORD_LEN+1)
*		int *Port : �|�[�g�ԍ����R�s�[����o�b�t�@
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*
*	"\"�͑S��"/"�ɒu��������
*----------------------------------------------------------------------------*/

int SplitUNCpath(char *unc, char *Host, char *Path, char *File, char *User, char *Pass, int *Port)
{
	int Sts;
	char *Pos1;
	char *Pos2;
	char Tmp[FMAX_PATH+1];

	memset(Host, NUL, HOST_ADRS_LEN+1);
	memset(Path, NUL, FMAX_PATH+1);
	memset(File, NUL, FMAX_PATH+1);
	memset(User, NUL, USER_NAME_LEN+1);
	memset(Pass, NUL, PASSWORD_LEN+1);
	*Port = PORT_NOR;

	ReplaceAll(unc, '\\', '/');

	if((Pos1 = _mbsstr(unc, "//")) != NULL)
		Pos1 += 2;
	else
		Pos1 = unc;

	if((Pos2 = _mbschr(Pos1, '@')) != NULL)
	{
		memset(Tmp, NUL, FMAX_PATH+1);
		memcpy(Tmp, Pos1, Pos2-Pos1);
		Pos1 = Pos2 + 1;

		if((Pos2 = _mbschr(Tmp, ':')) != NULL)
		{
			memcpy(User, Tmp, min1(Pos2-Tmp, USER_NAME_LEN));
			strncpy(Pass, Pos2+1, PASSWORD_LEN);
		}
		else
			strncpy(User, Tmp, USER_NAME_LEN);
	}

	if((Pos2 = _mbschr(Pos1, ':')) != NULL)
	{
		memcpy(Host, Pos1, min1(Pos2-Pos1, HOST_ADRS_LEN));
		Pos2++;
		if(IsDigit(*Pos2))
		{
			*Port = atoi(Pos2);
			while(*Pos2 != NUL)
			{
				if(IsDigit(*Pos2) == 0)
					break;
				Pos2++;
			}
		}
		RemoveFileName(Pos2, Path);
		strncpy(File, GetFileName(Pos2), FMAX_PATH);
	}
	else if((Pos2 = _mbschr(Pos1, '/')) != NULL)
	{
		memcpy(Host, Pos1, min1(Pos2-Pos1, HOST_ADRS_LEN));
		RemoveFileName(Pos2, Path);
		strncpy(File, GetFileName(Pos2), FMAX_PATH);
	}
	else
	{
		strncpy(Host, Pos1, HOST_ADRS_LEN);
	}

	Sts = FAIL;
	if(strlen(Host) > 0)
		Sts = SUCCESS;

	return(Sts);
}


/*----- ���t������(JST)��FILETIME(UTC)�ɕϊ� ----------------------------------
*
*	Parameter
*		char *Time : ���t������ ("yyyy/mm/dd hh:mm")
*		FILETIME *Buf : �t�@�C���^�C����Ԃ����[�N
*
*	Return Value
*		int �X�e�[�^�X
*			YES/NO=���t��񂪂Ȃ�����
*----------------------------------------------------------------------------*/

int TimeString2FileTime(char *Time, FILETIME *Buf)
{
	SYSTEMTIME sTime;
	FILETIME fTime;
	int Ret;

	Ret = NO;
    Buf->dwLowDateTime = 0;
    Buf->dwHighDateTime = 0;

	if(strlen(Time) >= 16)
	{
		if(IsDigit(Time[0]) && IsDigit(Time[5]) && IsDigit(Time[8]) && 
		   IsDigit(Time[12]) && IsDigit(Time[14]))
		{
			Ret = YES;
		}

		sTime.wYear = atoi(Time);
		sTime.wMonth = atoi(Time + 5);
		sTime.wDay = atoi(Time + 8);
		if(Time[11] != ' ')
			sTime.wHour = atoi(Time + 11);
		else
			sTime.wHour = atoi(Time + 12);
		sTime.wMinute = atoi(Time + 14);
		sTime.wSecond = 0;
		sTime.wMilliseconds = 0;

		SystemTimeToFileTime(&sTime, &fTime);
		LocalFileTimeToFileTime(&fTime, Buf);
	}
	return(Ret);
}


/*----- FILETIME(UTC)����t������(JST)�ɕϊ� ----------------------------------
*
*	Parameter
*		FILETIME *Time : �t�@�C���^�C��
*		char *Buf : ���t�������Ԃ����[�N
*		int Mode : ���[�h (DISPFORM_xxx)
*		int InfoExist : ��񂪂��邩�ǂ��� (FINFO_xxx)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void FileTime2TimeString(FILETIME *Time, char *Buf, int Mode, int InfoExist)
{
	SYSTEMTIME sTime;
	FILETIME fTime;

	if(Mode == DISPFORM_LEGACY)
	{
		if((Time->dwLowDateTime == 0) && (Time->dwHighDateTime == 0))
			InfoExist = 0;

		/* "yyyy/mm/dd hh:mm" */
		FileTimeToLocalFileTime(Time, &fTime);
		FileTimeToSystemTime(&fTime, &sTime);

		if(InfoExist & FINFO_DATE)
			sprintf(Buf, "%04d/%02d/%02d ", sTime.wYear, sTime.wMonth, sTime.wDay);
		else
			sprintf(Buf, "           ");

		if(InfoExist & FINFO_TIME)
			sprintf(Buf+11, "%2d:%02d", sTime.wHour, sTime.wMinute);
		else
			sprintf(Buf+11, "     ");
	}
	else
	{
//		if (!strftime((char *)str, 100, "%c",  (const struct tm *)thetime))
//			SetTaskMsg("strftime �����s���܂���!\n");
	}
	return;
}


/*----- �t�@�C���^�C�����w��^�C���]�[���̃��[�J���^�C������GMT�ɕϊ� ---------
*
*	Parameter
*		FILETIME *Time : �t�@�C���^�C��
*		int TimeZone : �^�C���]�[��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SpecificLocalFileTime2FileTime(FILETIME *Time, int TimeZone)
{
	unsigned __int64 Tmp64;

	Tmp64 = (unsigned __int64)Time->dwLowDateTime +
			((unsigned __int64)Time->dwHighDateTime << 32);

	Tmp64 -= (__int64)TimeZone * (__int64)36000000000;

	Time->dwHighDateTime = (DWORD)(Tmp64 >> 32);
	Time->dwLowDateTime = (DWORD)(Tmp64 & 0xFFFFFFFF);

	return;
}


/*----- �����������l�ɕϊ� --------------------------------------------------
*
*	Parameter
*		char *Str : ���������� ("rwxrwxrwx")
*
*	Return Value
*		int �l
*----------------------------------------------------------------------------*/

int AttrString2Value(char *Str)
{
	int Ret;
	char Tmp[10];

	Ret = 0;
	memset(Tmp, 0, 10);
	strncpy(Tmp, Str, 9);

	if(Tmp[0] != '-')
		Ret |= 0x400;
	if(Tmp[1] != '-')
		Ret |= 0x200;
	if(Tmp[2] != '-')
		Ret |= 0x100;

	if(Tmp[3] != '-')
		Ret |= 0x40;
	if(Tmp[4] != '-')
		Ret |= 0x20;
	if(Tmp[5] != '-')
		Ret |= 0x10;

	if(Tmp[6] != '-')
		Ret |= 0x4;
	if(Tmp[7] != '-')
		Ret |= 0x2;
	if(Tmp[8] != '-')
		Ret |= 0x1;

	return(Ret);
}


/*----- �����̒l�𕶎���ɕϊ� ------------------------------------------------
*
*	Parameter
*		int Attr : �����̒l
*		char *Buf : ������������Z�b�g����o�b�t�@ ("rwxrwxrwx")
*
*	Return Value
*		int �l
*----------------------------------------------------------------------------*/

void AttrValue2String(int Attr, char *Buf)
{
	strcpy(Buf, "---------");

	if(Attr & 0x400)
		Buf[0] = 'r';
	if(Attr & 0x200)
		Buf[1] = 'w';
	if(Attr & 0x100)
		Buf[2] = 'x';

	if(Attr & 0x40)
		Buf[3] = 'r';
	if(Attr & 0x20)
		Buf[4] = 'w';
	if(Attr & 0x10)
		Buf[5] = 'x';

	if(Attr & 0x4)
		Buf[6] = 'r';
	if(Attr & 0x2)
		Buf[7] = 'w';
	if(Attr & 0x1)
		Buf[8] = 'x';

	return;
}


/*----- INI�t�@�C��������𐮌` -----------------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void FormatIniString(char *Str)
{
	char *Put;

	Put = Str;
	while(*Str != NUL)
	{
		if((*Str != ' ') && (*Str != '\t') && (*Str != '\n'))
			*Put++ = *Str;
		if(*Str++ == '=')
			break;
	}

	while(*Str != NUL)
	{
		if((*Str != 0x22) && (*Str != '\n'))
			*Put++ = *Str;
		Str++;
	}
	*Put = NUL;

	return;
}


/*----- �t�@�C���I�� ----------------------------------------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		char *Fname : �t�@�C������Ԃ��o�b�t�@
*		char *Title : �^�C�g��
*		char *Filters : �t�B���^�[������
*		char *Ext : �f�t�H���g�g���q
*		int Flags : �ǉ�����t���O
*		int Save : �u�J���v���u�ۑ��v�� (0=�J��, 1=�ۑ�)
*
*	Return Value
*		int �X�e�[�^�X
*			TRUE/FALSE=���
*----------------------------------------------------------------------------*/

int SelectFile(HWND hWnd, char *Fname, char *Title, char *Filters, char *Ext, int Flags, int Save)
{
	OPENFILENAME OpenFile;
	char Tmp[FMAX_PATH+1];
	char Cur[FMAX_PATH+1];
	int Sts;

	GetCurrentDirectory(FMAX_PATH, Cur);

	strcpy(Tmp, Fname);
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = hWnd;
	OpenFile.hInstance = 0;
	OpenFile.lpstrFilter = Filters;
	OpenFile.lpstrCustomFilter = NULL;
	OpenFile.nFilterIndex = 1;
	OpenFile.lpstrFile = Tmp;
	OpenFile.nMaxFile = FMAX_PATH;
	OpenFile.lpstrFileTitle = NULL;
	OpenFile.nMaxFileTitle = 0;
	OpenFile.lpstrInitialDir = NULL;
	OpenFile.lpstrTitle = Title;
	OpenFile.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | Flags;
	OpenFile.nFileOffset = 0;
	OpenFile.nFileExtension = 0;
	OpenFile.lpstrDefExt = Ext;
	OpenFile.lCustData = 0;
	OpenFile.lpfnHook = NULL;
	OpenFile.lpTemplateName = NULL;

	if(Save == 0)
	{
		if((Sts = GetOpenFileName(&OpenFile)) == TRUE)
			strcpy(Fname,Tmp);
	}
	else
	{
		if((Sts = GetSaveFileName(&OpenFile)) == TRUE)
			strcpy(Fname,Tmp);
	}
	SetCurrentDirectory(Cur);
	return(Sts);
}


/*----- �f�B���N�g����I�� ----------------------------------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		char *Buf : �f�B���N�g������Ԃ��o�b�t�@�i�����f�B���N�g�����j
*		int MaxLen : �o�b�t�@�̃T�C�Y
*
*	Return Value
*		int �X�e�[�^�X
*			TRUE/FALSE=���
*----------------------------------------------------------------------------*/

int SelectDir(HWND hWnd, char *Buf, int MaxLen)
{
	char Tmp[FMAX_PATH+1];
	char Cur[FMAX_PATH+1];
	BROWSEINFO  Binfo;
	LPITEMIDLIST lpIdll;
	int Sts;
	LPMALLOC lpMalloc;

	Sts = FALSE;
	GetCurrentDirectory(FMAX_PATH, Cur);

	if(SHGetMalloc(&lpMalloc) == NOERROR)
	{
		Binfo.hwndOwner = hWnd;
		Binfo.pidlRoot = NULL;
		Binfo.pszDisplayName = Tmp;
		Binfo.lpszTitle = MSGJPN185;
		Binfo.ulFlags = BIF_RETURNONLYFSDIRS;
		Binfo.lpfn = NULL;
		Binfo.lParam = 0;
		Binfo.iImage = 0;
		if((lpIdll = SHBrowseForFolder(&Binfo)) != NULL)
		{
			SHGetPathFromIDList(lpIdll, Tmp);
			memset(Buf, NUL, MaxLen);
			strncpy(Buf, Tmp, MaxLen-1);
			Sts = TRUE;
			lpMalloc->lpVtbl->Free(lpMalloc, lpIdll);
	    }
	    lpMalloc->lpVtbl->Release(lpMalloc);
		SetCurrentDirectory(Cur);
	}
	return(Sts);
}


/*----- �l�Ɋ֘A�t����ꂽ���W�I�{�^�����`�F�b�N���� --------------------------
*
*	Parameter
*		HWND hDlg : �_�C�A���O�{�b�N�X�̃E�C���h�E�n���h��
*		int Value : �l
*		const RADIOBUTTON *Buttons : ���W�I�{�^���ƒl�̊֘A�t���e�[�u��
*		int Num : �{�^���̐�
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�l�Ɋ֘A�t����ꂽ�{�^�����������́A�e�[�u���̍ŏ��ɓo�^����Ă���{�^
*		�����`�F�b�N����
*----------------------------------------------------------------------------*/

void SetRadioButtonByValue(HWND hDlg, int Value, const RADIOBUTTON *Buttons, int Num)
{
	int i;
	int Def;

	Def = Buttons->ButID;
	for(i = 0; i < Num; i++)
	{
		if(Value == Buttons->Value)
		{
			SendDlgItemMessage(hDlg, Buttons->ButID, BM_SETCHECK, 1, 0);
			/* ���W�I�{�^����ύX�������ɑ��̍��ڂ̃n�C�h�Ȃǂ��s�Ȃ�����	*/
			/* ����̂ŁA���̂��߂� WM_COMMAND �𑗂� 						*/
			SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(Buttons->ButID, 0), 0);
			break;
		}
		Buttons++;
	}
	if(i == Num)
	{
		SendDlgItemMessage(hDlg, Def, BM_SETCHECK, 1, 0);
		SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(Def, 0), 0);
	}
	return;
}


/*----- �`�F�b�N����Ă���{�^���Ɋ֘A�t����ꂽ�l��Ԃ� ----------------------
*
*	Parameter
*		HWND hDlg : �_�C�A���O�{�b�N�X�̃E�C���h�E�n���h��
*		const RADIOBUTTON *Buttons : ���W�I�{�^���ƒl�̊֘A�t���e�[�u��
*		int Num : �{�^���̐�
*
*	Return Value
*		int �l
*
*	Note
*		�ǂ̃{�^�����`�F�b�N����Ă��Ȃ����́A�e�[�u���̍ŏ��ɓo�^����Ă���{
*		�^���̒l��Ԃ�
*----------------------------------------------------------------------------*/

int AskRadioButtonValue(HWND hDlg, const RADIOBUTTON *Buttons, int Num)
{
	int i;
	int Ret;

	Ret = Buttons->Value;
	for(i = 0; i < Num; i++)
	{
		if(SendDlgItemMessage(hDlg, Buttons->ButID, BM_GETCHECK, 0, 0) == 1)
		{
			Ret = Buttons->Value;
			break;
		}
		Buttons++;
	}
	return(Ret);
}


/*----- �P�U�i������𐔒l�ɕϊ� ----------------------------------------------
*
*	Parameter
*		char *Str : ������
*
*	Return Value
*		int �l
*----------------------------------------------------------------------------*/

int xtoi(char *Str)
{
	int Ret;

	Ret = 0;
	while(*Str != NUL)
	{
		Ret *= 0x10;
		if((*Str >= '0') && (*Str <= '9'))
			Ret += *Str - '0';
		else if((*Str >= 'A') && (*Str <= 'F'))
			Ret += *Str - 'A' + 10;
		else if((*Str >= 'a') && (*Str <= 'f'))
			Ret += *Str - 'a' + 10;
		else
			break;

		Str++;
	}
	return(Ret);
}


/*----- �t�@�C�����ǂݎ��\���ǂ�����Ԃ� ----------------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int CheckFileReadable(char *Fname)
{
	int Sts;
	HANDLE iFileHandle;
	SECURITY_ATTRIBUTES Sec;

	Sts = FAIL;

	Sec.nLength = sizeof(SECURITY_ATTRIBUTES);
	Sec.lpSecurityDescriptor = NULL;
	Sec.bInheritHandle = FALSE;

	if((iFileHandle = CreateFile(Fname, GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE, &Sec, OPEN_EXISTING, 0, NULL)) != INVALID_HANDLE_VALUE)
	{
		Sts = SUCCESS;
		CloseHandle(iFileHandle);
	}
	return(Sts);
}





int max1(int n, int m)
{
	if(n > m)
		return(n);
	else
		return(m);
}



int min1(int n, int m)
{
	if(n < m)
		return(n);
	else
		return(m);
}


void ExcEndianDWORD(DWORD *x)
{
	BYTE *Pos;
	BYTE Tmp;

	Pos = (BYTE *)x;
	Tmp = *(Pos + 0);
	*(Pos + 0) = *(Pos + 3);
	*(Pos + 3) = Tmp;
	Tmp = *(Pos + 1);
	*(Pos + 1) = *(Pos + 2);
	*(Pos + 2) = Tmp;
	return;
}




/*----- int�l�̓���ւ� -------------------------------------------------------
*
*	Parameter
*		int *Num1 : ���l�P
*		int *Num2 : ���l�Q
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SwapInt(int *Num1, int *Num2)
{
	int Tmp;

	Tmp = *Num1;
	*Num1 = *Num2;
	*Num2 = Tmp;
	return;
}


/*----- �w�肳�ꂽ�t�H���_�����邩�ǂ����`�F�b�N -------------------------------
*
*	Parameter
*		char *Path : �p�X
*
*	Return Value
*		int �X�e�[�^�X (YES/NO)
*----------------------------------------------------------------------------*/

int IsFolderExist(char *Path)
{
	int Sts;
	char Tmp[FMAX_PATH+1];
	DWORD Attr;

	Sts = YES;
	if(strlen(Path) > 0)
	{
		strcpy(Tmp, Path);
		if(_mbscmp(Tmp+1, ":\\") != 0)
			RemoveYenTail(Tmp);

		Attr = GetFileAttributes(Tmp);
		if((Attr == 0xFFFFFFFF) || ((Attr & FILE_ATTRIBUTE_DIRECTORY) == 0))
			Sts = NO;
	}
	return(Sts);
}


/*----- �e�[�u���ɂ��������Đ��l��o�^ -----------------------------------------
*
*	Parameter
*		int x : ���l
*		int Dir : �ϊ�����
*		INTCONVTBL *Tbl : �e�[�u��
*		int Num : �e�[�u���̐��l�̐�
*
*	Return Value
*		int ���l
*----------------------------------------------------------------------------*/

int ConvertNum(int x, int Dir, const INTCONVTBL *Tbl, int Num)
{
	int i;
	int Ret;

	Ret = x;
	for(i = 0; i < Num; i++)
	{
		if((Dir == 0) && (Tbl->Num1 == x))
		{
			Ret = Tbl->Num2;
			break;
		}
		else if((Dir == 1) && (Tbl->Num2 == x))
		{
			Ret = Tbl->Num1;
			break;
		}
		Tbl++;
	}
	return(Ret);
}






/*----- �t�@�C�����S�~���ɍ폜 ------------------------------------------------
*
*	Parameter
*		char *Path : �t�@�C����
*
*	Return Value
*		int �X�e�[�^�X (0=����I��)
*----------------------------------------------------------------------------*/

int MoveFileToTrashCan(char *Path)
{
	SHFILEOPSTRUCT FileOp;
	char Tmp[FMAX_PATH+2];

	memset(Tmp, 0, FMAX_PATH+2);
	strcpy(Tmp, Path);
	FileOp.hwnd = NULL;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = Tmp;
	FileOp.pTo = "";
	FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_ALLOWUNDO;
	FileOp.lpszProgressTitle = "";
	return(SHFileOperation(&FileOp));
}




LONGLONG MakeLongLong(DWORD High, DWORD Low)
{
	LONGLONG z;
	LONGLONG x1, y1;

	x1 = (LONGLONG)Low;
	y1 = (LONGLONG)High;
	z = x1 | (y1 << 32);
	return(z);
}


char *MakeNumString(LONGLONG Num, char *Buf, BOOL Comma)
{
	int i;
	char *Pos;

	Pos = Buf;
	*Pos = '\0';

	i = 1;
	do
	{
		*Pos++ = (char)(Num % 10) + '0';
		Num /= 10;
		if((Comma == TRUE) && ((i % 3) == 0) && (Num != 0))
			*Pos++ = ',';
		i++;
	}
	while(Num != 0);
	*Pos = NUL;
	_strrev(Buf);

	return(Buf);
}


