// DlgCorruptSettings.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgCorruptSettings.h"
#include "afxdialogex.h"


// DlgCorruptSettings ダイアログ

IMPLEMENT_DYNAMIC(DlgCorruptSettings, CDialogEx)

DlgCorruptSettings::DlgCorruptSettings(CWnd* pParent /*=nullptr*/)
	: CDialogEx(corruptsettings_dlg, pParent)
{

}

DlgCorruptSettings::~DlgCorruptSettings()
{
}

void DlgCorruptSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgCorruptSettings, CDialogEx)
END_MESSAGE_MAP()


// DlgCorruptSettings メッセージ ハンドラー
