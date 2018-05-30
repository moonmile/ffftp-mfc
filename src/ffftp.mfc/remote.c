/*=============================================================================
*
*							�����[�g���̃t�@�C������
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

/* ���̃\�[�X�͈ꕔ�AWS_FTP Version 93.12.05 �̃\�[�X���Q�l�ɂ��܂����B */

#define	STRICT
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <mbstring.h>
#include <time.h>
#include <winsock.h>
#include <windowsx.h>
#include <commctrl.h>

#include "common.h"
#include "resource.h"

#define PWD_XPWD		0
#define PWD_PWD			1

/*===== �v���g�^�C�v =====*/

static int DoPWD(char *Buf);
static int ReadOneLine(SOCKET cSkt, char *Buf, int Max, int *CancelCheckWork);
static int DoDirList(HWND hWnd, SOCKET cSkt, char *AddOpt, char *Path, int Num, int *CancelCheckWork);
static void ChangeSepaLocal2Remote(char *Fname);
static void ChangeSepaRemote2Local(char *Fname);

/*===== �O���Q�� =====*/

extern TRANSPACKET MainTransPkt;

/* �ݒ�l */
extern int TimeOut;
extern int SendQuit;

/*===== ���[�J���ȃ��[�N =====*/

static int PwdCommandType;

static int CheckCancelFlg = NO;



/*----- �����[�g���̃J�����g�f�B���N�g���ύX ----------------------------------
*
*	Parameter
*		char *Path : �p�X��
*		int Disp : �f�B���N�g�����X�g�Ƀp�X����\�����邩�ǂ���(YES/NO)
*		int ForceGet : ���s���Ă��J�����g�f�B���N�g�����擾����
*		int ErrorBell : �G���[���̉���炷���ǂ���(YES/NO)
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoCWD(char *Path, int Disp, int ForceGet, int ErrorBell)
{
	int Sts;
	char Buf[FMAX_PATH+1];

	Sts = FTP_COMPLETE * 100;

	if(strcmp(Path, "..") == 0)
		Sts = CommandProcCmd(NULL, "CDUP");
	else if(strcmp(Path, "") != 0)
	{
		if((AskHostType() != HTYPE_VMS) || (strchr(Path, '[') != NULL))
			Sts = CommandProcCmd(NULL, "CWD %s", Path);
		else
			Sts = CommandProcCmd(NULL, "CWD [.%s]", Path);	/* VMS�p */
	}

	if((Sts/100 >= FTP_CONTINUE) && (ErrorBell == YES))
		SoundPlay(SND_ERROR);

	if((Sts/100 == FTP_COMPLETE) ||
	   (ForceGet == YES))
	{
		if(Disp == YES)
		{
			if(DoPWD(Buf) != FTP_COMPLETE)
			{
				/*===== PWD���g���Ȃ������ꍇ =====*/

				if(*Path == '/')
					strcpy(Buf, Path);
				else
				{
					AskRemoteCurDir(Buf, FMAX_PATH);
					if(strlen(Buf) == 0)
						strcpy(Buf, "/");

					while(*Path != NUL)
					{
						if(strcmp(Path, ".") == 0)
							Path++;
						else if(strncmp(Path, "./", 2) == 0)
							Path += 2;
						else if(strcmp(Path, "..") == 0)
						{
							GetUpperDir(Buf);
							Path += 2;
						}
						else if(strncmp(Path, "../", 2) == 0)
						{
							GetUpperDir(Buf);
							Path += 3;
						}
						else
						{
							SetSlashTail(Buf);
							strcat(Buf, Path);
							break;
						}
					}
				}
			}
			SetRemoteDirHist(Buf);
		}
	}
	return(Sts/100);
}




/*----- �����[�g���̃J�����g�f�B���N�g���ύX�i���̂Q�j-------------------------
*
*	Parameter
*		char *Path : �p�X��
*		char *Cur : �J�����g�f�B���N�g��
*
*	Return Value
*		int �����R�[�h�̂P����
*
*	Note
*		�p�X���� "xxx/yyy/zzz" �̌`��
*		�f�B���N�g���ύX�����s������A�J�����g�f�B���N�g���ɖ߂��Ă���
*----------------------------------------------------------------------------*/

int DoCWDStepByStep(char *Path, char *Cur)
{
	int Sts;
	char *Set;
	char *Set2;
	char Tmp[FMAX_PATH+2];

	Sts = FTP_COMPLETE;

	memset(Tmp, NUL, FMAX_PATH+2);
	strcpy(Tmp, Path);
	Set = Tmp;
	while(*Set != NUL)
	{
		if((Set2 = strchr(Set, '/')) != NULL)
			*Set2 = NUL;
		if((Sts = DoCWD(Set, NO, NO, NO)) != FTP_COMPLETE)
			break;
		if(Set2 == NULL)
			break;
		Set = Set2 + 1;
	}

	if(Sts != FTP_COMPLETE)
		DoCWD(Cur, NO, NO, NO);

	return(Sts);
}


