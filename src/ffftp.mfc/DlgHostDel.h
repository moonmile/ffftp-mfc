#pragma once


// DlgHostDel ダイアログ

class DlgHostDel : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHostDel)

public:
	DlgHostDel(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHostDel();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hostdel_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
