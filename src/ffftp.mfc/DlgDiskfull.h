#pragma once


// DlgDiskfull ダイアログ

class DlgDiskfull : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDiskfull)

public:
	DlgDiskfull(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgDiskfull();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = diskfull_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
