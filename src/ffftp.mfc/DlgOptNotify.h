#pragma once


// DlgOptNotify ダイアログ

class DlgOptNotify : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptNotify)

public:
	DlgOptNotify(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptNotify();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_notify_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
