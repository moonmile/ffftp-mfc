#pragma once


// DlgDownExist ダイアログ

class DlgDownExist : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDownExist)

public:
	DlgDownExist(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgDownExist();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = down_exist_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
