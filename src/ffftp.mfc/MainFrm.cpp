
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "ffftp.mfc.h"

#include "MainFrm.h"
/// FFFtpCore の呼び出し
#include "../ffftp.core/FFFtpCore.h"
FFFtp::FFFtpCore ffftpcore;

#include "DlgAbout.h"
#include "DlgHostName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)

	// ツールバー
	ON_COMMAND(ID_FTP_CONNECT, &CMainFrame::OnFtpConnect)
	ON_UPDATE_COMMAND_UI(ID_FTP_CONNECT, &CMainFrame::OnUpdateFtpConnect)
	ON_COMMAND(ID_FTP_DISCONNECT, &CMainFrame::OnFtpDisconnect)
	ON_UPDATE_COMMAND_UI(ID_FTP_DISCONNECT, &CMainFrame::OnUpdateFtpDisconnect)
	ON_COMMAND(ID_FTP_DELETE, &CMainFrame::OnFtpDelete)
	ON_UPDATE_COMMAND_UI(ID_FTP_DELETE, &CMainFrame::OnUpdateFtpDelete)
	ON_COMMAND(ID_FTP_DOWNLOAD, &CMainFrame::OnFtpDownload)
	ON_UPDATE_COMMAND_UI(ID_FTP_DOWNLOAD, &CMainFrame::OnUpdateFtpDownload)
	ON_COMMAND(ID_FTP_CODE_HOST_EUC, &CMainFrame::OnFtpCodeHostEuc)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_HOST_EUC, &CMainFrame::OnUpdateFtpCodeHostEuc)
	ON_COMMAND(ID_FTP_CODE_HOST_JIS, &CMainFrame::OnFtpCodeHostJis)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_HOST_JIS, &CMainFrame::OnUpdateFtpCodeHostJis)
	ON_COMMAND(ID_FTP_CODE_HOST_SJIS, &CMainFrame::OnFtpCodeHostSjis)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_HOST_SJIS, &CMainFrame::OnUpdateFtpCodeHostSjis)
	ON_COMMAND(ID_FTP_CODE_HOST_UTF8, &CMainFrame::OnFtpCodeHostUtf8)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_HOST_UTF8, &CMainFrame::OnUpdateFtpCodeHostUtf8)
	ON_COMMAND(ID_FTP_CODE_HOST_UTF8BOM, &CMainFrame::OnFtpCodeHostUtf8bom)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_HOST_UTF8BOM, &CMainFrame::OnUpdateFtpCodeHostUtf8bom)
	ON_COMMAND(ID_FTP_CODE_KANA, &CMainFrame::OnFtpCodeKana)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_KANA, &CMainFrame::OnUpdateFtpCodeKana)
	ON_COMMAND(ID_FTP_CODE_LOCAL_EUC, &CMainFrame::OnFtpCodeLocalEuc)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_LOCAL_EUC, &CMainFrame::OnUpdateFtpCodeLocalEuc)
	ON_COMMAND(ID_FTP_CODE_LOCAL_JIS, &CMainFrame::OnFtpCodeLocalJis)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_LOCAL_JIS, &CMainFrame::OnUpdateFtpCodeLocalJis)
	ON_COMMAND(ID_FTP_CODE_LOCAL_SJIS, &CMainFrame::OnFtpCodeLocalSjis)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_LOCAL_SJIS, &CMainFrame::OnUpdateFtpCodeLocalSjis)
	ON_COMMAND(ID_FTP_CODE_LOCAL_UTF8, &CMainFrame::OnFtpCodeLocalUtf8)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_LOCAL_UTF8, &CMainFrame::OnUpdateFtpCodeLocalUtf8)
	ON_COMMAND(ID_FTP_CODE_LOCAL_UTF8BOM, &CMainFrame::OnFtpCodeLocalUtf8bom)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_LOCAL_UTF8BOM, &CMainFrame::OnUpdateFtpCodeLocalUtf8bom)
	ON_COMMAND(ID_FTP_CODE_NONE, &CMainFrame::OnFtpCodeNone)
	ON_UPDATE_COMMAND_UI(ID_FTP_CODE_NONE, &CMainFrame::OnUpdateFtpCodeNone)
	ON_COMMAND(ID_FTP_LV_LIST, &CMainFrame::OnFtpLvList)
	ON_UPDATE_COMMAND_UI(ID_FTP_LV_LIST, &CMainFrame::OnUpdateFtpLvList)
	ON_COMMAND(ID_FTP_LV_REPORT, &CMainFrame::OnFtpLvReport)
	ON_UPDATE_COMMAND_UI(ID_FTP_LV_REPORT, &CMainFrame::OnUpdateFtpLvReport)
	ON_COMMAND(ID_FTP_MIRROR, &CMainFrame::OnFtpMirror)
	ON_UPDATE_COMMAND_UI(ID_FTP_MIRROR, &CMainFrame::OnUpdateFtpMirror)
	ON_COMMAND(ID_FTP_MKDIR, &CMainFrame::OnFtpMkdir)
	ON_UPDATE_COMMAND_UI(ID_FTP_MKDIR, &CMainFrame::OnUpdateFtpMkdir)
	ON_COMMAND(ID_FTP_QUICK_CONNECT, &CMainFrame::OnFtpQuickConnect)
	ON_UPDATE_COMMAND_UI(ID_FTP_QUICK_CONNECT, &CMainFrame::OnUpdateFtpQuickConnect)
	ON_COMMAND(ID_FTP_REFRESH, &CMainFrame::OnFtpRefresh)
	ON_UPDATE_COMMAND_UI(ID_FTP_REFRESH, &CMainFrame::OnUpdateFtpRefresh)
	ON_COMMAND(ID_FTP_RENAME, &CMainFrame::OnFtpRename)
	ON_UPDATE_COMMAND_UI(ID_FTP_RENAME, &CMainFrame::OnUpdateFtpRename)
	ON_COMMAND(ID_FTP_SAMEMOVE, &CMainFrame::OnFtpSamemove)
	ON_UPDATE_COMMAND_UI(ID_FTP_SAMEMOVE, &CMainFrame::OnUpdateFtpSamemove)
	ON_COMMAND(ID_FTP_SORT_ASC, &CMainFrame::OnFtpSortAsc)
	ON_UPDATE_COMMAND_UI(ID_FTP_SORT_ASC, &CMainFrame::OnUpdateFtpSortAsc)
	ON_COMMAND(ID_FTP_SORT_DESC, &CMainFrame::OnFtpSortDesc)
	ON_UPDATE_COMMAND_UI(ID_FTP_SORT_DESC, &CMainFrame::OnUpdateFtpSortDesc)
	ON_COMMAND(ID_FTP_SORT_EXT, &CMainFrame::OnFtpSortExt)
	ON_UPDATE_COMMAND_UI(ID_FTP_SORT_EXT, &CMainFrame::OnUpdateFtpSortExt)
	ON_COMMAND(ID_FTP_SORT_NAME, &CMainFrame::OnFtpSortName)
	ON_UPDATE_COMMAND_UI(ID_FTP_SORT_NAME, &CMainFrame::OnUpdateFtpSortName)
	ON_COMMAND(ID_FTP_SORT_SIZE, &CMainFrame::OnFtpSortSize)
	ON_UPDATE_COMMAND_UI(ID_FTP_SORT_SIZE, &CMainFrame::OnUpdateFtpSortSize)
	ON_COMMAND(ID_FTP_SORT_TIME, &CMainFrame::OnFtpSortTime)
	ON_UPDATE_COMMAND_UI(ID_FTP_SORT_TIME, &CMainFrame::OnUpdateFtpSortTime)
	ON_COMMAND(ID_FTP_STOP, &CMainFrame::OnFtpStop)
	ON_UPDATE_COMMAND_UI(ID_FTP_STOP, &CMainFrame::OnUpdateFtpStop)
	ON_COMMAND(ID_FTP_TR_ASCII, &CMainFrame::OnFtpTrAscii)
	ON_UPDATE_COMMAND_UI(ID_FTP_TR_ASCII, &CMainFrame::OnUpdateFtpTrAscii)
	ON_COMMAND(ID_FTP_TR_AUTO, &CMainFrame::OnFtpTrAuto)
	ON_UPDATE_COMMAND_UI(ID_FTP_TR_AUTO, &CMainFrame::OnUpdateFtpTrAuto)
	ON_COMMAND(ID_FTP_TR_BINARY, &CMainFrame::OnFtpTrBinary)
	ON_UPDATE_COMMAND_UI(ID_FTP_TR_BINARY, &CMainFrame::OnUpdateFtpTrBinary)
	ON_COMMAND(ID_FTP_UPLOAD, &CMainFrame::OnFtpUpload)
	ON_UPDATE_COMMAND_UI(ID_FTP_UPLOAD, &CMainFrame::OnUpdateFtpUpload)

	// メニュー
	ON_COMMAND(MENU_CONNECT, &CMainFrame::OnFtpConnect)
	ON_UPDATE_COMMAND_UI(MENU_CONNECT, &CMainFrame::OnUpdateFtpConnect)
	ON_COMMAND(MENU_QUICK, &CMainFrame::OnFtpQuickConnect)
	ON_UPDATE_COMMAND_UI(MENU_QUICK, &CMainFrame::OnUpdateFtpQuickConnect)
	ON_COMMAND(MENU_DISCONNECT, &CMainFrame::OnFtpDisconnect)
	ON_UPDATE_COMMAND_UI(MENU_DISCONNECT, &CMainFrame::OnUpdateFtpDisconnect)
	ON_COMMAND(MENU_DOWNLOAD, &CMainFrame::OnFtpDownload)
	ON_UPDATE_COMMAND_UI(MENU_DOWNLOAD, &CMainFrame::OnUpdateFtpDownload)
	ON_COMMAND(MENU_UPLOAD, &CMainFrame::OnFtpUpload)
	ON_UPDATE_COMMAND_UI(MENU_UPLOAD, &CMainFrame::OnUpdateFtpUpload)
	ON_COMMAND(MENU_MIRROR_UPLOAD, &CMainFrame::OnFtpMirror)
	ON_UPDATE_COMMAND_UI(MENU_MIRROR_UPLOAD, &CMainFrame::OnUpdateFtpMirror)
	ON_COMMAND(MENU_DELETE, &CMainFrame::OnFtpDelete)
	ON_UPDATE_COMMAND_UI(MENU_DELETE, &CMainFrame::OnUpdateFtpDelete)
	ON_COMMAND(MENU_RENAME, &CMainFrame::OnFtpRename)
	ON_UPDATE_COMMAND_UI(MENU_RENAME, &CMainFrame::OnUpdateFtpRename)
	ON_COMMAND(MENU_MKDIR, &CMainFrame::OnFtpMkdir)
	ON_UPDATE_COMMAND_UI(MENU_MKDIR, &CMainFrame::OnUpdateFtpMkdir)
	ON_COMMAND(MENU_SYNC, &CMainFrame::OnFtpSamemove)
	ON_UPDATE_COMMAND_UI(MENU_SYNC, &CMainFrame::OnUpdateFtpSamemove)
	ON_COMMAND(MENU_LIST, &CMainFrame::OnFtpLvList)
	ON_UPDATE_COMMAND_UI(MENU_LIST, &CMainFrame::OnUpdateFtpLvList)
	ON_COMMAND(MENU_REPORT, &CMainFrame::OnFtpLvReport)
	ON_UPDATE_COMMAND_UI(MENU_REPORT, &CMainFrame::OnUpdateFtpLvReport)
	ON_COMMAND(MENU_REFRESH, &CMainFrame::OnFtpRefresh)
	ON_UPDATE_COMMAND_UI(MENU_REFRESH, &CMainFrame::OnUpdateFtpRefresh)


	ON_COMMAND(MENU_SET_CONNECT, &CMainFrame::OnSetConnect)
	ON_COMMAND(MENU_EXIT, &CMainFrame::OnExit)
	ON_COMMAND(MENU_DOWNLOAD_AS, &CMainFrame::OnDownloadAs)
	ON_UPDATE_COMMAND_UI(MENU_DOWNLOAD_AS, &CMainFrame::OnUpdateDownloadAs)
	ON_COMMAND(MENU_UPLOAD_AS, &CMainFrame::OnUploadAs)
	ON_UPDATE_COMMAND_UI(MENU_UPLOAD_AS, &CMainFrame::OnUpdateUploadAs)
	ON_COMMAND(MENU_DOWNLOAD_NAME, &CMainFrame::OnDownloadName)
	ON_UPDATE_COMMAND_UI(MENU_DOWNLOAD_NAME, &CMainFrame::OnUpdateDownloadName)
	ON_COMMAND(MENU_DOWNLOAD_AS_FILE, &CMainFrame::OnDownloadAsFile)
	ON_UPDATE_COMMAND_UI(MENU_DOWNLOAD_AS_FILE, &CMainFrame::OnUpdateDownloadAsFile)
	ON_COMMAND(MENU_FILESIZE, &CMainFrame::OnFilesize)
	ON_COMMAND(MENU_MIRROR_DOWNLOAD, &CMainFrame::OnMirrorDownload)
	ON_UPDATE_COMMAND_UI(MENU_MIRROR_DOWNLOAD, &CMainFrame::OnUpdateMirrorDownload)
	ON_COMMAND(MENU_CHMOD, &CMainFrame::OnChmod)
	ON_UPDATE_COMMAND_UI(MENU_CHMOD, &CMainFrame::OnUpdateChmod)
	ON_COMMAND(MENU_SOMECMD, &CMainFrame::OnSomecmd)
	ON_UPDATE_COMMAND_UI(MENU_SOMECMD, &CMainFrame::OnUpdateSomecmd)
	ON_COMMAND(MENU_BMARK_ADD, &CMainFrame::OnBmarkAdd)
	ON_UPDATE_COMMAND_UI(MENU_BMARK_ADD, &CMainFrame::OnUpdateBmarkAdd)
	ON_COMMAND(MENU_BMARK_ADD_LOCAL, &CMainFrame::OnBmarkAddLocal)
	ON_UPDATE_COMMAND_UI(MENU_BMARK_ADD_LOCAL, &CMainFrame::OnUpdateBmarkAddLocal)
	ON_COMMAND(MENU_BMARK_ADD_BOTH, &CMainFrame::OnBmarkAddBoth)
	ON_UPDATE_COMMAND_UI(MENU_BMARK_ADD_BOTH, &CMainFrame::OnUpdateBmarkAddBoth)
	ON_COMMAND(MENU_BMARK_EDIT, &CMainFrame::OnBmarkEdit)
	ON_UPDATE_COMMAND_UI(MENU_BMARK_EDIT, &CMainFrame::OnUpdateBmarkEdit)
	ON_COMMAND(MENU_FILTER, &CMainFrame::OnFilter)
	ON_COMMAND(MENU_FIND, &CMainFrame::OnFind)
	ON_COMMAND(MENU_FINDNEXT, &CMainFrame::OnFindnext)
	ON_COMMAND(MENU_SELECT, &CMainFrame::OnSelect)
	ON_COMMAND(MENU_SELECT_ALL, &CMainFrame::OnSelectAll)
	ON_COMMAND(MENU_SORT, &CMainFrame::OnSort)
	ON_COMMAND(MENU_DIRINFO, &CMainFrame::OnDirinfo)
	ON_UPDATE_COMMAND_UI(MENU_DIRINFO, &CMainFrame::OnUpdateDirinfo)
	ON_COMMAND(MENU_TASKINFO, &CMainFrame::OnTaskinfo)
	ON_UPDATE_COMMAND_UI(MENU_TASKINFO, &CMainFrame::OnUpdateTaskinfo)
	ON_COMMAND(MENU_OTPCALC, &CMainFrame::OnOtpcalc)
	ON_COMMAND(MENU_FW_FTP_FILTER, &CMainFrame::OnFwFtpFilter)
	ON_COMMAND(MENU_OPTION, &CMainFrame::OnOption)
	ON_COMMAND(MENU_HELP_TROUBLE, &CMainFrame::OnHelpTrouble)
	ON_COMMAND(MENU_HELP, &CMainFrame::OnHelp)
	ON_COMMAND(MENU_UPDATES_CHECK, &CMainFrame::OnUpdatesCheck)
	ON_COMMAND(MENU_ABOUT, &CMainFrame::OnAbout)

	// コンテキストメニュー	
	ON_COMMAND(MENU_OPEN, &CMainFrame::OnOpen)
	ON_UPDATE_COMMAND_UI(MENU_OPEN, &CMainFrame::OnUpdateOpen)
	ON_COMMAND(MENU_OPEN_NOTEPAD, &CMainFrame::OnOpenNotepad)
	ON_UPDATE_COMMAND_UI(MENU_OPEN_NOTEPAD, &CMainFrame::OnUpdateOpenNotepad)
		ON_WM_CLOSE()
		END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケーター
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	// TODO: メンバー初期化コードをここに追加してください。
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("メニュー バーを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// アクティブになったときメニュー バーにフォーカスを移動しない
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できない場合
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// ユーザー定義のツール バーの操作を許可します:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できない場合
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ツール バーおよびメニュー バーをドッキング可能にしない場合は、この 5 つの行を削除します
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// Visual Studio 2005 スタイルのドッキング ウィンドウ動作を有効にします
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 スタイルのドッキング ウィンドウの自動非表示動作を有効にします
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ドッキング ウィンドウを作成します
	if (!CreateDockingWindows())
	{
		TRACE0("ドッキング ウィンドウを作成できませんでした\n");
		return -1;
	}
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);


	// ツール バーとドッキング ウィンドウ メニューの配置変更を有効にします
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ツール バーのクイック (Alt キーを押しながらドラッグ) カスタマイズを有効にします
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// ユーザー定義のツール バー イメージを読み込みます
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}
#if 0
	// メニューのパーソナル化 (最近使用されたコマンド) を有効にします
	// TODO: ユーザー固有の基本コマンドを定義し、各メニューをクリックしたときに基本コマンドが 1 つ以上表示されるようにします。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);
