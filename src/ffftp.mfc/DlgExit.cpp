// DlgExit.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgExit.h"
#include "afxdialogex.h"


// DlgExit ダイアログ

IMPLEMENT_DYNAMIC(DlgExit, CDialogEx)

DlgExit::DlgExit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(exit_dlg, pParent)
{

}

DlgExit::~DlgExit()
{
}

void DlgExit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgExit, CDialogEx)
END_MESSAGE_MAP()


// DlgExit メッセージ ハンドラー
