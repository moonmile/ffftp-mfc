#pragma once


// DlgMasterPasswd ダイアログ

class DlgMasterPasswd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMasterPasswd)

public:
	DlgMasterPasswd(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgMasterPasswd();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = masterpasswd_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
