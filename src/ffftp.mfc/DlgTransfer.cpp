// DlgTransfer.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgTransfer.h"
#include "afxdialogex.h"


// DlgTransfer ダイアログ

IMPLEMENT_DYNAMIC(DlgTransfer, CDialogEx)

DlgTransfer::DlgTransfer(CWnd* pParent /*=nullptr*/)
	: CDialogEx(transfer_dlg, pParent)
{

}

DlgTransfer::~DlgTransfer()
{
}

void DlgTransfer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgTransfer, CDialogEx)
END_MESSAGE_MAP()


// DlgTransfer メッセージ ハンドラー