/*----- �����[�g���̃J�����g�f�B���N�g���擾 ----------------------------------
*
*	Parameter
*		char *Buf : �p�X����Ԃ��o�b�t�@
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

static int DoPWD(char *Buf)
{
	char *Pos;
	char Tmp[1024];
	int Sts;

	if(PwdCommandType == PWD_XPWD)
	{
		Sts = CommandProcCmd(Tmp, "XPWD");
		if(Sts/100 != FTP_COMPLETE)
			PwdCommandType = PWD_PWD;
	}
	if(PwdCommandType == PWD_PWD)
		Sts = CommandProcCmd(Tmp, "PWD");

	if(Sts/100 == FTP_COMPLETE)
	{
		if((Pos = strchr(Tmp, '"')) != NULL)
		{
			memmove(Tmp, Pos+1, strlen(Pos+1)+1);
			if((Pos = strchr(Tmp, '"')) != NULL)
				*Pos = NUL;
		}
		else
			memmove(Tmp, Tmp+4, strlen(Tmp+4)+1);

		if(strlen(Tmp) < FMAX_PATH)
		{
			strcpy(Buf, Tmp);
			ReplaceAll(Buf, '\\', '/');
			ChangeSepaRemote2Local(Buf);
			ChangeFnameRemote2Local(Buf, FMAX_PATH);
		}
		else
			Sts = FTP_ERROR*100;
	}
	return(Sts/100);
}


/*----- PWD�R�}���h�̃^�C�v������������ ---------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void InitPWDcommand()
{
	PwdCommandType = PWD_XPWD;
}


/*----- �����[�g���̃f�B���N�g���쐬 ----------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoMKD(char *Path)
{
	int Sts;

	Sts = CommandProcCmd(NULL, "MKD %s", Path);

	if(Sts/100 >= FTP_CONTINUE)
		SoundPlay(SND_ERROR);

	return(Sts/100);
}


/*----- �����[�g���̃f�B���N�g���폜 ------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoRMD(char *Path)
{
	int Sts;

	Sts = CommandProcCmd(NULL, "RMD %s", Path);

	if(Sts/100 >= FTP_CONTINUE)
		SoundPlay(SND_ERROR);

	return(Sts/100);
}


/*----- �����[�g���̃t�@�C���폜 ----------------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoDELE(char *Path)
{
	int Sts;

	Sts = CommandProcCmd(NULL, "DELE %s", Path);

	if(Sts/100 >= FTP_CONTINUE)
		SoundPlay(SND_ERROR);

	return(Sts/100);
}


/*----- �����[�g���̃t�@�C�����ύX --------------------------------------------
*
*	Parameter
*		char *Src : ���t�@�C����
*		char *Dst : �ύX��̃t�@�C����
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoRENAME(char *Src, char *Dst)
{
	int Sts;

	Sts = CommandProcCmd(NULL, "RNFR %s", Src);
	if(Sts == 350)
		Sts = command(AskCmdCtrlSkt(), NULL, &CheckCancelFlg, "RNTO %s", Dst);

	if(Sts/100 >= FTP_CONTINUE)
		SoundPlay(SND_ERROR);

	return(Sts/100);
}


/*----- �����[�g���̃t�@�C���̑����ύX ----------------------------------------
*
*	Parameter
*		char *Path : �p�X��
*		char *Mode : ���[�h������
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoCHMOD(char *Path, char *Mode)
{
	int Sts;

	Sts = CommandProcCmd(NULL, "%s %s %s", AskHostChmodCmd(), Mode, Path);

	if(Sts/100 >= FTP_CONTINUE)
		SoundPlay(SND_ERROR);

	return(Sts/100);
}


/*----- �����[�g���̃t�@�C���̃T�C�Y���擾�i�]���\�P�b�g�g�p�j-----------------
*
*	Parameter
*		char *Path : �p�X��
*		LONGLONG *Size : �t�@�C���̃T�C�Y��Ԃ����[�N
*
*	Return Value
*		int �����R�[�h�̂P����
*
*	Note
*		�����]���\�P�b�g���g�p���遚��
*		�T�C�Y���I���Ȃ����� Size = -1 ��Ԃ�
*----------------------------------------------------------------------------*/

int DoSIZE(char *Path, LONGLONG *Size)
{
	int Sts;
	char Tmp[1024];

	Sts = CommandProcTrn(Tmp, "SIZE %s", Path);

	*Size = -1;
	if((Sts/100 == FTP_COMPLETE) && (strlen(Tmp) > 4) && IsDigit(Tmp[4]))
		*Size = _atoi64(&Tmp[4]);

	return(Sts/100);
}


