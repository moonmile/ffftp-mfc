#pragma once


// DlgSslConfirm ダイアログ

class DlgSslConfirm : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSslConfirm)

public:
	DlgSslConfirm(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgSslConfirm();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = ssl_confirm_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
