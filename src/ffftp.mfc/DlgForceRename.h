#pragma once


// DlgForceRename ダイアログ

class DlgForceRename : public CDialogEx
{
	DECLARE_DYNAMIC(DlgForceRename)

public:
	DlgForceRename(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgForceRename();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = forcerename_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
