// DlgPasswd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgPasswd.h"
#include "afxdialogex.h"


// DlgPasswd ダイアログ

IMPLEMENT_DYNAMIC(DlgPasswd, CDialogEx)

DlgPasswd::DlgPasswd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(passwd_dlg, pParent)
{

}

DlgPasswd::~DlgPasswd()
{
}

void DlgPasswd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgPasswd, CDialogEx)
END_MESSAGE_MAP()


// DlgPasswd メッセージ ハンドラー
