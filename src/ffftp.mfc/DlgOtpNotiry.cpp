// DlgOtpNotiry.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOtpNotiry.h"
#include "afxdialogex.h"


// DlgOtpNotiry ダイアログ

IMPLEMENT_DYNAMIC(DlgOtpNotiry, CDialogEx)

DlgOtpNotiry::DlgOtpNotiry(CWnd* pParent /*=nullptr*/)
	: CDialogEx(otp_notify_dlg, pParent)
{

}

DlgOtpNotiry::~DlgOtpNotiry()
{
}

void DlgOtpNotiry::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOtpNotiry, CDialogEx)
END_MESSAGE_MAP()


// DlgOtpNotiry メッセージ ハンドラー
