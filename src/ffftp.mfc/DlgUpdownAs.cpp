// DlgUpdownAs.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgUpdownAs.h"
#include "afxdialogex.h"


// DlgUpdownAs ダイアログ

IMPLEMENT_DYNAMIC(DlgUpdownAs, CDialogEx)

DlgUpdownAs::DlgUpdownAs(CWnd* pParent /*=nullptr*/)
	: CDialogEx(updown_as_dlg, pParent)
{

}

DlgUpdownAs::~DlgUpdownAs()
{
}

void DlgUpdownAs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgUpdownAs, CDialogEx)
END_MESSAGE_MAP()


// DlgUpdownAs メッセージ ハンドラー
