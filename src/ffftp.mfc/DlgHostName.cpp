// DlgHostName.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHostName.h"
#include "afxdialogex.h"


// DlgHostName ダイアログ

IMPLEMENT_DYNAMIC(DlgHostName, CDialogEx)

DlgHostName::DlgHostName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hostname_dlg, pParent)
{

}

DlgHostName::~DlgHostName()
{
}

void DlgHostName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHostName, CDialogEx)
END_MESSAGE_MAP()


// DlgHostName メッセージ ハンドラー
