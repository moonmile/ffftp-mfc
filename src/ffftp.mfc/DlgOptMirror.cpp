// DlgOptMirror.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptMirror.h"
#include "afxdialogex.h"


// DlgOptMirror ダイアログ

IMPLEMENT_DYNAMIC(DlgOptMirror, CDialogEx)

DlgOptMirror::DlgOptMirror(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_mirror_dlg, pParent)
{

}

DlgOptMirror::~DlgOptMirror()
{
}

void DlgOptMirror::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptMirror, CDialogEx)
END_MESSAGE_MAP()


// DlgOptMirror メッセージ ハンドラー
