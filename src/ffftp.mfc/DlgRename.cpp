// DlgRename.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgRename.h"
#include "afxdialogex.h"


// DlgRename ダイアログ

IMPLEMENT_DYNAMIC(DlgRename, CDialogEx)

DlgRename::DlgRename(CWnd* pParent /*=nullptr*/)
	: CDialogEx(rename_dlg, pParent)
{

}

DlgRename::~DlgRename()
{
}

void DlgRename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgRename, CDialogEx)
END_MESSAGE_MAP()


// DlgRename メッセージ ハンドラー
