// DlgDefAttr.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgDefAttr.h"
#include "afxdialogex.h"


// DlgDefAttr ダイアログ

IMPLEMENT_DYNAMIC(DlgDefAttr, CDialogEx)

DlgDefAttr::DlgDefAttr(CWnd* pParent /*=nullptr*/)
	: CDialogEx(def_attr_dlg, pParent)
{

}

DlgDefAttr::~DlgDefAttr()
{
}

void DlgDefAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgDefAttr, CDialogEx)
END_MESSAGE_MAP()


// DlgDefAttr メッセージ ハンドラー
