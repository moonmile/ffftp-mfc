#pragma once


// DlgHSetAdv3 ダイアログ

class DlgHSetAdv3 : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHSetAdv3)

public:
	DlgHSetAdv3(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHSetAdv3();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hset_adv3_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
