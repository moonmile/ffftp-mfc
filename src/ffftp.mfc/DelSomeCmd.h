#pragma once


// DelSomeCmd ダイアログ

class DelSomeCmd : public CDialogEx
{
	DECLARE_DYNAMIC(DelSomeCmd)

public:
	DelSomeCmd(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DelSomeCmd();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = somecmd_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
