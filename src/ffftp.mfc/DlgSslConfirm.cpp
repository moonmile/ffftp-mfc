// DlgSslConfirm.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgSslConfirm.h"
#include "afxdialogex.h"


// DlgSslConfirm ダイアログ

IMPLEMENT_DYNAMIC(DlgSslConfirm, CDialogEx)

DlgSslConfirm::DlgSslConfirm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ssl_confirm_dlg, pParent)
{

}

DlgSslConfirm::~DlgSslConfirm()
{
}

void DlgSslConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSslConfirm, CDialogEx)
END_MESSAGE_MAP()


// DlgSslConfirm メッセージ ハンドラー
