#pragma once


// DlgNewMasterPasswd ダイアログ

class DlgNewMasterPasswd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgNewMasterPasswd)

public:
	DlgNewMasterPasswd(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgNewMasterPasswd();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = newmasterpasswd_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