/*----- �����[�g���̃t�@�C���̓��t���擾�i�]���\�P�b�g�g�p�j-------------------
*
*	Parameter
*		char *Path : �p�X��
*		FILETIME *Time : ���t��Ԃ����[�N
*
*	Return Value
*		int �����R�[�h�̂P����
*
*	Note
*		�����]���\�P�b�g���g�p���遚��
*		���t���I���Ȃ����� Time = 0 ��Ԃ�
*----------------------------------------------------------------------------*/

int DoMDTM(char *Path, FILETIME *Time)
{
	int Sts;
	char Tmp[1024];
	SYSTEMTIME sTime;

    Time->dwLowDateTime = 0;
    Time->dwHighDateTime = 0;

	Sts = CommandProcTrn(Tmp, "MDTM %s", Path);
	if(Sts/100 == FTP_COMPLETE)
	{
		sTime.wMilliseconds = 0;
		if(sscanf(Tmp+4, "%04d%02d%02d%02d%02d%02d",
			&sTime.wYear, &sTime.wMonth, &sTime.wDay,
			&sTime.wHour, &sTime.wMinute, &sTime.wSecond) == 6)
		{
			SystemTimeToFileTime(&sTime, Time);
			SpecificLocalFileTime2FileTime(Time, AskHostTimeZone());

		}
	}
	return(Sts/100);
}


/*----- �����[�g���̃R�}���h�����s --------------------------------------------
*
*	Parameter
*		char *CmdStr : �R�}���h������
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoQUOTE(char *CmdStr)
{
	int Sts;

	Sts = CommandProcCmd(NULL, "%s", CmdStr);

	if(Sts/100 >= FTP_CONTINUE)
		SoundPlay(SND_ERROR);

	return(Sts/100);
}


/*----- �\�P�b�g����� ------------------------------------------------------
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		SOCKET ������̃\�P�b�g
*----------------------------------------------------------------------------*/

SOCKET DoClose(SOCKET Sock)
{
	if(Sock != INVALID_SOCKET)
	{
//		if(WSAIsBlocking())
//		{
//			DoPrintf("Skt=%u : Cancelled blocking call", Sock);
//			WSACancelBlockingCall();
//		}
		do_closesocket(Sock);
		DoPrintf("Skt=%u : Socket closed.", Sock);
		Sock = INVALID_SOCKET;
	}
	if(Sock != INVALID_SOCKET)
		DoPrintf("Skt=%u : Failed to close socket.", Sock);

	return(Sock);
}


/*----- �z�X�g���烍�O�A�E�g���� ----------------------------------------------
*
*	Parameter
*		kSOCKET ctrl_skt : �\�P�b�g
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoQUIT(SOCKET ctrl_skt)
{
	int Ret;

	Ret = FTP_COMPLETE;
	if(SendQuit == YES)
		Ret = command(ctrl_skt, NULL, &CheckCancelFlg, "QUIT") / 100;

	return(Ret);
}


/*----- �����[�g���̃f�B���N�g�����X�g���擾�i�R�}���h�R���g���[���\�P�b�g���g�p)
*
*	Parameter
*		char *AddOpt : �ǉ��̃I�v�V����
*		char *Path : �p�X��
*		int Num : �t�@�C�����ԍ�
*
*	Return Value
*		int �����R�[�h�̂P����
*----------------------------------------------------------------------------*/

int DoDirListCmdSkt(char *AddOpt, char *Path, int Num, int *CancelCheckWork)
{
	int Sts;

	if(AskTransferNow() == YES)
		SktShareProh();

//	if((Sts = DoDirList(NULL, AskCmdCtrlSkt(), AddOpt, Path, Num)) == 429)
//	{
//		ReConnectCmdSkt();
		Sts = DoDirList(NULL, AskCmdCtrlSkt(), AddOpt, Path, Num, CancelCheckWork);

		if(Sts/100 >= FTP_CONTINUE)
			SoundPlay(SND_ERROR);
//	}
	return(Sts/100);
}


