#pragma once


// DlgFind ダイアログ

class DlgFind : public CDialogEx
{
	DECLARE_DYNAMIC(DlgFind)

public:
	DlgFind(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgFind();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = find_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
