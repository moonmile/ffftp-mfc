#pragma once


// DlgSelLocal ダイアログ

class DlgSelLocal : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSelLocal)

public:
	DlgSelLocal(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgSelLocal();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = sel_local_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
