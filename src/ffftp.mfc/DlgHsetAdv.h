#pragma once


// DlgHsetAdv ダイアログ

class DlgHsetAdv : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHsetAdv)

public:
	DlgHsetAdv(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHsetAdv();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hset_adv_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
