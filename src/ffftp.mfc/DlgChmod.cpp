// DlgChmod.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgChmod.h"
#include "afxdialogex.h"


// DlgChmod ダイアログ

IMPLEMENT_DYNAMIC(DlgChmod, CDialogEx)

DlgChmod::DlgChmod(CWnd* pParent /*=nullptr*/)
	: CDialogEx(chmod_dlg, pParent)
{

}

DlgChmod::~DlgChmod()
{
}

void DlgChmod::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgChmod, CDialogEx)
END_MESSAGE_MAP()


// DlgChmod メッセージ ハンドラー