/*----- �����[�g���̃f�B���N�g�����X�g���擾 ----------------------------------
*
*	Parameter
*		HWND hWnd : �]�����_�C�A���O�̃E�C���h�E�n���h��
*		SOCKET cSkt : �R���g���[���\�P�b�g
*		char *AddOpt : �ǉ��̃I�v�V����
*		char *Path : �p�X�� (""=�J�����g�f�B���N�g��)
*		int Num : �t�@�C�����ԍ�
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

static int DoDirList(HWND hWnd, SOCKET cSkt, char *AddOpt, char *Path, int Num, int *CancelCheckWork)
{
	char Tmp[FMAX_PATH];
	int Sts;

//#pragma aaa
//DoPrintf("===== DoDirList %d = %s", Num, Path);

	MakeCacheFileName(Num, Tmp);
//	MainTransPkt.ctrl_skt = cSkt;

	if(AskListCmdMode() == NO)
	{
		strcpy(MainTransPkt.Cmd, "NLST");
		if(strlen(AskHostLsName()) > 0)
		{
			strcat(MainTransPkt.Cmd, " ");
			if((AskHostType() == HTYPE_ACOS) || (AskHostType() == HTYPE_ACOS_4))
				strcat(MainTransPkt.Cmd, "'");
			strcat(MainTransPkt.Cmd, AskHostLsName());
			if((AskHostType() == HTYPE_ACOS) || (AskHostType() == HTYPE_ACOS_4))
				strcat(MainTransPkt.Cmd, "'");
		}
		if(strlen(AddOpt) > 0)
			strcat(MainTransPkt.Cmd, AddOpt);
	}
	else
	{
		strcpy(MainTransPkt.Cmd, "LIST");
		if(strlen(AddOpt) > 0)
		{
			strcat(MainTransPkt.Cmd, " -");
			strcat(MainTransPkt.Cmd, AddOpt);
		}
	}

	if(strlen(Path) > 0)
		strcat(MainTransPkt.Cmd, " ");

	strcpy(MainTransPkt.RemoteFile, Path);
	strcpy(MainTransPkt.LocalFile, Tmp);
	MainTransPkt.Type = TYPE_A;
	MainTransPkt.Size = -1;
	/* �t�@�C�����X�g�̒��̊����̃t�@�C�����́A�ʓr	*/
	/* ChangeFnameRemote2Local �ŕϊ����� 			*/
	MainTransPkt.KanjiCode = KANJI_NOCNV;
	MainTransPkt.KanaCnv = YES;
	MainTransPkt.Mode = EXIST_OVW;
	MainTransPkt.ExistSize = 0;
	MainTransPkt.hWndTrans = hWnd;
	MainTransPkt.Next = NULL;

	Sts = DoDownLoad(cSkt, &MainTransPkt, YES, CancelCheckWork);

//#pragma aaa
//DoPrintf("===== DoDirList Done.");

	return(Sts);
}


/*----- �����[�g���փR�}���h�𑗂胊�v���C��҂i�R�}���h�\�P�b�g�j-----------
*
*	Parameter
*		char *Reply : ���v���C�̃R�s�[�� (NULL=�R�s�[���Ȃ�)
*		char *fmt : �t�H�[�}�b�g������
*		... : �p�����[�^
*
*	Return Value
*		int �����R�[�h
*
*	Note
*		�R�}���h�R���g���[���\�P�b�g���g��
*----------------------------------------------------------------------------*/

int CommandProcCmd(char *Reply, char *fmt, ...)
{
	va_list Args;
	char Cmd[1024];
	int Sts;

	va_start(Args, fmt);
	wvsprintf(Cmd, fmt, Args);
	va_end(Args);

	if(AskTransferNow() == YES)
		SktShareProh();

//#pragma aaa
//DoPrintf("**CommandProcCmd : %s", Cmd);

//	if((Sts = command(AskCmdCtrlSkt(), Reply, "%s", Cmd)) == 429)
//	{
//		if(ReConnectCmdSkt() == SUCCESS)
//		{
			Sts = command(AskCmdCtrlSkt(), Reply, &CheckCancelFlg, "%s", Cmd);
//		}
//	}
	return(Sts);
}


/*----- �����[�g���փR�}���h�𑗂胊�v���C��҂i�]���\�P�b�g�j---------------
*
*	Parameter
*		char *Reply : ���v���C�̃R�s�[�� (NULL=�R�s�[���Ȃ�)
*		char *fmt : �t�H�[�}�b�g������
*		... : �p�����[�^
*
*	Return Value
*		int �����R�[�h
*
*	Note
*		�]���R���g���[���\�P�b�g���g��
*----------------------------------------------------------------------------*/

int CommandProcTrn(char *Reply, char *fmt, ...)
{
	va_list Args;
	char Cmd[1024];
	int Sts;

	va_start(Args, fmt);
	wvsprintf(Cmd, fmt, Args);
	va_end(Args);

//#pragma aaa
//DoPrintf("**CommandProcTrn : %s", Cmd);

//	if((Sts = command(AskTrnCtrlSkt(), Reply, "%s", Cmd)) == 429)
//	{
//		if(ReConnectTrnSkt() == SUCCESS)
			Sts = command(AskTrnCtrlSkt(), Reply, &CheckCancelFlg, "%s", Cmd);
//	}
	return(Sts);
}


