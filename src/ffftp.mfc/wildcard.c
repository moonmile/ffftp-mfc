/*=============================================================================
*
*							���C���h�J�[�h����
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


/*===== �v���g�^�C�v =====*/

static int CheckNameMatch(char *str, char *regexp);



/*----- ���C���h�J�[�h�Ƀ}�b�`���邩�ǂ�����Ԃ� ------------------------------
*
*	Parameter
*		char *str : ������
*		char *regexp : ���C���h�J�[�h������
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*
*	Note
*		VAX VMS�̎��� ; �ȍ~�͖�������
*----------------------------------------------------------------------------*/

int CheckFname(char *str, char *regexp)
{
	int Sts;
	char p1[FMAX_PATH+1];
	char p2[FMAX_PATH+1];
	char *p;

	strcpy(p1, regexp);
	strcpy(p2, str);

	/* VAX VMS�̎��̂��߂̏��� */
	if(AskHostType() == HTYPE_VMS)
	{
		if((p = strchr(p2, ';')) != NULL)
			*p = NUL;
	}

	/* *? �Ƃ� ** �Ƃ����폜 */
	for(p = p1; *p != NUL; p++)
	{
		while((*p == '*') && ((*(p+1) == '?') || (*(p+1) == '*')))
			memmove(p+1, p+2, strlen(p+2)+1);
	}

	if((strcmp(p1, "*.*") == 0) || (strcmp(p1, "????????.???") == 0))
		strcpy(p1, "*");

	Sts = SUCCESS;
	if(strcmp(p1, "*") != 0)
	{
		if(strcmp(p1, "*.") == 0)
		{
			p = strchr(p2, '.');
			if((p != NULL) && (*(p+1) != NUL))
				Sts = FAIL;
		}
		else
			Sts = CheckNameMatch(p2, p1);
	}
	return(Sts);
}


/*----- ���C���h�J�[�h�����T�u���[�`�� ----------------------------------------
*
*	Parameter
*		char *str : ������
*		char *regexp : ���C���h�J�[�h������
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

static int CheckNameMatch(char *str, char *regexp)
{
	char *p;

	for(p = regexp; (*p != NUL) && (*str != NUL); )
	{
		switch(*p)
		{
			case '?':
				str++;
				p++;
				break;

			case '*':
				/* Look for a character matching the one after the '*' */
				p++;
				if(*p == NUL)
					return SUCCESS; /* Automatic match */
				while(*str != NUL)
				{
					while((*str != NUL) && (toupper(*p)!=toupper(*str)))
						str++;
					if(CheckNameMatch(str, p))
						return SUCCESS;
					if(*str == NUL)
						return FAIL;
					else
						str++;
				}
				return FAIL;

			default:
				if(toupper(*str) != toupper(*p))
					return FAIL;
				str++;
				p++;
				break;
		}
	}

	if((*p == NUL) && (*str == NUL))
		return SUCCESS;

	if ((*p != NUL) && (str[0] == '.') && (str[1] == 0))
		return(SUCCESS);
  
	if ((*str == NUL) && (*p == '?'))
	{
		while (*p == '?')
			p++;
		return(*p == NUL);
	}

	if((*str == NUL) && (*p == '*') && (p[1] == '\0'))
		return SUCCESS;

	return FAIL;
}

