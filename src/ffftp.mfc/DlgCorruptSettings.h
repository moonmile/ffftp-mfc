#pragma once


// DlgCorruptSettings ダイアログ

class DlgCorruptSettings : public CDialogEx
{
	DECLARE_DYNAMIC(DlgCorruptSettings)

public:
	DlgCorruptSettings(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgCorruptSettings();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = corruptsettings_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
