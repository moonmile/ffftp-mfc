#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "ffftp.mfc.h"
#endif
#include "PaneRight.h"

IMPLEMENT_DYNCREATE(PaneRight, CFormView)

PaneRight::PaneRight() : CFormView(IDD_FFFTPMFC_PANE_RIGHT)
{
}
void PaneRight::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}
