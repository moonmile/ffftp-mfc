// DlgOptUser.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptUser.h"
#include "afxdialogex.h"


// DlgOptUser ダイアログ

IMPLEMENT_DYNAMIC(DlgOptUser, CDialogEx)

DlgOptUser::DlgOptUser(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_user_dlg, pParent)
{

}

DlgOptUser::~DlgOptUser()
{
}

void DlgOptUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptUser, CDialogEx)
END_MESSAGE_MAP()


// DlgOptUser メッセージ ハンドラー
