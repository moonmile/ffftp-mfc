#pragma once


// DlgNoRename ダイアログ

class DlgNoRename : public CDialogEx
{
	DECLARE_DYNAMIC(DlgNoRename)

public:
	DlgNoRename(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgNoRename();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = noresume_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
