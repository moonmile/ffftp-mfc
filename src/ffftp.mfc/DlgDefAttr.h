#pragma once


// DlgDefAttr ダイアログ

class DlgDefAttr : public CDialogEx
{
	DECLARE_DYNAMIC(DlgDefAttr)

public:
	DlgDefAttr(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgDefAttr();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = def_attr_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
