// DlgOptMisc.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptMisc.h"
#include "afxdialogex.h"


// DlgOptMisc ダイアログ

IMPLEMENT_DYNAMIC(DlgOptMisc, CDialogEx)

DlgOptMisc::DlgOptMisc(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_misc_dlg, pParent)
{

}

DlgOptMisc::~DlgOptMisc()
{
}

void DlgOptMisc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptMisc, CDialogEx)
END_MESSAGE_MAP()


// DlgOptMisc メッセージ ハンドラー
