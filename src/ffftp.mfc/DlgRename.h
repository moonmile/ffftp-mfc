#pragma once


// DlgRename ダイアログ

class DlgRename : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRename)

public:
	DlgRename(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgRename();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = rename_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
