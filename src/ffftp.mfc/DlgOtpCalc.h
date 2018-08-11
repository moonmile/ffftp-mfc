#pragma once


// DlgOtpCalc ダイアログ

class DlgOtpCalc : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOtpCalc)

public:
	DlgOtpCalc(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOtpCalc();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = otp_calc_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
