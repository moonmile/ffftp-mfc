/*=============================================================================
*
*								�z�X�g�ꗗ
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
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commctrl.h>
#include <windowsx.h>

#include "common.h"
#include "resource.h"

#include <htmlhelp.h>
#include "helpid.h"


/*===== �v���g�^�C�v =====*/

static BOOL CALLBACK SelectHostProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK HostListWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static HOSTLISTDATA *GetNextNode(HOSTLISTDATA *Pos);
static int GetNodeLevel(int Num);
static int GetNodeLevelByData(HOSTLISTDATA *Data);
static int GetNodeNumByData(HOSTLISTDATA *Data);
static HOSTLISTDATA *GetNodeByNum(int Num);
static int SetNodeLevelAll(void);
static int UpdateHostToList(int Num, HOSTDATA *Set);
static int DelHostFromList(int Num);
static int DeleteChildAndNext(HOSTLISTDATA *Pos);
static void SendAllHostNames(HWND hWnd, int Cur);
static int IsNodeGroup(int Num);
static int DispHostSetDlg(HWND hDlg);
static BOOL CALLBACK MainSettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static BOOL CALLBACK AdvSettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static BOOL CALLBACK CodeSettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static BOOL CALLBACK DialupSettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
static BOOL CALLBACK Adv2SettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

/*===== �O���Q�� =====*/

extern HWND hHelpWin;

/* �ݒ�l */
extern char UserMailAdrs[USER_MAIL_LEN+1];
extern HFONT ListFont;
extern char DefaultLocalPath[FMAX_PATH+1];
extern int ConnectAndSet;
extern SIZE HostDlgSize;

/*===== ���[�J���ȃ��[�N =====*/

static int Hosts = 0;						/* �z�X�g�� */
static int ConnectingHost;					/* �ڑ����̃z�X�g */
static int CurrentHost;						/* �J�[�\���ʒu�̃z�X�g */
static HOSTLISTDATA *HostListTop = NULL;	/* �z�X�g�ꗗ�f�[�^ */
static HOSTDATA TmpHost;					/* �z�X�g���R�s�[�p */
static int Apply;							/* �v���p�e�B�V�[�g��OK���������t���O */
static WNDPROC HostListProcPtr;



/*----- �z�X�g�ꗗ�E�C���h�E --------------------------------------------------
*
*	Parameter
*		int Type : �_�C�A���O�̃^�C�v (DLG_TYPE_xxx)
*
*	Return Value
*		�X�e�[�^�X (YES=���s/NO=���)
*----------------------------------------------------------------------------*/

int SelectHost(int Type)
{
	int Sts;
	int Dlg;

	Dlg = hostconnect_dlg;
	if((ConnectAndSet == YES) || (Type == DLG_TYPE_SET))
		Dlg = hostlist_dlg;

	Sts = DialogBox(GetFtpInst(), MAKEINTRESOURCE(Dlg), GetMainHwnd(), SelectHostProc);

	/* �z�X�g�ݒ��ۑ� */
	SetNodeLevelAll();
	SaveRegistory();

	return(Sts);
}


/*----- �z�X�g�ꗗ�E�C���h�E�̃R�[���o�b�N ------------------------------------
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

static BOOL CALLBACK SelectHostProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static DIALOGSIZE DlgSize = {
		{ HOST_NEW, HOST_FOLDER, HOST_SET, HOST_COPY, HOST_DEL, HOST_DOWN, HOST_UP, IDHELP, HOST_SIZEGRIP, -1 },
		{ IDOK, IDCANCEL, HOST_SIZEGRIP, -1 },
		{ HOST_LIST, -1 },
		{ 0, 0 },
		{ 0, 0 }
	};
	static HIMAGELIST hImage;
	HTREEITEM hItem;
	TV_ITEM Item;
	int Level1;
	int Level2;
	HOSTLISTDATA *Data1;
	HOSTLISTDATA *Data2;
	NM_TREEVIEW *tView;
	HTREEITEM tViewPos;
	TV_HITTESTINFO HitInfo;

	switch (message)
	{
		case WM_INITDIALOG :
			/* TreeView�ł̃_�u���N���b�N�����܂��邽�� */
			HostListProcPtr = (WNDPROC)SetWindowLong(GetDlgItem(hDlg, HOST_LIST), GWL_WNDPROC, (LONG)HostListWndProc);


