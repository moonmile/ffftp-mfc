#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "ffftp.mfc.h"
#endif
#include "PaneRight.h"
#include "MainFrm.h"
#include "../ffftp.core/FFFtpCore.h"
extern FFFtp::FFFtpCore ffftpcore;

IMPLEMENT_DYNCREATE(PaneRight, CFormView)

BEGIN_MESSAGE_MAP(PaneRight, CFormView)
	// 標準印刷コマンド
	ON_BN_CLICKED(IDC_BUTTON1, &PaneRight::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON2, &PaneRight::OnBnClickedButtonOpen)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMainFrame::OnLvnItemchangedListRight)
END_MESSAGE_MAP()


PaneRight::PaneRight() : CFormView(IDD_FFFTPMFC_PANE_RIGHT)
{
}
void PaneRight::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// ボタンにアイコンを表示
	m_imageBtnUp.LoadBitmap(IDB_BTN_UP);
	m_btnUp.ModifyStyle(0, BS_BITMAP);
	m_btnUp.SetBitmap((HBITMAP)m_imageBtnUp);
	m_imageBtnOpen.LoadBitmap(IDB_BTN_OPEN);
	m_btnOpen.ModifyStyle(0, BS_BITMAP);
	m_btnOpen.SetBitmap((HBITMAP)m_imageBtnOpen);
	// リストコントロールの初期化
	// m_lv.SetView(1);
	LVCOLUMN lvcol;
	memset(&lvcol, 0, sizeof(lvcol));
	m_lv.InsertColumn(0, "名前", 0, 100, 0);
	m_lv.InsertColumn(1, "日付", 0, 50, 0);
	m_lv.InsertColumn(2, "サイズ", 0, 50, 0);
	m_lv.InsertColumn(3, "種類", 0, 50, 0);

	ffftpcore.Remote.SetHwnd(m_lv.m_hWnd);

}

void PaneRight::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnUp);
	DDX_Control(pDX, IDC_BUTTON2, m_btnOpen);
	DDX_Control(pDX, IDC_COMBO1, m_cbFolder);
	DDX_Control(pDX, IDC_LIST1, m_lv);
}

void PaneRight::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CFormView::OnClose();
}


void PaneRight::OnBnClickedButtonUp()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ffftpcore.Local.Chdir("..");
}

void PaneRight::OnBnClickedButtonOpen()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	AfxMessageBox("開くボタンを押した");
}

void PaneRight::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	TRACE("in OnSize %d %d", cx, cy);
	{
		CMainFrame *main = (CMainFrame*)AfxGetMainWnd();
		if (main != nullptr) {
			CSplitterWnd *splitter = main->GetSplitter();
			if (splitter != nullptr && splitter->m_hWnd != nullptr)
			{
				int scx = 0;
				int scx1 = 0;
				int scxmin = 0;
				splitter->GetColumnInfo(0, scx1, scxmin);
				splitter->GetColumnInfo(1, scx, scxmin);
				if (scx != 0) {
					MoveWindow(1 + scx1 + 10, 1, scx, cy);
					if (this->m_cbFolder.m_hWnd != nullptr) {
						CRect rc, rcFolder, rcLv;
						this->m_cbFolder.GetWindowRect(rcFolder);
						this->m_lv.GetWindowRect(rcLv);
						this->ScreenToClient(&rcFolder);
						this->ScreenToClient(&rcLv);
						// コンボボックスとリスト表示だけサイズを変える
						rcFolder.right = scx - 10;
						rcLv.right = scx - 10;
						rcLv.bottom = cy - 10;
						this->m_cbFolder.MoveWindow(&rcFolder, TRUE);
						this->m_lv.MoveWindow(&rcLv, TRUE);
					}
				}
			}
		}
	}
}
