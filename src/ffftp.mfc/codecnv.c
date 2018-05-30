/*=============================================================================
*
*							�����R�[�h�ϊ��^���s�R�[�h�ϊ�
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
#include <string.h>
#include <winsock.h>
#include <mbstring.h>
#include <windowsx.h>

#include "common.h"
#include "resource.h"



#define CONV_ASCII		0		/* ASCII���������� */
#define CONV_KANJI		1		/* ���������� */
#define CONV_KANA		2		/* ���p�J�^�J�i������ */


/*===== �v���g�^�C�v =====*/

static char *ConvEUCtoSJISkanaProc(CODECONVINFO *cInfo, char Dt, char *Put);
static char *ConvJIStoSJISkanaProc(CODECONVINFO *cInfo, char Dt, char *Put);
static char *ConvSJIStoEUCkanaProc(CODECONVINFO *cInfo, char Dt, char *Put);
static char *ConvSJIStoJISkanaProc(CODECONVINFO *cInfo, char Dt, char *Put);
static int HanKataToZen(char Ch);
static int AskDakuon(char Ch, char Daku);

static int CheckOnSJIS(uchar *Pos, uchar *Btm);
static int CheckOnEUC(uchar *Pos, uchar *Btm);
static int ConvertIBMExtendedChar(int code);



#if 0
/*----- �����R�[�h�ϊ��̃e�X�g�v���O���� ------------------------------------*/

void CodeCnvTest(void)
{
	#define BUFBUF	43
	#define BUFBUF2	BUFBUF+3

	CODECONVINFO cInfo;
	char Buf[BUFBUF];
	char Buf2[BUFBUF2];
	FILE *Strm1;
	FILE *Strm2;
	int Byte;
	int Continue;

//	DoPrintf("---START ZEN");

	Strm1 = fopen("in.txt", "rb");
	Strm2 = fopen("out_zen.txt", "wb");

	InitCodeConvInfo(&cInfo);
	cInfo.KanaCnv = YES;


	while((Byte = fread(Buf, 1, BUFBUF, Strm1)) != 0)
	{
		cInfo.Str = Buf;
		cInfo.StrLen = Byte;
		cInfo.Buf = Buf2;
		cInfo.BufSize = BUFBUF2;

//		DoPrintf("READ %d", Byte);

		do
		{
//			Continue = ConvEUCtoSJIS(&cInfo);
//			Continue = ConvJIStoSJIS(&cInfo);
//			Continue = ConvSJIStoEUC(&cInfo);
//			Continue = ConvSJIStoJIS(&cInfo);
			Continue = ConvSMBtoSJIS(&cInfo);
//			Continue = ConvSJIStoSMB_HEX(&cInfo);
//			Continue = ConvSJIStoSMB_CAP(&cInfo);

			fwrite(Buf2, cInfo.OutLen, 1, Strm2);
//			DoPrintf("WRITE %d", cInfo.OutLen);

		}
		while(Continue == YES);
	}

	cInfo.Buf = Buf2;
	cInfo.BufSize = BUFBUF2;
	FlushRestData(&cInfo);
	fwrite(Buf2, cInfo.OutLen, 1, Strm2);
//	DoPrintf("WRITE %d", cInfo.OutLen);


	fclose(Strm1);
	fclose(Strm2);


//	DoPrintf("---START HAN");

	Strm1 = fopen("in.txt", "rb");
	Strm2 = fopen("out_han.txt", "wb");

	InitCodeConvInfo(&cInfo);
	cInfo.KanaCnv = NO;


	while((Byte = fread(Buf, 1, BUFBUF, Strm1)) != 0)
	{
		cInfo.Str = Buf;
		cInfo.StrLen = Byte;
		cInfo.Buf = Buf2;
		cInfo.BufSize = BUFBUF2;

//		DoPrintf("READ %d", Byte);

		do
		{
//			Continue = ConvEUCtoSJIS(&cInfo);
//			Continue = ConvJIStoSJIS(&cInfo);
//			Continue = ConvSJIStoEUC(&cInfo);
//			Continue = ConvSJIStoJIS(&cInfo);
			Continue = ConvSMBtoSJIS(&cInfo);
//			Continue = ConvSJIStoSMB_HEX(&cInfo);
//			Continue = ConvSJIStoSMB_CAP(&cInfo);
			fwrite(Buf2, cInfo.OutLen, 1, Strm2);
//			DoPrintf("WRITE %d", cInfo.OutLen);

		}
		while(Continue == YES);
	}

	cInfo.Buf = Buf2;
	cInfo.BufSize = BUFBUF2;
	FlushRestData(&cInfo);
	fwrite(Buf2, cInfo.OutLen, 1, Strm2);
//	DoPrintf("WRITE %d", cInfo.OutLen);

	fclose(Strm1);
	fclose(Strm2);

//	DoPrintf("---END");

	return;
}
#endif



#if 0
/*----- ���s�R�[�h�ϊ��̃e�X�g�v���O���� ------------------------------------*/

void TermCodeCnvTest(void)
{
	#define BUFBUF	10
	#define BUFBUF2	BUFBUF

	TERMCODECONVINFO cInfo;
	char Buf[BUFBUF];
	char Buf2[BUFBUF2];
	FILE *Strm1;
	FILE *Strm2;
	int Byte;
	int Continue;

//	DoPrintf("---START");

	Strm1 = fopen("in.txt", "rb");
	Strm2 = fopen("out.txt", "wb");

	InitTermCodeConvInfo(&cInfo);

	while((Byte = fread(Buf, 1, BUFBUF, Strm1)) != 0)
	{
		cInfo.Str = Buf;
		cInfo.StrLen = Byte;
		cInfo.Buf = Buf2;
		cInfo.BufSize = BUFBUF2;

//		DoPrintf("READ %d", Byte);

		do
		{
			Continue = ConvTermCodeToCRLF(&cInfo);

			fwrite(Buf2, cInfo.OutLen, 1, Strm2);
//			DoPrintf("WRITE %d", cInfo.OutLen);

		}
		while(Continue == YES);
	}

	cInfo.Buf = Buf2;
	cInfo.BufSize = BUFBUF2;
	FlushRestTermCodeConvData(&cInfo);
	fwrite(Buf2, cInfo.OutLen, 1, Strm2);
//	DoPrintf("WRITE %d", cInfo.OutLen);

	fclose(Strm1);
	fclose(Strm2);

//	DoPrintf("---END");

	return;
}
#endif












