// DlgBmark.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgBmark.h"
#include "afxdialogex.h"


// DlgBmark ダイアログ

IMPLEMENT_DYNAMIC(DlgBmark, CDialogEx)

DlgBmark::DlgBmark(CWnd* pParent /*=nullptr*/)
	: CDialogEx(bmark_dlg, pParent)
{

}

DlgBmark::~DlgBmark()
{
}

void DlgBmark::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgBmark, CDialogEx)
END_MESSAGE_MAP()


// DlgBmark メッセージ ハンドラー
