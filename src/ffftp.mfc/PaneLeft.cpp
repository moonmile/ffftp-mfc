#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "ffftp.mfc.h"
#endif
#include "PaneLeft.h"

IMPLEMENT_DYNCREATE(PaneLeft, CFormView)

PaneLeft::PaneLeft()  : CFormView(IDD_FFFTPMFC_PANE_LEFT)
{
}
void PaneLeft::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}
