#pragma once


// DlgGroupDel ダイアログ

class DlgGroupDel : public CDialogEx
{
	DECLARE_DYNAMIC(DlgGroupDel)

public:
	DlgGroupDel(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgGroupDel();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = groupdel_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
