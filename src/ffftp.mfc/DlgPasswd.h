#pragma once


// DlgPasswd ダイアログ

class DlgPasswd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPasswd)

public:
	DlgPasswd(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgPasswd();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = passwd_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
