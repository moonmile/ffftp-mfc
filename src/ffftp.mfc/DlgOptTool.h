#pragma once


// DlgOptTool ダイアログ

class DlgOptTool : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptTool)

public:
	DlgOptTool(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptTool();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_tool_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
