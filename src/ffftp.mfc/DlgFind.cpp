// DlgFind.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgFind.h"
#include "afxdialogex.h"


// DlgFind ダイアログ

IMPLEMENT_DYNAMIC(DlgFind, CDialogEx)

DlgFind::DlgFind(CWnd* pParent /*=nullptr*/)
	: CDialogEx(find_dlg, pParent)
{

}

DlgFind::~DlgFind()
{
}

void DlgFind::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgFind, CDialogEx)
END_MESSAGE_MAP()


// DlgFind メッセージ ハンドラー
