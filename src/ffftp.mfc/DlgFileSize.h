#pragma once


// DlgFileSize ダイアログ

class DlgFileSize : public CDialogEx
{
	DECLARE_DYNAMIC(DlgFileSize)

public:
	DlgFileSize(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgFileSize();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = filesize_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
