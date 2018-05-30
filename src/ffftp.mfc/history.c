/*=============================================================================
*
*										�q�X�g��
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

static void CopyHostToHistory(HOSTDATA *Host, HISTORYDATA *New);
static void AddOneFnameToMenu(char *Host, char *User, char *Remote, int Num);
static void RemoveAllHistoryFromMenu(void);

/*===== �O���Q�� =====*/

/* �ݒ�l */
extern int FileHist;
extern int PassToHist;

/*===== ���[�J���ȃ��[�N =====*/

static HISTORYDATA *HistoryBase = NULL;
static int HistoryNum = 0;

/* �q�X�g���̃��j���[���ڂ�ID */
static int MenuHistId[HISTORY_MAX] = {
	MENU_HIST_1,  MENU_HIST_2,  MENU_HIST_3,  MENU_HIST_4,  MENU_HIST_5,
	MENU_HIST_6,  MENU_HIST_7,  MENU_HIST_8,  MENU_HIST_9,  MENU_HIST_10,
	MENU_HIST_11, MENU_HIST_12, MENU_HIST_13, MENU_HIST_14, MENU_HIST_15,
	MENU_HIST_16, MENU_HIST_17, MENU_HIST_18, MENU_HIST_19, MENU_HIST_20
};



/*----- �z�X�g�����q�X�g�����X�g�̐擪�ɒǉ����� ----------------------------
*
*	Parameter
*		HOSTDATA *Host : �z�X�g�f�[�^
*		int TrMode : �]�����[�h
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void AddHostToHistory(HOSTDATA *Host, int TrMode)
{
	HISTORYDATA New;

	CopyHostToHistory(Host, &New);
	New.Type = TrMode;
	AddHistoryToHistory(&New);
	return;
}


/*----- �q�X�g�����q�X�g�����X�g�̐擪�ɒǉ����� ------------------------------
*
*	Parameter
*		HISTORYDATA *Hist : �q�X�g���f�[�^
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void AddHistoryToHistory(HISTORYDATA *Hist)
{
	HISTORYDATA *New;

	CheckHistoryNum(1);
	if(FileHist > HistoryNum)
	{
		New = malloc(sizeof(HISTORYDATA));
		if(New != NULL)
		{
			memcpy(New, Hist, sizeof(HISTORYDATA));
			New->Next = HistoryBase;
			HistoryBase = New;
			HistoryNum++;
		}
	}
	return;
}


/*----- �q�X�g���̐���Ԃ� ----------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �q�X�g���̐�
*----------------------------------------------------------------------------*/

int AskHistoryNum(void)
{
	return(HistoryNum);
}


/*----- �q�X�g���̐����`�F�b�N������������폜 --------------------------------
*
*	Parameter
*		int Space : �󂯂Ă����� (0�`)
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void CheckHistoryNum(int Space)
{
	int i;
	HISTORYDATA *Prev;
	HISTORYDATA *Pos;
	HISTORYDATA *Next;

	if(HistoryNum > FileHist-Space)
	{
		/* �c���ׂ��q�X�g����T�� */
		Pos = HistoryBase;
		Prev = NULL;
		for(i = 0; i < FileHist-Space; i++)
		{
			Prev = Pos;
			Pos = Pos->Next;
		}

		/* ����Ȃ��q�X�g�������� */
		if(Prev == NULL)
			HistoryBase = NULL;
		else
			Prev->Next = NULL;

		while(Pos != NULL)
		{
			Next = Pos->Next;
			free(Pos);
			Pos = Next;
			HistoryNum--;
		}
	}
	return;
}


