#pragma once
/// export —pƒNƒ‰ƒX
#include <windows.h>

extern "C" {
	void SetFtpInst(HINSTANCE instance);
	void SetMainHwnd(HWND hwnd);
	void SetLocalHwnd(HWND);
	void SetRemoteHwnd(HWND);
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


}

namespace FFFtp {
	class FFFtpLocalHost
	{
	public:
		void SetHwnd(HWND hwnd) { SetLocalHwnd(hwnd); }
		void ChangeDir(char *path) { ::ChangeDir(0, path); }
	};
	class FFFtpRemoteHost
	{
	public:
		void SetHwnd(HWND hwnd) { SetRemoteHwnd(hwnd); }
		void ChangeDir(char *path) { ::ChangeDir(1, path); }
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
	class FFFtpCore {
	public:
		void Initialize(HINSTANCE inst, HWND hwnd) {
			SetFtpInst(inst);
			SetMainHwnd(hwnd);
		}
	public:
		FFFtpLocalHost Local;
		FFFtpRemoteHost Remote;
		FFFtpMenu Menu;
	};
}
