#pragma once


// DlgRasReconnect ダイアログ

class DlgRasReconnect : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRasReconnect)

public:
	DlgRasReconnect(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DlgRasReconnect();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = rasreconnect_dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
