// DlgRasNotify.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgRasNotify.h"
#include "afxdialogex.h"


// DlgRasNotify ダイアログ

IMPLEMENT_DYNAMIC(DlgRasNotify, CDialogEx)

DlgRasNotify::DlgRasNotify(CWnd* pParent /*=nullptr*/)
	: CDialogEx(rasnotify_dlg, pParent)
{

}

DlgRasNotify::~DlgRasNotify()
{
}

void DlgRasNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgRasNotify, CDialogEx)
END_MESSAGE_MAP()


// DlgRasNotify メッセージ ハンドラー
