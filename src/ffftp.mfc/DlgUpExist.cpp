// DlgUpExist.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgUpExist.h"
#include "afxdialogex.h"


// DlgUpExist ダイアログ

IMPLEMENT_DYNAMIC(DlgUpExist, CDialogEx)

DlgUpExist::DlgUpExist(CWnd* pParent /*=nullptr*/)
	: CDialogEx(up_exist_dlg, pParent)
{

}

DlgUpExist::~DlgUpExist()
{
}

void DlgUpExist::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgUpExist, CDialogEx)
END_MESSAGE_MAP()


// DlgUpExist メッセージ ハンドラー
