// DelSomeCmd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DelSomeCmd.h"
#include "afxdialogex.h"


// DelSomeCmd ダイアログ

IMPLEMENT_DYNAMIC(DelSomeCmd, CDialogEx)

DelSomeCmd::DelSomeCmd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(somecmd_dlg, pParent)
{

}

DelSomeCmd::~DelSomeCmd()
{
}

void DelSomeCmd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DelSomeCmd, CDialogEx)
END_MESSAGE_MAP()


// DelSomeCmd メッセージ ハンドラー
