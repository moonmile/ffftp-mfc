// DlgOptFire.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptFire.h"
#include "afxdialogex.h"


// DlgOptFire ダイアログ

IMPLEMENT_DYNAMIC(DlgOptFire, CDialogEx)

DlgOptFire::DlgOptFire(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_fire_dlg, pParent)
{

}

DlgOptFire::~DlgOptFire()
{
}

void DlgOptFire::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptFire, CDialogEx)
END_MESSAGE_MAP()


// DlgOptFire メッセージ ハンドラー
