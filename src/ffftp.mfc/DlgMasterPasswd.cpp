// DlgMasterPasswd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgMasterPasswd.h"
#include "afxdialogex.h"


// DlgMasterPasswd ダイアログ

IMPLEMENT_DYNAMIC(DlgMasterPasswd, CDialogEx)

DlgMasterPasswd::DlgMasterPasswd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(masterpasswd_dlg, pParent)
{

}

DlgMasterPasswd::~DlgMasterPasswd()
{
}

void DlgMasterPasswd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMasterPasswd, CDialogEx)
END_MESSAGE_MAP()


// DlgMasterPasswd メッセージ ハンドラー