/*----- �R�}���h�𑗂胊�v���C��҂� ------------------------------------------
*
*	Parameter
*		SOCKET cSkt : �R���g���[���\�P�b�g
*		char *Reply : ���v���C�̃R�s�[�� (NULL=�R�s�[���Ȃ�)
*		char *fmt : �t�H�[�}�b�g������
*		... : �p�����[�^
*
*	Return Value
*		int �����R�[�h
*
*	Note
*		�z�X�g�̃t�@�C�����̊����R�[�h�ɉ����āA�����Ŋ����R�[�h�̕ϊ����s�Ȃ�
*----------------------------------------------------------------------------*/

//#pragma aaa
//static int cntcnt = 0;

int command(SOCKET cSkt, char *Reply, int *CancelCheckWork, char *fmt, ...)
{
	va_list Args;
	char Cmd[FMAX_PATH*2];
	int Sts;
	char TmpBuf[ONELINE_BUF_SIZE];

	if(cSkt != INVALID_SOCKET)
	{
		va_start(Args, fmt);
		wvsprintf(Cmd, fmt, Args);
		va_end(Args);

		if(strncmp(Cmd, "PASS ", 5) == 0)
			SetTaskMsg(">PASS [xxxxxx]");
		else if((strncmp(Cmd, "USER ", 5) == 0) ||
				(strncmp(Cmd, "OPEN ", 5) == 0))
		{
			SetTaskMsg(">%s", Cmd);
		}
		else
		{
			ChangeSepaLocal2Remote(Cmd);
			SetTaskMsg(">%s", Cmd);
			ChangeFnameLocal2Remote(Cmd, FMAX_PATH*2);
		}

//		DoPrintf("SEND : %s", Cmd);
		strcat(Cmd, "\x0D\x0A");

		if(Reply != NULL)
			strcpy(Reply, "");

		Sts = 429;
		if(SendData(cSkt, Cmd, strlen(Cmd), 0, CancelCheckWork) == SUCCESS)
		{
			Sts = ReadReplyMessage(cSkt, Reply, 1024, CancelCheckWork, TmpBuf);
		}

//#pragma aaa
//if(Reply != NULL)
//	DoPrintf("%x : %x : %s : %s", cSkt, &TmpBuf, Cmd, Reply);
//else
//	DoPrintf("%x : %x : %s : NULL", cSkt, &TmpBuf, Cmd);

//		DoPrintf("command() RET=%d", Sts);
	}
	else
		Sts = 429;

	return(Sts);
}


/*----- �f�[�^�𑗂� ----------------------------------------------------------
*
*	Parameter
*		SOCKET Skt : �\�P�b�g
*		char *Data : �f�[�^
*		int Size : ����f�[�^�̃T�C�Y
*		int Mode : �R�[�����[�h
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int SendData(SOCKET Skt, char *Data, int Size, int Mode, int *CancelCheckWork)
{
	int Sts;
	int Tmp;
//	fd_set SendFds;
//	struct timeval Tout;
//	struct timeval *ToutPtr;
	int TimeOutErr;

	Sts = FAIL;
	if(Skt != INVALID_SOCKET)
	{
		Sts = SUCCESS;
		while(Size > 0)
		{
//			FD_ZERO(&SendFds);
//			FD_SET(Skt, &SendFds);
//			ToutPtr = NULL;
//			if(TimeOut != 0)
//			{
//				Tout.tv_sec = TimeOut;
//				Tout.tv_usec = 0;
//				ToutPtr = &Tout;
//			}
//			Tmp = select(0, NULL, &SendFds, NULL, ToutPtr);
//			if(Tmp == SOCKET_ERROR)
//			{
//				Sts = FAIL;
//				ReportWSError("select", WSAGetLastError());
//				break;
//			}
//			else if(Tmp == 0)
//			{
//				Sts = FAIL;
//				SetTaskMsg(MSGJPN241);
//				break;
//			}

			Tmp = do_send(Skt, Data, Size, Mode, &TimeOutErr, CancelCheckWork);
			if(TimeOutErr == YES)
			{
				Sts = FAIL;
				SetTaskMsg(MSGJPN241);
				break;
			}
			else if(Tmp == SOCKET_ERROR)
			{
				Sts = FAIL;
				ReportWSError("send", WSAGetLastError());
				break;
			}

			Size -= Tmp;
			Data += Tmp;
		}
	}
	return(Sts);
}


/*----- �������b�Z�[�W���󂯎�� ----------------------------------------------
*
*	Parameter
*		SOCKET cSkt : �R���g���[���\�P�b�g
*		char *Buf : ���b�Z�[�W���󂯎��o�b�t�@ (NULL=�R�s�[���Ȃ�)
*		int Max : �o�b�t�@�̃T�C�Y
*		int *CancelCheckWork :
*		char *Tmp : �e���|�������[�N
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

int ReadReplyMessage(SOCKET cSkt, char *Buf, int Max, int *CancelCheckWork, char *Tmp)
{
	int iRetCode;
	int iContinue;
	int FirstCode;
	int Lines;
	int i;

	if(Buf != NULL)
		memset(Buf, NUL, Max);
	Max--;

	FirstCode = 0;
	if(cSkt != INVALID_SOCKET)
	{
		Lines = 0;
		do
		{
			iContinue = NO;
			iRetCode = ReadOneLine(cSkt, Tmp, ONELINE_BUF_SIZE, CancelCheckWork);

			SetTaskMsg("%s", Tmp);

			if(Buf != NULL)
			{
				// �Q�s�ڈȍ~�̉����R�[�h�͏���
				if(Lines > 0)
				{
					for(i = 0; ; i++)
					{
						if(IsDigit(Tmp[i]) == 0)
							break;
						Tmp[i] = ' ';
					}
				}
				strncat(Buf, Tmp, Max);
				Max = max1(0, Max-strlen(Tmp));

//				strncpy(Buf, Tmp, Max);
			}

			if((iRetCode != 421) && (iRetCode != 429))
			{
				if((FirstCode == 0) &&
				   (iRetCode >= 100) && (iRetCode <= 599))
				{
					FirstCode = iRetCode;
				}

				if((iRetCode < 100) || (iRetCode > 599) ||
				   (*(Tmp + 3) == '-') ||
				   ((FirstCode > 0) && (iRetCode != FirstCode)))
				{
					iContinue = YES;
				}
			}
			else
				FirstCode = iRetCode;

			Lines++;
		}
		while(iContinue == YES);
	}
	return(FirstCode);
}


/*----- �P�s���̃f�[�^���󂯎�� ----------------------------------------------
*
*	Parameter
*		SOCKET cSkt : �R���g���[���\�P�b�g
*		char *Buf : ���b�Z�[�W���󂯎��o�b�t�@
*		int Max : �o�b�t�@�̃T�C�Y
*		int *CancelCheckWork : 
*
*	Return Value
*		int �����R�[�h
*----------------------------------------------------------------------------*/

