// DlgFileSize.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgFileSize.h"
#include "afxdialogex.h"


// DlgFileSize ダイアログ

IMPLEMENT_DYNAMIC(DlgFileSize, CDialogEx)

DlgFileSize::DlgFileSize(CWnd* pParent /*=nullptr*/)
	: CDialogEx(filesize_dlg, pParent)
{

}

DlgFileSize::~DlgFileSize()
{
}

void DlgFileSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgFileSize, CDialogEx)
END_MESSAGE_MAP()


// DlgFileSize メッセージ ハンドラー
