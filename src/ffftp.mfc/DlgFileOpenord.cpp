// DlgFileOpenord.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgFileOpenord.h"
#include "afxdialogex.h"


// DlgFileOpenord ダイアログ

IMPLEMENT_DYNAMIC(DlgFileOpenord, CDialogEx)

DlgFileOpenord::DlgFileOpenord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(FILEOPENORD_1, pParent)
{

}

DlgFileOpenord::~DlgFileOpenord()
{
}

void DlgFileOpenord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgFileOpenord, CDialogEx)
END_MESSAGE_MAP()


// DlgFileOpenord メッセージ ハンドラー
