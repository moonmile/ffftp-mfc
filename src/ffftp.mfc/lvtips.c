/*=============================================================================
*
*							���X�g�r���[�e�B�b�v�X
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

/* ���̃\�[�X�� MFC Programmer's SourceBook (http://www.codeguru.com/)���Q�l�ɂ��܂��� */

#define  STRICT
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windowsx.h>
#include <commctrl.h>

#include "common.h"
#include "resource.h"


/*===== �v���g�^�C�v =====*/

static void TipsShow(HWND hWnd, RECT rectTitle, LPCTSTR lpszTitleText, int xoffset, int xoffset2, int InRect);
static int CellRectFromPoint(HWND hWnd, POINT  point, RECT *cellrect, int *col);
static LRESULT CALLBACK TitleTipWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*===== ���[�J���ȃ��[�N =====*/

static HWND hWndTips;	/* tips�̃E�C���h�E�n���h�� */



/*----- ���X�g�r���[�e�B�b�v�X�̃E�C���h�E���쐬 ------------------------------
*
*	Parameter
*		HWND hWnd : �e�E�C���h�E�̃n���h��
*		HINSTANCE hInst : �C���X�^���X�n���h��
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int InitListViewTips(HWND hWnd, HINSTANCE hInst)
{
	WNDCLASSEX wClass;
	int Ret;

	Ret = FAIL;

	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.style         = 0;
	wClass.lpfnWndProc   = TitleTipWndProc;
	wClass.cbClsExtra    = 0;
	wClass.cbWndExtra    = 0;
	wClass.hInstance     = hInst;
	wClass.hIcon         = NULL;
	wClass.hCursor       = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(GetSysColor(COLOR_INFOBK));
	wClass.lpszMenuName  = NULL;
	wClass.lpszClassName = "XTitleTip";
	wClass.hIconSm       = NULL;
	RegisterClassEx(&wClass);

	hWndTips = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
				"XTitleTip", NULL,
				WS_BORDER | WS_POPUP,
				0, 0, 0, 0,
				hWnd, NULL, hInst, NULL);

	if(hWndTips != NULL)
		Ret = SUCCESS;

	return(Ret);
}


/*----- ���X�g�r���[�e�B�b�v�X�̃E�C���h�E���폜 ------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DeleteListViewTips(void)
{
	if(hWndTips != NULL)
		DestroyWindow(hWndTips);
	return;
}


/*----- ���X�g�r���[�e�B�b�v�X�̃E�C���h�E������ ------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void EraseListViewTips(void)
{
	ReleaseCapture();
    ShowWindow(hWndTips, SW_HIDE);
	return;
}


/*----- ���X�g�r���[�e�B�b�v�X�̃E�C���h�E�n���h����Ԃ� ----------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		HWND �E�C���h�E�n���h��
*----------------------------------------------------------------------------*/

HWND GetListViewTipsHwnd(void)
{
	return(hWndTips);
}


/*----- ���X�g�r���[�e�B�b�v�X�̕\���`�F�b�N ----------------------------------
*
*	Parameter
*		HWND hWnd : ListView�̃E�C���h�E�n���h��
*		LPARAM lParam : WM_MOUSEMOVE��LPARAM�l
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void CheckTipsDisplay(HWND hWnd, LPARAM lParam)
{
	RECT cellrect;
	static RECT cur_rect;
	static int InRect;
	POINT Point;
	int row, col;
	int offset;
	int offset2;
	RECT rcLabel;
	char Buf[256];

	Point.x = LOWORD(lParam);
	Point.y = HIWORD(lParam);
	if(InRect == NO)
	{
		row = CellRectFromPoint(hWnd, Point, &cellrect, &col);
        if(row != -1)
        {
			cur_rect=cellrect;
			offset = 6;
			offset2 = offset;
			if( col == 0 ) 
			{
				ListView_GetItemRect(hWnd,  row, &rcLabel, LVIR_LABEL);
				offset = rcLabel.left - cellrect.left + offset / 2;
				offset2 = 1;
			}
			cellrect.top--;
			strcpy(Buf, "");
			ListView_GetItemText(hWnd,  row, col, Buf, 256 );
			if(strlen(Buf) > 0)
				TipsShow(hWnd, cellrect, Buf, offset-1, offset2-1, InRect);
			InRect = YES;
		}
	}
	else
	{
		if(PtInRect(&cur_rect, Point) == FALSE)
		{
			EraseListViewTips();
			InRect = NO;
		}
	}
	return;
}


/*----- ���X�g�r���[�e�B�b�v�X��\�� ------------------------------------------
*
*	Parameter
*		HWND hWnd : ListView�̃E�C���h�E�̃n���h��
*		RECT rectTitle : ListView�R���g���[���̃A�C�e���̋�`
*		LPCTSTR lpszTitleText : ������
*		int xoffset : �I�t�Z�b�g
*		int xoffset2 : �I�t�Z�b�g�Q
*		int InRect : �\�����邩�ǂ���
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void TipsShow(HWND hWnd, RECT rectTitle, LPCTSTR lpszTitleText, int xoffset, int xoffset2, int InRect)
{
	HDC dc;
	HFONT pFont;
    HFONT pFontDC;
	RECT rectDisplay;
	SIZE size;

	if(InRect == NO)
	{
        if(GetFocus() != NULL)
		{
			RectClientToScreen(hWnd, &rectTitle);

			/* ListView�E�C���h�E�̃t�H���g�𓾂� */
			dc = GetDC(hWnd);
			pFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
			ReleaseDC(hWnd, dc);

			dc = GetDC(hWndTips);
			pFontDC = SelectObject(dc, pFont);

			SetTextColor(dc, GetSysColor(COLOR_INFOTEXT));
			SetBkMode(dc, TRANSPARENT);

			rectDisplay = rectTitle;
			GetTextExtentPoint32(dc, lpszTitleText, strlen(lpszTitleText), &size);
			rectDisplay.left += xoffset;
			rectDisplay.right = rectDisplay.left + size.cx + 2;

			if(rectDisplay.right > rectTitle.right-xoffset2)
			{
				rectDisplay.right += 1;

		        SetWindowPos(hWndTips, HWND_TOPMOST, 
					rectDisplay.left, rectDisplay.top, 
					rectDisplay.right - rectDisplay.left, 
					rectDisplay.bottom - rectDisplay.top, 
					SWP_SHOWWINDOW|SWP_NOACTIVATE );

				TextOut(dc, 0, 0, lpszTitleText, strlen(lpszTitleText));

		        SetCapture(hWnd);
			}

			SelectObject(dc, pFontDC);
			ReleaseDC(hWndTips, dc);
		}
	}
	return;
}


