// DlgOptUpdates.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptUpdates.h"
#include "afxdialogex.h"


// DlgOptUpdates ダイアログ

IMPLEMENT_DYNAMIC(DlgOptUpdates, CDialogEx)

DlgOptUpdates::DlgOptUpdates(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_updates_dlg, pParent)
{

}

DlgOptUpdates::~DlgOptUpdates()
{
}

void DlgOptUpdates::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptUpdates, CDialogEx)
END_MESSAGE_MAP()


// DlgOptUpdates メッセージ ハンドラー
