// DlgHSetDialup.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHSetDialup.h"
#include "afxdialogex.h"


// DlgHSetDialup ダイアログ

IMPLEMENT_DYNAMIC(DlgHSetDialup, CDialogEx)

DlgHSetDialup::DlgHSetDialup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hset_dialup_dlg, pParent)
{

}

DlgHSetDialup::~DlgHSetDialup()
{
}

void DlgHSetDialup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHSetDialup, CDialogEx)
END_MESSAGE_MAP()


// DlgHSetDialup メッセージ ハンドラー
