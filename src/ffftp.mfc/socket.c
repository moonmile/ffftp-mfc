/*=============================================================================
*
*									�\�P�b�g
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
#include <windowsx.h>
#include <commctrl.h>

#include "common.h"
#include "resource.h"

#define USE_THIS	1
#define DBG_MSG		0




#define FD_CONNECT_BIT		0x0001
#define FD_CLOSE_BIT		0x0002
#define FD_ACCEPT_BIT		0x0004
#define FD_READ_BIT			0x0008
#define FD_WRITE_BIT		0x0010





typedef struct {
	SOCKET Socket;
	int FdConnect;
	int FdClose;
	int FdAccept;
	int FdRead;
	int FdWrite;
	int Error;
} ASYNCSIGNAL;


typedef struct {
	HANDLE Async;
	int Done;
	int ErrorDb;
} ASYNCSIGNALDATABASE;


#define MAX_SIGNAL_ENTRY		10
#define MAX_SIGNAL_ENTRY_DBASE	5




/*===== �v���g�^�C�v =====*/

static LRESULT CALLBACK SocketWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int AskAsyncDone(SOCKET s, int *Error, int Mask);
static int AskAsyncDoneDbase(HANDLE Async, int *Error);
static int RegistAsyncTable(SOCKET s);
static int RegistAsyncTableDbase(HANDLE Async);
static int UnRegistAsyncTable(SOCKET s);
static int UnRegistAsyncTableDbase(HANDLE Async);


/*===== �O���Q�� =====*/

extern int TimeOut;


/*===== ���[�J���ȃ��[�N =====*/

static const char SocketWndClass[] = "FFFTPSocketWnd";
static HWND hWndSocket;

static ASYNCSIGNAL Signal[MAX_SIGNAL_ENTRY];
static ASYNCSIGNALDATABASE SignalDbase[MAX_SIGNAL_ENTRY_DBASE];

//static HANDLE hAsyncTblAccMutex;





/*----- 
*
*	Parameter
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int MakeSocketWin(HWND hWnd, HINSTANCE hInst)
{
	int i;
	int Sts;
	WNDCLASSEX wClass;

	wClass.cbSize        = sizeof(WNDCLASSEX);
	wClass.style         = 0;
	wClass.lpfnWndProc   = SocketWndProc;
	wClass.cbClsExtra    = 0;
	wClass.cbWndExtra    = 0;
	wClass.hInstance     = hInst;
	wClass.hIcon         = NULL;
	wClass.hCursor       = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(GetSysColor(COLOR_INFOBK));
	wClass.lpszMenuName  = NULL;
	wClass.lpszClassName = SocketWndClass;
	wClass.hIconSm       = NULL;
	RegisterClassEx(&wClass);

	Sts = FAIL;
	hWndSocket = CreateWindowEx(0, SocketWndClass, NULL,
			WS_BORDER | WS_POPUP,
			0, 0, 0, 0,
			hWnd, NULL, hInst, NULL);

	if(hWndSocket != NULL)
	{
//		hAsyncTblAccMutex = CreateMutex(NULL, FALSE, NULL);

		for(i = 0; i < MAX_SIGNAL_ENTRY; i++)
			Signal[i].Socket = INVALID_SOCKET;
		for(i = 0; i < MAX_SIGNAL_ENTRY_DBASE; i++)
			SignalDbase[i].Async = 0;
		Sts = SUCCESS;
	}
	return(Sts);
}


/*----- 
*
*	Parameter
*		�Ȃ�
*
*	Return Value
*		�Ȃ�
*----------------------------------------------------------------------------*/

void DeleteSocketWin(void)
{
//	CloseHandle(hAsyncTblAccMutex);

	if(hWndSocket != NULL)
		DestroyWindow(hWndSocket);
	return;
}


/*----- 
*
*	Parameter
*		HWND hWnd : �E�C���h�E�n���h��
*		UINT message : ���b�Z�[�W�ԍ�
*		WPARAM wParam : ���b�Z�[�W�� WPARAM ����
*		LPARAM lParam : ���b�Z�[�W�� LPARAM ����
*
*	Return Value
*		BOOL TRUE/FALSE
*----------------------------------------------------------------------------*/

