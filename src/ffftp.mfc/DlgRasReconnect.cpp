// DlgRasReconnect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgRasReconnect.h"
#include "afxdialogex.h"


// DlgRasReconnect ダイアログ

IMPLEMENT_DYNAMIC(DlgRasReconnect, CDialogEx)

DlgRasReconnect::DlgRasReconnect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(rasreconnect_dlg, pParent)
{

}

DlgRasReconnect::~DlgRasReconnect()
{
}

void DlgRasReconnect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgRasReconnect, CDialogEx)
END_MESSAGE_MAP()


// DlgRasReconnect メッセージ ハンドラー
