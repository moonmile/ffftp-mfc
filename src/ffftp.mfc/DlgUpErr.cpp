// DlgUpErr.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgUpErr.h"
#include "afxdialogex.h"


// DlgUpErr ダイアログ

IMPLEMENT_DYNAMIC(DlgUpErr, CDialogEx)

DlgUpErr::DlgUpErr(CWnd* pParent /*=nullptr*/)
	: CDialogEx(uperr_dlg, pParent)
{

}

DlgUpErr::~DlgUpErr()
{
}

void DlgUpErr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgUpErr, CDialogEx)
END_MESSAGE_MAP()


// DlgUpErr メッセージ ハンドラー
