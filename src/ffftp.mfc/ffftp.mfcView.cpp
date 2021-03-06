
// ffftp.mfcView.cpp : CffftpmfcView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "ffftp.mfc.h"
#endif

#include "ffftp.mfcDoc.h"
#include "ffftp.mfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CffftpmfcView

IMPLEMENT_DYNCREATE(CffftpmfcView, CFormView)

BEGIN_MESSAGE_MAP(CffftpmfcView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CffftpmfcView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CffftpmfcView コンストラクション/デストラクション

CffftpmfcView::CffftpmfcView()
	: CFormView(IDD_FFFTPMFC_FORM)
{
	// TODO: 構築コードをここに追加します。

}

CffftpmfcView::~CffftpmfcView()
{
}

void CffftpmfcView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CffftpmfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CFormView::PreCreateWindow(cs);
}

void CffftpmfcView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CffftpmfcView の印刷


void CffftpmfcView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CffftpmfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CffftpmfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CffftpmfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CffftpmfcView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷用のコードをここに追加してください。
}

void CffftpmfcView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CffftpmfcView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CffftpmfcView の診断

#ifdef _DEBUG
void CffftpmfcView::AssertValid() const
{
	CFormView::AssertValid();
}

void CffftpmfcView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CffftpmfcDoc* CffftpmfcView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CffftpmfcDoc)));
	return (CffftpmfcDoc*)m_pDocument;
}
#endif //_DEBUG


// CffftpmfcView メッセージ ハンドラー
