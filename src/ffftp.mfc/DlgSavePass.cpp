// DlgSavePass.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgSavePass.h"
#include "afxdialogex.h"


// DlgSavePass ダイアログ

IMPLEMENT_DYNAMIC(DlgSavePass, CDialogEx)

DlgSavePass::DlgSavePass(CWnd* pParent /*=nullptr*/)
	: CDialogEx(savepass_dlg, pParent)
{

}

DlgSavePass::~DlgSavePass()
{
}

void DlgSavePass::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSavePass, CDialogEx)
END_MESSAGE_MAP()


// DlgSavePass メッセージ ハンドラー
