// DlgOptTool.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptTool.h"
#include "afxdialogex.h"


// DlgOptTool ダイアログ

IMPLEMENT_DYNAMIC(DlgOptTool, CDialogEx)

DlgOptTool::DlgOptTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_tool_dlg, pParent)
{

}

DlgOptTool::~DlgOptTool()
{
}

void DlgOptTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptTool, CDialogEx)
END_MESSAGE_MAP()


// DlgOptTool メッセージ ハンドラー
