// DlgSelRemote.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgSelRemote.h"
#include "afxdialogex.h"


// DlgSelRemote ダイアログ

IMPLEMENT_DYNAMIC(DlgSelRemote, CDialogEx)

DlgSelRemote::DlgSelRemote(CWnd* pParent /*=nullptr*/)
	: CDialogEx(sel_remote_dlg, pParent)
{

}

DlgSelRemote::~DlgSelRemote()
{
}

void DlgSelRemote::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSelRemote, CDialogEx)
END_MESSAGE_MAP()


// DlgSelRemote メッセージ ハンドラー
