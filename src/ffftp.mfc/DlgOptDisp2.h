#pragma once


// DlgOptDisp2 ダイアログ

class DlgOptDisp2 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptDisp2)

public:
	DlgOptDisp2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptDisp2();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_disp2_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