static LRESULT CALLBACK SocketWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int Pos;

	switch(message)
	{
		case WM_ASYNC_SOCKET :
			for(Pos = 0; Pos < MAX_SIGNAL_ENTRY; Pos++)
			{
				if(Signal[Pos].Socket == (SOCKET)wParam)
				{
					Signal[Pos].Error = WSAGETSELECTERROR(lParam);
#if DBG_MSG
					if(WSAGETSELECTERROR(lParam) != 0)
						DoPrintf("####### Signal: error (%d)", WSAGETSELECTERROR(lParam));
#endif

					switch(WSAGETSELECTEVENT(lParam))
					{
						case FD_CONNECT :
							Signal[Pos].FdConnect = 1;
#if DBG_MSG
							DoPrintf("####### Signal: connect (S=%x)", Signal[Pos].Socket);
#endif
							break;

						case FD_CLOSE :
							Signal[Pos].FdClose = 1;
#if DBG_MSG
							DoPrintf("####### Signal: close (S=%x)", Signal[Pos].Socket);
#endif
//SetTaskMsg("####### Signal: close (%d) (S=%x)", Pos, Signal[Pos].Socket);
							break;

						case FD_ACCEPT :
							Signal[Pos].FdAccept = 1;
#if DBG_MSG
							DoPrintf("####### Signal: accept (S=%x)", Signal[Pos].Socket);
#endif
							break;

						case FD_READ :
							Signal[Pos].FdRead = 1;
#if DBG_MSG
							DoPrintf("####### Signal: read (S=%x)", Signal[Pos].Socket);
#endif
							break;

						case FD_WRITE :
							Signal[Pos].FdWrite = 1;
#if DBG_MSG
							DoPrintf("####### Signal: write (S=%x)", Signal[Pos].Socket);
#endif
							break;
					}
					break;
				}
			}
			break;

		case WM_ASYNC_DBASE :
			for(Pos = 0; Pos < MAX_SIGNAL_ENTRY_DBASE; Pos++)
			{
				if(SignalDbase[Pos].Async == (HANDLE)wParam)
				{
					if(HIWORD(lParam) != 0)
					{
						SignalDbase[Pos].ErrorDb = 1;
#if DBG_MSG
						DoPrintf("##### SignalDatabase: error");
#endif
					}
					SignalDbase[Pos].Done = 1;
#if DBG_MSG
					DoPrintf("##### SignalDatabase: Done");
#endif
					break;
				}
			}
			break;

		default :
			return(DefWindowProc(hWnd, message, wParam, lParam));
	}
    return(0);
}




/*----- 
*
*	Parameter
*		
*
*	Return Value
*		
*----------------------------------------------------------------------------*/

static int AskAsyncDone(SOCKET s, int *Error, int Mask)
{
	int Sts;
	int Pos;

	Sts = NO;
	*Error = 0;
	for(Pos = 0; Pos < MAX_SIGNAL_ENTRY; Pos++)
	{
		if(Signal[Pos].Socket == s)
		{
			*Error = Signal[Pos].Error;
			if(Signal[Pos].Error != 0)
				Sts = YES;
			if((Mask & FD_CONNECT_BIT) && (Signal[Pos].FdConnect != 0))
			{
				Sts = YES;
#if DBG_MSG
				DoPrintf("### Ask: connect (Sts=%d, Error=%d)", Sts, *Error);
#endif
			}
			if((Mask & FD_CLOSE_BIT) && (Signal[Pos].FdClose != 0))
//			if(Mask & FD_CLOSE_BIT)
			{
				Sts = YES;
#if DBG_MSG
				DoPrintf("### Ask: close (Sts=%d, Error=%d)", Sts, *Error);
#endif
			}
			if((Mask & FD_ACCEPT_BIT) && (Signal[Pos].FdAccept != 0))
			{
				Signal[Pos].FdAccept = 0;
				Sts = YES;
#if DBG_MSG
				DoPrintf("### Ask: accept (Sts=%d, Error=%d)", Sts, *Error);
#endif
			}
			if((Mask & FD_READ_BIT) && (Signal[Pos].FdRead != 0))
			{
				Signal[Pos].FdRead = 0;
				Sts = YES;
#if DBG_MSG
				DoPrintf("### Ask: read (Sts=%d, Error=%d)", Sts, *Error);
#endif
			}
			if((Mask & FD_WRITE_BIT) && (Signal[Pos].FdWrite != 0))
			{
				Signal[Pos].FdWrite = 0;
				Sts = YES;
#if DBG_MSG
				DoPrintf("### Ask: write (Sts=%d, Error=%d)", Sts, *Error);
#endif
			}
			break;
		}
	}

	if(Pos == MAX_SIGNAL_ENTRY)
	{
		if(Mask & FD_CLOSE_BIT)
		{
				Sts = YES;
		}
		else
		{
			MessageBox(GetMainHwnd(), "AskAsyncDone called with unregisterd socket.", "FFFTP inner error", MB_OK);
			exit(1);
		}
	}
	return(Sts);
}


