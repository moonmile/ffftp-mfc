
// ffftp.mfcView.h : CffftpmfcView クラスのインターフェイス
//

#pragma once


class CffftpmfcView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CffftpmfcView();
	DECLARE_DYNCREATE(CffftpmfcView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_FFFTPMFC_FORM };
#endif

// 属性
public:
	CffftpmfcDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnInitialUpdate(); // 構築後に初めて呼び出されます。
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CffftpmfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ffftp.mfcView.cpp のデバッグ バージョン
inline CffftpmfcDoc* CffftpmfcView::GetDocument() const
   { return reinterpret_cast<CffftpmfcDoc*>(m_pDocument); }
#endif

