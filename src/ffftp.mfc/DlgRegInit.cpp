// DlgRegInit.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgRegInit.h"
#include "afxdialogex.h"


// DlgRegInit ダイアログ

IMPLEMENT_DYNAMIC(DlgRegInit, CDialogEx)

DlgRegInit::DlgRegInit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(reginit_dlg, pParent)
{

}

DlgRegInit::~DlgRegInit()
{
}

void DlgRegInit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgRegInit, CDialogEx)
END_MESSAGE_MAP()


// DlgRegInit メッセージ ハンドラー
