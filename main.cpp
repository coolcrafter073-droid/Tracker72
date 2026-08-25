#include <windows.h>
#include <vector>
#include <string>
const int BUTTON_ID = 1;
HWND txt;
HWND edt;
struct Entry{
    std::string title;
    std::string description;
};
std::vector<Entry> V = {};

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED){
                if(LOWORD(wParam) == BUTTON_ID){
                    Entry entry = {"Entry", std::to_string(V.size())};
                    V.push_back(entry);
                    TCHAR buff[1024] = {0};
                    GetWindowTextA(edt , buff ,1024);
                    V[V.size() - 1].title = buff;
                    SetWindowTextA(txt,std::to_string(V.size()).c_str());
                    MessageBoxA(hwnd, std::to_string(V.size()).c_str(), "U got msg", MB_OK);
                    
                    
                }
            }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int nCmdShow
) {
    const char CLASS_NAME[] = "MyTrackerWindow";

    WNDCLASSA wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "I am still alive",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    HWND button = CreateWindowExA(
        0,
        "BUTTON",
        "GG",
        WS_CHILD | WS_VISIBLE,
        10,
        10,
        100,
        100,
        hwnd,
        (HMENU)BUTTON_ID,
        hInstance,
        nullptr  
    );
    txt = CreateWindowExA(
        0,
        "STATIC",
        std::to_string(V.size()).c_str(),
        WS_CHILD | WS_VISIBLE,  
        90,
        180,
        100,
        100,
        hwnd,
        nullptr,
        hInstance,
        nullptr    
    );
    edt = CreateWindowExA(
        0,
        "EDIT",
        "",
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
        300,
        300,
        100,
        200,
        hwnd,
        nullptr,
        hInstance,
        nullptr 
    );

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg{};

    while (GetMessageA(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}