#endif

	/// ffftp.coreの初期設定
	ffftpcore.Initialize(AfxGetInstanceHandle(), AfxGetMainWnd()->m_hWnd);
	ffftpcore.Registry.Load();

	return 0;
}

void CMainFrame::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	try {
		ffftpcore.Registry.Save();
	} catch ( ... ) {}
	CFrameWndEx::OnClose();
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this, 1, 2 );
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(PaneLeft), CSize(300, 0), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(PaneRight), CSize(0, 0), pContext);
	return TRUE;
}


BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 出力ウィンドウを作成します
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("出力ウィンドウを作成できませんでした\n");
		return FALSE; // 作成できませんでした
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_SIZEBOX
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// CMainFrame の診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame メッセージ ハンドラー

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* メニューをスキャンします*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// 基底クラスが実際の動作を行います

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// すべてのユーザー定義ツール バーのボタンのカスタマイズを有効にします
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////
/// ツールバーの制御
////////////////////////////////////////////////////////////////

void CMainFrame::OnFtpConnect()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	ffftpcore.Menu.Connect();
}


void CMainFrame::OnUpdateFtpConnect(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpConnect);
}


void CMainFrame::OnFtpDisconnect()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	ffftpcore.Menu.Disconnect();
}


void CMainFrame::OnUpdateFtpDisconnect(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpDisconnect);
}


