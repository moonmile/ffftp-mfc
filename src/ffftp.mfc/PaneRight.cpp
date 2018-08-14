#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "ffftp.mfc.h"
#endif
#include "PaneRight.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(PaneRight, CFormView)

BEGIN_MESSAGE_MAP(PaneRight, CFormView)
	// �W������R�}���h
	ON_BN_CLICKED(IDC_BUTTON1, &PaneRight::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON2, &PaneRight::OnBnClickedButtonOpen)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


PaneRight::PaneRight() : CFormView(IDD_FFFTPMFC_PANE_RIGHT)
{
}
void PaneRight::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// ���X�g�R���g���[���̏�����
	int i = 0;
	HDITEM lvc;

	auto hc = m_lv.GetHeaderCtrl();

	lvc.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
	lvc.fmt = HDF_STRING | HDF_CENTER;
	lvc.pszText = L"���O";
	lvc.cxy = 100;
	hc->InsertItem(i++, &lvc);
	lvc.pszText = L"���t";
	lvc.cxy = 50;
	hc->InsertItem(i++, &lvc);
	lvc.pszText = L"�T�C�Y";
	lvc.cxy = 50;
	hc->InsertItem(i++, &lvc);
	lvc.pszText = L"���";
	lvc.cxy = 50;
	hc->InsertItem(i++, &lvc);
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
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	CFormView::OnClose();
}


void PaneRight::OnBnClickedButtonUp()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	AfxMessageBox(L"��փ{�^����������");
}

void PaneRight::OnBnClickedButtonOpen()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	AfxMessageBox(L"�J���{�^����������");
}

void PaneRight::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
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
						// �R���{�{�b�N�X�ƃ��X�g�\�������T�C�Y��ς���
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
