#pragma once


// DlgMoveNotify ダイアログ

class DlgMoveNotify : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMoveNotify)

public:
	DlgMoveNotify(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgMoveNotify();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = move_notify_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