/*----- ���s�R�[�h�ϊ����������� --------------------------------------------
*
*	Parameter
*		TERMCODECONVINFO *cInfo : ���s�R�[�h�ϊ����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void InitTermCodeConvInfo(TERMCODECONVINFO *cInfo)
{
	cInfo->Term = 0;
	return;
}


/*----- ���s�R�[�h�ϊ��̎c������o�� ----------------------------------------
*
*	Parameter
*		TERMCODECONVINFO *cInfo : ���s�R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (=NO)
*
*	Note
*		���s�R�[�h�ϊ��̍Ō�ɌĂԎ�
*----------------------------------------------------------------------------*/

int FlushRestTermCodeConvData(TERMCODECONVINFO *cInfo)
{
	char *Put;

	Put = cInfo->Buf;

	if(cInfo->Term == 0x0D)
		*Put++ = 0x0A;

	cInfo->OutLen = Put - cInfo->Buf;

	return(NO);
}


/*----- ���s�R�[�h��CRLF�ɕϊ� -------------------------------------------------
*
*	Parameter
*		TERMCODECONVINFO *cInfo : ���s�R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*----------------------------------------------------------------------------*/

int ConvTermCodeToCRLF(TERMCODECONVINFO *cInfo)
{
	char *Str;
	char *Put;
	char *Limit;
	int Continue;

	Continue = NO;
	Str = cInfo->Str;
	Put = cInfo->Buf;
	Limit = cInfo->Buf + cInfo->BufSize - 1;

	for(; cInfo->StrLen > 0; cInfo->StrLen--)
	{
		if(Put >= Limit)
		{
			Continue = YES;
			break;
		}

		if(*Str == 0x0D)
		{
			if(cInfo->Term == 0x0D)
				*Put++ = 0x0A;
			*Put++ = 0x0D;
			cInfo->Term = *Str++;
		}
		else
		{
			if(*Str == 0x0A)
			{
				if(cInfo->Term != 0x0D)
					*Put++ = 0x0D;
			}
			else
			{
				if(cInfo->Term == 0x0D)
					*Put++ = 0x0A;
			}
			cInfo->Term = 0;
			*Put++ = *Str++;
		}
	}

	cInfo->Str = Str;
	cInfo->OutLen = Put - cInfo->Buf;

	return(Continue);
}


/*----- �����R�[�h�ϊ����������� --------------------------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void InitCodeConvInfo(CODECONVINFO *cInfo)
{
	cInfo->KanaCnv = YES;

	cInfo->EscProc = 0;
	cInfo->KanjiMode = CONV_ASCII;
	cInfo->KanjiFst = 0;
	cInfo->KanaPrev = 0;
	cInfo->KanaProc = NULL;
	return;
}


/*----- �����R�[�h�ϊ��̎c������o�� ----------------------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (=NO)
*
*	Note
*		�����R�[�h�ϊ��̍Ō�ɌĂԎ�
*----------------------------------------------------------------------------*/

int FlushRestData(CODECONVINFO *cInfo)
{
	char *Put;

	Put = cInfo->Buf;

	if(cInfo->KanaProc != NULL)
		Put = (cInfo->KanaProc)(cInfo, 0, Put);

	if(cInfo->KanjiFst != 0)
		*Put++ = cInfo->KanjiFst;
	if(cInfo->EscProc >= 1)
		*Put++ = cInfo->EscCode[0];
	if(cInfo->EscProc == 2)
		*Put++ = cInfo->EscCode[1];

	cInfo->OutLen = Put - cInfo->Buf;

	return(NO);
}


/*----- EUC�����R�[�h��SHIFT-JIS�����R�[�h�ɕϊ� ------------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*----------------------------------------------------------------------------*/

int ConvEUCtoSJIS(CODECONVINFO *cInfo)
{
	int Kcode;
	char *Str;
	char *Put;
	char *Limit;
	int Continue;

	cInfo->KanaProc = &ConvEUCtoSJISkanaProc;

	Continue = NO;
	Str = cInfo->Str;
	Put = cInfo->Buf;
	Limit = cInfo->Buf + cInfo->BufSize - 2;

	for(; cInfo->StrLen > 0; cInfo->StrLen--)
	{
		if(Put >= Limit)
		{
			Continue = YES;
			break;
		}

		if((*Str & 0x80) != 0)
		{
			if(cInfo->KanjiFst == 0)
				cInfo->KanjiFst = *Str++;
			else
			{
				if((uchar)cInfo->KanjiFst == (uchar)0x8E)	/* ���p�J�^�J�i */
				{
					Put = ConvEUCtoSJISkanaProc(cInfo, *Str++, Put);
				}
				else
				{
					Put = ConvEUCtoSJISkanaProc(cInfo, 0, Put);

					Kcode = _mbcjistojms(((cInfo->KanjiFst & 0x7F) * 0x100) + (*Str++ & 0x7F));
					*Put++ = HIGH8(Kcode);
					*Put++ = LOW8(Kcode);
				}
				cInfo->KanjiFst = 0;
			}
		}
		else
		{
			Put = ConvEUCtoSJISkanaProc(cInfo, 0, Put);

			if(cInfo->KanjiFst != 0)
			{
				*Put++ = cInfo->KanjiFst;
				cInfo->KanjiFst = 0;
			}
			*Put++ = *Str++;
		}
	}

	cInfo->Str = Str;
	cInfo->OutLen = Put - cInfo->Buf;

	return(Continue);
}


/*----- EUC-->SHIFT-JIS�����R�[�h�ɕϊ��̔��p�J�^�J�i�̏��� -------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*		char Dt : ����
*		char *Put : �f�[�^�Z�b�g�ʒu
*
*	Return Value
*		char *���̃f�[�^�Z�b�g�ʒu
*----------------------------------------------------------------------------*/

