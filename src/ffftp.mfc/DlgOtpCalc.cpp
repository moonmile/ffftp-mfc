// DlgOtpCalc.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOtpCalc.h"
#include "afxdialogex.h"


// DlgOtpCalc ダイアログ

IMPLEMENT_DYNAMIC(DlgOtpCalc, CDialogEx)

DlgOtpCalc::DlgOtpCalc(CWnd* pParent /*=nullptr*/)
	: CDialogEx(otp_calc_dlg, pParent)
{

}

DlgOtpCalc::~DlgOtpCalc()
{
}

void DlgOtpCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOtpCalc, CDialogEx)
END_MESSAGE_MAP()


// DlgOtpCalc メッセージ ハンドラー
