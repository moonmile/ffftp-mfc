// DlgDiskfull.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgDiskfull.h"
#include "afxdialogex.h"


// DlgDiskfull ダイアログ

IMPLEMENT_DYNAMIC(DlgDiskfull, CDialogEx)

DlgDiskfull::DlgDiskfull(CWnd* pParent /*=nullptr*/)
	: CDialogEx(diskfull_dlg, pParent)
{

}

DlgDiskfull::~DlgDiskfull()
{
}

void DlgDiskfull::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgDiskfull, CDialogEx)
END_MESSAGE_MAP()


// DlgDiskfull メッセージ ハンドラー
