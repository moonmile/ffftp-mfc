#pragma once


// DlgFnameIn ダイアログ

class DlgFnameIn : public CDialogEx
{
	DECLARE_DYNAMIC(DlgFnameIn)

public:
	DlgFnameIn(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgFnameIn();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = fname_in_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
