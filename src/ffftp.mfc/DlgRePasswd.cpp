// DlgRePasswd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgRePasswd.h"
#include "afxdialogex.h"


// DlgRePasswd ダイアログ

IMPLEMENT_DYNAMIC(DlgRePasswd, CDialogEx)

DlgRePasswd::DlgRePasswd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(re_passwd_dlg, pParent)
{

}

DlgRePasswd::~DlgRePasswd()
{
}

void DlgRePasswd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgRePasswd, CDialogEx)
END_MESSAGE_MAP()


// DlgRePasswd メッセージ ハンドラー
