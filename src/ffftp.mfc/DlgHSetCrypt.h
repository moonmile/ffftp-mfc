#pragma once


// DlgHSetCrypt ダイアログ

class DlgHSetCrypt : public CDialogEx
{
	DECLARE_DYNAMIC(DlgHSetCrypt)

public:
	DlgHSetCrypt(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgHSetCrypt();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = hset_crypt_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
