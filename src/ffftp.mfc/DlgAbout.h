#pragma once


// DlgAbout ダイアログ

class DlgAbout : public CDialogEx
{
	DECLARE_DYNAMIC(DlgAbout)

public:
	DlgAbout(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgAbout();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = about_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
