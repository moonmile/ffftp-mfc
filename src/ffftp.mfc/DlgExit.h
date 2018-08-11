#pragma once


// DlgExit ダイアログ

class DlgExit : public CDialogEx
{
	DECLARE_DYNAMIC(DlgExit)

public:
	DlgExit(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgExit();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = exit_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