static char *ConvEUCtoSJISkanaProc(CODECONVINFO *cInfo, char Dt, char *Put)
{
	int Kcode;
	int Daku;

	if(cInfo->KanaCnv == NO)
	{
		if(Dt != 0)
			*Put++ = Dt;
	}
	else
	{
		if(cInfo->KanaPrev != 0)
		{
			Daku = AskDakuon(cInfo->KanaPrev, Dt);

			Kcode = _mbcjistojms(HanKataToZen(cInfo->KanaPrev)) + Daku;
			*Put++ = HIGH8(Kcode);
			*Put++ = LOW8(Kcode);

			if(Daku == 0)
				cInfo->KanaPrev = Dt;
			else
				cInfo->KanaPrev = 0;
		}
		else
			cInfo->KanaPrev = Dt;
	}
	return(Put);
}


/*----- JIS�����R�[�h��SHIFT-JIS�����R�[�h�ɕϊ� ------------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*
*		�G�X�P�[�v�R�[�h�́A���̂��̂ɑΉ����Ă���
*			�����J�n		<ESC>$B		<ESC>$@
*			���p�J�i�J�n	<ESC>(I
*			�����I��		<ESC>(B		<ESC>(J		<ESC>(H
*----------------------------------------------------------------------------*/

int ConvJIStoSJIS(CODECONVINFO *cInfo)
{
	int Kcode;
	char *Str;
	char *Put;
	char *Limit;
	int Continue;

	cInfo->KanaProc = &ConvJIStoSJISkanaProc;

	Continue = NO;
	Str = cInfo->Str;
	Put = cInfo->Buf;
	Limit = cInfo->Buf + cInfo->BufSize - 3;

	for(; cInfo->StrLen > 0; cInfo->StrLen--)
	{
		if(Put >= Limit)
		{
			Continue = YES;
			break;
		}

		if(cInfo->EscProc == 0)
		{
			if(*Str == 0x1B)
			{
				if(cInfo->KanjiFst != 0)
				{
					*Put++ = cInfo->KanjiFst;
					cInfo->KanjiFst = 0;
				}
				Put = ConvJIStoSJISkanaProc(cInfo, 0, Put);

				cInfo->EscCode[cInfo->EscProc] = *Str++;
				cInfo->EscProc++;
			}
			else
			{
				if(cInfo->KanjiMode == CONV_KANA)
				{
					if(cInfo->KanjiFst != 0)
					{
						*Put++ = cInfo->KanjiFst;
						cInfo->KanjiFst = 0;
					}

					if((*Str >= 0x21) && (*Str <= 0x5F))
					{
						Put = ConvJIStoSJISkanaProc(cInfo, *Str++, Put);
					}
					else
					{
						Put = ConvJIStoSJISkanaProc(cInfo, 0, Put);
						*Put++ = *Str++;
					}
				}
				else if(cInfo->KanjiMode == CONV_KANJI)
				{
					Put = ConvJIStoSJISkanaProc(cInfo, 0, Put);
					if((*Str >= 0x21) && (*Str <= 0x7E))
					{
						if(cInfo->KanjiFst == 0)
							cInfo->KanjiFst = *Str++;
						else
						{
							Kcode = _mbcjistojms((cInfo->KanjiFst * 0x100) + *Str++);
							*Put++ = HIGH8(Kcode);
							*Put++ = LOW8(Kcode);
							cInfo->KanjiFst = 0;
						}
					}
					else
					{
						if(cInfo->KanjiFst == 0)
							*Put++ = *Str++;
						else
						{
							*Put++ = cInfo->KanjiFst;
							*Put++ = *Str++;
							cInfo->KanjiFst = 0;
						}
					}
				}
				else
				{
					Put = ConvJIStoSJISkanaProc(cInfo, 0, Put);
					*Put++ = *Str++;
				}
			}
		}
		else if(cInfo->EscProc == 1)
		{
			if((*Str == '$') || (*Str == '('))
			{
				cInfo->EscCode[cInfo->EscProc] = *Str++;
				cInfo->EscProc++;
			}
			else
			{
				*Put++ = cInfo->EscCode[0];
				*Put++ = *Str++;
				cInfo->EscProc = 0;
			}
		}
		else if(cInfo->EscProc == 2)
		{
			if((cInfo->EscCode[1] == '$') && ((*Str == 'B') || (*Str == '@')))
				cInfo->KanjiMode = CONV_KANJI;
			else if((cInfo->EscCode[1] == '(') && (*Str == 'I'))
				cInfo->KanjiMode = CONV_KANA;
			else if((cInfo->EscCode[1] == '(') && ((*Str == 'B') || (*Str == 'J') || (*Str == 'H')))
				cInfo->KanjiMode = CONV_ASCII;
			else
			{
				*Put++ = cInfo->EscCode[0];
				*Put++ = cInfo->EscCode[1];
				if((cInfo->KanjiMode == CONV_KANJI) && ((*Str >= 0x21) && (*Str <= 0x7E)))
					cInfo->KanjiFst = *Str;
				else
					*Put++ = *Str;
			}
			Str++;
			cInfo->EscProc = 0;
		}
	}

	cInfo->Str = Str;
	cInfo->OutLen = Put - cInfo->Buf;

	return(Continue);
}


/*----- JIS-->SHIFT-JIS�����R�[�h�ɕϊ��̔��p�J�^�J�i�̏��� -------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*		char Dt : ����
*		char *Put : �f�[�^�Z�b�g�ʒu
*
*	Return Value
*		char *���̃f�[�^�Z�b�g�ʒu
*----------------------------------------------------------------------------*/

static char *ConvJIStoSJISkanaProc(CODECONVINFO *cInfo, char Dt, char *Put)
{
	int Kcode;
	int Daku;

	Dt = (uchar)Dt + (uchar)0x80;
	if(cInfo->KanaCnv == NO)
	{
		if((uchar)Dt != (uchar)0x80)
			*Put++ = Dt;
	}
	else
	{
		if(cInfo->KanaPrev != 0)
		{
			Daku = AskDakuon(cInfo->KanaPrev, Dt);
			Kcode = _mbcjistojms(HanKataToZen(cInfo->KanaPrev)) + Daku;
			*Put++ = HIGH8(Kcode);
			*Put++ = LOW8(Kcode);

			if((Daku == 0) && ((uchar)Dt != (uchar)0x80))
				cInfo->KanaPrev = Dt;
			else
				cInfo->KanaPrev = 0;
		}
		else if((uchar)Dt != (uchar)0x80)
			cInfo->KanaPrev = Dt;
	}
	return(Put);
}


