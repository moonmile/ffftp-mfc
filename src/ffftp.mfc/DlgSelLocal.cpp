// DlgSelLocal.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgSelLocal.h"
#include "afxdialogex.h"


// DlgSelLocal ダイアログ

IMPLEMENT_DYNAMIC(DlgSelLocal, CDialogEx)

DlgSelLocal::DlgSelLocal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(sel_local_dlg, pParent)
{

}

DlgSelLocal::~DlgSelLocal()
{
}

void DlgSelLocal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSelLocal, CDialogEx)
END_MESSAGE_MAP()


// DlgSelLocal メッセージ ハンドラー
