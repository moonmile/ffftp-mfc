// DlgHsetCode.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHsetCode.h"
#include "afxdialogex.h"


// DlgHsetCode ダイアログ

IMPLEMENT_DYNAMIC(DlgHsetCode, CDialogEx)

DlgHsetCode::DlgHsetCode(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hset_code_dlg, pParent)
{

}

DlgHsetCode::~DlgHsetCode()
{
}

void DlgHsetCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHsetCode, CDialogEx)
END_MESSAGE_MAP()


// DlgHsetCode メッセージ ハンドラー
