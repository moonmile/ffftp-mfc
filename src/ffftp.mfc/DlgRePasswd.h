#pragma once


// DlgRePasswd ダイアログ

class DlgRePasswd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRePasswd)

public:
	DlgRePasswd(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgRePasswd();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = re_passwd_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