/*----- 
*
*	Parameter
*		
*
*	Return Value
*		
*----------------------------------------------------------------------------*/

static int AskAsyncDoneDbase(HANDLE Async, int *Error)
{
	int Sts;
	int Pos;

	Sts = NO;
	*Error = 0;
	for(Pos = 0; Pos < MAX_SIGNAL_ENTRY_DBASE; Pos++)
	{
		if(SignalDbase[Pos].Async == Async)
		{
			if(SignalDbase[Pos].Done != 0)
			{
				*Error = SignalDbase[Pos].ErrorDb;
				Sts = YES;
#if DBG_MSG
				DoPrintf("### Ask: Dbase (Sts=%d, Error=%d)", Sts, *Error);
#endif
			}
			break;
		}
	}

	if(Pos == MAX_SIGNAL_ENTRY_DBASE)
	{
		MessageBox(GetMainHwnd(), "AskAsyncDoneDbase called with unregisterd handle.", "FFFTP inner error", MB_OK);
		exit(1);
	}
	return(Sts);
}



/*----- 
*
*	Parameter
*		
*
*	Return Value
*		
*----------------------------------------------------------------------------*/

static int RegistAsyncTable(SOCKET s)
{
	int Sts;
	int Pos;

	Sts = NO;
	for(Pos = 0; Pos < MAX_SIGNAL_ENTRY; Pos++)
	{
		if(Signal[Pos].Socket == s)
		{
			MessageBox(GetMainHwnd(), "Async socket already registerd.", "FFFTP inner error", MB_OK);
			break;
		}
	}

	if(Pos == MAX_SIGNAL_ENTRY)
	{
		for(Pos = 0; Pos < MAX_SIGNAL_ENTRY; Pos++)
		{
			if(Signal[Pos].Socket == INVALID_SOCKET)
			{

//SetTaskMsg("############### Regist socket (%d)", Pos);

				Signal[Pos].Socket = s;
				Signal[Pos].Error = 0;
				Signal[Pos].FdConnect = 0;
				Signal[Pos].FdClose = 0;
				Signal[Pos].FdAccept = 0;
				Signal[Pos].FdRead = 0;
				Signal[Pos].FdWrite = 0;
				Sts = YES;
				break;
			}
		}

		if(Pos == MAX_SIGNAL_ENTRY)
		{
			MessageBox(GetMainHwnd(), "No more async regist space.", "FFFTP inner error", MB_OK);
			exit(1);
		}
	}

	return(Sts);
}


/*----- 
*
*	Parameter
*		
*
*	Return Value
*		
*----------------------------------------------------------------------------*/

static int RegistAsyncTableDbase(HANDLE Async)
{
	int Sts;
	int Pos;

	Sts = NO;
	for(Pos = 0; Pos < MAX_SIGNAL_ENTRY_DBASE; Pos++)
	{
		if(SignalDbase[Pos].Async == Async)
		{
			MessageBox(GetMainHwnd(), "Async handle already registerd.", "FFFTP inner error", MB_OK);
			break;
		}
	}

	if(Pos == MAX_SIGNAL_ENTRY_DBASE)
	{
		for(Pos = 0; Pos < MAX_SIGNAL_ENTRY; Pos++)
		{
			if(SignalDbase[Pos].Async == 0)
			{

//SetTaskMsg("############### Regist dbase (%d)", Pos);

				SignalDbase[Pos].Async = Async;
				SignalDbase[Pos].Done = 0;
				SignalDbase[Pos].ErrorDb = 0;
				Sts = YES;
				break;
			}
		}

		if(Pos == MAX_SIGNAL_ENTRY_DBASE)
		{
			MessageBox(GetMainHwnd(), "No more async dbase regist space.", "FFFTP inner error", MB_OK);
			exit(1);
		}
	}

	return(Sts);
}


/*----- 
*
*	Parameter
*		
*
*	Return Value
*		
*----------------------------------------------------------------------------*/

