#pragma once


// DlgOptDisp1 ダイアログ

class DlgOptDisp1 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptDisp1)

public:
	DlgOptDisp1(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptDisp1();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_disp1_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