/*----- Samba-HEX/Samba-CAP�����R�[�h��SHIFT-JIS�����R�[�h�ɕϊ� --------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*		�������ꂽ���͕�����̕ϊ��̓T�|�[�g���Ă��Ȃ�
*		���p�J�^�J�i�̕ϊ��ݒ�ɂ͑Ή����Ă��Ȃ�
*----------------------------------------------------------------------------*/

int ConvSMBtoSJIS(CODECONVINFO *cInfo)
{
	char *Str;
	char *Put;
	char *Limit;
	int Continue;

	Continue = NO;
	Str = cInfo->Str;
	Put = cInfo->Buf;
	Limit = cInfo->Buf + cInfo->BufSize - 2;

	for(; cInfo->StrLen > 0; cInfo->StrLen--)
	{
		if(Put >= Limit)
		{
			Continue = YES;
			break;
		}

		if((*Str == SAMBA_HEX_TAG) && (cInfo->StrLen >= 3))
		{
			if(isxdigit(*(Str+1)) && isxdigit(*(Str+2)))
			{
				*Put++ = N2INT(hex2bin(*(Str+1)), hex2bin(*(Str+2)));
				Str += 3;
				cInfo->StrLen -= 2;
			}
			else
				*Put++ = *Str++;
		}
		else
			*Put++ = *Str++;
	}

	cInfo->Str = Str;
	cInfo->OutLen = Put - cInfo->Buf;

	return(Continue);
}


/*----- SHIFT-JIS�����R�[�h��EUC�����R�[�h�ɕϊ� ------------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*----------------------------------------------------------------------------*/

int ConvSJIStoEUC(CODECONVINFO *cInfo)
{
	int Kcode;
	char *Str;
	char *Put;
	char *Limit;
	int Continue;

	cInfo->KanaProc = &ConvSJIStoEUCkanaProc;

	Continue = NO;
	Str = cInfo->Str;
	Put = cInfo->Buf;
	Limit = cInfo->Buf + cInfo->BufSize - 2;

	for(; cInfo->StrLen > 0; cInfo->StrLen--)
	{
		if(Put >= Limit)
		{
			Continue = YES;
			break;
		}

		if(cInfo->KanjiFst == 0)
		{
			if((((uchar)*Str >= (uchar)0x81) && ((uchar)*Str <= (uchar)0x9F)) ||
			   ((uchar)*Str >= (uchar)0xE0))
			{
				Put = ConvSJIStoEUCkanaProc(cInfo, 0, Put);
				cInfo->KanjiFst = *Str++;
			}
			else if(((uchar)*Str >= (uchar)0xA0) && ((uchar)*Str <= (uchar)0xDF))
			{
				Put = ConvSJIStoEUCkanaProc(cInfo, *Str++, Put);
			}
			else
			{
				Put = ConvSJIStoEUCkanaProc(cInfo, 0, Put);
				*Put++ = *Str++;
			}
		}
		else
		{
			if((uchar)*Str >= (uchar)0x40)
			{
				Kcode = ConvertIBMExtendedChar(((uchar)cInfo->KanjiFst * 0x100) + (uchar)*Str++);
				Kcode = _mbcjmstojis(Kcode);
				*Put++ = HIGH8(Kcode) | 0x80;
				*Put++ = LOW8(Kcode) | 0x80;
			}
			else
			{
				*Put++ = cInfo->KanjiFst;
				*Put++ = *Str++;
			}
			cInfo->KanjiFst = 0;
		}
	}

	cInfo->Str = Str;
	cInfo->OutLen = Put - cInfo->Buf;

	return(Continue);
}


/*----- SHIFT-JIS-->EUC�����R�[�h�ɕϊ��̔��p�J�^�J�i�̏��� -------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*		char Dt : ����
*		char *Put : �f�[�^�Z�b�g�ʒu
*
*	Return Value
*		char *���̃f�[�^�Z�b�g�ʒu
*----------------------------------------------------------------------------*/

static char *ConvSJIStoEUCkanaProc(CODECONVINFO *cInfo, char Dt, char *Put)
{
	int Kcode;
	int Daku;

	if(cInfo->KanaCnv == NO)
	{
		if(Dt != 0)
		{
			Kcode = 0x8E00 + (uchar)Dt;
			*Put++ = HIGH8(Kcode) | 0x80;
			*Put++ = LOW8(Kcode) | 0x80;
		}
	}
	else
	{
		if(cInfo->KanaPrev != 0)
		{
			Daku = AskDakuon(cInfo->KanaPrev, Dt);
			Kcode = HanKataToZen(cInfo->KanaPrev) + Daku;
			*Put++ = HIGH8(Kcode) | 0x80;
			*Put++ = LOW8(Kcode) | 0x80;

			if(Daku == 0)
				cInfo->KanaPrev = Dt;
			else
				cInfo->KanaPrev = 0;
		}
		else
			cInfo->KanaPrev = Dt;
	}
	return(Put);
}


/*----- SHIFT-JIS�����R�[�h��JIS�����R�[�h�ɕϊ� ------------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*
*		�G�X�P�[�v�R�[�h�́A���̂��̂��g�p����
*			�����J�n		<ESC>$B
*			���p�J�i�J�n	<ESC>(I
*			�����I��		<ESC>(B
*----------------------------------------------------------------------------*/

