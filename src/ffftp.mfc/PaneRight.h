#pragma once
class PaneRight : public CFormView
{
protected:
	PaneRight();
	DECLARE_DYNCREATE(PaneRight)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:
	void OnInitialUpdate();

	CButton m_btnUp;
	CButton m_btnOpen;
	CComboBox m_cbFolder;
	CListCtrl m_lv;
	CBitmap m_imageBtnUp, m_imageBtnOpen;

protected:
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
