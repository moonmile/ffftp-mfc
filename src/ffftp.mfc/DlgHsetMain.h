#pragma once


// DlgHsetMain ダイアログ

class DlgHsetMain : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHsetMain)

public:
	DlgHsetMain(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHsetMain();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hset_main_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
