// DlgMirrorDown.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgMirrorDown.h"
#include "afxdialogex.h"


// DlgMirrorDown ダイアログ

IMPLEMENT_DYNAMIC(DlgMirrorDown, CDialogEx)

DlgMirrorDown::DlgMirrorDown(CWnd* pParent /*=nullptr*/)
	: CDialogEx(mirror_down_dlg, pParent)
{

}

DlgMirrorDown::~DlgMirrorDown()
{
}

void DlgMirrorDown::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMirrorDown, CDialogEx)
END_MESSAGE_MAP()


// DlgMirrorDown メッセージ ハンドラー
