#pragma once


// DlgOptUser ダイアログ

class DlgOptUser : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptUser)

public:
	DlgOptUser(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptUser();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_user_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
