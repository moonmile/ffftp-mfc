// DlgHostList.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHostList.h"
#include "afxdialogex.h"


// DlgHostList ダイアログ

IMPLEMENT_DYNAMIC(DlgHostList, CDialogEx)

DlgHostList::DlgHostList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hostlist_dlg, pParent)
{

}

DlgHostList::~DlgHostList()
{
}

void DlgHostList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHostList, CDialogEx)
END_MESSAGE_MAP()


// DlgHostList メッセージ ハンドラー
