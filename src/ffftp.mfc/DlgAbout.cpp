// DlgAbout.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgAbout.h"
#include "afxdialogex.h"


// DlgAbout ダイアログ

IMPLEMENT_DYNAMIC(DlgAbout, CDialogEx)

DlgAbout::DlgAbout(CWnd* pParent /*=nullptr*/)
	: CDialogEx(about_dlg, pParent)
{

}

DlgAbout::~DlgAbout()
{
}

void DlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgAbout, CDialogEx)
END_MESSAGE_MAP()


// DlgAbout メッセージ ハンドラー