static int UnRegistAsyncTable(SOCKET s)
{
	int Sts;
	int Pos;

	Sts = NO;
	for(Pos = 0; Pos < MAX_SIGNAL_ENTRY; Pos++)
	{
		if(Signal[Pos].Socket == s)
		{

//SetTaskMsg("############### UnRegist socket (%d)", Pos);

			Signal[Pos].Socket = INVALID_SOCKET;
			Sts = YES;
			break;
		}
	}
	return(Sts);
}


/*----- 
*
*	Parameter
*		
*
*	Return Value
*		
*----------------------------------------------------------------------------*/

static int UnRegistAsyncTableDbase(HANDLE Async)
{
	int Sts;
	int Pos;

	Sts = NO;
	for(Pos = 0; Pos < MAX_SIGNAL_ENTRY_DBASE; Pos++)
	{
		if(SignalDbase[Pos].Async == Async)
		{

//SetTaskMsg("############### UnRegist dbase (%d)", Pos);

			SignalDbase[Pos].Async = 0;
			Sts = YES;
			break;
		}
	}
	return(Sts);
}








struct hostent *do_gethostbyname(const char *Name, char *Buf, int Len, int *CancelCheckWork)
{
#if USE_THIS
	struct hostent *Ret;
	HANDLE hAsync;
	int Error;

#if DBG_MSG
	DoPrintf("# Start gethostbyname");
#endif
	Ret = NULL;
	*CancelCheckWork = NO;

	hAsync = WSAAsyncGetHostByName(hWndSocket, WM_ASYNC_DBASE, Name, Buf, Len);
	if(hAsync != NULL)
	{
		RegistAsyncTableDbase(hAsync);
		while((*CancelCheckWork == NO) && (AskAsyncDoneDbase(hAsync, &Error) != YES))
		{
			Sleep(1);
			if(BackgrndMessageProc() == YES)
				*CancelCheckWork = YES;
		}

		if(*CancelCheckWork == YES)
		{
			WSACancelAsyncRequest(hAsync);
		}
		else if(Error == 0)
		{
			Ret = (struct hostent *)Buf;
		}
		UnRegistAsyncTableDbase(hAsync);
	}
	return(Ret);
#else
	return(gethostbyname(Name));
#endif
}





SOCKET do_socket(int af, int type, int protocol)
{
	SOCKET Ret;

	Ret = socket(af, type, protocol);
	if(Ret != INVALID_SOCKET)
	{
		RegistAsyncTable(Ret);
	}
#if DBG_MSG
	DoPrintf("# do_socket (S=%x)", Ret);
#endif
	return(Ret);
}



int do_closesocket(SOCKET s)
{
#if USE_THIS
	int Ret;
	int Error;
	int CancelCheckWork;

#if DBG_MSG
	DoPrintf("# Start close (S=%x)", s);
#endif
	CancelCheckWork = NO;

	Ret = closesocket(s);
	if(Ret == SOCKET_ERROR)
	{
		Error = 0;
		while((CancelCheckWork == NO) && (AskAsyncDone(s, &Error, FD_CLOSE_BIT) != YES))
		{
			Sleep(1);
			if(BackgrndMessageProc() == YES)
				CancelCheckWork = YES;
		}

		if((CancelCheckWork == NO) && (Error == 0))
			Ret = 0;
	}

	WSAAsyncSelect(s, hWndSocket, WM_ASYNC_SOCKET, 0);
	if(BackgrndMessageProc() == YES)
		CancelCheckWork = YES;
	UnRegistAsyncTable(s);

#if DBG_MSG
	DoPrintf("# Exit close");
#endif
	return(Ret);
#else
	return(closesocket(s));
#endif
}






int do_connect(SOCKET s, const struct sockaddr *name, int namelen, int *CancelCheckWork)
{
#if USE_THIS
	int Ret;
	int Error;

#if DBG_MSG
	DoPrintf("# Start connect (S=%x)", s);
#endif
	*CancelCheckWork = NO;

#if DBG_MSG
	DoPrintf("## Async set: FD_CONNECT|FD_CLOSE|FD_ACCEPT|FD_READ|FD_WRITE");
#endif
	Ret = WSAAsyncSelect(s, hWndSocket, WM_ASYNC_SOCKET, FD_CONNECT | FD_CLOSE | FD_ACCEPT | FD_READ | FD_WRITE);
	if(Ret != SOCKET_ERROR)
	{
		Ret = connect(s, name, namelen);
		if(Ret == SOCKET_ERROR)
		{
			do
			{
				Error = 0;
				while((*CancelCheckWork == NO) && (AskAsyncDone(s, &Error, FD_CONNECT_BIT) != YES))
				{
					Sleep(1);
					if(BackgrndMessageProc() == YES)
						*CancelCheckWork = YES;
				}

				if(*CancelCheckWork == YES)
					break;
				if(Error == 0)
					Ret = 0;
				else
				{
//					Error = WSAGetLastError();
					DoPrintf("#### Connect: Error=%d", Error);
				}
			}
			while((Ret != 0) && (Error == WSAEWOULDBLOCK));
		}
	}
	else
		DoPrintf("#### Connect: AsyncSelect error (%d)", WSAGetLastError());

#if DBG_MSG
	DoPrintf("# Exit connect (%d)", Ret);
#endif
	return(Ret);
#else
	return(connect(s, name, namelen));
#endif
}