//		SetClassLong(hDlg, GCL_HICON, (LONG)LoadIcon(GetFtpInst(), MAKEINTRESOURCE(ffftp)));

			if(AskConnecting() == YES)
			{
				/* �ڑ����́u�ύX�v�̂݋��� */
				EnableWindow(GetDlgItem(hDlg, HOST_NEW), FALSE);
				EnableWindow(GetDlgItem(hDlg, HOST_FOLDER), FALSE);
				EnableWindow(GetDlgItem(hDlg, HOST_COPY), FALSE);
				EnableWindow(GetDlgItem(hDlg, HOST_DEL), FALSE);
				EnableWindow(GetDlgItem(hDlg, HOST_DOWN), FALSE);
				EnableWindow(GetDlgItem(hDlg, HOST_UP), FALSE);
			}
			if(ListFont != NULL)
				SendDlgItemMessage(hDlg, HOST_LIST, WM_SETFONT, (WPARAM)ListFont, MAKELPARAM(TRUE, 0));
			hImage = ImageList_LoadBitmap(GetFtpInst(), MAKEINTRESOURCE(hlist_bmp), 16, 8, RGB(255,0,0));
			SendDlgItemMessage(hDlg, HOST_LIST, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImage);
			CurrentHost = 0;
			if(ConnectingHost >= 0)
				CurrentHost = ConnectingHost;
			SendAllHostNames(GetDlgItem(hDlg, HOST_LIST), CurrentHost);
			DlgSizeInit(hDlg, &DlgSize, &HostDlgSize);
		    return(TRUE);

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK :
					if((hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0)) != NULL)
					{
						Item.hItem = hItem;
						Item.mask = TVIF_PARAM;
						SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);
						CurrentHost = Item.lParam;
						ConnectingHost = CurrentHost;
						AskDlgSize(hDlg, &DlgSize, &HostDlgSize);
						ImageList_Destroy(hImage);
						EndDialog(hDlg, YES);
						break;
					}
					/* ������break�͂Ȃ� */

				case IDCANCEL :
					AskDlgSize(hDlg, &DlgSize, &HostDlgSize);
					ImageList_Destroy(hImage);
					EndDialog(hDlg, NO);
					break;

				case HOST_NEW :
					CopyDefaultHost(&TmpHost);
					if(DispHostSetDlg(hDlg) == YES)
					{
						if((hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0)) != NULL)
						{
							Item.hItem = hItem;
							Item.mask = TVIF_PARAM;
							SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);

							TmpHost.Level = GetNodeLevel(Item.lParam);
							Level1 = Item.lParam + 1;
							CurrentHost = Level1;
						}
						else
						{
							TmpHost.Level = 0;
							Level1 = -1;
							CurrentHost = Hosts;
						}
						AddHostToList(&TmpHost, Level1, SET_LEVEL_SAME);
						SendAllHostNames(GetDlgItem(hDlg, HOST_LIST), CurrentHost);
					}
					break;

				case HOST_FOLDER :
					CopyDefaultHost(&TmpHost);
					if(InputDialogBox(group_dlg, hDlg, NULL, TmpHost.HostName, HOST_NAME_LEN+1, &Level1, IDH_HELP_TOPIC_0000001) == YES)
					{
						if((hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0)) != NULL)
						{
							Item.hItem = hItem;
							Item.mask = TVIF_PARAM;
							SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);

							TmpHost.Level = GetNodeLevel(Item.lParam) | SET_LEVEL_GROUP ;
							Level1 = Item.lParam + 1;
							CurrentHost = Level1;
						}
						else
						{
							TmpHost.Level = 0 | SET_LEVEL_GROUP;
							Level1 = -1;
							CurrentHost = Hosts;
						}
						AddHostToList(&TmpHost, Level1, SET_LEVEL_SAME);
						SendAllHostNames(GetDlgItem(hDlg, HOST_LIST), CurrentHost);
					}
					break;

				case HOST_SET :
					if((hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0)) != NULL)
					{
						Item.hItem = hItem;
						Item.mask = TVIF_PARAM;
						SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);
						CurrentHost = Item.lParam;

						CopyHostFromList(CurrentHost, &TmpHost);
						Level1 = IsNodeGroup(CurrentHost);
						if(((Level1 == NO) && (DispHostSetDlg(hDlg) == YES)) ||
						   ((Level1 == YES) && (InputDialogBox(group_dlg, hDlg, NULL, TmpHost.HostName, HOST_NAME_LEN+1, &Level1, IDH_HELP_TOPIC_0000001) == YES)))
						{
							UpdateHostToList(CurrentHost, &TmpHost);
							SendAllHostNames(GetDlgItem(hDlg, HOST_LIST), CurrentHost);
						}
					}
					break;

				case HOST_COPY :
					if((hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0)) != NULL)
					{
						Item.hItem = hItem;
						Item.mask = TVIF_PARAM;
						SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);
						CurrentHost = Item.lParam;

						CopyHostFromList(CurrentHost, &TmpHost);
						strcpy(TmpHost.BookMark, "\0");
						CurrentHost++;
						AddHostToList(&TmpHost, CurrentHost, SET_LEVEL_SAME);
						SendAllHostNames(GetDlgItem(hDlg, HOST_LIST), CurrentHost);
					}
					break;

				case HOST_DEL :
					if((hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0)) != NULL)
					{
						Item.hItem = hItem;
						Item.mask = TVIF_PARAM;
						SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);
						CurrentHost = Item.lParam;
						Level1 = IsNodeGroup(CurrentHost);

						if(((Level1 == YES) && (DialogBox(GetFtpInst(), MAKEINTRESOURCE(groupdel_dlg), GetMainHwnd(), ExeEscDialogProc) == YES)) ||
						   ((Level1 == NO) && (DialogBox(GetFtpInst(), MAKEINTRESOURCE(hostdel_dlg), GetMainHwnd(), ExeEscDialogProc) == YES)))
						{
							DelHostFromList(CurrentHost);
							if(CurrentHost >= Hosts)
								CurrentHost = max1(0, Hosts-1);
							SendAllHostNames(GetDlgItem(hDlg, HOST_LIST), CurrentHost);
						}
					}
					break;

				case HOST_UP :
					if((hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0)) != NULL)
					{
						Item.hItem = hItem;
						Item.mask = TVIF_PARAM;
						SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);
						CurrentHost = Item.lParam;

						if(CurrentHost > 0)
						{
							Data1 = HostListTop;
							for(Level1 = CurrentHost; Level1 > 0; Level1--)
								Data1 = GetNextNode(Data1);
							Level1 = GetNodeLevel(CurrentHost);

							Data2 = HostListTop;
							for(Level2 = CurrentHost-1; Level2 > 0; Level2--)
								Data2 = GetNextNode(Data2);
							Level2 = GetNodeLevel(CurrentHost-1);

							if((Level1 == Level2) && (Data2->Set.Level & SET_LEVEL_GROUP))
							{
								//Data2��child��
								if(Data1->Next != NULL)
									Data1->Next->Prev = Data1->Prev;
								if(Data1->Prev != NULL)
									Data1->Prev->Next = Data1->Next;
								if((Data1->Parent != NULL) && (Data1->Parent->Child == Data1))
									Data1->Parent->Child = Data1->Next;
								if((Data1->Parent == NULL) && (HostListTop == Data1))
									HostListTop = Data1->Next;

								Data1->Next = Data2->Child;
								Data1->Prev = NULL;
								Data1->Parent = Data2;
								Data2->Child = Data1;
							}
							else if(Level1 < Level2)
							{
								//Data1��Prev��Child��Next�̖�����
								Data2 = Data1->Prev->Child;
								while(Data2->Next != NULL)
									Data2 = Data2->Next;

								if(Data1->Next != NULL)
									Data1->Next->Prev = Data1->Prev;
								if(Data1->Prev != NULL)
									Data1->Prev->Next = Data1->Next;
								if((Data1->Parent != NULL) && (Data1->Parent->Child == Data1))
									Data1->Parent->Child = Data1->Next;
								if((Data1->Parent == NULL) && (HostListTop == Data1))
									HostListTop = Data1->Next;

								Data2->Next = Data1;
								Data1->Prev = Data2;
								Data1->Next = NULL;
								Data1->Parent = Data2->Parent;
							}
							else
							{
								//Data2��prev��
								if(Data1->Next != NULL)
									Data1->Next->Prev = Data1->Prev;
								if(Data1->Prev != NULL)
									Data1->Prev->Next = Data1->Next;
								if((Data1->Parent != NULL) && (Data1->Parent->Child == Data1))
									Data1->Parent->Child = Data1->Next;
								if((Data1->Parent == NULL) && (HostListTop == Data1))
									HostListTop = Data1->Next;

								if(Data2->Prev != NULL)
									Data2->Prev->Next = Data1;
								Data1->Prev = Data2->Prev;
								Data2->Prev = Data1;
								Data1->Next = Data2;
								Data1->Parent = Data2->Parent;

								if((Data1->Parent != NULL) && (Data1->Parent->Child == Data2))
									Data1->Parent->Child = Data1;
								if((Data1->Parent == NULL) && (HostListTop == Data2))
									HostListTop = Data1;
							}

							CurrentHost = GetNodeNumByData(Data1);
							SendAllHostNames(GetDlgItem(hDlg, HOST_LIST), CurrentHost);
						}
					}
					break;

				case HOST_DOWN :
					if((hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0)) != NULL)
					{
						Item.hItem = hItem;
						Item.mask = TVIF_PARAM;
						SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);
						CurrentHost = Item.lParam;

						Data1 = HostListTop;
						for(Level1 = CurrentHost; Level1 > 0; Level1--)
							Data1 = GetNextNode(Data1);
						Level1 = GetNodeLevel(CurrentHost);

						Data2 = NULL;
						Level2 = SET_LEVEL_SAME;
						if(CurrentHost < Hosts-1)
						{
							Data2 = HostListTop;
							for(Level2 = CurrentHost+1; Level2 > 0; Level2--)
								Data2 = GetNextNode(Data2);
							Level2 = GetNodeLevel(CurrentHost+1);

							if(Level1 < Level2)
							{
								if(Data1->Next != NULL)
								{
									//Data2 = Data1��Next
									Data2 = Data1->Next;
									Level2 = GetNodeLevelByData(Data2);
								}
								else if(Data1->Parent != NULL)
								{
									Data2 = NULL;
									Level2 = SET_LEVEL_SAME;
								}
							}
						}

						if(((Data2 == NULL) && (Level1 > 0)) ||
						   (Level1 > Level2))
						{
							//Data1��Parent��Next��
							Data2 = Data1->Parent;

							if(Data1->Next != NULL)
								Data1->Next->Prev = Data1->Prev;
							if(Data1->Prev != NULL)
								Data1->Prev->Next = Data1->Next;
							if((Data1->Parent != NULL) && (Data1->Parent->Child == Data1))
								Data1->Parent->Child = Data1->Next;
							if((Data1->Parent == NULL) && (HostListTop == Data1))
								HostListTop = Data1->Next;

							if(Data2->Next != NULL)
								Data2->Next->Prev = Data1;
							Data1->Next = Data2->Next;
							Data2->Next = Data1;
							Data1->Prev = Data2;
							Data1->Parent = Data2->Parent;

							if((Data1->Parent != NULL) && (Data1->Parent->Child == Data1))
								Data1->Parent->Child = Data2;
							if((Data1->Parent == NULL) && (HostListTop == Data1))
								HostListTop = Data2;
						}
						else if(Level1 == Level2)
						{
							if(Data2->Set.Level & SET_LEVEL_GROUP)
							{
								//Data2��Child��
								if(Data1->Next != NULL)
									Data1->Next->Prev = Data1->Prev;
								if(Data1->Prev != NULL)
									Data1->Prev->Next = Data1->Next;
								if((Data1->Parent != NULL) && (Data1->Parent->Child == Data1))
									Data1->Parent->Child = Data1->Next;
								if((Data1->Parent == NULL) && (HostListTop == Data1))
									HostListTop = Data1->Next;

								if(Data2->Child != NULL)
									Data2->Child->Prev = Data1;
								Data1->Next = Data2->Child;
								Data1->Prev = NULL;
								Data1->Parent = Data2;
								Data2->Child = Data1;
							}
							else
							{
								//Data2��Next��
								if(Data1->Next != NULL)
									Data1->Next->Prev = Data1->Prev;
								if(Data1->Prev != NULL)
									Data1->Prev->Next = Data1->Next;
								if((Data1->Parent != NULL) && (Data1->Parent->Child == Data1))
									Data1->Parent->Child = Data1->Next;
								if((Data1->Parent == NULL) && (HostListTop == Data1))
									HostListTop = Data1->Next;

								if(Data2->Next != NULL)
									Data2->Next->Prev = Data1;
								Data1->Next = Data2->Next;
								Data2->Next = Data1;
								Data1->Prev = Data2;
								Data1->Parent = Data2->Parent;

								if((Data1->Parent != NULL) && (Data1->Parent->Child == Data1))
									Data1->Parent->Child = Data2;
								if((Data1->Parent == NULL) && (HostListTop == Data1))
									HostListTop = Data2;
							}
						}

						CurrentHost = GetNodeNumByData(Data1);
						SendAllHostNames(GetDlgItem(hDlg, HOST_LIST), CurrentHost);
					}
					break;

				case HOST_LIST :
					if(HIWORD(wParam) == LBN_DBLCLK)
						PostMessage(hDlg, WM_COMMAND, MAKEWORD(IDOK, 0), 0);
					break;

				case IDHELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000027);
					break;
			}
			SetFocus(GetDlgItem(hDlg, HOST_LIST));
			return(TRUE);

		case WM_SIZING :
			DlgSizeChange(hDlg, &DlgSize, (RECT *)lParam, (int)wParam);
		    return(TRUE);

		case WM_SELECT_HOST :
			HitInfo.pt.x = LOWORD(lParam);
			HitInfo.pt.y = HIWORD(lParam);
			HitInfo.flags = TVHT_ONITEM;
			hItem = (HTREEITEM)SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETNEXTITEM, TVGN_CARET, 0);
			HitInfo.hItem = hItem;
			if((HTREEITEM)SendMessage(GetDlgItem(hDlg, HOST_LIST), TVM_HITTEST, 0, (LPARAM)&HitInfo) == hItem)
			{
				if(IsWindowEnabled(GetDlgItem(hDlg, IDOK)) == TRUE)
					PostMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDOK, 0), 0);
			}
			break;

		case WM_NOTIFY:
			tView = (NM_TREEVIEW FAR *)lParam;
			switch(tView->hdr.idFrom)
			{
				case HOST_LIST :
					tViewPos = tView->itemNew.hItem;
					hItem = tView->itemNew.hItem;
					switch(tView->hdr.code)
					{
						case TVN_SELCHANGED :
							/* �t�H���_���I�΂ꂽ�Ƃ��͐ڑ��A�R�s�[�{�^���͋֎~ */
							Item.hItem = hItem;
							Item.mask = TVIF_PARAM;
							SendDlgItemMessage(hDlg, HOST_LIST, TVM_GETITEM, TVGN_CARET, (LPARAM)&Item);
							if(IsNodeGroup(Item.lParam) == YES)
							{
								EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
								EnableWindow(GetDlgItem(hDlg, HOST_COPY), FALSE);
							}
							else
							{
								EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
								if(AskConnecting() == NO)
									EnableWindow(GetDlgItem(hDlg, HOST_COPY), TRUE);
							}
							break;
					}
					break;
			}
			break;
	}
    return(FALSE);
}


