// DlgForceRename.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgForceRename.h"
#include "afxdialogex.h"


// DlgForceRename ダイアログ

IMPLEMENT_DYNAMIC(DlgForceRename, CDialogEx)

DlgForceRename::DlgForceRename(CWnd* pParent /*=nullptr*/)
	: CDialogEx(forcerename_dlg, pParent)
{

}

DlgForceRename::~DlgForceRename()
{
}

void DlgForceRename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgForceRename, CDialogEx)
END_MESSAGE_MAP()


// DlgForceRename メッセージ ハンドラー
