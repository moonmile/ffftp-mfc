// DlgMailAdrs.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgMailAdrs.h"
#include "afxdialogex.h"


// DlgMailAdrs ダイアログ

IMPLEMENT_DYNAMIC(DlgMailAdrs, CDialogEx)

DlgMailAdrs::DlgMailAdrs(CWnd* pParent /*=nullptr*/)
	: CDialogEx(mailadrs_dlg, pParent)
{

}

DlgMailAdrs::~DlgMailAdrs()
{
}

void DlgMailAdrs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMailAdrs, CDialogEx)
END_MESSAGE_MAP()


// DlgMailAdrs メッセージ ハンドラー
