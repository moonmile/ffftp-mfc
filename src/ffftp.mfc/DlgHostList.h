#pragma once


// DlgHostList ダイアログ

class DlgHostList : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHostList)

public:
	DlgHostList(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHostList();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hostlist_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
