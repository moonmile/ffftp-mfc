// DlgFnameIn.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgFnameIn.h"
#include "afxdialogex.h"


// DlgFnameIn ダイアログ

IMPLEMENT_DYNAMIC(DlgFnameIn, CDialogEx)

DlgFnameIn::DlgFnameIn(CWnd* pParent /*=nullptr*/)
	: CDialogEx(fname_in_dlg, pParent)
{

}

DlgFnameIn::~DlgFnameIn()
{
}

void DlgFnameIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgFnameIn, CDialogEx)
END_MESSAGE_MAP()


// DlgFnameIn メッセージ ハンドラー