static int ReadOneLine(SOCKET cSkt, char *Buf, int Max, int *CancelCheckWork)
{
	char *Pos;
	int SizeOnce;
	int CopySize;
	int ResCode;
	int i;
//	fd_set ReadFds;
//	struct timeval Tout;
//	struct timeval *ToutPtr;
	char Tmp[1024];
	int TimeOutErr;

	ResCode = 0;
	if(cSkt != INVALID_SOCKET)
	{
		memset(Buf, NUL, Max);
		Max--;					/* ������NULL�̂Ԃ� */
		Pos = Buf;

		for(;;)
		{
//			FD_ZERO(&ReadFds);
//			FD_SET(cSkt, &ReadFds);
//			ToutPtr = NULL;
//			if(TimeOut != 0)
//			{
//				Tout.tv_sec = TimeOut;
//				Tout.tv_usec = 0;
//				ToutPtr = &Tout;
//			}
//			i = select(0, &ReadFds, NULL, NULL, ToutPtr);
//			if(i == SOCKET_ERROR)
//			{
//				ReportWSError("select", WSAGetLastError());
//				SizeOnce = -1;
//				break;
//			}
//			else if(i == 0)
//			{
//				SetTaskMsg(MSGJPN242);
//				SizeOnce = -2;
//				break;
//			}

			/* LF�܂ł���M���邽�߂ɁA�ŏ���PEEK�Ŏ�M */
			if((SizeOnce = do_recv(cSkt, (LPSTR)Tmp, 1024, MSG_PEEK, &TimeOutErr, CancelCheckWork)) <= 0)
			{
				if(TimeOutErr == YES)
				{
					SetTaskMsg(MSGJPN242);
					SizeOnce = -2;
				}
				else if(SizeOnce == SOCKET_ERROR)
				{
					SizeOnce = -1;
				}
				break;
			}

			/* LF��T���āA�������炻���܂ł̒������Z�b�g */
			for(i = 0; i < SizeOnce ; i++)
			{
				if(*(Tmp + i) == NUL || *(Tmp + i) == 0x0A)
				{
					SizeOnce = i + 1;
					break;
				}
			}

			/* �{��M */
			if((SizeOnce = do_recv(cSkt, Tmp, SizeOnce, 0, &TimeOutErr, CancelCheckWork)) <= 0)
				break;

			CopySize = min1(Max, SizeOnce);
			memcpy(Pos, Tmp, CopySize);
			Pos += CopySize;
			Max -= CopySize;

			/* �f�[�^��LF�ŏI����Ă�����P�s�I��� */
			if(*(Tmp + SizeOnce - 1) == 0x0A)
				break;
		}
		*Pos = NUL;

		if(SizeOnce <= 0)
		{
			ResCode = 429;
			memset(Buf, 0, Max);

			if((SizeOnce == -2) || (AskTransferNow() == YES))
				DisconnectSet();
		}
		else
		{
			if(IsDigit(*Buf) && IsDigit(*(Buf+1)) && IsDigit(*(Buf+2)))
			{
				memset(Tmp, NUL, 4);
				strncpy(Tmp, Buf, 3);
				ResCode = atoi(Tmp);
			}

			/* ������ CR,LF,�X�y�[�X����菜�� */
			while((i=strlen(Buf))>2 &&
				  (Buf[i-1]==0x0a || Buf[i-1]==0x0d || Buf[i-1]==' '))
				Buf[i-1]=0;
		}
	}
	return(ResCode);
}