/*----- �z�X�g�ꗗTreeView�̃��b�Z�[�W���� ------------------------------------
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

static LRESULT CALLBACK HostListWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_LBUTTONDBLCLK :
			PostMessage(GetParent(hWnd), WM_SELECT_HOST, 0, lParam);
			break;
	}
	return(CallWindowProc(HostListProcPtr, hWnd, message, wParam, lParam));
}


/*----- ���̐ݒ�ԍ��̃m�[�h��Ԃ� --------------------------------------------
*
*	Parameter
*		HOSTLISTDATA *Pos : �m�[�h�f�[�^
*
*	Return Value
*		HOSTLISTDATA *���̃m�[�h
*			NULL=���͂Ȃ�
*----------------------------------------------------------------------------*/

static HOSTLISTDATA *GetNextNode(HOSTLISTDATA *Pos)
{
	HOSTLISTDATA *Ret;

	Ret = NULL;
	if(Pos->Child != NULL)
		Ret = Pos->Child;
	else
	{
		if(Pos->Next != NULL)
			Ret = Pos->Next;
		else
		{
			while(Pos->Parent != NULL)
			{
				Pos = Pos->Parent;
				if(Pos->Next != NULL)
				{
					Ret = Pos->Next;
					break;
				}
			}
		}
	}
	return(Ret);
}


/*----- �m�[�h�̃��x������Ԃ��i�ݒ�ԍ��w��j --------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*
*	Return Value
*		int ���x���� (-1=�ݒ肪�Ȃ��j
*----------------------------------------------------------------------------*/

static int GetNodeLevel(int Num)
{
	int Ret;
	HOSTLISTDATA *Pos;

	Ret = -1;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		Ret = 0;
		while(Pos->Parent != NULL)
		{
			Pos = Pos->Parent;
			Ret++;
		}
	}
	return(Ret);
}


/*----- �m�[�h�̃��x������Ԃ��i�m�[�h�f�[�^�w��j-----------------------------
*
*	Parameter
*		HOSTLISTDATA *Data : �ݒ�l
*
*	Return Value
*		int ���x����
*----------------------------------------------------------------------------*/

static int GetNodeLevelByData(HOSTLISTDATA *Data)
{
	int Ret;

	Ret = 0;
	while(Data->Parent != NULL)
	{
		Data = Data->Parent;
		Ret++;
	}
	return(Ret);
}


/*----- �m�[�h�̐ݒ�ԍ���Ԃ� ------------------------------------------------
*
*	Parameter
*		HOSTLISTDATA *Data : �ݒ�l
*
*	Return Value
*		int �ݒ�ԍ�
*----------------------------------------------------------------------------*/

static int GetNodeNumByData(HOSTLISTDATA *Data)
{
	int Ret;
	HOSTLISTDATA *Pos;

	Ret = 0;
	Pos = HostListTop;
	while(Pos != Data)
	{
		Pos = GetNextNode(Pos);
		Ret++;
	}
	return(Ret);
}


/*----- �w��ԍ��̃m�[�h��Ԃ� ------------------------------------------------
*
*	Parameter
*		int Num : �ݒ�ԍ�
*
*	Return Value
*		HOSTLISTDATA * : �ݒ�l
*----------------------------------------------------------------------------*/

static HOSTLISTDATA *GetNodeByNum(int Num)
{
	HOSTLISTDATA *Pos;

	Pos = HostListTop;
	for(; Num > 0; Num--)
		Pos = GetNextNode(Pos);

	return(Pos);
}


/*----- �ݒ�l���X�g�̊e�m�[�h�̃��x���ԍ����Z�b�g ----------------------------
*
*	Parameter
*		int Num : �ݒ�ԍ�
*
*	Return Value
*		HOSTLISTDATA * : �ݒ�l
*----------------------------------------------------------------------------*/

static int SetNodeLevelAll(void)
{
	HOSTLISTDATA *Pos;
	int i;

	Pos = HostListTop;
	for(i = 0; i < Hosts; i++)
	{
		Pos->Set.Level &= ~SET_LEVEL_MASK;
		Pos->Set.Level |= GetNodeLevelByData(Pos);
		Pos = GetNextNode(Pos);
	}
	return(SUCCESS);
}