int do_listen(SOCKET s,	int backlog)
{
	int Ret;

	Ret = 1;
#if DBG_MSG
	DoPrintf("# Start listen (S=%x)", s);
	DoPrintf("## Async set: FD_CLOSE|FD_ACCEPT");
#endif

	Ret = WSAAsyncSelect(s, hWndSocket, WM_ASYNC_SOCKET, FD_CLOSE | FD_ACCEPT);
	if(Ret != SOCKET_ERROR)
		Ret = listen(s, backlog);

#if DBG_MSG
	DoPrintf("# Exit listen (%d)", Ret);
#endif
	return(Ret);
}



SOCKET do_accept(SOCKET s, struct sockaddr *addr, int *addrlen)
{
#if USE_THIS
	SOCKET Ret2;
	int CancelCheckWork;
	int Error;

#if DBG_MSG
	DoPrintf("# Start accept (S=%x)", s);
#endif
	CancelCheckWork = NO;
	Ret2 = INVALID_SOCKET;
	Error = 0;

	while((CancelCheckWork == NO) && (AskAsyncDone(s, &Error, FD_ACCEPT_BIT) != YES))
	{
		if(AskAsyncDone(s, &Error, FD_CLOSE_BIT) == YES)
		{
			Error = 1;
			break;
		}
		Sleep(1);
		if(BackgrndMessageProc() == YES)
			CancelCheckWork = YES;
	}

	if((CancelCheckWork == NO) && (Error == 0))
	{
		do
		{
			Ret2 = accept(s, addr, addrlen);
			if(Ret2 != INVALID_SOCKET)
			{
#if DBG_MSG
				DoPrintf("## do_sccept (S=%x)", Ret2);
				DoPrintf("## Async set: FD_CONNECT|FD_CLOSE|FD_ACCEPT|FD_READ|FD_WRITE");
#endif
				RegistAsyncTable(Ret2);
				if(WSAAsyncSelect(Ret2, hWndSocket, WM_ASYNC_SOCKET, FD_CONNECT | FD_CLOSE | FD_ACCEPT | FD_READ | FD_WRITE) == SOCKET_ERROR)
				{
					do_closesocket(Ret2);
					Ret2 = INVALID_SOCKET;
				}
				break;
			}
			Error = WSAGetLastError();
			Sleep(1);
			if(BackgrndMessageProc() == YES)
				break;
		}
		while(Error == WSAEWOULDBLOCK);
	}

#if DBG_MSG
	DoPrintf("# Exit accept");
#endif
	return(Ret2);
#else
	return(accept(s, addr, addrlen));
#endif
}




