#pragma once


// DlgRasNotify ダイアログ

class DlgRasNotify : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRasNotify)

public:
	DlgRasNotify(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgRasNotify();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = rasnotify_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
