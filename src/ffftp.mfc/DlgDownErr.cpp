// DlgDownErr.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgDownErr.h"
#include "afxdialogex.h"


// DlgDownErr ダイアログ

IMPLEMENT_DYNAMIC(DlgDownErr, CDialogEx)

DlgDownErr::DlgDownErr(CWnd* pParent /*=nullptr*/)
	: CDialogEx(downerr_dlg, pParent)
{

}

DlgDownErr::~DlgDownErr()
{
}

void DlgDownErr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgDownErr, CDialogEx)
END_MESSAGE_MAP()


// DlgDownErr メッセージ ハンドラー
