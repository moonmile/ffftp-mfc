#pragma once


// DlgHostConnect ダイアログ

class DlgHostConnect : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHostConnect)

public:
	DlgHostConnect(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHostConnect();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hostconnect_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
