// DlgDelete.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgDelete.h"
#include "afxdialogex.h"


// DlgDelete ダイアログ

IMPLEMENT_DYNAMIC(DlgDelete, CDialogEx)

DlgDelete::DlgDelete(CWnd* pParent /*=nullptr*/)
	: CDialogEx(delete_dlg, pParent)
{

}

DlgDelete::~DlgDelete()
{
}

void DlgDelete::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgDelete, CDialogEx)
END_MESSAGE_MAP()


// DlgDelete メッセージ ハンドラー