/*----- ���X�g�r���[�e�B�b�v�X��\������ʒu��Ԃ� ----------------------------
*
*	Parameter
*		HWND hWnd : ListView�̃E�C���h�E�n���h��
*		POINT point : �J�[�\���̈ʒu
*		RECT *cellrect : �A�C�e���̋�`��Ԃ����[�N
*		int *col : ���ԍ���Ԃ����[�N
*
*	Return Value
*		int �s�ԍ�
*			-1=�Y���Ȃ�
*----------------------------------------------------------------------------*/

static int CellRectFromPoint(HWND hWnd, POINT point, RECT *cellrect, int *col)
{
	int colnum;
	int nColumnCount;
	RECT rect;
	RECT rectClient;
	int row;
	int bottom;
	int colwidth;
	int Ret;

	Ret = -1;
	if((GetWindowLong(hWnd, GWL_STYLE) & LVS_TYPEMASK) == LVS_REPORT )
	{
		row = ListView_GetTopIndex(hWnd);
		bottom = row + ListView_GetCountPerPage(hWnd);
		if(bottom > ListView_GetItemCount(hWnd))
			bottom = ListView_GetItemCount(hWnd);

		nColumnCount = Header_GetItemCount(GetDlgItem(hWnd, 0));

		for(; row <= bottom; row++)
		{
			ListView_GetItemRect(hWnd, row, &rect, LVIR_BOUNDS);
			if(PtInRect(&rect, point))
			{
				for(colnum = 0; colnum < nColumnCount; colnum++)
				{
					colwidth = ListView_GetColumnWidth(hWnd, colnum);
					if((point.x >= rect.left) && 
					   (point.x < (rect.left + colwidth)))
					{
						GetClientRect(hWnd, &rectClient);
						if(point.x <= rectClient.right)
						{
							rect.right = rect.left + colwidth;
							if(rect.right > rectClient.right)
								rect.right = rectClient.right;
							*cellrect = rect;
    						*col = colnum;
							Ret = row;
							break;
						}
					}
					rect.left += colwidth;
				}
				break;
			}
		}
	}
	return(Ret);
}


/*----- ���X�g�r���[�e�B�b�v�X�E�C���h�E�̃R�[���o�b�N ------------------------
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

static LRESULT CALLBACK TitleTipWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return(DefWindowProc(hWnd, message, wParam, lParam));
}


