#pragma once


// DlgDelete ダイアログ

class DlgDelete : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDelete)

public:
	DlgDelete(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgDelete();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = delete_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
