#pragma once


// DlgHsetCode ダイアログ

class DlgHsetCode : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHsetCode)

public:
	DlgHsetCode(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHsetCode();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hset_code_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
