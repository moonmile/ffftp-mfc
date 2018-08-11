#pragma once


// DlgGroup ダイアログ

class DlgGroup : public CDialogEx
{
	DECLARE_DYNAMIC(DlgGroup)

public:
	DlgGroup(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgGroup();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = group_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
