#pragma once


// DlgHSetDialup ダイアログ

class DlgHSetDialup : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHSetDialup)

public:
	DlgHSetDialup(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHSetDialup();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hset_dialup_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
