// DlgCwdErr.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgCwdErr.h"
#include "afxdialogex.h"


// DlgCwdErr ダイアログ

IMPLEMENT_DYNAMIC(DlgCwdErr, CDialogEx)

DlgCwdErr::DlgCwdErr(CWnd* pParent /*=nullptr*/)
	: CDialogEx(cwderr_dlg, pParent)
{

}

DlgCwdErr::~DlgCwdErr()
{
}

void DlgCwdErr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgCwdErr, CDialogEx)
END_MESSAGE_MAP()


// DlgCwdErr メッセージ ハンドラー
