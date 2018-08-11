#pragma once


// DlgAccount ダイアログ

class DlgAccount : public CDialogEx
{
	DECLARE_DYNAMIC(DlgAccount)

public:
	DlgAccount(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgAccount();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = account_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
