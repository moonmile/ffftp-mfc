#pragma once


// DlgOptTrmode2 ダイアログ

class DlgOptTrmode2 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptTrmode2)

public:
	DlgOptTrmode2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptTrmode2();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_trmode2_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
