#pragma once


// DlgOtpNotiry ダイアログ

class DlgOtpNotiry : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOtpNotiry)

public:
	DlgOtpNotiry(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOtpNotiry();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = otp_notify_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