/*----- �ݒ�l���X�g�ɒǉ� ----------------------------------------------------
*
*	Parameter
*		HOSTDATA *Set : �ǉ�����ݒ�l
*		int Pos : �ǉ�����ʒu (0�` : -1=�Ō�)
*		int Level : ���x���� (SET_LEVEL_SAME=�ǉ��ʒu�̂��̂Ɠ����x��)
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int AddHostToList(HOSTDATA *Set, int Pos, int Level)
{
	int Sts;
	HOSTLISTDATA *New;
	HOSTLISTDATA *Last;
	int Cur;

	Sts = FAIL;
	if((Pos >= -1) && (Pos <= Hosts))
	{
		if(Pos == -1)
			Pos = Hosts;
		Level &= SET_LEVEL_MASK;

		if((New = malloc(sizeof(HOSTLISTDATA))) != NULL)
		{
			memcpy(&New->Set, Set, sizeof(HOSTDATA));
			New->Next = NULL;
			New->Prev = NULL;
			New->Child = NULL;
			New->Parent = NULL;

			if(HostListTop == NULL)
			{
				if(Pos == 0)
					HostListTop = New;
			}
			else
			{
				if(Pos == 0)
				{
					New->Next = HostListTop;
					HostListTop = New;
				}
				else
				{
					Cur = GetNodeLevel(Pos-1);
					Last = HostListTop;
					for(Pos--; Pos > 0; Pos--)
						Last = GetNextNode(Last);
					if((Level != SET_LEVEL_SAME) && (Level > Cur))
					{
						New->Next = Last->Child;
						New->Parent = Last;
						Last->Child = New;
						if(New->Next != NULL)
							New->Next->Prev = New;
					}
					else
					{
						if((Level >= 0) && (Level < SET_LEVEL_SAME))
						{
							for(; Level < Cur; Cur--)
								Last = Last->Parent;
						}
						New->Prev = Last;
						New->Next = Last->Next;
						New->Parent = Last->Parent;
						Last->Next = New;
						if(New->Next != NULL)
							New->Next->Prev = New;
					}
				}
			}
			Hosts++;
			Sts = SUCCESS;
		}
	}
	return(Sts);
}


/*----- �ݒ�l���X�g���X�V���� ------------------------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*		HOSTDATA *Set : �ݒ�l���R�s�[���郏�[�N
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

static int UpdateHostToList(int Num, HOSTDATA *Set)
{
	int Sts;
	HOSTLISTDATA *Pos;

	Sts = FAIL;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		memcpy(&Pos->Set, Set, sizeof(HOSTDATA));
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �ݒ�l���X�g����폜 --------------------------------------------------
*
*	Parameter
*		int Num : �폜����ԍ�
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

static int DelHostFromList(int Num)
{
	int Sts;
	HOSTLISTDATA *Pos;

	Sts = FAIL;
	if((Num >= 0) && (Num < Hosts))
	{
		if(Num == 0)
		{
			Pos = HostListTop;
			if(Pos->Child != NULL)
				DeleteChildAndNext(Pos->Child);
			HostListTop = Pos->Next;
		}
		else
		{
			Pos = GetNodeByNum(Num);
			if(Pos->Child != NULL)
				DeleteChildAndNext(Pos->Child);

			if(Pos->Next != NULL)
				Pos->Next->Prev = Pos->Prev;
			if(Pos->Prev != NULL)
				Pos->Prev->Next = Pos->Next;
			if((Pos->Parent != NULL) && (Pos->Parent->Child == Pos))
				Pos->Parent->Child = Pos->Next;
		}
		free(Pos);
		Hosts--;
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �ݒ�l���X�g����m�[�h�f�[�^���폜 ------------------------------------
*
*	Parameter
*		HOSTLISTDATA *Pos : �폜����m�[�h
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*
*	Note
*		Pos->Next, Pos->Child�̑S�Ẵm�[�h���폜����
*----------------------------------------------------------------------------*/

static int DeleteChildAndNext(HOSTLISTDATA *Pos)
{
	HOSTLISTDATA *Next;

	while(Pos != NULL)
	{
		if(Pos->Child != NULL)
			DeleteChildAndNext(Pos->Child);

		Next = Pos->Next;
		free(Pos);
		Hosts--;
		Pos = Next;
	}
	return(SUCCESS);
}


/*----- �ݒ�l���X�g����ݒ�l�����o�� --------------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*		HOSTDATA *Set : �ݒ�l���R�s�[���郏�[�N
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*
*	Note
*		���݃z�X�g�ɐڑ����̎��́ACopyHostFromListInConnect() ���g����
*----------------------------------------------------------------------------*/

