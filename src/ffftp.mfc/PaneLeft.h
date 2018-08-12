#pragma once
class PaneLeft : public CFormView {
protected:
	PaneLeft();
	DECLARE_DYNCREATE(PaneLeft)
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFFTPMFC_PANE_LEFT };
#endif
	void OnInitialUpdate();
};
