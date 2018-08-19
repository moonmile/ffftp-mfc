
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once
#include "OutputWnd.h"
#include "PaneLeft.h"
#include "PaneRight.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // シリアル化からのみ作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

protected:  // コントロール バー用メンバー
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	COutputWnd        m_wndOutput;
	CSplitterWnd	m_wndSplitter;
public:
	CSplitterWnd * GetSplitter() { return &m_wndSplitter; }

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	// ツールバーとメニュー制御
protected:
	BOOL m_FtpConnect = TRUE;
	BOOL m_FtpDisconnect = TRUE;
	BOOL m_FtpDelete = TRUE;
	BOOL m_FtpDownload = TRUE;
	BOOL m_FtpCodeHostEuc = TRUE;
	BOOL m_FtpCodeHostJis = TRUE;
	BOOL m_FtpCodeHostSjis = TRUE;
	BOOL m_FtpCodeHostUtf8 = TRUE;
	BOOL m_FtpCodeHostUtf8bom = TRUE;
	BOOL m_FtpCodeKana = TRUE;
	BOOL m_FtpCodeLocalEuc = TRUE;
	BOOL m_FtpCodeLocalJis = TRUE;
	BOOL m_FtpCodeLocalSjis = TRUE;
	BOOL m_FtpCodeLocalUtf8 = TRUE;
	BOOL m_FtpCodeLocalUtf8bom = TRUE;
	BOOL m_FtpCodeNone = TRUE;
	BOOL m_FtpLvList = TRUE;
	BOOL m_FtpLvReport = TRUE;
	BOOL m_FtpMirror = TRUE;
	BOOL m_FtpMkdir = TRUE;
	BOOL m_FtpQuickConnect = TRUE;
	BOOL m_FtpRefresh = TRUE;
	BOOL m_FtpRename = TRUE;
	BOOL m_FtpSamemove = TRUE;
	BOOL m_FtpSortAsc = TRUE;
	BOOL m_FtpSortDesc = TRUE;
	BOOL m_FtpSortExt = TRUE;
	BOOL m_FtpSortName = TRUE;
	BOOL m_FtpSortSize = TRUE;
	BOOL m_FtpSortTime = TRUE;
	BOOL m_FtpStop = TRUE;
	BOOL m_FtpTrAscii = TRUE;
	BOOL m_FtpTrAuto = TRUE;
	BOOL m_FtpTrBinary = TRUE;
	BOOL m_FtpUpload = TRUE;
