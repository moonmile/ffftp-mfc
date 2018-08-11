#pragma once


// DlgFileSizeNotify ダイアログ

class DlgFileSizeNotify : public CDialogEx
{
	DECLARE_DYNAMIC(DlgFileSizeNotify)

public:
	DlgFileSizeNotify(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgFileSizeNotify();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = filesize_notify_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
