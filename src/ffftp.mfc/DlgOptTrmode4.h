#pragma once


// DlgOptTrmode4 ダイアログ

class DlgOptTrmode4 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptTrmode4)

public:
	DlgOptTrmode4(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptTrmode4();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_trmode4_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
