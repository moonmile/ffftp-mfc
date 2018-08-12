#pragma once
class PaneRight : public CFormView {
protected:
	PaneRight();
	DECLARE_DYNCREATE(PaneRight)
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFFTPMFC_PANE_RIGHT };
#endif
	void OnInitialUpdate();
};

