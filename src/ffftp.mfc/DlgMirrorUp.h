#pragma once


// DlgMirrorUp ダイアログ

class DlgMirrorUp : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMirrorUp)

public:
	DlgMirrorUp(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgMirrorUp();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = mirror_up_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
