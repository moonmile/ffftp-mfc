// DlgHSetCrypt.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHSetCrypt.h"
#include "afxdialogex.h"


// DlgHSetCrypt ダイアログ

IMPLEMENT_DYNAMIC(DlgHSetCrypt, CDialogEx)

DlgHSetCrypt::DlgHSetCrypt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hset_crypt_dlg, pParent)
{

}

DlgHSetCrypt::~DlgHSetCrypt()
{
}

void DlgHSetCrypt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHSetCrypt, CDialogEx)
END_MESSAGE_MAP()


// DlgHSetCrypt メッセージ ハンドラー
