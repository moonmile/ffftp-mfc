#pragma once


// DlgChdirBr ダイアログ

class DlgChdirBr : public CDialogEx
{
	DECLARE_DYNAMIC(DlgChdirBr)

public:
	DlgChdirBr(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgChdirBr();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = chdir_br_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
