#pragma once


// DlgOptTrmode3 ダイアログ

class DlgOptTrmode3 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptTrmode3)

public:
	DlgOptTrmode3(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptTrmode3();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_trmode3_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
