#pragma once


// DlgSaveCrypt ダイアログ

class DlgSaveCrypt : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSaveCrypt)

public:
	DlgSaveCrypt(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgSaveCrypt();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = savecrypt_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
