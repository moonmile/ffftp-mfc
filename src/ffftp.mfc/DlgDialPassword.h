#pragma once


// DlgDialPassword ダイアログ

class DlgDialPassword : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDialPassword)

public:
	DlgDialPassword(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgDialPassword();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = dial_password_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
