// DlgFileSizeNotify.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgFileSizeNotify.h"
#include "afxdialogex.h"


// DlgFileSizeNotify ダイアログ

IMPLEMENT_DYNAMIC(DlgFileSizeNotify, CDialogEx)

DlgFileSizeNotify::DlgFileSizeNotify(CWnd* pParent /*=nullptr*/)
	: CDialogEx(filesize_notify_dlg, pParent)
{

}

DlgFileSizeNotify::~DlgFileSizeNotify()
{
}

void DlgFileSizeNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgFileSizeNotify, CDialogEx)
END_MESSAGE_MAP()


// DlgFileSizeNotify メッセージ ハンドラー
