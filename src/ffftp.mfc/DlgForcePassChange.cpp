// DlgForcePassChange.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgForcePassChange.h"
#include "afxdialogex.h"


// DlgForcePassChange ダイアログ

IMPLEMENT_DYNAMIC(DlgForcePassChange, CDialogEx)

DlgForcePassChange::DlgForcePassChange(CWnd* pParent /*=nullptr*/)
	: CDialogEx(forcepasschange_dlg, pParent)
{

}

DlgForcePassChange::~DlgForcePassChange()
{
}

void DlgForcePassChange::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgForcePassChange, CDialogEx)
END_MESSAGE_MAP()


// DlgForcePassChange メッセージ ハンドラー
