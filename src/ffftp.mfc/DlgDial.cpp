// DlgDial.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgDial.h"
#include "afxdialogex.h"


// DlgDial ダイアログ

IMPLEMENT_DYNAMIC(DlgDial, CDialogEx)

DlgDial::DlgDial(CWnd* pParent /*=nullptr*/)
	: CDialogEx(dial_dlg, pParent)
{

}

DlgDial::~DlgDial()
{
}

void DlgDial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgDial, CDialogEx)
END_MESSAGE_MAP()


// DlgDial メッセージ ハンドラー
