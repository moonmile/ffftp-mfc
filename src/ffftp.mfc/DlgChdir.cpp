// DlgChdir.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgChdir.h"
#include "afxdialogex.h"


// DlgChdir ダイアログ

IMPLEMENT_DYNAMIC(DlgChdir, CDialogEx)

DlgChdir::DlgChdir(CWnd* pParent /*=nullptr*/)
	: CDialogEx(chdir_dlg, pParent)
{

}

DlgChdir::~DlgChdir()
{
}

void DlgChdir::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgChdir, CDialogEx)
END_MESSAGE_MAP()


// DlgChdir メッセージ ハンドラー
