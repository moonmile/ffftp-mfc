#pragma once


// DlgFileOpenord ダイアログ

class DlgFileOpenord : public CDialogEx
{
	DECLARE_DYNAMIC(DlgFileOpenord)

public:
	DlgFileOpenord(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgFileOpenord();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = FILEOPENORD_1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
