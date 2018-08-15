#pragma once


// DlgFilter ダイアログ

class DlgFilter : public CDialogEx
{
	DECLARE_DYNAMIC(DlgFilter)

public:
	DlgFilter(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgFilter();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = filter_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