/*----- recv�����̊֐� --------------------------------------------------------
*
*	Parameter
*		SOCKET s : �\�P�b�g
*		char *buf : �f�[�^��ǂݍ��ރo�b�t�@
*		int len : ����
*		int flags : recv�ɗ^����t���O
*		int *TimeOutErr : �^�C���A�E�g�������ǂ�����Ԃ����[�N
*
*	Return Value
*		int : recv�̖߂�l�Ɠ���
*
*	Note
*		�^�C���A�E�g�̎��� TimeOut=YES�ARet=SOCKET_ERROR �ɂȂ�
*----------------------------------------------------------------------------*/
int do_recv(SOCKET s, char *buf, int len, int flags, int *TimeOutErr, int *CancelCheckWork)
{
#if USE_THIS
	int Ret;
	time_t StartTime;
	time_t ElapseTime;
	int Error;

#if DBG_MSG
	DoPrintf("# Start recv (S=%x)", s);
#endif
	*TimeOutErr = NO;
	*CancelCheckWork = NO;
	Ret = SOCKET_ERROR;
	Error = 0;

	if(TimeOut != 0)
		time(&StartTime);

	while((*CancelCheckWork == NO) && (AskAsyncDone(s, &Error, FD_READ_BIT) != YES))
	{
		if(AskAsyncDone(s, &Error, FD_CLOSE_BIT) == YES)
		{
			Ret = 0;
			break;
		}
		Sleep(1);
		if(BackgrndMessageProc() == YES)
			*CancelCheckWork = YES;
		else if(TimeOut != 0)
		{
			time(&ElapseTime);
			ElapseTime -= StartTime;
			if(ElapseTime >= TimeOut)
			{
				DoPrintf("do_recv timed out");
				*TimeOutErr = YES;
				*CancelCheckWork = YES;
			}
		}
	}

	if(/*(Ret != 0) && */(Error == 0) && (*CancelCheckWork == NO) && (*TimeOutErr == NO))
	{
		do
		{
#if DBG_MSG
			DoPrintf("## recv()");
#endif

			Ret = recv(s, buf, len, flags);
			if(Ret != SOCKET_ERROR)
				break;
			Error = WSAGetLastError();
			Sleep(1);
			if(BackgrndMessageProc() == YES)
				break;
		}
		while(Error == WSAEWOULDBLOCK);
	}

	if(BackgrndMessageProc() == YES)
		Ret = SOCKET_ERROR;

#if DBG_MSG
	DoPrintf("# Exit recv (%d)", Ret);
#endif
	return(Ret);
#else
	return(recv(s, buf, len, flags));
#endif
}



int do_send(SOCKET s, const char *buf, int len, int flags, int *TimeOutErr, int *CancelCheckWork)
{
#if USE_THIS
	int Ret;
	time_t StartTime;
	time_t ElapseTime;
	int Error;

#if DBG_MSG
	DoPrintf("# Start send (S=%x)", s);
#endif
	*TimeOutErr = NO;
	*CancelCheckWork = NO;
	Ret = SOCKET_ERROR;
	Error = 0;

	if(TimeOut != 0)
		time(&StartTime);

#if DBG_MSG
	DoPrintf("## Async set: FD_CONNECT|FD_CLOSE|FD_ACCEPT|FD_READ|FD_WRITE");
#endif
	WSAAsyncSelect(s, hWndSocket, WM_ASYNC_SOCKET, FD_CONNECT | FD_CLOSE | FD_ACCEPT | FD_READ | FD_WRITE);
	if(BackgrndMessageProc() == YES)
		*CancelCheckWork = YES;

	while((*CancelCheckWork == NO) && (AskAsyncDone(s, &Error, FD_WRITE_BIT) != YES))
	{
		if(AskAsyncDone(s, &Error, FD_CLOSE_BIT) == YES)
		{
			Error = 1;
			break;
		}

		Sleep(1);
		if(BackgrndMessageProc() == YES)
			*CancelCheckWork = YES;
		else if(TimeOut != 0)
		{
			time(&ElapseTime);
			ElapseTime -= StartTime;
			if(ElapseTime >= TimeOut)
			{
				DoPrintf("do_write timed out");
				*TimeOutErr = YES;
				*CancelCheckWork = YES;
			}
		}
	}

	if((Error == 0) && (*CancelCheckWork == NO) && (*TimeOutErr == NO))
	{
		do
		{
#if DBG_MSG
			DoPrintf("## send()");
#endif

			Ret = send(s, buf, len, flags);
			if(Ret != SOCKET_ERROR)
			{
#if DBG_MSG
				DoPrintf("## send() OK");
#endif
				break;
			}
			Error = WSAGetLastError();
			Sleep(1);
			if(BackgrndMessageProc() == YES)
				break;
		}
		while(Error == WSAEWOULDBLOCK);
	}

	if(BackgrndMessageProc() == YES)
		Ret = SOCKET_ERROR;

#if DBG_MSG
	DoPrintf("# Exit send (%d)", Ret);
#endif
	return(Ret);
#else
	return(send(s, buf, len, flags));
#endif
}


/*----- 
*
*	Parameter
*
*	Return Value
*		int �X�e�[�^�X
*			SUCCESS/FAIL
*----------------------------------------------------------------------------*/

int CheckClosedAndReconnect(void)
{
	int Error;
	int Sts;

//SetTaskMsg("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

	Sts = SUCCESS;
	if(AskAsyncDone(AskCmdCtrlSkt(), &Error, FD_CLOSE_BIT) == YES)
	{
		Sts = ReConnectCmdSkt();
	}
	return(Sts);
}



