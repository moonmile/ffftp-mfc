// DlgHsetMain.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHsetMain.h"
#include "afxdialogex.h"


// DlgHsetMain ダイアログ

IMPLEMENT_DYNAMIC(DlgHsetMain, CDialogEx)

DlgHsetMain::DlgHsetMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hset_main_dlg, pParent)
{

}

DlgHsetMain::~DlgHsetMain()
{
}

void DlgHsetMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHsetMain, CDialogEx)
END_MESSAGE_MAP()


// DlgHsetMain メッセージ ハンドラー