/*----- �Œ蒷�f�[�^���󂯎�� ------------------------------------------------
*
*	Parameter
*		SOCKET cSkt : �R���g���[���\�P�b�g
*		char *Buf : ���b�Z�[�W���󂯎��o�b�t�@
*		int Size : �o�C�g��
*		int *CancelCheckWork : 
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int ReadNchar(SOCKET cSkt, char *Buf, int Size, int *CancelCheckWork)
{
//	struct timeval Tout;
//	struct timeval *ToutPtr;
//	fd_set ReadFds;
//	int i;
	int SizeOnce;
	int Sts;
	int TimeOutErr;

	Sts = FAIL;
	if(cSkt != INVALID_SOCKET)
	{
		Sts = SUCCESS;
		while(Size > 0)
		{
//			FD_ZERO(&ReadFds);
//			FD_SET(cSkt, &ReadFds);
//			ToutPtr = NULL;
//			if(TimeOut != 0)
//			{
//				Tout.tv_sec = TimeOut;
//				Tout.tv_usec = 0;
//				ToutPtr = &Tout;
//			}
//			i = select(0, &ReadFds, NULL, NULL, ToutPtr);
//			if(i == SOCKET_ERROR)
//			{
//				ReportWSError("select", WSAGetLastError());
//				Sts = FAIL;
//				break;
//			}
//			else if(i == 0)
//			{
//				SetTaskMsg(MSGJPN243);
//				Sts = FAIL;
//				break;
//			}

			if((SizeOnce = do_recv(cSkt, Buf, Size, 0, &TimeOutErr, CancelCheckWork)) <= 0)
			{
				if(TimeOutErr == YES)
					SetTaskMsg(MSGJPN243);
				Sts = FAIL;
				break;
			}

			Buf += SizeOnce;
			Size -= SizeOnce;
		}
	}

	if(Sts == FAIL)
		SetTaskMsg(MSGJPN244);

	return(Sts);
}


/*----- �G���[��������擾 ----------------------------------------------------
*
*	Parameter
*		UINT Error : �G���[�ԍ�
*
*	Return Value
*		char *�G���[������
*----------------------------------------------------------------------------*/

char *ReturnWSError(UINT Error)
{
	static char Msg[128];
	char *Str;

	switch(Error)
	{
		case WSAVERNOTSUPPORTED:
			Str = "version of WinSock not supported";
			break;

		case WSASYSNOTREADY:
			Str = "WinSock not present or not responding";
			break;

		case WSAEINVAL:
			Str = "app version not supported by DLL";
			break;

		case WSAHOST_NOT_FOUND:
			Str = "Authoritive: Host not found";
			break;

		case WSATRY_AGAIN:
			Str = "Non-authoritive: host not found or server failure";
			break;

		case WSANO_RECOVERY:
			Str = "Non-recoverable: refused or not implemented";
			break;

		case WSANO_DATA:
			Str = "Valid name, no data record for type";
			break;

#if 0
		case WSANO_ADDRESS:
			Str = "Valid name, no MX record";
			break;
#endif

		case WSANOTINITIALISED:
			Str = "WSA Startup not initialized";
			break;

		case WSAENETDOWN:
			Str = "Network subsystem failed";
			break;

		case WSAEINPROGRESS:
			Str = "Blocking operation in progress";
			break;

		case WSAEINTR:
			Str = "Blocking call cancelled";
			break;

		case WSAEAFNOSUPPORT:
			Str = "address family not supported";
			break;

		case WSAEMFILE:
			Str = "no file descriptors available";
			break;

		case WSAENOBUFS:
			Str = "no buffer space available";
			break;

		case WSAEPROTONOSUPPORT:
			Str = "specified protocol not supported";
			break;

		case WSAEPROTOTYPE:
			Str = "protocol wrong type for this socket";
			break;

		case WSAESOCKTNOSUPPORT:
			Str = "socket type not supported for address family";
			break;

		case WSAENOTSOCK:
			Str = "descriptor is not a socket";
			break;

		case WSAEWOULDBLOCK:
			Str = "socket marked as non-blocking and SO_LINGER set not 0";
			break;

		case WSAEADDRINUSE:
			Str = "address already in use";
			break;

		case WSAECONNABORTED:
			Str = "connection aborted";
			break;

		case WSAECONNRESET:
			Str = "connection reset";
			break;

		case WSAENOTCONN:
			Str = "not connected";
			break;

		case WSAETIMEDOUT:
			Str = "connection timed out";
			break;

		case WSAECONNREFUSED:
			Str = "connection refused";
			break;

		case WSAEHOSTDOWN:
			Str = "host down";
			break;

		case WSAEHOSTUNREACH:
			Str = "host unreachable";
			break;

		case WSAEADDRNOTAVAIL:
			Str = "address not available";
			break;

		default:
			sprintf(Msg, "error %u", Error);
			return(Msg);
	}
	return(Str);
}


