#pragma once


// DlgOptMirror ダイアログ

class DlgOptMirror : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptMirror)

public:
	DlgOptMirror(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptMirror();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_mirror_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
