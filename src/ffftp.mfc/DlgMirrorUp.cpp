// DlgMirrorUp.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgMirrorUp.h"
#include "afxdialogex.h"


// DlgMirrorUp ダイアログ

IMPLEMENT_DYNAMIC(DlgMirrorUp, CDialogEx)

DlgMirrorUp::DlgMirrorUp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(mirror_up_dlg, pParent)
{

}

DlgMirrorUp::~DlgMirrorUp()
{
}

void DlgMirrorUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMirrorUp, CDialogEx)
END_MESSAGE_MAP()


// DlgMirrorUp メッセージ ハンドラー
