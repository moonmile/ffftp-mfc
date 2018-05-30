/*=============================================================================
*
*								�t�@�C���ꗗ�L���b�V��
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
#include <time.h>
#include <mbstring.h>
#include <malloc.h>
#include <windowsx.h>
#include <commctrl.h>

#include <stdarg.h>

#include "common.h"
#include "resource.h"




static BOOL CALLBACK CountPrevFfftpWindowsProc(HWND hWnd, LPARAM lParam);



/*===== �L���b�V���f�[�^�̃X�g���N�`�� =====*/

typedef struct {
	time_t Time;						/* ���X�g�ɓo�^�������� */
	char HostAdrs[HOST_ADRS_LEN+1];		/* �z�X�g�̃A�h���X */
	char UserName[USER_NAME_LEN+1];		/* ���[�U�� */
	char Path[FMAX_PATH+1];				/* �p�X�� */
} CACHELIST;

/*===== ���[�J���ȃ��[�N =====*/

static CACHELIST *RemoteCache = NULL;	/* �L���b�V���f�[�^ */
static int TmpCacheEntry = 0;			/* �L���b�V������Ă���f�[�^�̐� */
static int LastNum;						/* ���ݕ\�����Ă���f�[�^�̔ԍ� */
static int ProgNum;						/* FFFTP�̋N���ԍ� */


/*----- �t�@�C���ꗗ�L���b�V�����쐬���� --------------------------------------
*
*	Parameter
*		int Num : �L���b�V���\��
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int MakeCacheBuf(int Num)
{
	int Sts;
	int i;

	Sts = SUCCESS;
	if(Num > 0)
	{
		Sts = FAIL;
		if((RemoteCache = malloc(sizeof(CACHELIST) * Num)) != NULL)
		{
			TmpCacheEntry = Num;
			for(i = 0; i < TmpCacheEntry; i++)
				ClearCache(i);
			Sts = SUCCESS;
		}
	}
	return(Sts);
}


/*----- �t�@�C���ꗗ�L���b�V�����폜���� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DeleteCacheBuf(void)
{
	if(RemoteCache != NULL)
		free(RemoteCache);
	RemoteCache = NULL;
	TmpCacheEntry = 0;
	return;
}


/*----- �p�X���L���b�V������Ă��邩�ǂ�����Ԃ� ------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		int �L���b�V���f�[�^�̔ԍ�
*			-1 = �L���b�V������Ă��Ȃ�
*----------------------------------------------------------------------------*/

int AskCached(char *Path)
{
	int Ret;
	int i;
	CACHELIST *Pos;

	Ret = -1;
	if(TmpCacheEntry > 0)
	{
		Pos = RemoteCache;
		for(i = 0; i < TmpCacheEntry; i++)
		{
			if((_stricmp(AskHostAdrs(), Pos->HostAdrs) == 0) &&
			   (strcmp(AskHostUserName(), Pos->UserName) == 0) &&
			   (strcmp(Path, Pos->Path) == 0))
			{
				time(&(Pos->Time));		/* Refresh */
				Ret = i;
//				DoPrintf("Filelist cache found. (%d)", Ret);
				break;
			}
			Pos++;
		}
	}
	return(Ret);
}


/*----- ���g�p�̃L���b�V���f�[�^�ԍ���Ԃ� ------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int ���g�p�̃L���b�V���f�[�^�ԍ�
*
*	Note
*		���g�p�̕����������́A��ԌÂ����̂ɏ㏑������
*----------------------------------------------------------------------------*/

int AskFreeCache(void)
{
	int Ret;
	int i;
	time_t Oldest;
	CACHELIST *Pos;

	Ret = 0;
	if(TmpCacheEntry > 0)
	{
		Oldest = 0;
		Ret = -1;
		Pos = RemoteCache;
		for(i = 0; i < TmpCacheEntry; i++)
		{
			if(strlen(Pos->Path) == 0)
			{
				Ret = i;
				break;
			}
			else
			{
				if((Ret == -1) || (Pos->Time < Oldest))
				{
					Oldest = Pos->Time;
					Ret = i;
				}
			}
			Pos++;
		}
	}
//	DoPrintf("Set filelist cache. (%d)", Ret);
	return(Ret);
}


/*----- �L���b�V���f�[�^���Z�b�g���� ------------------------------------------
*
*	Parameter
*		int Num : �L���b�V���f�[�^�ԍ�
*		char *Path : �p�X��
*
*	Return Value
*		�Ȃ�
*
*	Note
*		���ݐڑ����̃z�X�g�����g�p����
*----------------------------------------------------------------------------*/

void SetCache(int Num, char *Path)
{
	if(TmpCacheEntry > 0)
	{
		strcpy((RemoteCache + Num)->HostAdrs, AskHostAdrs());
		strcpy((RemoteCache + Num)->UserName, AskHostUserName());
		strcpy((RemoteCache + Num)->Path, Path);
		time(&((RemoteCache + Num)->Time));
	}
	return;
}


/*----- �L���b�V���f�[�^���N���A���� ------------------------------------------
*
*	Parameter
*		int Num : �L���b�V���f�[�^�ԍ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ClearCache(int Num)
{
	if(TmpCacheEntry > 0)
	{
		strcpy((RemoteCache + Num)->HostAdrs, "");
		strcpy((RemoteCache + Num)->UserName, "");
		strcpy((RemoteCache + Num)->Path, "");
		(RemoteCache + Num)->Time = 0;
	}
	return;
}


/*----- ���ݕ\�����̃L���b�V���f�[�^�ԍ���Ԃ� --------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �L���b�V���f�[�^�ԍ�
*----------------------------------------------------------------------------*/

