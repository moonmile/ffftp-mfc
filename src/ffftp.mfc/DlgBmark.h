#pragma once


// DlgBmark ダイアログ

class DlgBmark : public CDialogEx
{
	DECLARE_DYNAMIC(DlgBmark)

public:
	DlgBmark(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgBmark();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = bmark_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