int ConvSJIStoJIS(CODECONVINFO *cInfo)
{
	int Kcode;
	char *Str;
	char *Put;
	char *Limit;
	int Continue;

	cInfo->KanaProc = &ConvSJIStoJISkanaProc;

	Continue = NO;
	Str = cInfo->Str;
	Put = cInfo->Buf;
	Limit = cInfo->Buf + cInfo->BufSize - 5;

	for(; cInfo->StrLen > 0; cInfo->StrLen--)
	{
		if(Put >= Limit)
		{
			Continue = YES;
			break;
		}

		if(cInfo->KanjiFst == 0)
		{
			if((((uchar)*Str >= (uchar)0x81) && ((uchar)*Str <= (uchar)0x9F)) ||
			   ((uchar)*Str >= (uchar)0xE0))
			{
				Put = ConvSJIStoJISkanaProc(cInfo, 0, Put);
				cInfo->KanjiFst = *Str++;
			}
			else if(((uchar)*Str >= (uchar)0xA0) && ((uchar)*Str <= (uchar)0xDF))
			{
				Put = ConvSJIStoJISkanaProc(cInfo, *Str++, Put);
			}
			else
			{
				Put = ConvSJIStoJISkanaProc(cInfo, 0, Put);
				if(cInfo->KanjiMode != CONV_ASCII)
				{
					*Put++ = 0x1B;
					*Put++ = '(';
					*Put++ = 'B';
					cInfo->KanjiMode = CONV_ASCII;
				}
				*Put++ = *Str++;
			}
		}
		else
		{
			Put = ConvSJIStoJISkanaProc(cInfo, 0, Put);
			if((uchar)*Str >= (uchar)0x40)
			{
				if(cInfo->KanjiMode != CONV_KANJI)
				{
					*Put++ = 0x1B;
					*Put++ = '$';
					*Put++ = 'B';
					cInfo->KanjiMode = CONV_KANJI;
				}

				Kcode = ConvertIBMExtendedChar(((uchar)cInfo->KanjiFst * 0x100) + (uchar)*Str++);
				Kcode = _mbcjmstojis(Kcode);
				*Put++ = HIGH8(Kcode);
				*Put++ = LOW8(Kcode);
			}
			else
			{
				if(cInfo->KanjiMode != CONV_ASCII)
				{
					*Put++ = 0x1B;
					*Put++ = '(';
					*Put++ = 'B';
					cInfo->KanjiMode = CONV_ASCII;
				}
				*Put++ = cInfo->KanjiFst;
				*Put++ = *Str++;
			}
			cInfo->KanjiFst = 0;
		}
	}

	cInfo->Str = Str;
	cInfo->OutLen = Put - cInfo->Buf;

	return(Continue);
}


/*----- SHIFT-JIS-->JIS�����R�[�h�ɕϊ��̔��p�J�^�J�i�̏��� -------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*		char Dt : ����
*		char *Put : �f�[�^�Z�b�g�ʒu
*
*	Return Value
*		char *���̃f�[�^�Z�b�g�ʒu
*----------------------------------------------------------------------------*/

static char *ConvSJIStoJISkanaProc(CODECONVINFO *cInfo, char Dt, char *Put)
{
	int Kcode;
	int Daku;

	if(cInfo->KanaCnv == NO)
	{
		if(Dt != 0)
		{
			if(cInfo->KanjiMode != CONV_KANA)
			{
				*Put++ = 0x1B;
				*Put++ = '(';
				*Put++ = 'I';
				cInfo->KanjiMode = CONV_KANA;
			}
			*Put++ = (uchar)Dt - (uchar)0x80;
		}
	}
	else
	{
		if(cInfo->KanaPrev != 0)
		{
			if(cInfo->KanjiMode != CONV_KANJI)
			{
				*Put++ = 0x1B;
				*Put++ = '$';
				*Put++ = 'B';
				cInfo->KanjiMode = CONV_KANJI;
			}
			Daku = AskDakuon(cInfo->KanaPrev, Dt);
			Kcode = HanKataToZen(cInfo->KanaPrev) + Daku;
			*Put++ = HIGH8(Kcode);
			*Put++ = LOW8(Kcode);

			if(Daku == 0)
				cInfo->KanaPrev = Dt;
			else
				cInfo->KanaPrev = 0;
		}
		else
			cInfo->KanaPrev = Dt;
	}
	return(Put);
}


/*----- SHIFT-JIS�����R�[�h��Samba-HEX�����R�[�h�ɕϊ� ------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*		�������ꂽ���͕�����̕ϊ��̓T�|�[�g���Ă��Ȃ�
*		���p�J�^�J�i�̕ϊ��ݒ�ɂ͑Ή����Ă��Ȃ�
*----------------------------------------------------------------------------*/

int ConvSJIStoSMB_HEX(CODECONVINFO *cInfo)
{
	char *Str;
	char *Put;
	char *Limit;
	int Continue;

	Continue = NO;
	Str = cInfo->Str;
	Put = cInfo->Buf;
	Limit = cInfo->Buf + cInfo->BufSize - 6;

	for(; cInfo->StrLen > 0; cInfo->StrLen--)
	{
		if(Put >= Limit)
		{
			Continue = YES;
			break;
		}

		if((cInfo->StrLen >= 2) &&
		   ((((uchar)*Str >= (uchar)0x81) && ((uchar)*Str <= (uchar)0x9F)) ||
		    ((uchar)*Str >= (uchar)0xE0)))
		{
			sprintf(Put, "%c%02x%c%02x", SAMBA_HEX_TAG, (uchar)*Str, SAMBA_HEX_TAG, (uchar)*(Str+1));
			Str += 2;
			Put += 6;
			cInfo->StrLen--;
		}
		else if((uchar)*Str >= (uchar)0x80)
		{
			sprintf(Put, "%c%02x", SAMBA_HEX_TAG, (uchar)*Str++);
			Put += 3;
		}
		else
			*Put++ = *Str++;
	}

	cInfo->Str = Str;
	cInfo->OutLen = Put - cInfo->Buf;

	return(Continue);
}


/*----- SHIFT-JIS�����R�[�h��Samba-CAP�����R�[�h�ɕϊ� ------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*		�������ꂽ���͕�����̕ϊ��̓T�|�[�g���Ă��Ȃ�
*----------------------------------------------------------------------------*/

