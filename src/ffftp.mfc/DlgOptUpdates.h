#pragma once


// DlgOptUpdates ダイアログ

class DlgOptUpdates : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptUpdates)

public:
	DlgOptUpdates(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptUpdates();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_updates_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
