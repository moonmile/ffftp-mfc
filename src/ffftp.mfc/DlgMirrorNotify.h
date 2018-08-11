#pragma once


// DlgMirrorNotify ダイアログ

class DlgMirrorNotify : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMirrorNotify)

public:
	DlgMirrorNotify(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgMirrorNotify();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = mirror_notify_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
