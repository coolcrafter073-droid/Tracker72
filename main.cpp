#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
const int BUTTON_ID = 1;
const int BUTTON_ID_SHOW = 2;
HWND txt;
HWND edt;
HWND txt_mass;
HWND edt_sec;
HWND lst;
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
                    char buff[1024] = {};
                    GetWindowTextA(edt , buff ,1024);
                    char buff2[1024] = {};
                    GetWindowTextA(edt_sec , buff2 ,1024);
                    bool all_space = std::all_of(buff, buff + std::strlen(buff), [](char s){
                        return s == ' ';
                    });
                    bool all_space2 = std::all_of(buff2, buff2 + std::strlen(buff2), [](char s){
                        return s == ' ';
                    });
                    if (buff2[0] == '\0' || all_space2 || buff[0] == '\0' || all_space){
                        MessageBoxA(hwnd, "Cannot create empty Entries", "U got msg", MB_OK);
                    }
                    else
                    {
                        Entry entry = {buff, buff2};
                        V.push_back(entry);
                        SendMessageA(lst, LB_ADDSTRING, 0 , (LPARAM) entry.title.c_str());
                        std::string inf_display = "Current Entries:\n";
                        for (int i = 0; i < V.size(); i++){
                            inf_display += "* " + V[i].title + "\n" + V[i].description + "\n";
                        };
                        SetWindowTextA(txt_mass, inf_display.c_str());
                        SetWindowTextA(edt, "");
                        SetWindowTextA(edt_sec, "");
                        SetWindowTextA(txt,std::to_string(V.size()).c_str());
                        MessageBoxA(hwnd, std::to_string(V.size()).c_str(), "U got msg", MB_OK);
                    };
                    

                }
                if(LOWORD(wParam) == BUTTON_ID_SHOW){
                   int index = SendMessage(lst, LB_GETCURSEL, 0 , 0);
                   SetWindowTextA(txt_mass, V[index].description.c_str());
                   
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
        400,
        100,
        100,
        hwnd,
        (HMENU)BUTTON_ID,
        hInstance,
        nullptr  
    );
    HWND show = CreateWindowExA(
        0,
        "BUTTON",
        "show",
        WS_CHILD | WS_VISIBLE,
        10,
        10,
        100,
        100,
        hwnd,
        (HMENU)BUTTON_ID_SHOW,
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
    txt_mass = CreateWindowExA(
        0,
        "STATIC",
        "Write something",
        WS_CHILD | WS_VISIBLE,  
        450,
        350,
        300,
        150,
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
        450,
        10,
        300,
        50,
        hwnd,
        nullptr,
        hInstance,
        nullptr 
    );
    edt_sec = CreateWindowExA(
        0,
        "EDIT",
        "",
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
        450,
        100,
        300,
        150,
        hwnd,
        nullptr,
        hInstance,
        nullptr 
    );
    lst = CreateWindowExA(
        0,
        "LISTBOX",
        "",
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL ,
        170,
        100,
        200,
        110,
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