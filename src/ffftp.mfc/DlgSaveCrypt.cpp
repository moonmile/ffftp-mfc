// DlgSaveCrypt.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgSaveCrypt.h"
#include "afxdialogex.h"


// DlgSaveCrypt ダイアログ

IMPLEMENT_DYNAMIC(DlgSaveCrypt, CDialogEx)

DlgSaveCrypt::DlgSaveCrypt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(savecrypt_dlg, pParent)
{

}

DlgSaveCrypt::~DlgSaveCrypt()
{
}

void DlgSaveCrypt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSaveCrypt, CDialogEx)
END_MESSAGE_MAP()


// DlgSaveCrypt メッセージ ハンドラー