int CopyHostFromList(int Num, HOSTDATA *Set)
{
	int Sts;
	HOSTLISTDATA *Pos;

	Sts = FAIL;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		memcpy(Set, &Pos->Set, sizeof(HOSTDATA));
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �ݒ�l���X�g����ݒ�l�����o�� --------------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*		HOSTDATA *Set : �ݒ�l���R�s�[���郏�[�N
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*
*	Note
*		���݃z�X�g�ɐڑ����̎��Ɏg��
*----------------------------------------------------------------------------*/

int CopyHostFromListInConnect(int Num, HOSTDATA *Set)
{
	int Sts;
	HOSTLISTDATA *Pos;

	Sts = FAIL;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		strcpy(Set->ChmodCmd, Pos->Set.ChmodCmd);
		Set->Port = Pos->Set.Port;
		Set->Anonymous = Pos->Set.Anonymous;
		Set->KanjiCode = Pos->Set.KanjiCode;
		Set->KanaCnv = Pos->Set.KanaCnv;
		Set->NameKanjiCode = Pos->Set.NameKanjiCode;
		Set->NameKanaCnv = Pos->Set.NameKanaCnv;
		Set->Pasv = Pos->Set.Pasv;
		Set->FireWall = Pos->Set.FireWall;
		Set->ListCmdOnly = Pos->Set.ListCmdOnly;
		Set->UseNLST_R = Pos->Set.UseNLST_R;
		Set->LastDir = Pos->Set.LastDir;
		Set->TimeZone = Pos->Set.TimeZone;
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �ݒ�l���X�g�̃u�b�N�}�[�N���X�V --------------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*		char *Bmask : �u�b�N�}�[�N������
*		int Len : �u�b�N�}�[�N������̒���
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int SetHostBookMark(int Num, char *Bmask, int Len)
{
	int Sts;
	HOSTLISTDATA *Pos;

	Sts = FAIL;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		memcpy(Pos->Set.BookMark, Bmask, Len);
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �ݒ�l���X�g�̃u�b�N�}�[�N�������Ԃ� --------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*
*	Return Value
*		char *�u�b�N�}�[�N������
*----------------------------------------------------------------------------*/

char *AskHostBookMark(int Num)
{
	char *Ret;
	HOSTLISTDATA *Pos;

	Ret = NULL;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		Ret = Pos->Set.BookMark;
	}
	return(Ret);
}


/*----- �ݒ�l���X�g�̃f�B���N�g�����X�V --------------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*		char *LocDir : ���[�J���̃f�B���N�g��
*		char *HostDir : �z�X�g�̃f�B���N�g��
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int SetHostDir(int Num, char *LocDir, char *HostDir)
{
	int Sts;
	HOSTLISTDATA *Pos;

	Sts = FAIL;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		strcpy(Pos->Set.LocalInitDir, LocDir);
		strcpy(Pos->Set.RemoteInitDir, HostDir);
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �ݒ�l���X�g�̃p�X���[�h���X�V ----------------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*		char *Pass : �p�X���[�h
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int SetHostPassword(int Num, char *Pass)
{
	int Sts;
	HOSTLISTDATA *Pos;

	Sts = FAIL;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		strcpy(Pos->Set.PassWord, Pass);
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �w��̐ݒ薼�����ݒ�̔ԍ���Ԃ� ------------------------------------
*
*	Parameter
*		char *Name : �ݒ薼
*
*	Return Value
*		int �ݒ�ԍ� (0�`)
*			-1=������Ȃ�
*----------------------------------------------------------------------------*/

int SearchHostName(char *Name)
{
	int Ret;
	int i;
	HOSTLISTDATA *Pos;

	Ret = -1;
	Pos = HostListTop;
	for(i = 0; i < Hosts; i++)
	{
		if(strcmp(Name, Pos->Set.HostName) == 0)
		{
			Ret = i;
			break;
		}
		Pos = GetNextNode(Pos);
	}
	return(Ret);
}


/*----- �ݒ�l���X�g�̃\�[�g���@���X�V ----------------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*		int LFSort : ���[�J���̃t�@�C���̃\�[�g���@
*		int LDSort : ���[�J���̃t�H���_�̃\�[�g���@
*		int RFSort : �����[�g�̃t�@�C���̃\�[�g���@
*		int RDSort : �����[�g�̃t�H���_�̃\�[�g���@
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int SetHostSort(int Num, int LFSort, int LDSort, int RFSort, int RDSort)
{
	int Sts;
	HOSTLISTDATA *Pos;

	Sts = FAIL;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		Pos->Set.Sort = LFSort * 0x1000000 | LDSort * 0x10000 | RFSort * 0x100 | RDSort;
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �o�^����Ă���\�[�g���@�𕪉����� ------------------------------------
*
*	Parameter
*		ulong Sort : �\�[�g���@ 
*		int *LFSort : ���[�J���̃t�@�C���̃\�[�g���@���i�[���郏�[�N
*		int *LDSort : ���[�J���̃t�H���_�̃\�[�g���@���i�[���郏�[�N
*		int *RFSort : �����[�g�̃t�@�C���̃\�[�g���@���i�[���郏�[�N
*		int *RDSort : �����[�g�̃t�H���_�̃\�[�g���@���i�[���郏�[�N
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DecomposeSortType(ulong Sort, int *LFSort, int *LDSort, int *RFSort, int *RDSort)
{
	*LFSort = (int)((Sort / 0x1000000) & 0xFF);
	*LDSort = (int)((Sort / 0x10000) & 0xFF);
	*RFSort = (int)((Sort / 0x100) & 0xFF);
	*RDSort = (int)(Sort & 0xFF);
	return;
}


/*----- ���ݐڑ����̐ݒ�ԍ���Ԃ� --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �ݒ�ԍ�
*----------------------------------------------------------------------------*/

int AskCurrentHost(void)
{
	return(ConnectingHost);
}


/*----- ���ݐڑ����̐ݒ�ԍ����Z�b�g���� --------------------------------------
*
*	Parameter
*		int Num : �ݒ�ԍ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetCurrentHost(int Num)
{
	ConnectingHost = Num;
	return;
}


/*----- �f�t�H���g�ݒ�l�����o�� --------------------------------------------
*
*	Parameter
*		HOSTDATA *Set : �ݒ�l���R�s�[���郏�[�N
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void CopyDefaultHost(HOSTDATA *Set)
{
	Set->Level = 0;
	strcpy(Set->HostName, "");
	strcpy(Set->HostAdrs, "");
	strcpy(Set->UserName, "");
	strcpy(Set->PassWord, "");
	strcpy(Set->Account, "");
	strcpy(Set->LocalInitDir, DefaultLocalPath);
	strcpy(Set->RemoteInitDir, "");
	memcpy(Set->BookMark, "\0\0", 2);
	strcpy(Set->ChmodCmd, CHMOD_CMD_NOR);
	strcpy(Set->LsName, LS_FNAME);
	strcpy(Set->InitCmd, "");
	Set->Port = PORT_NOR;
	Set->Anonymous = NO;
	Set->KanjiCode = KANJI_NOCNV;
	Set->KanaCnv = YES;
	Set->NameKanjiCode = KANJI_NOCNV;
	Set->NameKanaCnv = NO;
	Set->Pasv = YES;
	Set->FireWall = NO;
	Set->ListCmdOnly = YES;
	Set->UseNLST_R = YES;
	Set->LastDir = NO;
	Set->TimeZone = 9;				/* GMT+9 (JST) */
	Set->HostType = HTYPE_AUTO;
	Set->SyncMove = NO;
	Set->NoFullPath = NO;
	Set->Sort = SORT_NOTSAVED;
	Set->Security = SECURITY_AUTO;
	Set->Dialup = NO;
	Set->DialupAlways = NO;
	Set->DialupNotify = YES;
	strcpy(Set->DialEntry, "");
	return;
}


/*----- �ݒ薼�ꗗ���E�B���h�E�ɑ��� ------------------------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		int Cur : 
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void SendAllHostNames(HWND hWnd, int Cur)
{
	int i;
	HOSTLISTDATA *Pos;
	TV_INSERTSTRUCT tvIns;
	HTREEITEM hItem;
	HTREEITEM hItemCur;
	HTREEITEM *Level;
	int CurLevel;

	hItemCur = NULL;

	/* ������̂ōĕ`��֎~ */
	SendMessage(hWnd, WM_SETREDRAW, (WPARAM)FALSE, 0);

	SendMessage(hWnd, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);		/* �S�Ă��폜 */

	if((Level = malloc(sizeof(HTREEITEM*) * Hosts + 1)) != NULL)
	{
		Pos = HostListTop;
		for(i = 0; i < Hosts; i++)
		{
			if(Pos->Set.Level & SET_LEVEL_GROUP)
			{
				tvIns.item.iImage = 0;
				tvIns.item.iSelectedImage = 0;
			}
			else
			{
				tvIns.item.iImage = 2;
				tvIns.item.iSelectedImage = 2;
			}

			CurLevel = GetNodeLevel(i);
			if(CurLevel == 0)
				tvIns.hParent = TVI_ROOT;
			else
				tvIns.hParent = Level[CurLevel - 1];

			tvIns.hInsertAfter = TVI_LAST;
			tvIns.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
	//		tvIns.item.hItem = 0;
	//		tvIns.item.state = 0;
	//		tvIns.item.stateMask = 0;
			tvIns.item.pszText = Pos->Set.HostName;
			tvIns.item.cchTextMax = 0;
			tvIns.item.cChildren = 1;
			tvIns.item.lParam = i;
			hItem = (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvIns);

			if(Pos->Set.Level & SET_LEVEL_GROUP)
				Level[CurLevel] = hItem;

//			DoPrintf("%d = %x", i, hItem);
			if(i == Cur)
			{
				hItemCur = hItem;
			}
			Pos = GetNextNode(Pos);
		}
		free(Level);
	}

	/* �ĕ`�� */
	SendMessage(hWnd, WM_SETREDRAW, (WPARAM)TRUE, 0);

	if(hItemCur != NULL)
	{
		SendMessage(hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItemCur);
//		SendMessage(hWnd, TVM_ENSUREVISIBLE, 0, (LPARAM)hItemCur);
	}
	UpdateWindow(hWnd);

	return;
}


/*----- �ݒ�l���O���[�v���ǂ�����Ԃ� ----------------------------------------
*
*	Parameter
*		int Num : �ݒ�l���ԍ�
*
*	Return Value
*		int �O���[�v���ǂ���
*			YES/NO/-1=�ݒ�l���Ȃ�
*----------------------------------------------------------------------------*/

static int IsNodeGroup(int Num)
{
	int Ret;
	HOSTLISTDATA *Pos;

	Ret = -1;
	if((Num >= 0) && (Num < Hosts))
	{
		Pos = GetNodeByNum(Num);
		Ret = (Pos->Set.Level & SET_LEVEL_GROUP) ? YES : NO;
	}
	return(Ret);
}


/*----- WS_FTP.INI����̃C���|�[�g --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ImportFromWSFTP(void)
{
	FILE *Strm;
	char Buf[FMAX_PATH+1];
	HOSTDATA Host;
	int InHost;

	strcpy(Buf, "WS_FTP.INI");
	if(SelectFile(GetMainHwnd(), Buf, MSGJPN126, MSGJPN276, NULL, OFN_FILEMUSTEXIST, 0) == TRUE)
	{
		if((Strm = fopen(Buf, "rt")) != NULL)
		{
			InHost = NO;
			while(fgets(Buf, FMAX_PATH, Strm) != NULL)
			{
				if(Buf[0] == '[')
				{
					if(InHost == YES)
					{
						AddHostToList(&Host, -1, 0);
						InHost = NO;
					}
					if(_stricmp(Buf, "[_config_]\n") != 0)
					{
						CopyDefaultHost(&Host);

						*(Buf + strlen(Buf) - 2) = NUL;
						memset(Host.HostName, NUL, HOST_NAME_LEN+1);
						strncpy(Host.HostName, Buf+1, HOST_NAME_LEN);
						InHost = YES;
					}
				}
				else if(InHost == YES)
				{
					FormatIniString(Buf);

					if(_strnicmp(Buf, "HOST=", 5) == 0)
					{
						memset(Host.HostAdrs, NUL, HOST_ADRS_LEN+1);
						strncpy(Host.HostAdrs, Buf+5, HOST_ADRS_LEN);
					}
					else if(_strnicmp(Buf, "UID=", 4) == 0)
					{
						memset(Host.UserName, NUL, USER_NAME_LEN+1);
						strncpy(Host.UserName, Buf+4, USER_NAME_LEN);
						if(strcmp(Host.UserName, "anonymous") == 0)
							strcpy(Host.PassWord, UserMailAdrs);
					}
					else if(_strnicmp(Buf, "LOCDIR=", 7) == 0)
					{
						memset(Host.LocalInitDir, NUL, INIT_DIR_LEN+1);
						strncpy(Host.LocalInitDir, Buf+7, INIT_DIR_LEN);
					}
					else if(_strnicmp(Buf, "DIR=", 4) == 0)
					{
						memset(Host.RemoteInitDir, NUL, INIT_DIR_LEN+1);
						strncpy(Host.RemoteInitDir, Buf+4, INIT_DIR_LEN);
					}
					else if(_strnicmp(Buf, "PASVMODE=", 9) == 0)
						Host.Pasv = (atoi(Buf+9) == 0) ? 0 : 1;
					else if(_strnicmp(Buf, "FIREWALL=", 9) == 0)
						Host.FireWall = (atoi(Buf+9) == 0) ? 0 : 1;
				}
			}

			if(InHost == YES)
				AddHostToList(&Host, -1, 0);
			fclose(Strm);
		}
	}
	return;
}



/*----- �z�X�g�ݒ�̃v���p�e�B�V�[�g ------------------------------------------
*
*	Parameter
*		HWND hDlg : �e�E�C���h�E�̃n���h��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static int DispHostSetDlg(HWND hDlg)
{
	PROPSHEETPAGE psp[5];
	PROPSHEETHEADER psh;

	psp[0].dwSize = sizeof(PROPSHEETPAGE);
	psp[0].dwFlags = PSP_USETITLE | PSP_HASHELP;
	psp[0].hInstance = GetFtpInst();
	psp[0].pszTemplate = MAKEINTRESOURCE(hset_main_dlg);
	psp[0].pszIcon = NULL;
	psp[0].pfnDlgProc = MainSettingProc;
	psp[0].pszTitle = MSGJPN127;
	psp[0].lParam = 0;
	psp[0].pfnCallback = NULL;

	psp[1].dwSize = sizeof(PROPSHEETPAGE);
	psp[1].dwFlags = PSP_USETITLE | PSP_HASHELP;
	psp[1].hInstance = GetFtpInst();
	psp[1].pszTemplate = MAKEINTRESOURCE(hset_adv_dlg);
	psp[1].pszIcon = NULL;
	psp[1].pfnDlgProc = AdvSettingProc;
	psp[1].pszTitle = MSGJPN128;
	psp[1].lParam = 0;
	psp[1].pfnCallback = NULL;

	psp[2].dwSize = sizeof(PROPSHEETPAGE);
	psp[2].dwFlags = PSP_USETITLE | PSP_HASHELP;
	psp[2].hInstance = GetFtpInst();
	psp[2].pszTemplate = MAKEINTRESOURCE(hset_code_dlg);
	psp[2].pszIcon = NULL;
	psp[2].pfnDlgProc = CodeSettingProc;
	psp[2].pszTitle = MSGJPN129;
	psp[2].lParam = 0;
	psp[2].pfnCallback = NULL;

	psp[3].dwSize = sizeof(PROPSHEETPAGE);
	psp[3].dwFlags = PSP_USETITLE | PSP_HASHELP;
	psp[3].hInstance = GetFtpInst();
	psp[3].pszTemplate = MAKEINTRESOURCE(hset_dialup_dlg);
	psp[3].pszIcon = NULL;
	psp[3].pfnDlgProc = DialupSettingProc;
	psp[3].pszTitle = MSGJPN130;
	psp[3].lParam = 0;
	psp[3].pfnCallback = NULL;

	psp[4].dwSize = sizeof(PROPSHEETPAGE);
	psp[4].dwFlags = PSP_USETITLE | PSP_HASHELP;
	psp[4].hInstance = GetFtpInst();
	psp[4].pszTemplate = MAKEINTRESOURCE(hset_adv2_dlg);
	psp[4].pszIcon = NULL;
	psp[4].pfnDlgProc = Adv2SettingProc;
	psp[4].pszTitle = MSGJPN131;
	psp[4].lParam = 0;
	psp[4].pfnCallback = NULL;

	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_HASHELP | PSH_NOAPPLYNOW | PSH_PROPSHEETPAGE;
	psh.hwndParent = hDlg;
	psh.hInstance = GetFtpInst();
	psh.pszIcon = NULL;
	psh.pszCaption = MSGJPN132;
	psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
	psh.nStartPage = 0;
	psh.ppsp = (LPCPROPSHEETPAGE)&psp;
	psh.pfnCallback = NULL;

	Apply = NO;
	PropertySheet(&psh);

	return(Apply);
}


/*----- ��{�ݒ�E�C���h�E�̃R�[���o�b�N --------------------------------------
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

static BOOL CALLBACK MainSettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	long wStyle;
	char Tmp[FMAX_PATH+1];
	NMHDR *pnmhdr;

	switch (iMessage)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, HSET_HOST, EM_LIMITTEXT, HOST_NAME_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_ADRS, EM_LIMITTEXT, HOST_ADRS_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_USER, EM_LIMITTEXT, USER_NAME_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_PASS, EM_LIMITTEXT, PASSWORD_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_LOCAL, EM_LIMITTEXT, INIT_DIR_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_REMOTE, EM_LIMITTEXT, INIT_DIR_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_HOST, WM_SETTEXT, 0, (LPARAM)TmpHost.HostName);
			SendDlgItemMessage(hDlg, HSET_ADRS, WM_SETTEXT, 0, (LPARAM)TmpHost.HostAdrs);
			SendDlgItemMessage(hDlg, HSET_USER, WM_SETTEXT, 0, (LPARAM)TmpHost.UserName);
			SendDlgItemMessage(hDlg, HSET_PASS, WM_SETTEXT, 0, (LPARAM)TmpHost.PassWord);
			SendDlgItemMessage(hDlg, HSET_LOCAL, WM_SETTEXT, 0, (LPARAM)TmpHost.LocalInitDir);
			SendDlgItemMessage(hDlg, HSET_REMOTE, WM_SETTEXT, 0, (LPARAM)TmpHost.RemoteInitDir);
			SendDlgItemMessage(hDlg, HSET_ANONYMOUS, BM_SETCHECK, TmpHost.Anonymous, 0);
			SendDlgItemMessage(hDlg, HSET_LASTDIR, BM_SETCHECK, TmpHost.LastDir, 0);
			if(AskConnecting() == NO)
				EnableWindow(GetDlgItem(hDlg, HSET_REMOTE_CUR), FALSE);
			return(TRUE);

		case WM_NOTIFY:
			pnmhdr = (NMHDR FAR *)lParam;
			switch(pnmhdr->code)
			{
				case PSN_APPLY :
					SendDlgItemMessage(hDlg, HSET_HOST, WM_GETTEXT, HOST_NAME_LEN+1, (LPARAM)TmpHost.HostName);
					SendDlgItemMessage(hDlg, HSET_ADRS, WM_GETTEXT, HOST_ADRS_LEN+1, (LPARAM)TmpHost.HostAdrs);
					RemoveTailingSpaces(TmpHost.HostAdrs);
					SendDlgItemMessage(hDlg, HSET_USER, WM_GETTEXT, USER_NAME_LEN+1, (LPARAM)TmpHost.UserName);
					SendDlgItemMessage(hDlg, HSET_PASS, WM_GETTEXT, PASSWORD_LEN+1, (LPARAM)TmpHost.PassWord);
					SendDlgItemMessage(hDlg, HSET_LOCAL, WM_GETTEXT, INIT_DIR_LEN+1, (LPARAM)TmpHost.LocalInitDir);
					SendDlgItemMessage(hDlg, HSET_REMOTE, WM_GETTEXT, INIT_DIR_LEN+1, (LPARAM)TmpHost.RemoteInitDir);
					TmpHost.Anonymous = SendDlgItemMessage(hDlg, HSET_ANONYMOUS, BM_GETCHECK, 0, 0);
					TmpHost.LastDir = SendDlgItemMessage(hDlg, HSET_LASTDIR, BM_GETCHECK, 0, 0);
					if((strlen(TmpHost.HostName) == 0) && (strlen(TmpHost.HostAdrs) > 0))
					{
						memset(TmpHost.HostName, NUL, HOST_NAME_LEN+1);
						strncpy(TmpHost.HostName, TmpHost.HostAdrs, HOST_NAME_LEN);
					}
					else if((strlen(TmpHost.HostName) > 0) && (strlen(TmpHost.HostAdrs) == 0))
					{
						memset(TmpHost.HostAdrs, NUL, HOST_ADRS_LEN+1);
						strncpy(TmpHost.HostAdrs, TmpHost.HostName, HOST_ADRS_LEN);
					}
					Apply = YES;
					break;

				case PSN_RESET :
					break;

				case PSN_HELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000028);
					break;
			}
			break;

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case HSET_LOCAL_BR :
					if(SelectDir(hDlg, TmpHost.LocalInitDir, INIT_DIR_LEN) == TRUE)
						SendDlgItemMessage(hDlg, HSET_LOCAL, WM_SETTEXT, 0, (LPARAM)TmpHost.LocalInitDir);
					break;

				case HSET_REMOTE_CUR :
						AskRemoteCurDir(Tmp, FMAX_PATH);
						SendDlgItemMessage(hDlg, HSET_REMOTE, WM_SETTEXT, 0, (LPARAM)Tmp);
					break;

				case HSET_ANONYMOUS :
					if(SendDlgItemMessage(hDlg, HSET_ANONYMOUS, BM_GETCHECK, 0, 0) == 1)
					{
						SendDlgItemMessage(hDlg, HSET_USER, WM_SETTEXT, 0, (LPARAM)"anonymous");
						wStyle = GetWindowLong(GetDlgItem(hDlg, HSET_PASS), GWL_STYLE);
						wStyle &= ~ES_PASSWORD;
						SetWindowLong(GetDlgItem(hDlg, HSET_PASS), GWL_STYLE, wStyle);
						SendDlgItemMessage(hDlg, HSET_PASS, WM_SETTEXT, 0, (LPARAM)UserMailAdrs);
					}
					else
					{
						SendDlgItemMessage(hDlg, HSET_USER, WM_SETTEXT, 0, (LPARAM)"");
						wStyle = GetWindowLong(GetDlgItem(hDlg, HSET_PASS), GWL_STYLE);
						wStyle |= ES_PASSWORD;
						SetWindowLong(GetDlgItem(hDlg, HSET_PASS), GWL_STYLE, wStyle);
						SendDlgItemMessage(hDlg, HSET_PASS, WM_SETTEXT, 0, (LPARAM)"");
					}
					break;
			}
            return(TRUE);
	}
	return(FALSE);
}


/*----- �g���ݒ�E�C���h�E�̃R�[���o�b�N --------------------------------------
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

static BOOL CALLBACK AdvSettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	NMHDR *pnmhdr;
	char Tmp[20];
	int i;

	switch (iMessage)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, HSET_PORT, EM_LIMITTEXT, 5, 0);
			sprintf(Tmp, "%d", TmpHost.Port);
			SendDlgItemMessage(hDlg, HSET_PORT, WM_SETTEXT, 0, (LPARAM)Tmp);
			SendDlgItemMessage(hDlg, HSET_ACCOUNT, EM_LIMITTEXT, ACCOUNT_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_ACCOUNT, WM_SETTEXT, 0, (LPARAM)TmpHost.Account);
			SendDlgItemMessage(hDlg, HSET_PASV, BM_SETCHECK, TmpHost.Pasv, 0);
			SendDlgItemMessage(hDlg, HSET_FIREWALL, BM_SETCHECK, TmpHost.FireWall, 0);
			SendDlgItemMessage(hDlg, HSET_SYNCMOVE, BM_SETCHECK, TmpHost.SyncMove, 0);
			for(i = -12; i <= 12; i++)
			{
				if(i == 0)
					sprintf(Tmp, "GMT");
				else if(i == 9)
					sprintf(Tmp, MSGJPN133, i);
				else
					sprintf(Tmp, "GMT%+02d:00", i);
				SendDlgItemMessage(hDlg, HSET_TIMEZONE, CB_ADDSTRING, 0, (LPARAM)Tmp);
			}
			SendDlgItemMessage(hDlg, HSET_TIMEZONE, CB_SETCURSEL, TmpHost.TimeZone+12, 0);

			SendDlgItemMessage(hDlg, HSET_SECURITY, CB_ADDSTRING, 0, (LPARAM)MSGJPN134);
			SendDlgItemMessage(hDlg, HSET_SECURITY, CB_ADDSTRING, 0, (LPARAM)MSGJPN135);
			SendDlgItemMessage(hDlg, HSET_SECURITY, CB_ADDSTRING, 0, (LPARAM)MSGJPN136);
			SendDlgItemMessage(hDlg, HSET_SECURITY, CB_ADDSTRING, 0, (LPARAM)MSGJPN137);
			SendDlgItemMessage(hDlg, HSET_SECURITY, CB_ADDSTRING, 0, (LPARAM)MSGJPN138);
			SendDlgItemMessage(hDlg, HSET_SECURITY, CB_SETCURSEL, TmpHost.Security, 0);
			SendDlgItemMessage(hDlg, HSET_INITCMD, EM_LIMITTEXT, INITCMD_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_INITCMD, WM_SETTEXT, 0, (LPARAM)TmpHost.InitCmd);
			return(TRUE);

		case WM_NOTIFY:
			pnmhdr = (NMHDR FAR *)lParam;
			switch(pnmhdr->code)
			{
				case PSN_APPLY :
					TmpHost.Pasv = SendDlgItemMessage(hDlg, HSET_PASV, BM_GETCHECK, 0, 0);
					TmpHost.FireWall = SendDlgItemMessage(hDlg, HSET_FIREWALL, BM_GETCHECK, 0, 0);
					TmpHost.SyncMove = SendDlgItemMessage(hDlg, HSET_SYNCMOVE, BM_GETCHECK, 0, 0);
					SendDlgItemMessage(hDlg, HSET_PORT, WM_GETTEXT, 5+1, (LPARAM)Tmp);
					TmpHost.Port = atoi(Tmp);
					SendDlgItemMessage(hDlg, HSET_ACCOUNT, WM_GETTEXT, ACCOUNT_LEN+1, (LPARAM)TmpHost.Account);
					TmpHost.TimeZone = SendDlgItemMessage(hDlg, HSET_TIMEZONE, CB_GETCURSEL, 0, 0) - 12;
					TmpHost.Security = SendDlgItemMessage(hDlg, HSET_SECURITY, CB_GETCURSEL, 0, 0);
					SendDlgItemMessage(hDlg, HSET_INITCMD, WM_GETTEXT, INITCMD_LEN+1, (LPARAM)TmpHost.InitCmd);
					Apply = YES;
					break;

				case PSN_RESET :
					break;

				case PSN_HELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000029);
					break;
			}
			break;

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case HSET_PORT_NOR :
					sprintf(Tmp, "%d", PORT_NOR);
					SendDlgItemMessage(hDlg, HSET_PORT, WM_SETTEXT, 0, (LPARAM)Tmp);
					break;
			}
			return(TRUE);
	}
	return(FALSE);
}


/*----- �����R�[�h�ݒ�E�C���h�E�̃R�[���o�b�N --------------------------------
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

static BOOL CALLBACK CodeSettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	NMHDR *pnmhdr;

	static const RADIOBUTTON KanjiButton[] = {
		{ HSET_NO_CNV, KANJI_NOCNV },
		{ HSET_JIS_CNV, KANJI_JIS },
		{ HSET_EUC_CNV, KANJI_EUC }
	};
	#define KANJIBUTTONS	(sizeof(KanjiButton)/sizeof(RADIOBUTTON))

	static const RADIOBUTTON NameKanjiButton[] = {
		{ HSET_FN_NO_CNV, KANJI_NOCNV },
		{ HSET_FN_JIS_CNV, KANJI_JIS },
		{ HSET_FN_EUC_CNV, KANJI_EUC },
		{ HSET_FN_SMH_CNV, KANJI_SMB_HEX },
		{ HSET_FN_SMC_CNV, KANJI_SMB_CAP },
		{ HSET_FN_UTF8N_CNV, KANJI_UTF8N }		// UTF-8N�Ή�
	};
	#define NAMEKANJIBUTTONS	(sizeof(NameKanjiButton)/sizeof(RADIOBUTTON))

	switch (iMessage)
	{
		case WM_INITDIALOG :
			SetRadioButtonByValue(hDlg, TmpHost.KanjiCode, KanjiButton, KANJIBUTTONS);
			SendDlgItemMessage(hDlg, HSET_HANCNV, BM_SETCHECK, TmpHost.KanaCnv, 0);
			SetRadioButtonByValue(hDlg, TmpHost.NameKanjiCode, NameKanjiButton, NAMEKANJIBUTTONS);
			SendDlgItemMessage(hDlg, HSET_FN_HANCNV, BM_SETCHECK, TmpHost.NameKanaCnv, 0);
			return(TRUE);

		case WM_NOTIFY:
			pnmhdr = (NMHDR FAR *)lParam;
			switch(pnmhdr->code)
			{
				case PSN_APPLY :
					TmpHost.KanjiCode = AskRadioButtonValue(hDlg, KanjiButton, KANJIBUTTONS);
					TmpHost.KanaCnv = SendDlgItemMessage(hDlg, HSET_HANCNV, BM_GETCHECK, 0, 0);
					TmpHost.NameKanjiCode = AskRadioButtonValue(hDlg, NameKanjiButton, NAMEKANJIBUTTONS);
					TmpHost.NameKanaCnv = SendDlgItemMessage(hDlg, HSET_FN_HANCNV, BM_GETCHECK, 0, 0);
					Apply = YES;
					break;

				case PSN_RESET :
					break;

				case PSN_HELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000030);
					break;
			}
			break;

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case HSET_JIS_CNV :
				case HSET_EUC_CNV :
					EnableWindow(GetDlgItem(hDlg, HSET_HANCNV), TRUE);
					break;

				case HSET_NO_CNV :
					EnableWindow(GetDlgItem(hDlg, HSET_HANCNV), FALSE);
					break;

				case HSET_FN_JIS_CNV :
				case HSET_FN_EUC_CNV :
					EnableWindow(GetDlgItem(hDlg, HSET_FN_HANCNV), TRUE);
					break;

				case HSET_FN_NO_CNV :
				case HSET_FN_SMH_CNV :
				case HSET_FN_SMC_CNV :
				case HSET_FN_UTF8N_CNV :	// UTF-8N�Ή�
					EnableWindow(GetDlgItem(hDlg, HSET_FN_HANCNV), FALSE);
					break;
			}
			return(TRUE);
	}
	return(FALSE);
}


/*----- �_�C�A���A�b�v�ݒ�E�C���h�E�̃R�[���o�b�N ----------------------------
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

static BOOL CALLBACK DialupSettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	NMHDR *pnmhdr;

	switch (iMessage)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, HSET_DIALUP, BM_SETCHECK, TmpHost.Dialup, 0);
			SendDlgItemMessage(hDlg, HSET_DIALUSETHIS, BM_SETCHECK, TmpHost.DialupAlways, 0);
			SendDlgItemMessage(hDlg, HSET_DIALNOTIFY, BM_SETCHECK, TmpHost.DialupNotify, 0);
			if(AskRasUsable() == NO)
				EnableWindow(GetDlgItem(hDlg, HSET_DIALUP), FALSE);
			if((TmpHost.DialupAlways == NO) || (AskRasUsable() == NO))
				EnableWindow(GetDlgItem(hDlg, HSET_DIALNOTIFY), FALSE);
			if((TmpHost.Dialup == NO) || (AskRasUsable() == NO))
			{
				EnableWindow(GetDlgItem(hDlg, HSET_DIALENTRY), FALSE);
				EnableWindow(GetDlgItem(hDlg, HSET_DIALUSETHIS), FALSE);
				EnableWindow(GetDlgItem(hDlg, HSET_DIALNOTIFY), FALSE);
			}
			SetRasEntryToComboBox(hDlg, HSET_DIALENTRY, TmpHost.DialEntry);
			return(TRUE);

		case WM_NOTIFY:
			pnmhdr = (NMHDR FAR *)lParam;
			switch(pnmhdr->code)
			{
				case PSN_APPLY :
					TmpHost.Dialup = SendDlgItemMessage(hDlg, HSET_DIALUP, BM_GETCHECK, 0, 0);
					TmpHost.DialupAlways = SendDlgItemMessage(hDlg, HSET_DIALUSETHIS, BM_GETCHECK, 0, 0);
					TmpHost.DialupNotify = SendDlgItemMessage(hDlg, HSET_DIALNOTIFY, BM_GETCHECK, 0, 0);
					SendDlgItemMessage(hDlg, HSET_DIALENTRY, WM_GETTEXT, RAS_NAME_LEN+1, (LPARAM)TmpHost.DialEntry);
					Apply = YES;
					break;

				case PSN_RESET :
					break;

				case PSN_HELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000031);
					break;
			}
			break;

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case HSET_DIALUP :
					if(SendDlgItemMessage(hDlg, HSET_DIALUP, BM_GETCHECK, 0, 0) == 0)
					{
						EnableWindow(GetDlgItem(hDlg, HSET_DIALENTRY), FALSE);
						EnableWindow(GetDlgItem(hDlg, HSET_DIALUSETHIS), FALSE);
						EnableWindow(GetDlgItem(hDlg, HSET_DIALNOTIFY), FALSE);
						break;
					}
					else
					{
						EnableWindow(GetDlgItem(hDlg, HSET_DIALENTRY), TRUE);
						EnableWindow(GetDlgItem(hDlg, HSET_DIALUSETHIS), TRUE);
					}
					/* ������break�͂Ȃ� */

				case HSET_DIALUSETHIS :
					if(SendDlgItemMessage(hDlg, HSET_DIALUSETHIS, BM_GETCHECK, 0, 0) == 0)
						EnableWindow(GetDlgItem(hDlg, HSET_DIALNOTIFY), FALSE);
					else
						EnableWindow(GetDlgItem(hDlg, HSET_DIALNOTIFY), TRUE);
					break;
			}
			return(TRUE);
	}
	return(FALSE);
}


