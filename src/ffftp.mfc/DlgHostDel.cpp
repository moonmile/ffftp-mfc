// DlgHostDel.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHostDel.h"
#include "afxdialogex.h"


// DlgHostDel ダイアログ

IMPLEMENT_DYNAMIC(DlgHostDel, CDialogEx)

DlgHostDel::DlgHostDel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hostdel_dlg, pParent)
{

}

DlgHostDel::~DlgHostDel()
{
}

void DlgHostDel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHostDel, CDialogEx)
END_MESSAGE_MAP()


// DlgHostDel メッセージ ハンドラー
