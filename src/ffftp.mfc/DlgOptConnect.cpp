// DlgOptConnect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptConnect.h"
#include "afxdialogex.h"


// DlgOptConnect ダイアログ

IMPLEMENT_DYNAMIC(DlgOptConnect, CDialogEx)

DlgOptConnect::DlgOptConnect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_connect_dlg, pParent)
{

}

DlgOptConnect::~DlgOptConnect()
{
}

void DlgOptConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptConnect, CDialogEx)
END_MESSAGE_MAP()


// DlgOptConnect メッセージ ハンドラー
