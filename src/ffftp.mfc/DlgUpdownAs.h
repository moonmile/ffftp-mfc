#pragma once


// DlgUpdownAs ダイアログ

class DlgUpdownAs : public CDialogEx
{
	DECLARE_DYNAMIC(DlgUpdownAs)

public:
	DlgUpdownAs(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgUpdownAs();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = updown_as_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
