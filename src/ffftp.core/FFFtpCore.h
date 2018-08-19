#pragma once
/// export —pƒNƒ‰ƒX
#include <windows.h>

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

	HBITMAP ResizeBitmap(HBITMAP hBitmap, int UnitSizeX, int UnitSizeY, int ScaleNumerator, int ScaleDenominator);
}



namespace FFFtp {
	class FFFtpLocalHost
	{
	public:
		void SetHwnd(HWND hwnd) { SetLocalHwnd(hwnd); }
		void Chdir(char *path) { ::ChangeDir(0, path); }
		void DoubleClick() { ::DoubleClickProc(0, 0, -1);}

	};
	class FFFtpRemoteHost
	{
	public:
		void SetHwnd(HWND hwnd) { SetRemoteHwnd(hwnd); }
		void Chdir(char *path) { ::ChangeDir(1, path); }
		void DoubleClick() { ::DoubleClickProc(1, 0, -1); }
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
		FFFtpRegistry Registry;
	};
}