void CMainFrame::OnFtpDelete()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpDelete(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpDelete);
}


void CMainFrame::OnFtpDownload()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	ffftpcore.Menu.Download();
}


void CMainFrame::OnUpdateFtpDownload(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpDownload);
}

void CMainFrame::OnFtpCodeHostEuc()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeHostEuc(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeHostEuc);
}


void CMainFrame::OnFtpCodeHostJis()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeHostJis(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeHostJis);
}


void CMainFrame::OnFtpCodeHostSjis()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeHostSjis(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeHostSjis);
}


void CMainFrame::OnFtpCodeHostUtf8()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeHostUtf8(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeHostUtf8);
}


void CMainFrame::OnFtpCodeHostUtf8bom()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeHostUtf8bom(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeHostUtf8bom);
}


void CMainFrame::OnFtpCodeKana()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeKana(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeKana);
}


void CMainFrame::OnFtpCodeLocalEuc()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeLocalEuc(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeLocalEuc);
}


void CMainFrame::OnFtpCodeLocalJis()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeLocalJis(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeLocalJis);
}


void CMainFrame::OnFtpCodeLocalSjis()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeLocalSjis(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeLocalSjis);
}


void CMainFrame::OnFtpCodeLocalUtf8()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeLocalUtf8(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeLocalUtf8);
}


