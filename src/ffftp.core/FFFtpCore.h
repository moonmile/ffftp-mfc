#pragma once
/// export 用クラス
#include <windows.h>
#include <functional>

extern "C" {
	void SetFtpInst(HINSTANCE instance);
	void SetMainHwnd(HWND hwnd);
	void SetLocalHwnd(HWND);
	void SetRemoteHwnd(HWND);

	void SaveRegistry(void);
	int LoadRegistry(void);


	void ChangeDir(int Win, char *Path);

	void SetFilter(int *CancelCheckWork);
	void SetOption(int);
	void ConnectProc(int, int);
	void QuickConnectProc();

	void SaveBookMark();
	void SaveCurrentSetToHost();
	void DisconnectProc();
	void SetCurrentDirAsDirHist();
	void DownloadProc(int, int, int);
	void DoubleClickProc(int Win, int Mode, int App);

	// 元ffftpから呼び出し
	extern void(*FFFtpOutput_SetMessage)(char*);
	extern void(*FFFtpOutput_DebugMessage)(char*);
	extern void(*FFFtpLocalHost_AddHistory)(char*);
	extern void(*FFFtpRemoteHost_AddHistory)(char*);

}



namespace FFFtp {
	class FFFtpLocalHost
	{
		static CComboBox *m_cb;
	public:
		void SetHwnd(HWND hwnd, CComboBox *cb) { SetLocalHwnd(hwnd); m_cb = cb; }
		void Chdir(char *path) { ::ChangeDir(0, path); }
		void DoubleClick() { ::DoubleClickProc(0, 0, -1);}
		static void AddHistory(char *msg) {
			if (m_cb != nullptr && m_cb->SelectString(0, msg) < 0) {
				m_cb->AddString(msg);
				m_cb->SelectString(0, msg);
			}
		}

	};
	class FFFtpRemoteHost
	{
		static CComboBox *m_cb;
	public:
		void SetHwnd(HWND hwnd, CComboBox *cb) { SetRemoteHwnd(hwnd); m_cb = cb; }
		void Chdir(char *path) { ::ChangeDir(1, path); }
		void DoubleClick() { ::DoubleClickProc(1, 0, -1); }
		static void AddHistory(char *msg) {
			if (m_cb != nullptr && m_cb->SelectString(0, msg) < 0) {
				m_cb->AddString(msg);
				m_cb->SelectString(0, msg);
			}
		}
	};

	class FFFtpMenu
	{
	public:
		void SetOption() { ::SetOption(0); }
		void SetFilter() { int cancel = 0;  ::SetFilter(&cancel); }
		void Connect() { ::ConnectProc(0, -1); }
		void QuickConnect() { ::QuickConnectProc(); }
		void Disconnect() {
			SaveBookMark();
			SaveCurrentSetToHost();
			DisconnectProc();
		}
		void SetConnect() { ::ConnectProc(1, -1); }
		void Download() {
			SetCurrentDirAsDirHist();
			DownloadProc(0, 0, 0);
		}
	};
	class FFFtpRegistry {
	public:
		void Save(void) { ::SaveRegistry(); }
		int Load(void) { return ::LoadRegistry(); }
	};
	class  FFFtpOutput
	{
		static CListBox *m_lst, *m_dbg;
	public:
		void SetHwnd(CListBox *lst, CListBox *dbg = NULL) { m_lst = lst; m_dbg = dbg; }
		static void SetMessage(char *msg) {
			if (m_lst != nullptr) {
				m_lst->AddString(msg);
			}
		}
		static void DebugMessage(char *msg) {
			if (m_dbg != nullptr) {
				m_dbg->AddString(msg);
			}
		}
	};
	class FFFtpCore {
	public:
		FFFtpCore() {
		}
		void Initialize(HINSTANCE inst, HWND hwnd) {
			SetFtpInst(inst);
			SetMainHwnd(hwnd);
			FFFtpOutput_SetMessage = &(FFFtpOutput::SetMessage);
			FFFtpOutput_DebugMessage = &(FFFtpOutput::DebugMessage);
			FFFtpLocalHost_AddHistory = &(FFFtpLocalHost::AddHistory);
		}
	public:
		FFFtpLocalHost Local;
		FFFtpRemoteHost Remote;
		FFFtpMenu Menu;
		FFFtpRegistry Registry;
		FFFtpOutput Output;
	};
}
