#pragma once


// DlgMirrorDownNotify ダイアログ

class DlgMirrorDownNotify : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMirrorDownNotify)

public:
	DlgMirrorDownNotify(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgMirrorDownNotify();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = mirrordown_notify_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