int AskCurrentFileListNum(void)
{
	return(LastNum);
}


/*----- ���ݕ\�����̃L���b�V���f�[�^�ԍ����Z�b�g���� --------------------------
*
*	Parameter
*		int Num : �L���b�V���f�[�^�ԍ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void SetCurrentFileListNum(int Num)
{
	LastNum = Num;
}


/*----- �L���b�V���f�[�^��ۑ����� --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*
*	Note
*		�L���b�V���f�[�^���̂��̂̓t�@�C���ɂȂ��Ă���̂ŁA�����ł̓C���f�b�N
*		�X�t�@�C�����쐬����B
*		�܂��A�s�v�ȃt�@�C���͍폜����B
*----------------------------------------------------------------------------*/

void SaveCache(void)
{
	char Buf[FMAX_PATH+1];
	FILE *fd;
	CACHELIST *Pos;
	int i;

	if(ProgNum == 0)	/* �ŏ��̃C���X�^���X�������L���b�V����ۑ��ł��� */
	{
		if(TmpCacheEntry > 0)
		{
			strcpy(Buf, AskTmpFilePath());
			SetYenTail(Buf);
			strcat(Buf, "_ffftp.idx");
			if((fd = fopen(Buf, "wt"))!=NULL)
			{
				Pos = RemoteCache;
				for(i = 0; i < TmpCacheEntry; i++)
				{
					if(strlen(Pos->Path) != 0)
						fprintf(fd, "%s %s %s %ld\n", Pos->HostAdrs, Pos->UserName, Pos->Path, Pos->Time);
					Pos++;
				}
				fclose(fd);
			}
		}
	}
	else
		DeleteCache();

	MakeCacheFileName(998, Buf);
	_unlink(Buf);

	MakeCacheFileName(999, Buf);
	_unlink(Buf);

	return;
}


/*----- �L���b�V���f�[�^��ǂݍ��� --------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void LoadCache(void)
{
	char Buf[FMAX_PATH+1];
	FILE *fd;
	CACHELIST *Pos;
	int Num;

	if(ProgNum == 0)	/* �ŏ��̃C���X�^���X�������L���b�V����ۑ��ł��� */
	{
		if(TmpCacheEntry > 0)
		{
			strcpy(Buf, AskTmpFilePath());
			SetYenTail(Buf);
			strcat(Buf, "_ffftp.idx");
			if((fd = fopen(Buf, "rt"))!=NULL)
			{
				Pos = RemoteCache;
				Num = 0;
				while(Num < TmpCacheEntry)
				{
					if(fgets(Buf, FMAX_PATH, fd) == NULL)
						break;

					if(sscanf(Buf, "%s %s %s %ld\n", Pos->HostAdrs, Pos->UserName, Pos->Path, &(Pos->Time)) == 4)
					{
						Pos++;
						Num++;
					}
					else
						ClearCache(Num);
				}
				fclose(fd);
			}
		}
	}
	return;
}


/*----- �L���b�V���f�[�^��S�č폜���� ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DeleteCache(void)
{
	char Buf[FMAX_PATH+1];
	int i;

	if(ProgNum == 0)
	{
		strcpy(Buf, AskTmpFilePath());
		SetYenTail(Buf);
		strcat(Buf, "_ffftp.idx");
		_unlink(Buf);
	}

	for(i = 0; i <= TmpCacheEntry; i++)
	{
		MakeCacheFileName(i, Buf);
		_unlink(Buf);
	}

	MakeCacheFileName(998, Buf);
	_unlink(Buf);

	MakeCacheFileName(999, Buf);
	_unlink(Buf);

	return;
}


/*----- �L���b�V���̃t�@�C�������쐬���� --------------------------------------
*
*	Parameter
*		int Num : �L���b�V���f�[�^�ԍ�
*		char *Buf : �t�@�C�������i�[����o�b�t�@
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void MakeCacheFileName(int Num, char *Buf)
{
	char Prog[10];
	char *Pos;

	strcpy(Buf, AskTmpFilePath());
	SetYenTail(Buf);
	Pos = strchr(Buf, NUL);

	strcpy(Prog, "");
	if(ProgNum > 0)
		sprintf(Prog, ".%d", ProgNum);

	sprintf(Pos, "_ffftp.%03d%s", Num, Prog);
	return;
}


/*----- �N�����Ă���FFFTP�̐��𐔂��� ------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*
*	Note
*		����ProgNum�Ɋi�[����
*----------------------------------------------------------------------------*/

void CountPrevFfftpWindows(void)
{
	ProgNum = 0;
	EnumWindows(CountPrevFfftpWindowsProc, 0);
	return;
}


/*----- �N�����Ă���FFFTP�̐��𐔂���R�[���o�b�N-------------------------------
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		LPARAM lParam : �p�����[�^
*
*	Return Value
*		BOOL TRUE/FALSE
*----------------------------------------------------------------------------*/

static BOOL CALLBACK CountPrevFfftpWindowsProc(HWND hWnd, LPARAM lParam)
{
	char Buf[FMAX_PATH+1];

	if(GetClassName(hWnd, Buf, FMAX_PATH) > 0)
	{
		if(strcmp(Buf, "FFFTPWin") == 0)
			ProgNum++;
	}
	return(TRUE);
}