int ConvSJIStoSMB_CAP(CODECONVINFO *cInfo)
{
	char *Str;
	char *Put;
	char *Limit;
	int Continue;

	Continue = NO;
	Str = cInfo->Str;
	Put = cInfo->Buf;
	Limit = cInfo->Buf + cInfo->BufSize - 6;

	for(; cInfo->StrLen > 0; cInfo->StrLen--)
	{
		if(Put >= Limit)
		{
			Continue = YES;
			break;
		}

		if((uchar)*Str >= (uchar)0x80)
		{
			sprintf(Put, "%c%02x", SAMBA_HEX_TAG, (uchar)*Str++);
			Put += 3;
		}
		else
			*Put++ = *Str++;
	}

	cInfo->Str = Str;
	cInfo->OutLen = Put - cInfo->Buf;

	return(Continue);
}


/*----- �P�o�C�g�J�^�J�i��JIS�����R�[�h�ɕϊ� ---------------------------------
*
*	Parameter
*		char Ch : �P�o�C�g�J�^�J�i�R�[�h
*
*	Return Value
*		int JIS�����R�[�h
*----------------------------------------------------------------------------*/

static int HanKataToZen(char Ch)
{
	static const int Katakana[] = {
		0x2121, 0x2123, 0x2156, 0x2157, 0x2122, 0x2126, 0x2572, 0x2521, 
		0x2523, 0x2525, 0x2527, 0x2529, 0x2563, 0x2565, 0x2567, 0x2543, 
		0x213C, 0x2522, 0x2524, 0x2526, 0x2528, 0x252A, 0x252B, 0x252D, 
		0x252F, 0x2531, 0x2533, 0x2535, 0x2537, 0x2539, 0x253B, 0x253D, 
		0x253F, 0x2541, 0x2544, 0x2546, 0x2548, 0x254A, 0x254B, 0x254C, 
		0x254D, 0x254E, 0x254F, 0x2552, 0x2555, 0x2558, 0x255B, 0x255E, 
		0x255F, 0x2560, 0x2561, 0x2562, 0x2564, 0x2566, 0x2568, 0x2569, 
		0x256A, 0x256B, 0x256C, 0x256D, 0x256F, 0x2573, 0x212B, 0x212C
	};

	return(Katakana[(uchar)Ch - (uchar)0xA0]);
}


/*----- �����^�������ɂȂ镶�����`�F�b�N --------------------------------------
*
*	Parameter
*		char Ch : �P�o�C�g�J�^�J�i�R�[�h
*		char Daku : ���_�^�����_
*
*	Return Value
*		int �����R�[�h�ɉ�����l (0=�����^�������ɂȂ�Ȃ�)
*----------------------------------------------------------------------------*/

static int AskDakuon(char Ch, char Daku)
{
	int Ret;

	Ret = 0;
	if((uchar)Daku == (uchar)0xDE)
	{
		if((((uchar)Ch >= (uchar)0xB6) && ((uchar)Ch <= (uchar)0xC4)) ||
		   (((uchar)Ch >= (uchar)0xCA) && ((uchar)Ch <= (uchar)0xCE)))
		{
			Ret = 1;
		}
	}
	else if((uchar)Daku == (uchar)0xDF)
	{
		if(((uchar)Ch >= (uchar)0xCA) && ((uchar)Ch <= (uchar)0xCE))
		{
			Ret = 2;
		}
	}
	return(Ret);
}












/*----- ������̊����R�[�h�𒲂ׁAShift-JIS�ɕϊ� -----------------------------
*
*	Parameter
*		char *Text : ������
*		int Pref : SJIS/EUC�̗D��w��
��			KANJI_SJIS / KANJI_EUC / KANJI_NOCNV=SJIS/EUC�̃`�F�b�N�͂��Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ConvAutoToSJIS(char *Text, int Pref)
{
	int Code;
	char *Buf;
	CODECONVINFO cInfo;

	Code = CheckKanjiCode(Text, strlen(Text), Pref);
	if(Code != KANJI_SJIS)
	{
		Buf = malloc(strlen(Text)+1);
		if(Buf != NULL)
		{
			InitCodeConvInfo(&cInfo);
			cInfo.KanaCnv = NO;
			cInfo.Str = Text;
			cInfo.StrLen = strlen(Text);
			cInfo.Buf = Buf;
			cInfo.BufSize = strlen(Text);

			switch(Code)
			{
				case KANJI_JIS :
					ConvJIStoSJIS(&cInfo);
					break;

				case KANJI_EUC :
					ConvEUCtoSJIS(&cInfo);
					break;
			}

			*(Buf + cInfo.OutLen) = NUL;
			strcpy(Text, Buf);
			free(Buf);
		}
	}
	return;
}


/*----- �g���Ă��銿���R�[�h�𒲂ׂ� ----------------------------------------
*
*	Parameter
*		char *Text : ������
*		int Size : ������̒���
*		int Pref : SJIS/EUC�̗D��w��
��			KANJI_SJIS / KANJI_EUC / KANJI_NOCNV=SJIS/EUC�̃`�F�b�N�͂��Ȃ�
*
*	Return Value
*		int �����R�[�h (KANJI_xxx)
*----------------------------------------------------------------------------*/

int CheckKanjiCode(char *Text, int Size, int Pref)
{
	uchar *Pos;
	uchar *Btm;
	int Ret;
	int PointSJIS;
	int PointEUC;

	Ret = KANJI_SJIS;
	if(Size >= 2)
	{
		Ret = -1;
		Btm = Text + Size;

		/* JIS�����R�[�h�̃`�F�b�N */
		Pos = Text;
		while((Pos = memchr(Pos, 0x1b, Btm-Pos-2)) != NULL)
		{
			Pos++;
			if((memcmp(Pos, "$B", 2) == 0) ||	/* <ESC>$B */
			   (memcmp(Pos, "$@", 2) == 0) ||	/* <ESC>$@ */
			   (memcmp(Pos, "(I", 2) == 0))		/* <ESC>(I */
			{
				Ret = KANJI_JIS;
				break;
			}
		}

		/* EUC��SHIFT-JIS�����R�[�h�̃`�F�b�N */
		if(Ret == -1)
		{
			if(Pref != KANJI_NOCNV)
			{
				Ret = Pref;
				Pos = Text;
				while(Pos < Btm)
				{
					PointSJIS = CheckOnSJIS(Pos, Btm);
					PointEUC = CheckOnEUC(Pos, Btm);
					if(PointSJIS > PointEUC)
					{
						Ret = KANJI_SJIS;
						break;
					}
					if(PointSJIS < PointEUC)
					{
						Ret = KANJI_EUC;
						break;
					}
					if((Pos = memchr(Pos, '\n', Btm-Pos)) == NULL)
						break;
					Pos++;
				}
			}
			else
				Ret = KANJI_SJIS;
		}
	}
	return(Ret);
}


