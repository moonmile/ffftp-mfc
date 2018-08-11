#pragma once


// DlgTransfer ダイアログ

class DlgTransfer : public CDialogEx
{
	DECLARE_DYNAMIC(DlgTransfer)

public:
	DlgTransfer(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgTransfer();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = transfer_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
