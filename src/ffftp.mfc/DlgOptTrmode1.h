#pragma once


// DlgOptTrmode1 ダイアログ

class DlgOptTrmode1 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptTrmode1)

public:
	DlgOptTrmode1(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptTrmode1();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_trmode1_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
