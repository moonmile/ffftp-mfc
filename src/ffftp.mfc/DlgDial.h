#pragma once


// DlgDial ダイアログ

class DlgDial : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDial)

public:
	DlgDial(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgDial();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = dial_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
