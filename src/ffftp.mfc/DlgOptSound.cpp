// DlgOptSound.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgOptSound.h"
#include "afxdialogex.h"


// DlgOptSound ダイアログ

IMPLEMENT_DYNAMIC(DlgOptSound, CDialogEx)

DlgOptSound::DlgOptSound(CWnd* pParent /*=nullptr*/)
	: CDialogEx(opt_sound_dlg, pParent)
{

}

DlgOptSound::~DlgOptSound()
{
}

void DlgOptSound::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptSound, CDialogEx)
END_MESSAGE_MAP()


// DlgOptSound メッセージ ハンドラー
