// DlgMirrorDownNotify.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgMirrorDownNotify.h"
#include "afxdialogex.h"


// DlgMirrorDownNotify ダイアログ

IMPLEMENT_DYNAMIC(DlgMirrorDownNotify, CDialogEx)

DlgMirrorDownNotify::DlgMirrorDownNotify(CWnd* pParent /*=nullptr*/)
	: CDialogEx(mirrordown_notify_dlg, pParent)
{

}

DlgMirrorDownNotify::~DlgMirrorDownNotify()
{
}

void DlgMirrorDownNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMirrorDownNotify, CDialogEx)
END_MESSAGE_MAP()


// DlgMirrorDownNotify メッセージ ハンドラー
