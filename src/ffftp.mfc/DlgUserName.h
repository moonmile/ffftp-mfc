#pragma once


// DlgUserName ダイアログ

class DlgUserName : public CDialogEx
{
	DECLARE_DYNAMIC(DlgUserName)

public:
	DlgUserName(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgUserName();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = username_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
