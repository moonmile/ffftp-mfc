// DlgHsetAdv.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHsetAdv.h"
#include "afxdialogex.h"


// DlgHsetAdv ダイアログ

IMPLEMENT_DYNAMIC(DlgHsetAdv, CDialogEx)

DlgHsetAdv::DlgHsetAdv(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hset_adv_dlg, pParent)
{

}

DlgHsetAdv::~DlgHsetAdv()
{
}

void DlgHsetAdv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgHsetAdv, CDialogEx)
END_MESSAGE_MAP()


// DlgHsetAdv メッセージ ハンドラー
