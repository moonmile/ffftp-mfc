// DlgNewMasterPasswd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgNewMasterPasswd.h"
#include "afxdialogex.h"


// DlgNewMasterPasswd ダイアログ

IMPLEMENT_DYNAMIC(DlgNewMasterPasswd, CDialogEx)

DlgNewMasterPasswd::DlgNewMasterPasswd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(newmasterpasswd_dlg, pParent)
{

}

DlgNewMasterPasswd::~DlgNewMasterPasswd()
{
}

void DlgNewMasterPasswd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgNewMasterPasswd, CDialogEx)
END_MESSAGE_MAP()


// DlgNewMasterPasswd メッセージ ハンドラー
