#pragma once


// DlgHostName ダイアログ

class DlgHostName : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHostName)

public:
	DlgHostName(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHostName();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hostname_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
