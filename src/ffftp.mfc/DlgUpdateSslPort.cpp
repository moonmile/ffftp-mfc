// DlgUpdateSslPort.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgUpdateSslPort.h"
#include "afxdialogex.h"


// DlgUpdateSslPort ダイアログ

IMPLEMENT_DYNAMIC(DlgUpdateSslPort, CDialogEx)

DlgUpdateSslPort::DlgUpdateSslPort(CWnd* pParent /*=nullptr*/)
	: CDialogEx(updatesslroot_dlg, pParent)
{

}

DlgUpdateSslPort::~DlgUpdateSslPort()
{
}

void DlgUpdateSslPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgUpdateSslPort, CDialogEx)
END_MESSAGE_MAP()


// DlgUpdateSslPort メッセージ ハンドラー
