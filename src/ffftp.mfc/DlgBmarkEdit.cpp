// DlgBmarkEdit.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgBmarkEdit.h"
#include "afxdialogex.h"


// DlgBmarkEdit ダイアログ

IMPLEMENT_DYNAMIC(DlgBmarkEdit, CDialogEx)

DlgBmarkEdit::DlgBmarkEdit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(bmark_edit_dlg, pParent)
{

}

DlgBmarkEdit::~DlgBmarkEdit()
{
}

void DlgBmarkEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgBmarkEdit, CDialogEx)
END_MESSAGE_MAP()


// DlgBmarkEdit メッセージ ハンドラー
