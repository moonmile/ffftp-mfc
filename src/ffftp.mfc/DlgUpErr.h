#pragma once


// DlgUpErr ダイアログ

class DlgUpErr : public CDialogEx
{
	DECLARE_DYNAMIC(DlgUpErr)

public:
	DlgUpErr(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgUpErr();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = uperr_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
