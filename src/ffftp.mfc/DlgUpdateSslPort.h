#pragma once


// DlgUpdateSslPort ダイアログ

class DlgUpdateSslPort : public CDialogEx
{
	DECLARE_DYNAMIC(DlgUpdateSslPort)

public:
	DlgUpdateSslPort(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgUpdateSslPort();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = updatesslroot_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
