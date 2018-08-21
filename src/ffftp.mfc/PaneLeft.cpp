#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "ffftp.mfc.h"
#endif
#include "PaneLeft.h"
#include "MainFrm.h"
#include "../ffftp.core/FFFtpCore.h"
extern FFFtp::FFFtpCore ffftpcore;

IMPLEMENT_DYNCREATE(PaneLeft, CFormView)

BEGIN_MESSAGE_MAP(PaneLeft, CFormView)
	// 標準印刷コマンド
	ON_BN_CLICKED(IDC_BUTTON1, &PaneLeft::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON2, &PaneLeft::OnBnClickedButtonOpen)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMainFrame::OnLvnItemchangedListLeft)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMainFrame::OnDblclkListLeft)
END_MESSAGE_MAP()


PaneLeft::PaneLeft() : CFormView(IDD_FFFTPMFC_PANE_LEFT)
{
}

bool m_init = false;
void PaneLeft::OnInitialUpdate()
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
	m_lv.SetExtendedStyle(m_lv.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	// m_lv.SetView(1);
	m_image.Create(MAKEINTRESOURCE(dirattr_bmp), 16, 9, RGB(255, 0, 0));
	m_lv.SetImageList(&m_image, 5);
	// static auto ListImg = ImageList_LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(dirattr_bmp), 16, 9, RGB(255, 0, 0));
	// SendMessage(LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)ListImg);

	LVCOLUMN lvcol;
	memset(&lvcol, 0, sizeof(lvcol));
	m_lv.InsertColumn(0, "名前", 0, 150, 0);
	m_lv.InsertColumn(1, "日付", 0, 120, 1);
	m_lv.InsertColumn(2, "サイズ", LVCFMT_RIGHT, 60, 2);
	m_lv.InsertColumn(3, "種類", 0, 37, 3);


	// SetLocalHwnd(m_lv.m_hWnd);
	// ローカル(PaneLeft)を設定
	// ChangeDir(0, "C:\\");	// WIN_LOCAL
	ffftpcore.Local.SetHwnd(m_lv.m_hWnd, &m_cbFolder);
	ffftpcore.Local.Chdir("c:\\");
	m_init = true;
}

void PaneLeft::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnUp);
	DDX_Control(pDX, IDC_BUTTON2, m_btnOpen);
	DDX_Control(pDX, IDC_COMBO1, m_cbFolder);
	DDX_Control(pDX, IDC_LIST1, m_lv);
}

void PaneLeft::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CFormView::OnClose();
}


void PaneLeft::OnBnClickedButtonUp()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ffftpcore.Local.Chdir("..");
}

void PaneLeft::OnBnClickedButtonOpen()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString text;
	m_cbFolder.GetWindowText(text);
	ffftpcore.Local.Chdir(const_cast<char*>((LPCSTR)text));
}

void PaneLeft::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_init == false) return;
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	TRACE("in OnSize %d %d", cx, cy);
	if (this->m_hWnd != nullptr)
	{
		CMainFrame *main = (CMainFrame*)AfxGetMainWnd();
		if (main != nullptr) {
			CSplitterWnd *splitter = main->GetSplitter();
			if (splitter != nullptr && splitter->m_hWnd != nullptr)
			{
				int scx = 0;
				int scxmin = 0;
				splitter->GetColumnInfo(0, scx, scxmin);
				if (scx != 0) {
					MoveWindow(0, 0, scx, cy);
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