/*----- �z�X�g�����q�X�g���ɃZ�b�g ------------------------------------------
*
*	Parameter
*		HOSTDATA *Host : �z�X�g�f�[�^
*		HISTORYDATA *New : �q�X�g�����Z�b�g���郏�[�N
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void CopyHostToHistory(HOSTDATA *Host, HISTORYDATA *New)
{
	strcpy(New->HostAdrs, Host->HostAdrs);
	strcpy(New->UserName, Host->UserName);
	if(PassToHist == YES)
		strcpy(New->PassWord, Host->PassWord);
	else
		strcpy(New->PassWord, "");
	strcpy(New->Account, Host->Account);
	strcpy(New->LocalInitDir, Host->LocalInitDir);
	strcpy(New->RemoteInitDir, Host->RemoteInitDir);
	strcpy(New->ChmodCmd, Host->ChmodCmd);
	strcpy(New->LsName, Host->LsName);
	strcpy(New->InitCmd, Host->InitCmd);
	New->Port = Host->Port;
	New->KanjiCode = Host->KanjiCode;
	New->KanaCnv = Host->KanaCnv;
	New->NameKanjiCode = Host->NameKanjiCode;
	New->NameKanaCnv = Host->NameKanaCnv;
	New->Pasv = Host->Pasv;
	New->FireWall = Host->FireWall;
	New->ListCmdOnly = Host->ListCmdOnly;
	New->UseNLST_R = Host->UseNLST_R;
	New->TimeZone = Host->TimeZone;
	New->HostType = Host->HostType;
	New->SyncMove = Host->SyncMove;
	New->NoFullPath = Host->NoFullPath;
	New->Sort = Host->Sort;
	New->Security = Host->Security;
	New->Dialup = Host->Dialup;
	New->DialupAlways = Host->DialupAlways;
	New->DialupNotify = Host->DialupNotify;
	strcpy(New->DialEntry, Host->DialEntry);
	return;
}


/*----- �q�X�g�������z�X�g���ɃZ�b�g --------------------------------------
*
*	Parameter
*		HISTORYDATA *Hist : �q�X�g��
*		HOSTDATA *Host : �z�X�g�f�[�^���Z�b�g���郏�[�N
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void CopyHistoryToHost(HISTORYDATA *Hist, HOSTDATA *Host)
{
	CopyDefaultHost(Host);

	strcpy(Host->HostAdrs, Hist->HostAdrs);
	strcpy(Host->UserName, Hist->UserName);
	if(PassToHist == YES)
		strcpy(Host->PassWord, Hist->PassWord);
	else
		strcpy(Host->PassWord, "");
	strcpy(Host->Account, Hist->Account);
	strcpy(Host->LocalInitDir, Hist->LocalInitDir);
	strcpy(Host->RemoteInitDir, Hist->RemoteInitDir);
	strcpy(Host->ChmodCmd, Hist->ChmodCmd);
	strcpy(Host->LsName, Hist->LsName);
	strcpy(Host->InitCmd, Hist->InitCmd);
	Host->Port = Hist->Port;
	Host->KanjiCode = Hist->KanjiCode;
	Host->KanaCnv = Hist->KanaCnv;
	Host->NameKanjiCode = Hist->NameKanjiCode;
	Host->NameKanaCnv = Hist->NameKanaCnv;
	Host->Pasv = Hist->Pasv;
	Host->FireWall = Hist->FireWall;
	Host->ListCmdOnly = Hist->ListCmdOnly;
	Host->UseNLST_R = Hist->UseNLST_R;
	Host->TimeZone = Hist->TimeZone;
	Host->HostType = Hist->HostType;
	Host->SyncMove = Hist->SyncMove;
	Host->NoFullPath = Hist->NoFullPath;
	Host->Sort = Hist->Sort;
	Host->Security = Hist->Security;
	Host->Dialup = Hist->Dialup;
	Host->DialupAlways = Hist->DialupAlways;
	Host->DialupNotify = Hist->DialupNotify;
	strcpy(Host->DialEntry, Hist->DialEntry);
	return;
}


/*----- �q�X�g�����̏����l���擾 --------------------------------------------
*
*	Parameter
*		HISTORYDATA *Set : �q�X�g�����Z�b�g���郏�[�N
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void CopyDefaultHistory(HISTORYDATA *Set)
{
	HOSTDATA Host;

	CopyDefaultHost(&Host);
	CopyHostToHistory(&Host, Set);
	return;
}


/*----- �S�q�X�g�������j���[�ɃZ�b�g ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetAllHistoryToMenu(void)
{
	int i;
	HISTORYDATA *Pos;

	RemoveAllHistoryFromMenu();

	Pos = HistoryBase;
	for(i = 0; i < HistoryNum; i++)
	{
		AddOneFnameToMenu(Pos->HostAdrs, Pos->UserName, Pos->RemoteInitDir, i);
		Pos = Pos->Next;
	}
	return;
}


/*----- �q�X�g�������j���[�ɒǉ� ----------------------------------------------
*
*	Parameter
*		char *Host : �z�X�g��
*		char *User : ���[�U��
*		char *Remote : �z�X�g�̃t�H���_
*		int Num : �ԍ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void AddOneFnameToMenu(char *Host, char *User, char *Remote, int Num)
{
	HMENU hMenu;
	char Tmp[HOST_ADRS_LEN+USER_NAME_LEN+INIT_DIR_LEN+7+1];

	hMenu = GetSubMenu(GetMenu(GetMainHwnd()), 0);

	if(Num == 0)
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);

	if(Num < 9)
		sprintf(Tmp, "&%d %s (%s) %s", Num+1, Host, User, Remote);
	else if(Num == 9)
		sprintf(Tmp, "&0 %s (%s) %s", Host, User, Remote);
	else
		sprintf(Tmp, "&* %s (%s) %s", Host, User, Remote);

	AppendMenu(hMenu, MF_STRING, MenuHistId[Num], Tmp);

	return;
}


/*----- �S�q�X�g�������j���[����폜 ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

static void RemoveAllHistoryFromMenu(void)
{
	HMENU hMenu;
	int Cnt;
	int i;

	hMenu = GetSubMenu(GetMenu(GetMainHwnd()), 0);
	Cnt = GetMenuItemCount(hMenu);
	for(i = DEF_FMENU_ITEMS; i < Cnt; i++)
	{
		DeleteMenu(hMenu, DEF_FMENU_ITEMS, MF_BYPOSITION);
	}
	return;
}


/*----- �w�胁�j���[�R�}���h�ɑΉ�����q�X�g����Ԃ� --------------------------
*
*	Parameter
*		int MenuCmd : ���o���q�X�g���Ɋ��蓖�Ă�ꂽ���j���[�R�}���h (MENU_xxx)
*		HISTORYDATA *Buf : �q�X�g���f�[�^��Ԃ��o�b�t�@
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int GetHistoryByCmd(int MenuCmd, HISTORYDATA *Buf)
{
	int Sts;
	int i;
	HISTORYDATA *Pos;

	Sts = FAIL;
	Pos = HistoryBase;
	for(i = 0; i < HistoryNum; i++)
	{
		if(MenuHistId[i] == MenuCmd)
		{
			memcpy(Buf, Pos, sizeof(HISTORYDATA));
			Sts = SUCCESS;
		}
		Pos = Pos->Next;
	}
	return(Sts);
}


/*----- �w��ԍ��ɑΉ�����q�X�g����Ԃ� --------------------------------------
*
*	Parameter
*		int Num : �ԍ�(0�`)
*		HISTORYDATA *Buf : �q�X�g���f�[�^��Ԃ��o�b�t�@
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int GetHistoryByNum(int Num, HISTORYDATA *Buf)
{
	return(GetHistoryByCmd(MenuHistId[Num], Buf));
}


