#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "ffftp.mfc.h"
#endif
#include "PaneLeft.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(PaneLeft, CFormView)

BEGIN_MESSAGE_MAP(PaneLeft, CFormView)
	// 標準印刷コマンド
	ON_BN_CLICKED(IDC_BUTTON1, &PaneLeft::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON2, &PaneLeft::OnBnClickedButtonOpen)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


PaneLeft::PaneLeft() : CFormView(IDD_FFFTPMFC_PANE_LEFT)
{
}
void PaneLeft::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// リストコントロールの初期化
	int i = 0;
	HDITEM lvc;

	auto hc = m_lv.GetHeaderCtrl();

	lvc.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
	lvc.fmt = HDF_STRING | HDF_CENTER;
	lvc.pszText = L"名前";
	lvc.cxy = 100;
	hc->InsertItem(i++, &lvc);
	lvc.pszText = L"日付";
	lvc.cxy = 50;
	hc->InsertItem(i++, &lvc);
	lvc.pszText = L"サイズ";
	lvc.cxy = 50;
	hc->InsertItem(i++, &lvc);
	lvc.pszText = L"種類";
	lvc.cxy = 50;
	hc->InsertItem(i++, &lvc);
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
	AfxMessageBox(L"上へボタンを押した");
}

void PaneLeft::OnBnClickedButtonOpen()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	AfxMessageBox(L"開くボタンを押した");
}

void PaneLeft::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

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
