#pragma once


// DlgHsetAdv2 ダイアログ

class DlgHsetAdv2 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHsetAdv2)

public:
	DlgHsetAdv2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHsetAdv2();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hset_adv2_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
