#pragma once


// DlgSelRemote ダイアログ

class DlgSelRemote : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSelRemote)

public:
	DlgSelRemote(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgSelRemote();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = sel_remote_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
