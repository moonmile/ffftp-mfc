#pragma once


// DlgDownErr ダイアログ

class DlgDownErr : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDownErr)

public:
	DlgDownErr(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgDownErr();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = downerr_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