/*----- �f�o�b�O�R���\�[���ɃG���[��\�� --------------------------------------
*
*	Parameter
*		char *Msg : �G���[�̑O�ɕ\�����郁�b�Z�[�W
*		UINT Error : �G���[�ԍ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void ReportWSError(char *Msg, UINT Error)
{
	if(Msg != NULL)
		DoPrintf("[[%s : %s]]", Msg, ReturnWSError(Error));
	else
		DoPrintf("[[%s]]", ReturnWSError(Error));
}


/*----- �t�@�C���������[�J�����ň�����悤�ɕϊ����� --------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*		int Max : �ő咷
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int ChangeFnameRemote2Local(char *Fname, int Max)
{
	int Sts;
	char *Buf;
	char *Pos;
	CODECONVINFO cInfo;

	Sts = FAIL;
	if((Buf = malloc(Max)) != NULL)
	{
		InitCodeConvInfo(&cInfo);
		cInfo.KanaCnv = NO;			//AskHostNameKana();
		cInfo.Str = Fname;
		cInfo.StrLen = strlen(Fname);
		cInfo.Buf = Buf;
		cInfo.BufSize = Max - 1;

		switch(AskHostNameKanji())
		{
			case KANJI_JIS :
				ConvJIStoSJIS(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;

			case KANJI_EUC :
				ConvEUCtoSJIS(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;

			case KANJI_SMB_HEX :
			case KANJI_SMB_CAP :
				ConvSMBtoSJIS(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;

			case KANJI_UTF8N :
				ConvUTF8NtoSJIS(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;
		}
		free(Buf);
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �t�@�C�����������[�g���ň�����悤�ɕϊ����� --------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*		int Max : �ő咷
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int ChangeFnameLocal2Remote(char *Fname, int Max)
{
	int Sts;
	char *Buf;
	char *Pos;
	CODECONVINFO cInfo;

	Sts = FAIL;
	if((Buf = malloc(Max)) != NULL)
	{
		InitCodeConvInfo(&cInfo);
		cInfo.KanaCnv = AskHostNameKana();
		cInfo.Str = Fname;
		cInfo.StrLen = strlen(Fname);
		cInfo.Buf = Buf;
		cInfo.BufSize = Max - 1;

		switch(AskHostNameKanji())
		{
			case KANJI_JIS :
				ConvSJIStoJIS(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;

			case KANJI_EUC :
				ConvSJIStoEUC(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;

			case KANJI_SMB_HEX :
				ConvSJIStoSMB_HEX(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;

			case KANJI_SMB_CAP :
				ConvSJIStoSMB_CAP(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;

			case KANJI_UTF8N :
				ConvSJIStoUTF8N(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Fname, Buf);
				Pos = strchr(Fname, NUL);
				FlushRestData(&cInfo);
				*(Buf + cInfo.OutLen) = NUL;
				strcpy(Pos, Buf);
				break;
		}
		free(Buf);
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- �p�X�̋�؂蕶�����z�X�g�ɍ��킹�ĕύX���� ----------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/
static void ChangeSepaLocal2Remote(char *Fname)
{
	if(AskHostType() == HTYPE_STRATUS)
	{
		ReplaceAll(Fname, '/', '>');
	}
	return;
}


/*----- �p�X�̋�؂蕶�������[�J���ɍ��킹�ĕύX���� --------------------------
*
*	Parameter
*		char *Fname : �t�@�C����
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/
static void ChangeSepaRemote2Local(char *Fname)
{
	if(AskHostType() == HTYPE_STRATUS)
	{
		ReplaceAll(Fname, '>', '/');
	}
	return;
}







