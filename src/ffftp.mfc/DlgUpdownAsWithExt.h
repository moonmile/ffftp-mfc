#pragma once


// DlgUpdownAsWithExt ダイアログ

class DlgUpdownAsWithExt : public CDialogEx
{
	DECLARE_DYNAMIC(DlgUpdownAsWithExt)

public:
	DlgUpdownAsWithExt(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgUpdownAsWithExt();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = updown_as_with_ext_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
