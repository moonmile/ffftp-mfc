#pragma once


// DlgMailAdrs ダイアログ

class DlgMailAdrs : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMailAdrs)

public:
	DlgMailAdrs(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgMailAdrs();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = mailadrs_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
