// DlgMkdir.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgMkdir.h"
#include "afxdialogex.h"


// DlgMkdir ダイアログ

IMPLEMENT_DYNAMIC(DlgMkdir, CDialogEx)

DlgMkdir::DlgMkdir(CWnd* pParent /*=nullptr*/)
	: CDialogEx(mkdir_dlg, pParent)
{

}

DlgMkdir::~DlgMkdir()
{
}

void DlgMkdir::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMkdir, CDialogEx)
END_MESSAGE_MAP()


// DlgMkdir メッセージ ハンドラー
