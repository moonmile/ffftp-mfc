// DlgFilter.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgFilter.h"
#include "afxdialogex.h"


// DlgFilter ダイアログ

IMPLEMENT_DYNAMIC(DlgFilter, CDialogEx)

DlgFilter::DlgFilter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(filter_dlg, pParent)
{

}

DlgFilter::~DlgFilter()
{
}

void DlgFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgFilter, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgFilter::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgFilter メッセージ ハンドラー


void DlgFilter::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}
