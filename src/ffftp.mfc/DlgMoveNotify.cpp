// DlgMoveNotify.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgMoveNotify.h"
#include "afxdialogex.h"


// DlgMoveNotify ダイアログ

IMPLEMENT_DYNAMIC(DlgMoveNotify, CDialogEx)

DlgMoveNotify::DlgMoveNotify(CWnd* pParent /*=nullptr*/)
	: CDialogEx(move_notify_dlg, pParent)
{

}

DlgMoveNotify::~DlgMoveNotify()
{
}

void DlgMoveNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMoveNotify, CDialogEx)
END_MESSAGE_MAP()


// DlgMoveNotify メッセージ ハンドラー