/*----- SHIFT-JIS�R�[�h�̉\�������邩�`�F�b�N --------------------------------
*
*	Parameter
*		uchar *Pos : ������
*		uchar *Btm : ������̖���
*
*	Return Value
*		int ���_
*
*	Note
*		High	81-FF (A0-DF�͔��p)	(EB�ȍ~�͂قƂ�ǖ���)
*		Low		40-FC
*----------------------------------------------------------------------------*/

static int CheckOnSJIS(uchar *Pos, uchar *Btm)
{
	int FstOnTwo;
	int Point;

	FstOnTwo = NO;
	Point = 100;
	while((Point > 0) && (Pos < Btm) && (*Pos != '\n'))
	{
		if(FstOnTwo == YES)
		{
			if((*Pos < 0x40) || (*Pos > 0xFC))	/* 2�o�C�g�ڂ� 0x40�`0xFC */
				Point = 0;
			FstOnTwo = NO;
		}
		else if(*Pos >= 0x81)
		{
			if((*Pos < 0xA0) || (*Pos > 0xDF))	/* ���p�J�i�łȂ���� */
			{
				if(*Pos >= 0xEB)		/* 1�o�C�g�ڂ�0xEB�ȍ~�͂قƂ�ǖ��� */
					Point -= 50;
				FstOnTwo = YES;
			}
		}
		Pos++;
	}
	if(FstOnTwo == YES)		/* �P�o�C�g�ڂŏI����Ă���̂͂�������  */
		Point = 0;

	return(Point);
}


/*----- EUC�R�[�h�̉\�������邩�`�F�b�N -------------------------------------
*
*	Parameter
*		uchar *Pos : ������
*		uchar *Btm : ������̖���
*
*	Return Value
*		int ���_
*
*	Note
*		High	A1-FE , 8E
*		Low		A1-FE
*----------------------------------------------------------------------------*/

static int CheckOnEUC(uchar *Pos, uchar *Btm)
{
	int FstOnTwo;
	int Point;

	FstOnTwo = 0;
	Point = 100;
	while((Point > 0) && (Pos < Btm) && (*Pos != '\n'))
	{
		if(FstOnTwo == 1)
		{
			if((*Pos < 0xA1) || (*Pos > 0xFE))	/* 2�o�C�g�ڂ� 0xA1�`0xFE */
				Point = 0;
			FstOnTwo = 0;
		}
		else if(FstOnTwo == 2)		/* ���p�J�i */
		{
			if((*Pos < 0xA0) || (*Pos > 0xDF))	/* 2�o�C�g�ڂ� 0xA0�`0xDF */
				Point = 0;
			FstOnTwo = 0;
		}
		else
		{
			if(*Pos == 0x8E)		/* 0x8E??�͔��p�J�i */
				FstOnTwo = 2;
			else if((*Pos >= 0xA1) && (*Pos <= 0xFE))
				FstOnTwo = 1;
		}
		Pos++;
	}
	if(FstOnTwo != 0)		/* �P�o�C�g�ڂŏI����Ă���̂͂�������  */
		Point = 0;

	return(Point);
}


// UTF-8N�Ή� �������火
/*----- UTF-8N�����R�[�h��SHIFT-JIS�����R�[�h�ɕϊ� ------------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*----------------------------------------------------------------------------*/

int ConvUTF8NtoSJIS(CODECONVINFO *cInfo)
{
	int Continue;

	char temp_string[2048];
	int string_length;

	Continue = NO;

	// ��������钆�ԃR�[�h�̃T�C�Y�𒲂ׂ�
	string_length = MultiByteToWideChar(
						CP_UTF8,		// �ϊ��敶���R�[�h
						0,				// �t���O(0:�Ȃ�)
						cInfo->Str,		// �ϊ���������
						-1,				// �ϊ���������o�C�g��(-1:����)
						NULL,			// �ϊ�����������̊i�[��
						0				// �i�[��T�C�Y
					);

	// �T�C�Y0 or �o�b�t�@�T�C�Y���傫���ꍇ��
	// cInfo->Buf�̍ŏ���'\0'�����āA
	// cInfo->BufSize��0�����ĕԂ��B
	if( string_length == 0 ||
		string_length >= 1024 ){
		*(cInfo->Buf) = '\0';
		cInfo->BufSize = 0;
		return(Continue);
	}

	// ���ԃR�[�h(unicode)�ɕϊ�
	MultiByteToWideChar(
		CP_UTF8,						// �ϊ��敶���R�[�h
		0,								// �t���O(0:�Ȃ�)
		cInfo->Str,						// �ϊ���������
		-1,								// �ϊ���������o�C�g��(-1:����)
		(unsigned short *)temp_string,	// �ϊ�����������̊i�[��
		1024							// �i�[��T�C�Y
	);

	// ���������UTF-8�R�[�h�̃T�C�Y�𒲂ׂ�
	string_length = WideCharToMultiByte(
						CP_ACP,			// �ϊ��敶���R�[�h
						0,				// �t���O(0:�Ȃ�)
						(unsigned short *)temp_string,	// �ϊ���������
						-1,				// �ϊ���������o�C�g��(-1:����)
						NULL,			// �ϊ�����������̊i�[��
						0,				// �i�[��T�C�Y
						NULL,NULL
					);

	// �T�C�Y0 or �o�̓o�b�t�@�T�C�Y���傫���ꍇ�́A
	// cInfo->Buf�̍ŏ���'\0'�����āA
	// cInfo->BufSize��0�����ĕԂ��B
	if( string_length == 0 ||
		string_length >= cInfo->BufSize ){
		*(cInfo->Buf) = '\0';
		cInfo->BufSize = 0;
		return(Continue);
	}

	// �o�̓T�C�Y��ݒ�
	cInfo->OutLen = string_length;

	// UTF-8�R�[�h�ɕϊ�
	WideCharToMultiByte(
		CP_ACP,							// �ϊ��敶���R�[�h
		0,								// �t���O(0:�Ȃ�)
		(unsigned short *)temp_string,	// �ϊ���������
		-1,								// �ϊ���������o�C�g��(-1:����)
		cInfo->Buf,						// �ϊ�����������̊i�[��(BOM:3bytes)
		cInfo->BufSize,					// �i�[��T�C�Y
		NULL,NULL
	);

	return(Continue);
}

