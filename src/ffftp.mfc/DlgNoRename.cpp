// DlgNoRename.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgNoRename.h"
#include "afxdialogex.h"


// DlgNoRename ダイアログ

IMPLEMENT_DYNAMIC(DlgNoRename, CDialogEx)

DlgNoRename::DlgNoRename(CWnd* pParent /*=nullptr*/)
	: CDialogEx(noresume_dlg, pParent)
{

}

DlgNoRename::~DlgNoRename()
{
}

void DlgNoRename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgNoRename, CDialogEx)
END_MESSAGE_MAP()


// DlgNoRename メッセージ ハンドラー
