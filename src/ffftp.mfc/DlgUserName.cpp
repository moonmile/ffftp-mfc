// DlgUserName.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgUserName.h"
#include "afxdialogex.h"


// DlgUserName ダイアログ

IMPLEMENT_DYNAMIC(DlgUserName, CDialogEx)

DlgUserName::DlgUserName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(username_dlg, pParent)
{

}

DlgUserName::~DlgUserName()
{
}

void DlgUserName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgUserName, CDialogEx)
END_MESSAGE_MAP()


// DlgUserName メッセージ ハンドラー