/*----- SHIFT-JIS�����R�[�h��UTF-8N�����R�[�h�ɕϊ� ------------------------------
*
*	Parameter
*		CODECONVINFO *cInfo : �����R�[�h�ϊ����
*
*	Return Value
*		int ����Ԃ��t���O (YES/NO)
*
*	Note
*		����Ԃ��t���O��YES�̎��́AcInfo�̓��e��ς����ɂ�����x�ĂԂ���
*----------------------------------------------------------------------------*/
int ConvSJIStoUTF8N(CODECONVINFO *cInfo)
{
	int Continue;

	char temp_string[2048];
	int string_length;

	Continue = NO;

	// ��������钆�ԃR�[�h�̃T�C�Y�𒲂ׂ�
	string_length = MultiByteToWideChar(
						CP_ACP,			// �ϊ��敶���R�[�h
						0,				// �t���O(0:�Ȃ�)
						cInfo->Str,		// �ϊ���������
						-1,				// �ϊ���������o�C�g��(-1:����)
						NULL,			// �ϊ�����������̊i�[��
						0				// �i�[��T�C�Y
					);

	// �T�C�Y0 or �o�b�t�@�T�C�Y���傫���ꍇ�́A
	// cInfo->Buf�̍ŏ���'\0'�����āA
	// cInfo->BufSize��0�����ĕԂ��B
	if( string_length == 0 ||
		string_length >= 1024 ){
		*(cInfo->Buf) = '\0';
		cInfo->BufSize = 0;
		return(Continue);
	}

	// ���ԃR�[�h(unicode)�ɕϊ�
	MultiByteToWideChar(
		CP_ACP,							// �ϊ��敶���R�[�h
		0,								// �t���O(0:�Ȃ�)
		cInfo->Str,						// �ϊ���������
		-1,								// �ϊ���������o�C�g��(-1:����)
		(unsigned short *)temp_string,	// �ϊ�����������̊i�[��
		1024							// �i�[��T�C�Y
	);

	// ���������UTF-8�R�[�h�̃T�C�Y�𒲂ׂ�
	string_length = WideCharToMultiByte(
						CP_UTF8,		// �ϊ��敶���R�[�h
						0,				// �t���O(0:�Ȃ�)
						(unsigned short *)temp_string,	// �ϊ���������
						-1,				// �ϊ���������o�C�g��(-1:����)
						NULL,			// �ϊ�����������̊i�[��
						0,				// �i�[��T�C�Y
						NULL,NULL
					);

	// �T�C�Y0 or �o�̓o�b�t�@�T�C�Y���傫���ꍇ�́A
	// cInfo->Buf�̍ŏ���'\0'�����āA
	// cInfo->BufSize��0�����ĕԂ��B
	if( string_length == 0 ||
		string_length >= cInfo->BufSize ){
		*(cInfo->Buf) = '\0';
		cInfo->BufSize = 0;
		return(Continue);
	}

	// �o�̓T�C�Y��ݒ�
	cInfo->OutLen = string_length;

	/*
	// ���t�����Ⴞ�� �R�}���h�ɂ��ǉ�����Ă��܂�
	// �o�͕�����̐擪��BOM(byte order mark)������
	*(cInfo->Buf) = (char)0xef;
	*(cInfo->Buf+1) = (char)0xbb;
	*(cInfo->Buf+2) = (char)0xbf;
	*/

	// UTF-8�R�[�h�ɕϊ�
	WideCharToMultiByte(
		CP_UTF8,						// �ϊ��敶���R�[�h
		0,								// �t���O(0:�Ȃ�)
		(unsigned short *)temp_string,	// �ϊ���������
		-1,								// �ϊ���������o�C�g��(-1:����)
		cInfo->Buf,					// �ϊ�����������̊i�[��(BOM:3bytes)
		cInfo->BufSize,					// �i�[��T�C�Y
		NULL,NULL
	);

	return(Continue);
}
// UTF-8N�Ή� �����܂Ł�


/*----- IBM�g��������NEC�I��IBM�g���������ɕϊ� -------------------------------
*
*	Parameter
*		code	�����R�[�h
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/
static int ConvertIBMExtendedChar(int code)
{
	if((code >= 0xfa40) && (code <= 0xfa49))		code -= (0xfa40 - 0xeeef);
	else if((code >= 0xfa4a) && (code <= 0xfa53))	code -= (0xfa4a - 0x8754);
	else if((code >= 0xfa54) && (code <= 0xfa57))	code -= (0xfa54 - 0xeef9);
	else if(code == 0xfa58)							code = 0x878a;
	else if(code == 0xfa59)							code = 0x8782;
	else if(code == 0xfa5a)							code = 0x8784;
	else if(code == 0xfa5b)							code = 0x879a;
	else if((code >= 0xfa5c) && (code <= 0xfa7e))	code -= (0xfa5c - 0xed40);
	else if((code >= 0xfa80) && (code <= 0xfa9b))	code -= (0xfa80 - 0xed63);
	else if((code >= 0xfa9c) && (code <= 0xfafc))	code -= (0xfa9c - 0xed80);
	else if((code >= 0xfb40) && (code <= 0xfb5b))	code -= (0xfb40 - 0xede1);
	else if((code >= 0xfb5c) && (code <= 0xfb7e))	code -= (0xfb5c - 0xee40);
	else if((code >= 0xfb80) && (code <= 0xfb9b))	code -= (0xfb80 - 0xee63);
	else if((code >= 0xfb9c) && (code <= 0xfbfc))	code -= (0xfb9c - 0xee80);
	else if((code >= 0xfc40) && (code <= 0xfc4b))	code -= (0xfc40 - 0xeee1);
	return code;
}

