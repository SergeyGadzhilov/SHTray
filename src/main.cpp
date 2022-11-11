/*
 * SHTray
 * Copyright (C) 2022  Hadzhilov Serhii
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <windows.h>
#include "SHPopupMenu.h"

#define THIS_CLASSNAME "SH Tray"
#define ID_ABOUT       0
#define ID_EXIT        1

static BOOL g_bModalState = FALSE; //Is messagebox shown

void ShowPopupMenu(HWND hWnd)
{
    if (!g_bModalState) {
        SHPopupMenu menu(hWnd);
        menu.addItem("About...");
        menu.addItem("Exit");
        menu.show();
    }
}

void RemoveTrayIcon(HWND hWnd, UINT uID)
{
    NOTIFYICONDATA  nid;
    nid.hWnd = hWnd;
    nid.uID  = uID;
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void AddTrayIcon(HWND hWnd, UINT uID, UINT uCallbackMsg)
{
    NOTIFYICONDATA nid = {0};
    nid.hWnd = hWnd;
    nid.uID  = uID;
    nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
    nid.uCallbackMessage = uCallbackMsg;

    ExtractIconEx("C:/Temp/icon.ico", 0, NULL, &(nid.hIcon), 1);
    strcpy_s(nid.szTip, sizeof(nid.szTip), "Tool Tip");
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void create(HWND hWnd)
{
    AddTrayIcon(hWnd, 1, WM_APP);
}

LRESULT close(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RemoveTrayIcon(hWnd, 1);
    PostQuitMessage(0); //Sends WM_QUIT message
    return DefWindowProc(hWnd, uMsg, wParam, lParam); //Calls WM_DESTROY
}

int command(HWND hWnd, WPARAM wParam)
{
    auto result = g_bModalState;

    if (!g_bModalState) {
        switch (LOWORD(wParam)) {
        case ID_ABOUT:
            g_bModalState = TRUE;
            MessageBox(hWnd, TEXT("Hi!"), TEXT("Title"), MB_ICONINFORMATION | MB_OK );
            g_bModalState = FALSE;
            break;
        case ID_EXIT:
            PostMessage( hWnd, WM_CLOSE, 0, 0 );
            break;
        default:
            break;
      }
    }

    return result;
}

void appMessage(HWND hWnd, LPARAM lParam)
{
    switch (lParam) {
        case WM_LBUTTONDBLCLK:
            MessageBox(hWnd, TEXT("Hi!"), TEXT("Title"), MB_ICONINFORMATION|MB_OK);
            break;
        case WM_RBUTTONUP:
            SetForegroundWindow(hWnd);
            ShowPopupMenu(hWnd);
            PostMessage(hWnd, WM_APP + 1, 0, 0);
            break;
        default:
            break;
    }
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (uMsg){
        case WM_CREATE:
            create(hWnd);
        break;
        case WM_CLOSE:
            result = close(hWnd, uMsg, wParam, lParam);
        break;
        case WM_COMMAND:
            result = command(hWnd, wParam);
          break;
        case WM_APP:
            appMessage(hWnd, lParam);
            break;
        default:
            result = DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
    }
    return result;
}

bool isAlreadyRunning()
{
    return FindWindow(THIS_CLASSNAME, TEXT("Title"));
}

void registerWindow(HINSTANCE hInst)
{
    WNDCLASSEX wclx    = {0};
    wclx.cbSize        = sizeof(wclx);
    wclx.style         = 0;
    wclx.lpfnWndProc   = &WndProc;
    wclx.cbClsExtra    = 0;
    wclx.cbWndExtra    = 0;
    wclx.hInstance     = hInst;
    wclx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wclx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wclx.lpszMenuName  = NULL;
    wclx.lpszClassName = THIS_CLASSNAME;
    RegisterClassEx(&wclx);
}

bool createWindow(HINSTANCE hInst)
{
    return CreateWindow(THIS_CLASSNAME, TEXT("Title"),
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        100, 100, 250, 150, NULL, NULL, hInst, NULL);
}

int listenMessages()
{
    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0)){
      TranslateMessage(&msg);
      DispatchMessage (&msg);
    }

    return static_cast<int>(msg.wParam);
}

void destroyWindow(HINSTANCE hInst)
{
    UnregisterClass(THIS_CLASSNAME, hInst);
}

int showWindow(HINSTANCE hInst)
{
    auto result = 0;

    registerWindow(hInst);
    if (createWindow(hInst)) {
        result = listenMessages();
        destroyWindow(hInst);
    }
    else {
        MessageBox(NULL, "Can't create window!", TEXT("Warning!"), MB_ICONERROR|MB_OK|MB_TOPMOST);
        result = 1;
    }

    return result;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    int result = 0;

    if (!isAlreadyRunning()) {
        result = showWindow(hInst);
    }
    else {
        MessageBox(NULL, TEXT("Previous instance alredy running!"), TEXT("Warning"), MB_OK);
    }

    return result;
}