/*----- ���x�ݒ�E�C���h�E�̃R�[���o�b�N --------------------------------------
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

static BOOL CALLBACK Adv2SettingProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	NMHDR *pnmhdr;
	int Num;

	switch (iMessage)
	{
		case WM_INITDIALOG :
			SendDlgItemMessage(hDlg, HSET_CHMOD_CMD, EM_LIMITTEXT, CHMOD_CMD_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_CHMOD_CMD, WM_SETTEXT, 0, (LPARAM)TmpHost.ChmodCmd);
			SendDlgItemMessage(hDlg, HSET_LS_FNAME, EM_LIMITTEXT, NLST_NAME_LEN, 0);
			SendDlgItemMessage(hDlg, HSET_LS_FNAME, WM_SETTEXT, 0, (LPARAM)TmpHost.LsName);
			SendDlgItemMessage(hDlg, HSET_LISTCMD, BM_SETCHECK, TmpHost.ListCmdOnly, 0);
			if(TmpHost.ListCmdOnly == YES)
				EnableWindow(GetDlgItem(hDlg, HSET_NLST_R), FALSE);
			SendDlgItemMessage(hDlg, HSET_NLST_R, BM_SETCHECK, TmpHost.UseNLST_R, 0);
			SendDlgItemMessage(hDlg, HSET_FULLPATH, BM_SETCHECK, TmpHost.NoFullPath, 0);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_ADDSTRING, 0, (LPARAM)MSGJPN139);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_ADDSTRING, 0, (LPARAM)MSGJPN140);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_ADDSTRING, 0, (LPARAM)MSGJPN141);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_ADDSTRING, 0, (LPARAM)MSGJPN142);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_ADDSTRING, 0, (LPARAM)MSGJPN143);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_ADDSTRING, 0, (LPARAM)MSGJPN144);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_ADDSTRING, 0, (LPARAM)MSGJPN289);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_ADDSTRING, 0, (LPARAM)MSGJPN295);
			SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_SETCURSEL, TmpHost.HostType, 0);
			if(TmpHost.HostType == 2)
			{
				EnableWindow(GetDlgItem(hDlg, HSET_NLST_R), FALSE);
				EnableWindow(GetDlgItem(hDlg, HSET_LISTCMD), FALSE);
				EnableWindow(GetDlgItem(hDlg, HSET_FULLPATH), FALSE);
			}
			return(TRUE);

		case WM_NOTIFY:
			pnmhdr = (NMHDR FAR *)lParam;
			switch(pnmhdr->code)
			{
				case PSN_APPLY :
					SendDlgItemMessage(hDlg, HSET_CHMOD_CMD, WM_GETTEXT, CHMOD_CMD_LEN+1, (LPARAM)TmpHost.ChmodCmd);
					SendDlgItemMessage(hDlg, HSET_LS_FNAME, WM_GETTEXT, NLST_NAME_LEN+1, (LPARAM)TmpHost.LsName);
					TmpHost.ListCmdOnly = SendDlgItemMessage(hDlg, HSET_LISTCMD, BM_GETCHECK, 0, 0);
					TmpHost.UseNLST_R = SendDlgItemMessage(hDlg, HSET_NLST_R, BM_GETCHECK, 0, 0);
					TmpHost.NoFullPath = SendDlgItemMessage(hDlg, HSET_FULLPATH, BM_GETCHECK, 0, 0);
					TmpHost.HostType = SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_GETCURSEL, 0, 0);
					Apply = YES;
					break;

				case PSN_RESET :
					break;

				case PSN_HELP :
					hHelpWin = HtmlHelp(NULL, AskHelpFilePath(), HH_HELP_CONTEXT, IDH_HELP_TOPIC_0000032);
					break;
			}
			break;

		case WM_COMMAND :
			switch(GET_WM_COMMAND_ID(wParam, lParam))
			{
				case HSET_CHMOD_NOR :
					SendDlgItemMessage(hDlg, HSET_CHMOD_CMD, WM_SETTEXT, 0, (LPARAM)CHMOD_CMD_NOR);
					break;

				case HSET_LS_FNAME_NOR :
					SendDlgItemMessage(hDlg, HSET_LS_FNAME, WM_SETTEXT, 0, (LPARAM)LS_FNAME);
					break;

				case HSET_LISTCMD :
					if(SendDlgItemMessage(hDlg, HSET_LISTCMD, BM_GETCHECK, 0, 0) == 0)
						EnableWindow(GetDlgItem(hDlg, HSET_NLST_R), TRUE);
					else
						EnableWindow(GetDlgItem(hDlg, HSET_NLST_R), FALSE);
					break;

				case HSET_HOSTTYPE :
					Num = SendDlgItemMessage(hDlg, HSET_HOSTTYPE, CB_GETCURSEL, 0, 0);
					if(Num == 2)
					{
						EnableWindow(GetDlgItem(hDlg, HSET_NLST_R), FALSE);
						EnableWindow(GetDlgItem(hDlg, HSET_LISTCMD), FALSE);
						EnableWindow(GetDlgItem(hDlg, HSET_FULLPATH), FALSE);
					}
					else
					{
						EnableWindow(GetDlgItem(hDlg, HSET_NLST_R), TRUE);
						EnableWindow(GetDlgItem(hDlg, HSET_LISTCMD), TRUE);
						EnableWindow(GetDlgItem(hDlg, HSET_FULLPATH), TRUE);
					}
					break;
			}
			return(TRUE);
	}
	return(FALSE);
}


