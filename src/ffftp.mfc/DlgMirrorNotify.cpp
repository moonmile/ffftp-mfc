// DlgMirrorNotify.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgMirrorNotify.h"
#include "afxdialogex.h"


// DlgMirrorNotify ダイアログ

IMPLEMENT_DYNAMIC(DlgMirrorNotify, CDialogEx)

DlgMirrorNotify::DlgMirrorNotify(CWnd* pParent /*=nullptr*/)
	: CDialogEx(mirror_notify_dlg, pParent)
{

}

DlgMirrorNotify::~DlgMirrorNotify()
{
}

void DlgMirrorNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMirrorNotify, CDialogEx)
END_MESSAGE_MAP()


// DlgMirrorNotify メッセージ ハンドラー
