#pragma once


// DlgOptSound ダイアログ

class DlgOptSound : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOptSound)

public:
	DlgOptSound(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgOptSound();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = opt_sound_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
