// DlgGroupDel.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgGroupDel.h"
#include "afxdialogex.h"


// DlgGroupDel ダイアログ

IMPLEMENT_DYNAMIC(DlgGroupDel, CDialogEx)

DlgGroupDel::DlgGroupDel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(groupdel_dlg, pParent)
{

}

DlgGroupDel::~DlgGroupDel()
{
}

void DlgGroupDel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgGroupDel, CDialogEx)
END_MESSAGE_MAP()


// DlgGroupDel メッセージ ハンドラー
