#pragma once


// DlgForcePassChange ダイアログ

class DlgForcePassChange : public CDialogEx
{
	DECLARE_DYNAMIC(DlgForcePassChange)

public:
	DlgForcePassChange(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgForcePassChange();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = forcepasschange_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
