#pragma once


// DlgOptMisc ダイアログ

class DlgOptMisc : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptMisc)

public:
	DlgOptMisc(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptMisc();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_misc_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