void CMainFrame::OnFtpCodeLocalUtf8bom()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeLocalUtf8bom(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeLocalUtf8bom);
}


void CMainFrame::OnFtpCodeNone()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpCodeNone(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpCodeNone);
}


void CMainFrame::OnFtpLvList()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpLvList(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpLvList);
}


void CMainFrame::OnFtpLvReport()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpLvReport(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpLvReport);
}


void CMainFrame::OnFtpMirror()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpMirror(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpMirror);
}


void CMainFrame::OnFtpMkdir()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpMkdir(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpMkdir);
}


void CMainFrame::OnFtpQuickConnect()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	// DlgHostName dlg;
	// dlg.DoModal();
	ffftpcore.Menu.QuickConnect();
}


void CMainFrame::OnUpdateFtpQuickConnect(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpQuickConnect);
}


void CMainFrame::OnFtpRefresh()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpRefresh(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpRefresh);
}


void CMainFrame::OnFtpRename()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpRename(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpRename);
}


void CMainFrame::OnFtpSamemove()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpSamemove(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpSamemove);
}


void CMainFrame::OnFtpSortAsc()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpSortAsc(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpSortAsc);
}


void CMainFrame::OnFtpSortDesc()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpSortDesc(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpSortDesc);
}


void CMainFrame::OnFtpSortExt()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpSortExt(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpSortExt);
}


