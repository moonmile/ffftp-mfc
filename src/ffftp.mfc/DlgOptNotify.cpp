// DlgOptNotify.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptNotify.h"
#include "afxdialogex.h"


// DlgOptNotify ダイアログ

IMPLEMENT_DYNAMIC(DlgOptNotify, CDialogEx)

DlgOptNotify::DlgOptNotify(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_notify_dlg, pParent)
{

}

DlgOptNotify::~DlgOptNotify()
{
}

void DlgOptNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptNotify, CDialogEx)
END_MESSAGE_MAP()


// DlgOptNotify メッセージ ハンドラー
