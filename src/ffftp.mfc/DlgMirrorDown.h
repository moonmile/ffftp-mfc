#pragma once


// DlgMirrorDown ダイアログ

class DlgMirrorDown : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMirrorDown)

public:
	DlgMirrorDown(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgMirrorDown();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = mirror_down_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
