// DlgSort.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgSort.h"
#include "afxdialogex.h"


// DlgSort ダイアログ

IMPLEMENT_DYNAMIC(DlgSort, CDialogEx)

DlgSort::DlgSort(CWnd* pParent /*=nullptr*/)
	: CDialogEx(sort_dlg, pParent)
{

}

DlgSort::~DlgSort()
{
}

void DlgSort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSort, CDialogEx)
END_MESSAGE_MAP()


// DlgSort メッセージ ハンドラー
