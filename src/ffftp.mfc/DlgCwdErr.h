#pragma once


// DlgCwdErr ダイアログ

class DlgCwdErr : public CDialogEx
{
	DECLARE_DYNAMIC(DlgCwdErr)

public:
	DlgCwdErr(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgCwdErr();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = cwderr_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
