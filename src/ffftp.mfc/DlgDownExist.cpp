// DlgDownExist.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgDownExist.h"
#include "afxdialogex.h"


// DlgDownExist ダイアログ

IMPLEMENT_DYNAMIC(DlgDownExist, CDialogEx)

DlgDownExist::DlgDownExist(CWnd* pParent /*=nullptr*/)
	: CDialogEx(down_exist_dlg, pParent)
{

}

DlgDownExist::~DlgDownExist()
{
}

void DlgDownExist::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgDownExist, CDialogEx)
END_MESSAGE_MAP()


// DlgDownExist メッセージ ハンドラー
