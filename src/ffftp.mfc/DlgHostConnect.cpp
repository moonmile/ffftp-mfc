// DlgHostConnect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHostConnect.h"
#include "afxdialogex.h"


// DlgHostConnect ダイアログ

IMPLEMENT_DYNAMIC(DlgHostConnect, CDialogEx)

DlgHostConnect::DlgHostConnect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hostconnect_dlg, pParent)
{

}

DlgHostConnect::~DlgHostConnect()
{
}

void DlgHostConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHostConnect, CDialogEx)
END_MESSAGE_MAP()


// DlgHostConnect メッセージ ハンドラー
