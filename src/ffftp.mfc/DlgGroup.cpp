// DlgGroup.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgGroup.h"
#include "afxdialogex.h"


// DlgGroup ダイアログ

IMPLEMENT_DYNAMIC(DlgGroup, CDialogEx)

DlgGroup::DlgGroup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(group_dlg, pParent)
{

}

DlgGroup::~DlgGroup()
{
}

void DlgGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgGroup, CDialogEx)
END_MESSAGE_MAP()


// DlgGroup メッセージ ハンドラー
