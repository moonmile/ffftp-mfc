// DlgDialPassword.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgDialPassword.h"
#include "afxdialogex.h"


// DlgDialPassword ダイアログ

IMPLEMENT_DYNAMIC(DlgDialPassword, CDialogEx)

DlgDialPassword::DlgDialPassword(CWnd* pParent /*=nullptr*/)
	: CDialogEx(dial_password_dlg, pParent)
{

}

DlgDialPassword::~DlgDialPassword()
{
}

void DlgDialPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgDialPassword, CDialogEx)
END_MESSAGE_MAP()


// DlgDialPassword メッセージ ハンドラー
