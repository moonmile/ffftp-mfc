// DlgDownName.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgDownName.h"
#include "afxdialogex.h"


// DlgDownName ダイアログ

IMPLEMENT_DYNAMIC(DlgDownName, CDialogEx)

DlgDownName::DlgDownName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(downname_dlg, pParent)
{

}

DlgDownName::~DlgDownName()
{
}

void DlgDownName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgDownName, CDialogEx)
END_MESSAGE_MAP()


// DlgDownName メッセージ ハンドラー
