// DlgChdirBr.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgChdirBr.h"
#include "afxdialogex.h"


// DlgChdirBr ダイアログ

IMPLEMENT_DYNAMIC(DlgChdirBr, CDialogEx)

DlgChdirBr::DlgChdirBr(CWnd* pParent /*=nullptr*/)
	: CDialogEx(chdir_br_dlg, pParent)
{

}

DlgChdirBr::~DlgChdirBr()
{
}

void DlgChdirBr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgChdirBr, CDialogEx)
END_MESSAGE_MAP()


// DlgChdirBr メッセージ ハンドラー
