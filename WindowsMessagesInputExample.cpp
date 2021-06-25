// WindowsMessagesInputExample.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsMessagesInputExample.h"

#include <cstdio>

#include <Windowsx.h>

#include <Xinput.h>

#include <hidusage.h>

#include <hidsdi.h>
#include <hidpi.h>

#include <vector>
#include <memory>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSMESSAGESINPUTEXAMPLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSMESSAGESINPUTEXAMPLE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSMESSAGESINPUTEXAMPLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSMESSAGESINPUTEXAMPLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   RAWINPUTDEVICE rid[] =
   {
       {
           HID_USAGE_PAGE_GENERIC,
           0,
           RIDEV_DEVNOTIFY | RIDEV_INPUTSINK | RIDEV_PAGEONLY,
           hWnd
       }
   };

   RegisterRawInputDevices(rid, 1, sizeof(RAWINPUTDEVICE));

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char buffer[256] = { 0 };

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    /*case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_CHAR:
    case WM_DEADCHAR:
    {
        // https://docs.microsoft.com/windows/win32/inputdev/about-keyboard-input

        BOOL scanCodeE0 = (HIWORD(lParam) & KF_EXTENDED) == KF_EXTENDED; // 0xE0 scancode prefix
        BYTE scanCode = LOBYTE(HIWORD(lParam));

        BOOL repeatFlag = (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT; // 1 on autorepeat
        BOOL upFlag = (HIWORD(lParam) & KF_UP) == KF_UP; // 0 on keydown

        BOOL altDownFlag = (HIWORD(lParam) & KF_ALTDOWN) == KF_ALTDOWN; // ALT was pressed
        WORD repeatCount = LOWORD(lParam); // several keydown was combined into one message

        BOOL dlgModeFlag = (HIWORD(lParam) & KF_DLGMODE) == KF_DLGMODE;
        BOOL menuModeFlag = (HIWORD(lParam) & KF_MENUMODE) == KF_MENUMODE;

        const char* messageName = nullptr;
        switch (message)
        {
        case WM_KEYDOWN:
            messageName = "WM_KEYDOWN";
            break;
        case WM_KEYUP:
            messageName = "WM_KEYUP";
            break;
        case WM_SYSKEYDOWN:
            messageName = "WM_SYSKEYDOWN";
            break;
        case WM_SYSKEYUP:
            messageName = "WM_SYSKEYUP";
            break;
        case WM_CHAR:
            messageName = "WM_CHAR";
            break;
        case WM_DEADCHAR:
            messageName = "WM_DEADCHAR";
            break;
        }

        char buffer[256] = { 0 };
        if (message == WM_CHAR || message == WM_DEADCHAR)
        {
            WCHAR charCode = wParam;
            snprintf(buffer, sizeof(buffer), "%s: Char='%c', SC=0x%02x, fUp=%d, fRepeat=%d, RepeatCount=%d, fExtended=%d, fDlgMode=%d, fMenuMode=%d, fAltDown=%d\n",
                messageName,
                charCode,
                scanCode,
                upFlag,
                repeatFlag,
                repeatCount,
                extendedFlag,
                dlgModeFlag,
                menuModeFlag,
                altDownFlag);
        }
        else
        {
            WORD vkCode = LOWORD(wParam);
            snprintf(buffer, sizeof(buffer), "%s: VK_%02X, SC=0x%02x, fUp=%d, fRepeat=%d, RepeatCount=%d, fExtended=%d, fDlgMode=%d, fMenuMode=%d, fAltDown=%d\n",
                messageName,
                vkCode,
                scanCode,
                upFlag,
                repeatFlag,
                repeatCount,
                extendedFlag,
                dlgModeFlag,
                menuModeFlag,
                altDownFlag);
        }
        OutputDebugStringA(buffer);
    }
    break;

    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    {
        INT xPos = GET_X_LPARAM(lParam);
        INT yPos = GET_Y_LPARAM(lParam);

        BOOL lmbDown = (GET_KEYSTATE_WPARAM(wParam) & MK_LBUTTON) == MK_LBUTTON;
        BOOL rmbDown = (GET_KEYSTATE_WPARAM(wParam) & MK_RBUTTON) == MK_RBUTTON;
        BOOL mmbDown = (GET_KEYSTATE_WPARAM(wParam) & MK_MBUTTON) == MK_MBUTTON;
        BOOL xmb1Down = (GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1;
        BOOL xmb2Down = (GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2;

        BOOL shiftKeyDown = (GET_KEYSTATE_WPARAM(wParam) & MK_SHIFT) == MK_SHIFT;
        BOOL controlKeyDown = (GET_KEYSTATE_WPARAM(wParam) & MK_CONTROL) == MK_CONTROL;

        char buffer1[256] = { 0 };
        if (message == WM_MOUSEWHEEL)
        {
            SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            snprintf(buffer1, sizeof(buffer1), ", zDelta=%d", zDelta);
        }
        else if (message == WM_XBUTTONDOWN || message == WM_XBUTTONUP)
        {
            BOOL xmb1Down = (HIWORD(wParam) & XBUTTON1) == XBUTTON1;
            BOOL xmb2Down = (HIWORD(wParam) & XBUTTON2) == XBUTTON2;
            snprintf(buffer1, sizeof(buffer1), ", xb1Down=%d, xb1Down=%d", xmb1Down, xmb2Down);
        }

        const char* messageName = nullptr;
        switch (message)
        {
        case WM_MOUSEMOVE:
            messageName = "WM_MOUSEMOVE";
            break;
        case WM_MOUSEWHEEL:
            messageName = "WM_MOUSEWHEEL";
            break;
        case WM_LBUTTONDOWN:
            messageName = "WM_LBUTTONDOWN";
            break;
        case WM_LBUTTONUP:
            messageName = "WM_LBUTTONUP";
            break;
        case WM_RBUTTONDOWN:
            messageName = "WM_RBUTTONDOWN";
            break;
        case WM_RBUTTONUP:
            messageName = "WM_RBUTTONUP";
            break;
        case WM_MBUTTONDOWN:
            messageName = "WM_MBUTTONDOWN";
            break;
        case WM_MBUTTONUP:
            messageName = "WM_MBUTTONUP";
            break;
        case WM_XBUTTONDOWN:
            messageName = "WM_XBUTTONDOWN";
            break;
        case WM_XBUTTONUP:
            messageName = "WM_XBUTTONUP";
            break;
        }

        char buffer[256] = { 0 };
        snprintf(buffer, sizeof(buffer), "%s: x=%d, y=%d, lmbDown=%d, rmbDown=%d, mmbDown=%d",
            messageName,
            xPos,
            yPos,
            lmbDown,
            rmbDown,
            mmbDown);
        OutputDebugStringA(buffer);

        if (buffer1)
            OutputDebugStringA(buffer1);
        OutputDebugStringA("\n");
    }
    break;*/

    case WM_INPUT:
    {
        UINT size = 0;

        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
        std::vector<uint8_t> inputBuffer(size, 0);
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, inputBuffer.data(), &size, sizeof(RAWINPUTHEADER));
        PRAWINPUT raw = (PRAWINPUT)inputBuffer.data();

        if (raw->header.dwType == RIM_TYPEKEYBOARD)
        {
            RAWKEYBOARD& keyboard = raw->data.keyboard;

            BOOL keyReleased = (keyboard.Flags & RI_KEY_BREAK) == RI_KEY_BREAK;

            WORD scanCode = keyboard.MakeCode;
            BOOL scanCodeE0 = (keyboard.Flags & RI_KEY_E0) == RI_KEY_E0;
            BOOL scanCodeE1 = (keyboard.Flags & RI_KEY_E1) == RI_KEY_E1;

            WORD vkCode = keyboard.VKey;

            // ...

            snprintf(buffer, sizeof(buffer),
                " Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n",
                keyboard.MakeCode,
                keyboard.Flags,
                keyboard.Reserved,
                keyboard.ExtraInformation,
                keyboard.Message,
                keyboard.VKey);
            OutputDebugStringA(buffer);
        }
        else if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            RAWMOUSE& mouse = raw->data.mouse;

            bool btn1Down = (mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN) == RI_MOUSE_BUTTON_1_DOWN;
            bool btn1Up = (mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP) == RI_MOUSE_BUTTON_1_UP;
            bool btn2Down = (mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN) == RI_MOUSE_BUTTON_2_DOWN;
            bool btn2Up = (mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP) == RI_MOUSE_BUTTON_2_UP;
            bool btn3Down = (mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN) == RI_MOUSE_BUTTON_3_DOWN;
            bool btn3Up = (mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP) == RI_MOUSE_BUTTON_3_UP;
            bool btn4Down = (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) == RI_MOUSE_BUTTON_4_DOWN;
            bool btn4Up = (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) == RI_MOUSE_BUTTON_4_UP;
            bool btn5Down = (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) == RI_MOUSE_BUTTON_5_DOWN;
            bool btn5Up = (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) == RI_MOUSE_BUTTON_5_UP;

            if ((mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE) // True under RDP
            {
                bool isVirtualDesktop = (mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;

                int width = GetSystemMetrics(isVirtualDesktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
                int height = GetSystemMetrics(isVirtualDesktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);

                int absoluteX = (int)(((float)mouse.lLastX / USHRT_MAX) * width);
                int absoluteY = (int)(((float)mouse.lLastY / USHRT_MAX) * height);
            }
            else if (mouse.lLastX || mouse.lLastY)
            {
                int relativeX = mouse.lLastX;
                int relativeY = mouse.lLastY;
            }

            bool vecticalScroll = (mouse.usButtonFlags & RI_MOUSE_WHEEL) == RI_MOUSE_WHEEL;
            bool horizontalScroll = (mouse.usButtonFlags & RI_MOUSE_HWHEEL) == RI_MOUSE_HWHEEL;
            if (vecticalScroll || horizontalScroll)
            {
                float wheelDelta = (float)(short)mouse.usButtonData;
            }

            // ...

            snprintf(buffer, sizeof(buffer),
                "Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\n",
                mouse.usFlags,
                mouse.ulButtons,
                mouse.usButtonFlags,
                mouse.usButtonData,
                mouse.ulRawButtons,
                mouse.lLastX,
                mouse.lLastY,
                mouse.ulExtraInformation);
            OutputDebugStringA(buffer);
        }
        else if (raw->header.dwType == RIM_TYPEHID)
        {
            RAWHID& hid = raw->data.hid;

            GetRawInputDeviceInfoW(raw->header.hDevice, RIDI_PREPARSEDDATA, 0, &size);
            std::vector<uint8_t> ppdBuffer(size, 0);
            GetRawInputDeviceInfoW(raw->header.hDevice, RIDI_PREPARSEDDATA, ppdBuffer.data(), &size);
            PHIDP_PREPARSED_DATA ppd = (PHIDP_PREPARSED_DATA)ppdBuffer.data();

            // Request additional device info
            GetRawInputDeviceInfoW(raw->header.hDevice, RIDI_DEVICENAME, nullptr, &size);
            std::unique_ptr<wchar_t[]> deviceName(new wchar_t[size]);
            GetRawInputDeviceInfoW(raw->header.hDevice, RIDI_DEVICENAME, deviceName.get(), &size);

            // Device handle for RAW IO
            HANDLE hidDeviceHandle = CreateFile(deviceName.get(), GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);

            // Get Optional Device Strings
            std::unique_ptr<wchar_t[]> deviceStringBuffer(new wchar_t[128]);

            HidD_GetManufacturerString(hidDeviceHandle, deviceStringBuffer.get(), 128);
            HidD_GetProductString(hidDeviceHandle, deviceStringBuffer.get(), 128);
            HidD_GetSerialNumberString(hidDeviceHandle, deviceStringBuffer.get(), 128);

            HIDP_CAPS caps;
            HidP_GetCaps(ppd, &caps);

            std::vector<HIDP_VALUE_CAPS> valueCaps(caps.NumberInputValueCaps, HIDP_VALUE_CAPS{});
            HidP_GetValueCaps(HidP_Input, valueCaps.data(), &caps.NumberInputValueCaps, ppd);

            std::vector<HIDP_BUTTON_CAPS> buttonCaps(caps.NumberInputButtonCaps, HIDP_BUTTON_CAPS{});
            HidP_GetButtonCaps(HidP_Input, buttonCaps.data(), &caps.NumberInputButtonCaps, ppd);

            for (DWORD i = 0; i < hid.dwCount; ++i)
            {
                DWORD reportSize = hid.dwSizeHid;
                PCHAR report = (PCHAR)(hid.bRawData + (i * reportSize));

                OutputDebugStringA("Buttons: ");
                for (USHORT i = 0; i < caps.NumberInputButtonCaps; ++i)
                {
                    ULONG usageCount = buttonCaps[i].Range.UsageMax - buttonCaps[i].Range.UsageMin + 1;
                    std::vector<USAGE> usages(usageCount, USAGE{});
                    HidP_GetUsages(HidP_Input, buttonCaps[i].UsagePage, 0, usages.data(), &usageCount, ppd, report, reportSize);
                    for (ULONG usageIndex = 0; usageIndex < usageCount; ++usageIndex)
                    {
                        snprintf(buffer, sizeof(buffer), "%d ", usages[usageIndex]);
                        OutputDebugStringA(buffer);
                    }
                }

                OutputDebugStringA("Values: ");
                for (USHORT i = 0; i < caps.NumberInputValueCaps; ++i)
                {
                    ULONG value;
                    HidP_GetUsageValue(HidP_Input, valueCaps[i].UsagePage, 0, valueCaps[i].Range.UsageMin, &value, ppd, report, reportSize);
                    snprintf(buffer, sizeof(buffer), "%d:%5d ", i, value);
                    OutputDebugStringA(buffer);
                }
            }

            // Sending output report example
            {
                std::vector<uint8_t> outputReport(caps.OutputReportByteLength, 0);
                outputReport[0] = 0x05; //report ID in first byte

                WriteFile(hidDeviceHandle, outputReport.data(), outputReport.size(), nullptr, nullptr);
            }
        }

        return 0;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/*
void XInput()
{
    for (DWORD playerIndex = 0; playerIndex < XUSER_MAX_COUNT; ++playerIndex)
    {
        XINPUT_STATE state;
        DWORD result = XInputGetState(playerIndex, &state);
        if (result == ERROR_SUCCESS && state.dwPacketNumber)
        {
            printf("Player %d ", playerIndex);
            printf("LX:%6d ", state.Gamepad.sThumbLX);
            printf("LY:%6d ", state.Gamepad.sThumbLY);
            printf("RX:%6d ", state.Gamepad.sThumbRX);
            printf("RY:%6d ", state.Gamepad.sThumbRY);
            printf("LT:%3u ", state.Gamepad.bLeftTrigger);
            printf("RT:%3u ", state.Gamepad.bRightTrigger);
            printf("Buttons: ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)        printf("up ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)      printf("down ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)      printf("left ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)     printf("right ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)          printf("start ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)           printf("back ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)     printf("LS ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)    printf("RS ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  printf("LB ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) printf("RB ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)              printf("A ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)              printf("B ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)              printf("X ");
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)              printf("Y ");
            printf("\n");

            XINPUT_VIBRATION vibration;
            vibration.wLeftMotorSpeed = state.Gamepad.bLeftTrigger * 0xFFFF / 0xFF;
            vibration.wRightMotorSpeed = state.Gamepad.bRightTrigger * 0xFFFF / 0xFF;
            XInputSetState(0, &vibration);
        }
    }
}*/
