/*=============================================================================
*
*								���K�\������
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
#include <winsock.h>
#include <windowsx.h>

#include "common.h"
#include "jreusr.h"


/*===== ���[�J���ȃ��[�N =====*/

static JRE2 m_jreData;
static HINSTANCE m_hDll = NULL;
static LPJRE2OPEN m_lpJre2Open = NULL;
static LPJRE2COMPILE m_lpJre2Compile = NULL;
static LPJRE2GETMATCHINFO m_lpJre2MatchInfo = NULL;
static LPJRE2CLOSE m_lpJre2Close = NULL;
static LPJREGETVERSION m_lpJreGetVersion = NULL;
static LPGETJREMESSAGE m_lpGetJreMessage = NULL;



/*----- ���K�\�����C�u���������[�h���� ----------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �X�e�[�^�X
*			TRUE/FALSE
*----------------------------------------------------------------------------*/

int LoadJre(void)
{
	int Sts;

	Sts = FALSE;
	if((m_hDll = LoadLibrary("jre32.dll")) != NULL)
	{
		m_lpJre2Open = (LPJRE2OPEN)GetProcAddress(m_hDll, "Jre2Open");
		m_lpJre2Compile = (LPJRE2COMPILE)GetProcAddress(m_hDll, "Jre2Compile");
		m_lpJre2MatchInfo = (LPJRE2GETMATCHINFO)GetProcAddress(m_hDll, "Jre2GetMatchInfo");
		m_lpJre2Close = (LPJRE2CLOSE)GetProcAddress(m_hDll, "Jre2Close");
		m_lpJreGetVersion = (LPJREGETVERSION)GetProcAddress(m_hDll, "JreGetVersion");
		m_lpGetJreMessage = (LPGETJREMESSAGE)GetProcAddress(m_hDll, "GetJreMessage");

		if((m_lpJre2Open != NULL) &&
		   (m_lpJre2Compile != NULL) &&
		   (m_lpJre2MatchInfo != NULL) &&
		   (m_lpJre2Close != NULL) &&
		   (m_lpJreGetVersion != NULL) &&
		   (m_lpJreGetVersion != NULL))
		{
			memset(&m_jreData, 0, sizeof(JRE2));
			m_jreData.dwSize = sizeof(JRE2);
			m_jreData.wTranslate = 1;

			if((*m_lpJre2Open)(&m_jreData) == TRUE)
				Sts = TRUE;
			else
				ReleaseJre();
		}

	}
	return(Sts);;
}


/*----- ���K�\�����C�u�����������[�X���� --------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ReleaseJre(void)
{
	if(m_hDll != NULL)
		FreeLibrary(m_hDll);
	m_hDll = NULL;

	return;
}


/*----- ���K�\�����C�u�������g���邩�ǂ�����Ԃ� ------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int ���K�\�����C�u�������g���邩�ǂ���
*			TRUE/FALSE
*----------------------------------------------------------------------------*/

int AskJreUsable(void)
{
	int Sts;

	Sts = FALSE;
	if(m_hDll != NULL)
		Sts = TRUE;

	return(Sts);
}


/*----- ���K�\�����C�u�����̃o�[�W������Ԃ� a---------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		int �o�[�W����
*----------------------------------------------------------------------------*/

int GetJreVersion(void)
{
	int Ver;

	Ver = -1;
	if(m_hDll != NULL)
		Ver = (*m_lpJreGetVersion)();

	return(Ver);
}


/*----- ���K�\�����������R���p�C������ ----------------------------------------
*
*	Parameter
*		kchar *Str : ������
*
*	Return Value
*		int �X�e�[�^�X
*			TRUE/FALSE
*----------------------------------------------------------------------------*/

int JreCompileStr(char *Str)
{
	int Sts;

	Sts = FALSE;
	if(m_hDll != NULL)
		Sts = (*m_lpJre2Compile)(&m_jreData, Str);

	return(Sts);
}


/*----- �����񂪈�v���邩�ǂ�����Ԃ� ----------------------------------------
*
*	Parameter
*		char *Str : ������
*		UINT nStart : �����J�n�ʒu
*
*	Return Value
*		char *���������ʒu
*			NULL=������Ȃ�����
*----------------------------------------------------------------------------*/

char *JreGetStrMatchInfo(char *Str, UINT nStart)
{
	char *Ret;

	Ret = NULL;
	if(m_hDll != NULL)
	{
		m_jreData.nStart = nStart;
		if((*m_lpJre2MatchInfo)(&m_jreData, Str) == TRUE)
			 Ret = Str + m_jreData.nPosition;
	}
	return(Ret);
}


