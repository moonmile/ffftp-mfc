#pragma once


// DlgUpExist ダイアログ

class DlgUpExist : public CDialogEx
{
	DECLARE_DYNAMIC(DlgUpExist)

public:
	DlgUpExist(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgUpExist();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = up_exist_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
