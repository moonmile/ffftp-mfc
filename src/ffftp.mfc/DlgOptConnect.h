#pragma once


// DlgOptConnect ダイアログ

class DlgOptConnect : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptConnect)

public:
	DlgOptConnect(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptConnect();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_connect_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
