/*=============================================================================
*
*						���[�J�����̃t�@�C���A�N�Z�X
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
#include <direct.h>
#include <string.h>
#include <windowsx.h>

#include "common.h"
#include "resource.h"



/*----- ���[�J�����̃f�B���N�g���ύX -------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int DoLocalCWD(char *Path)
{
	int Sts;

	Sts = SUCCESS;
	SetTaskMsg(">>CD %s", Path);
	if(SetCurrentDirectory(Path) != TRUE)
	{
		SetTaskMsg(MSGJPN145);
		Sts = FAIL;
	}
	return(Sts);
}


/*----- ���[�J�����̃f�B���N�g���쐬 -------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DoLocalMKD(char *Path)
{
	SetTaskMsg(">>MKDIR %s", Path);
	if(_mkdir(Path) != 0)
		SetTaskMsg(MSGJPN146);
	return;
}


/*----- ���[�J�����̃J�����g�f�B���N�g���擾 -----------------------------------
*
*	Parameter
*		char *Buf : �p�X����Ԃ��o�b�t�@
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DoLocalPWD(char *Buf)
{
	if(GetCurrentDirectory(FMAX_PATH, Buf) == 0)
		strcpy(Buf, "");
	return;
}


/*----- ���[�J�����̃f�B���N�g���폜 ------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DoLocalRMD(char *Path)
{
#if 0
	SetTaskMsg(">>RMDIR %s", Path);
	if(rmdir(Path) != 0)
		SetTaskMsg(MSGJPN147);
#else
	SetTaskMsg(">>RMDIR %s", Path);

	if(MoveFileToTrashCan(Path) != 0)
		SetTaskMsg(MSGJPN148);
#endif
	return;
}


/*----- ���[�J�����̃t�@�C���폜 -----------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DoLocalDELE(char *Path)
{
#if 0
	SetTaskMsg(">>DEL %s", Path);
	if(DeleteFile(Path) != TRUE)
		SetTaskMsg(MSGJPN149);
#else
	SetTaskMsg(">>DEL %s", Path);

	if(MoveFileToTrashCan(Path) != 0)
		SetTaskMsg(MSGJPN150);
#endif
	return;
}


/*----- ���[�J�����̃t�@�C�����ύX ---------------------------------------------
*
*	Parameter
*		char *Src : ���t�@�C����
*		char *Dst : �ύX��̃t�@�C����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DoLocalRENAME(char *Src, char *Dst)
{
	SetTaskMsg(">>REN %s %s", Src, Dst);
	if(MoveFile(Src, Dst) != TRUE)
		SetTaskMsg(MSGJPN151);
	return;
}


/*----- �t�@�C���̃v���p�e�B��\������ ----------------------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DispFileProperty(char *Fname)
{
	SHELLEXECUTEINFO sInfo;

	memset(&sInfo, NUL, sizeof(SHELLEXECUTEINFO));
	sInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	sInfo.fMask = SEE_MASK_INVOKEIDLIST;
	sInfo.hwnd = NULL;		//GetMainHwnd();
	sInfo.lpVerb = "Properties";
	sInfo.lpFile = Fname;
	sInfo.lpParameters = NULL;
	sInfo.lpDirectory = NULL;
	sInfo.nShow = SW_NORMAL;
	sInfo.lpIDList = NULL;
	ShellExecuteEx(&sInfo);
	return;
}


/*----- �������`�F�b�N���� FindFirstFile --------------------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*		WIN32_FIND_DATA *FindData : �����f�[�^
*		int IgnHide : �B���t�@�C���𖳎����邩�ǂ���(YES/NO)
*
*	Return Value
*		HANDLE �n���h��
*----------------------------------------------------------------------------*/

HANDLE FindFirstFileAttr(char *Fname, WIN32_FIND_DATA *FindData, int IgnHide)
{
	HANDLE hFind;

	if((hFind = FindFirstFile(Fname, FindData)) != INVALID_HANDLE_VALUE)
	{
		if(IgnHide == YES)
		{
			while(FindData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				if(FindNextFile(hFind, FindData) == FALSE)
				{
					FindClose(hFind);
					hFind = INVALID_HANDLE_VALUE;
					break;
				}
			}
		}
	}
	return(hFind);
}


/*----- �������`�F�b�N���� FindNextFile ---------------------------------------
*
*	Parameter
*		HANDLE hFind : �n���h��
*		WIN32_FIND_DATA *FindData : �����f�[�^
*		int IgnHide : �B���t�@�C���𖳎����邩�ǂ���(YES/NO)
*
*	Return Value
*		HANDLE �n���h��
*----------------------------------------------------------------------------*/

BOOL FindNextFileAttr(HANDLE hFind, WIN32_FIND_DATA *FindData, int IgnHide)
{
	BOOL Ret;

	while((Ret = FindNextFile(hFind, FindData)) == TRUE)
	{
		if(IgnHide == NO)
			break;
		if((FindData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
			break;
	}
	return(Ret);
}


