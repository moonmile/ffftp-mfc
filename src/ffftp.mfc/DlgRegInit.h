#pragma once


// DlgRegInit ダイアログ

class DlgRegInit : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRegInit)

public:
	DlgRegInit(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgRegInit();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = reginit_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