void CMainFrame::OnFtpSortName()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpSortName(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpSortName);
}


void CMainFrame::OnFtpSortSize()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpSortSize(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpSortSize);
}


void CMainFrame::OnFtpSortTime()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpSortTime(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpSortTime);
}


void CMainFrame::OnFtpStop()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpStop(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpStop);
}


void CMainFrame::OnFtpTrAscii()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpTrAscii(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpTrAscii);
}


void CMainFrame::OnFtpTrAuto()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpTrAuto(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpTrAuto);
}


void CMainFrame::OnFtpTrBinary()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpTrBinary(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpTrBinary);
}


void CMainFrame::OnFtpUpload()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateFtpUpload(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	pCmdUI->Enable(m_FtpUpload);
}


// ホストの設定
void CMainFrame::OnSetConnect()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}

// 終了
void CMainFrame::OnExit()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnDownloadAs()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateDownloadAs(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnUploadAs()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateUploadAs(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnDownloadName()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateDownloadName(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnDownloadAsFile()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateDownloadAsFile(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnFilesize()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnMirrorDownload()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateMirrorDownload(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnChmod()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateChmod(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnSomecmd()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateSomecmd(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnBmarkAdd()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateBmarkAdd(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnBmarkAddLocal()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateBmarkAddLocal(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnBmarkAddBoth()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateBmarkAddBoth(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnBmarkEdit()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateBmarkEdit(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnFilter()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	ffftpcore.Menu.SetFilter();

}


void CMainFrame::OnFind()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnFindnext()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnSelect()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnSelectAll()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnSort()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnDirinfo()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateDirinfo(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnTaskinfo()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateTaskinfo(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnOtpcalc()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnFwFtpFilter()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}

void CMainFrame::OnOption()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	ffftpcore.Menu.SetOption();
}


void CMainFrame::OnHelpTrouble()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnHelp()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdatesCheck()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}

void CMainFrame::OnAbout()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	DlgAbout dlg;
	dlg.DoModal();
}


/// ローカルリストの右クリック
void CMainFrame::OnLvnItemchangedListLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	// アイテム上以外での右クリック
	if (pNMLV->iItem == -1) {
		*pResult = 0;
		return;
	}
	// コンテキストメニューを表示する
	CMenu menu, *popup;
	menu.LoadMenu(IDR_POPUP_LOCAL);
	popup = menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	popup->TrackPopupMenu(TPM_LEFTBUTTON, pos.x, pos.y, this);
	menu.DestroyMenu();

	*pResult = 0;
}

/// リモートリストの右クリック
void CMainFrame::OnLvnItemchangedListRight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	// アイテム上以外での右クリック
	if (pNMLV->iItem == -1) {
		*pResult = 0;
		return;
	}
	// コンテキストメニューを表示する
	CMenu menu, *popup;
	menu.LoadMenu(IDR_POPUP_REMOTE);
	popup = menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	popup->TrackPopupMenu(TPM_LEFTBUTTON, pos.x, pos.y, this);
	menu.DestroyMenu();

	*pResult = 0;
}

/// ダブルクリックでフォルダ移動
/// ダブルクリックでアップロード
void CMainFrame::OnDblclkListLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (pNMItemActivate->iItem == -1) {
		*pResult = 0;
		return;
	}
	ffftpcore.Local.DoubleClick();

	*pResult = 0;
}

void CMainFrame::OnDblclkListRight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (pNMItemActivate->iItem == -1) {
		*pResult = 0;
		return;
	}
	ffftpcore.Remote.DoubleClick();
	*pResult = 0;
}

void CMainFrame::OnOpen()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateOpen(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CMainFrame::OnOpenNotepad()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}


void CMainFrame::OnUpdateOpenNotepad(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


