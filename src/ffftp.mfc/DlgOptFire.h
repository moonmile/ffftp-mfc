#pragma once


// DlgOptFire ダイアログ

class DlgOptFire : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptFire)

public:
	DlgOptFire(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptFire();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_fire_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
