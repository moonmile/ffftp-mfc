#pragma once


// DlgChdir ダイアログ

class DlgChdir : public CDialogEx
{
	DECLARE_DYNAMIC(DlgChdir)

public:
	DlgChdir(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgChdir();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = chdir_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
