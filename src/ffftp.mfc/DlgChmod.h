#pragma once


// DlgChmod ダイアログ

class DlgChmod : public CDialogEx
{
	DECLARE_DYNAMIC(DlgChmod)

public:
	DlgChmod(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgChmod();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = chmod_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
