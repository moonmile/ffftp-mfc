#pragma once


// DlgBmarkEdit ダイアログ

class DlgBmarkEdit : public CDialogEx
{
	DECLARE_DYNAMIC(DlgBmarkEdit)

public:
	DlgBmarkEdit(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgBmarkEdit();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = bmark_edit_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
