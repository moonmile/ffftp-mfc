// DlgAccount.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgAccount.h"
#include "afxdialogex.h"


// DlgAccount ダイアログ

IMPLEMENT_DYNAMIC(DlgAccount, CDialogEx)

DlgAccount::DlgAccount(CWnd* pParent /*=nullptr*/)
	: CDialogEx(account_dlg, pParent)
{

}

DlgAccount::~DlgAccount()
{
}

void DlgAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgAccount, CDialogEx)
END_MESSAGE_MAP()


// DlgAccount メッセージ ハンドラー
