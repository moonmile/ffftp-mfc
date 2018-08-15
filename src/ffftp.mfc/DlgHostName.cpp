// DlgHostName.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ffftp.mfc.h"
#include "DlgHostName.h"
#include "afxdialogex.h"


// DlgHostName ダイアログ

IMPLEMENT_DYNAMIC(DlgHostName, CDialogEx)

DlgHostName::DlgHostName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(hostname_dlg, pParent)
	, m_User(_T(""))
	, m_Pass(_T(""))
	, m_isFirewall(FALSE)
	, m_isPasv(FALSE)
	, m_Hostname(_T(""))
{

}

DlgHostName::~DlgHostName()
{
}

void DlgHostName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, QHOST_HOST, m_cbHost);
	DDX_Text(pDX, QHOST_USER, m_User);
	DDX_Text(pDX, QHOST_PASS, m_Pass);
	DDX_Check(pDX, QHOST_FWALL, m_isFirewall);
	DDX_Check(pDX, QHOST_PASV, m_isPasv);
	DDX_CBString(pDX, QHOST_HOST, m_Hostname);
}


BEGIN_MESSAGE_MAP(DlgHostName, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgHostName::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DlgHostName::OnBnClickedCancel)
END_MESSAGE_MAP()


// DlgHostName メッセージ ハンドラー


void DlgHostName::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UpdateData();
#if 0
	// コールバックを登録
	ffftpCore.eventConnect = &PaneRight::OnConnect;
	// 接続メソッドを呼び出す
	ffftpCore.Connect(m_Hostname, m_User, ... );
	// 結果は PaneRight の m_lv にコールバックされる
#endif
	CDialogEx::OnOK();
}


void DlgHostName::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


/// 初期化処理
/// ffftpCore から初期値を取得して設定する
BOOL DlgHostName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	m_Hostname = L"";				// = ffftpCore.QuickConnect.GetHostname();
	m_Pass = L"mail@address.com";
	m_User = L"anonymous";
	m_isFirewall = FALSE;
	m_isPasv = TRUE;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
