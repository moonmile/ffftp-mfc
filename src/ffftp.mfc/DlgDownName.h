#pragma once


// DlgDownName ダイアログ

class DlgDownName : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDownName)

public:
	DlgDownName(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgDownName();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = downname_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
