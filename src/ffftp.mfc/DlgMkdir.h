#pragma once


// DlgMkdir ダイアログ

class DlgMkdir : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMkdir)

public:
	DlgMkdir(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgMkdir();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = mkdir_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
