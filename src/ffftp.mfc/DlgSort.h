#pragma once


// DlgSort ダイアログ

class DlgSort : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSort)

public:
	DlgSort(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgSort();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = sort_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
