#pragma once


// DlgSavePass ダイアログ

class DlgSavePass : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSavePass)

public:
	DlgSavePass(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgSavePass();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = savepass_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