public:
	afx_msg void OnFtpConnect();
	afx_msg void OnUpdateFtpConnect(CCmdUI *pCmdUI);
	afx_msg void OnFtpDisconnect();
	afx_msg void OnUpdateFtpDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnFtpDelete();
	afx_msg void OnUpdateFtpDelete(CCmdUI *pCmdUI);
	afx_msg void OnFtpDownload();
	afx_msg void OnUpdateFtpDownload(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeHostEuc();
	afx_msg void OnUpdateFtpCodeHostEuc(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeHostJis();
	afx_msg void OnUpdateFtpCodeHostJis(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeHostSjis();
	afx_msg void OnUpdateFtpCodeHostSjis(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeHostUtf8();
	afx_msg void OnUpdateFtpCodeHostUtf8(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeHostUtf8bom();
	afx_msg void OnUpdateFtpCodeHostUtf8bom(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeKana();
	afx_msg void OnUpdateFtpCodeKana(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeLocalEuc();
	afx_msg void OnUpdateFtpCodeLocalEuc(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeLocalJis();
	afx_msg void OnUpdateFtpCodeLocalJis(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeLocalSjis();
	afx_msg void OnUpdateFtpCodeLocalSjis(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeLocalUtf8();
	afx_msg void OnUpdateFtpCodeLocalUtf8(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeLocalUtf8bom();
	afx_msg void OnUpdateFtpCodeLocalUtf8bom(CCmdUI *pCmdUI);
	afx_msg void OnFtpCodeNone();
	afx_msg void OnUpdateFtpCodeNone(CCmdUI *pCmdUI);
	afx_msg void OnFtpLvList();
	afx_msg void OnUpdateFtpLvList(CCmdUI *pCmdUI);
	afx_msg void OnFtpLvReport();
	afx_msg void OnUpdateFtpLvReport(CCmdUI *pCmdUI);
	afx_msg void OnFtpMirror();
	afx_msg void OnUpdateFtpMirror(CCmdUI *pCmdUI);
	afx_msg void OnFtpMkdir();
	afx_msg void OnUpdateFtpMkdir(CCmdUI *pCmdUI);
	afx_msg void OnFtpQuickConnect();
	afx_msg void OnUpdateFtpQuickConnect(CCmdUI *pCmdUI);
	afx_msg void OnFtpRefresh();
	afx_msg void OnUpdateFtpRefresh(CCmdUI *pCmdUI);
	afx_msg void OnFtpRename();
	afx_msg void OnUpdateFtpRename(CCmdUI *pCmdUI);
	afx_msg void OnFtpSamemove();
	afx_msg void OnUpdateFtpSamemove(CCmdUI *pCmdUI);
	afx_msg void OnFtpSortAsc();
	afx_msg void OnUpdateFtpSortAsc(CCmdUI *pCmdUI);
	afx_msg void OnFtpSortDesc();
	afx_msg void OnUpdateFtpSortDesc(CCmdUI *pCmdUI);
	afx_msg void OnFtpSortExt();
	afx_msg void OnUpdateFtpSortExt(CCmdUI *pCmdUI);
	afx_msg void OnFtpSortName();
	afx_msg void OnUpdateFtpSortName(CCmdUI *pCmdUI);
	afx_msg void OnFtpSortSize();
	afx_msg void OnUpdateFtpSortSize(CCmdUI *pCmdUI);
	afx_msg void OnFtpSortTime();
	afx_msg void OnUpdateFtpSortTime(CCmdUI *pCmdUI);
	afx_msg void OnFtpStop();
	afx_msg void OnUpdateFtpStop(CCmdUI *pCmdUI);
	afx_msg void OnFtpTrAscii();
	afx_msg void OnUpdateFtpTrAscii(CCmdUI *pCmdUI);
	afx_msg void OnFtpTrAuto();
	afx_msg void OnUpdateFtpTrAuto(CCmdUI *pCmdUI);
	afx_msg void OnFtpTrBinary();
	afx_msg void OnUpdateFtpTrBinary(CCmdUI *pCmdUI);
	afx_msg void OnFtpUpload();
	afx_msg void OnUpdateFtpUpload(CCmdUI *pCmdUI);
	afx_msg void OnSetConnect();
	afx_msg void OnExit();
	afx_msg void OnDownloadAs();
	afx_msg void OnUpdateDownloadAs(CCmdUI *pCmdUI);
	afx_msg void OnUploadAs();
	afx_msg void OnUpdateUploadAs(CCmdUI *pCmdUI);
	afx_msg void OnDownloadName();
	afx_msg void OnUpdateDownloadName(CCmdUI *pCmdUI);
	afx_msg void OnDownloadAsFile();
	afx_msg void OnUpdateDownloadAsFile(CCmdUI *pCmdUI);
	afx_msg void OnFilesize();
	afx_msg void OnMirrorDownload();
	afx_msg void OnUpdateMirrorDownload(CCmdUI *pCmdUI);
	afx_msg void OnChmod();
	afx_msg void OnUpdateChmod(CCmdUI *pCmdUI);
	afx_msg void OnSomecmd();
	afx_msg void OnUpdateSomecmd(CCmdUI *pCmdUI);
	afx_msg void OnBmarkAdd();
	afx_msg void OnUpdateBmarkAdd(CCmdUI *pCmdUI);
	afx_msg void OnBmarkAddLocal();
	afx_msg void OnUpdateBmarkAddLocal(CCmdUI *pCmdUI);
	afx_msg void OnBmarkAddBoth();
	afx_msg void OnUpdateBmarkAddBoth(CCmdUI *pCmdUI);
	afx_msg void OnBmarkEdit();
	afx_msg void OnUpdateBmarkEdit(CCmdUI *pCmdUI);
	afx_msg void OnFilter();
	afx_msg void OnFind();
	afx_msg void OnFindnext();
	afx_msg void OnSelect();
	afx_msg void OnSelectAll();
	afx_msg void OnSort();
	afx_msg void OnDirinfo();
	afx_msg void OnUpdateDirinfo(CCmdUI *pCmdUI);
	afx_msg void OnTaskinfo();
	afx_msg void OnUpdateTaskinfo(CCmdUI *pCmdUI);
	afx_msg void OnOtpcalc();
	afx_msg void OnFwFtpFilter();
	afx_msg void OnOption();
	afx_msg void OnHelpTrouble();
	afx_msg void OnHelp();
	afx_msg void OnUpdatesCheck();
	afx_msg void OnAbout();
	afx_msg void OnLvnItemchangedListLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListRight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListRight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOpen();
	afx_msg void OnUpdateOpen(CCmdUI *pCmdUI);
	afx_msg void OnOpenNotepad();
	afx_msg void OnUpdateOpenNotepad(CCmdUI *pCmdUI);
	afx_msg void OnClose();
};


