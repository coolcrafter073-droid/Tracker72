#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
const int BUTTON_ID = 1;
const int BUTTON_ID_SHOW = 2;
const int BUTTON_ID_DELETE = 3;
const int BUTTON_ID_EDIT = 4;
const int BUTTON_ID_SAVE = 5;
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
                   if (index == LB_ERR){
                    MessageBoxA(hwnd, "Maaaan , pls select an Entry_title", "U got msg", MB_OK);
                   }
                   else {
                    SetWindowTextA(txt_mass, V[index].description.c_str()); 
                   }
                   
                   
                }
                if(LOWORD(wParam) == BUTTON_ID_DELETE){
                   int index = SendMessage(lst, LB_GETCURSEL, 0 , 0);
                   if (index == LB_ERR){
                    MessageBoxA(hwnd, "Maaaan , pls select an Entry_title", "U got msg", MB_OK);
                   }
                   else {
                    SendMessage(lst, LB_DELETESTRING, index , 0);
                    V.erase(V.begin() + index);
                    SetWindowTextA(txt,std::to_string(V.size()).c_str());
                    std::string inf_display = "Current Entries:\n";
                        for (int i = 0; i < V.size(); i++){
                            inf_display += "* " + V[i].title + "\n" + V[i].description + "\n";
                        };
                        SetWindowTextA(txt_mass, inf_display.c_str());
                   }
                   
                   
                }
                if(LOWORD(wParam) == BUTTON_ID_EDIT){
                   int index = SendMessage(lst, LB_GETCURSEL, 0 , 0);
                   if (index == LB_ERR){
                    MessageBoxA(hwnd, "Maaaan , pls select an Entry_title", "U got msg", MB_OK);
                   }
                   else {
                    SetWindowTextA(edt_sec, V[index].description.c_str()); 
                    SetWindowTextA(edt, V[index].title.c_str()); 
                   }
                   
                   
                }
                if(LOWORD(wParam) == BUTTON_ID_SAVE){
                    int index = SendMessage(lst, LB_GETCURSEL, 0 , 0);
                   if (index == LB_ERR){
                    MessageBoxA(hwnd, "Maaaan , pls select an Entry_title", "U got msg", MB_OK);
                   }
                   else {
                        SendMessage(lst, LB_DELETESTRING, index, 0);
                        char new_title[1024] = {};
                        char new_description[1024] = {};
                        GetWindowTextA(edt_sec , new_description ,1024);
                        GetWindowTextA(edt , new_title ,1024);
                        V[index].title = new_title;
                        V[index].description = new_description;
                        SendMessageA(lst, LB_INSERTSTRING, index , (LPARAM) V[index].title.c_str());
                     }
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
        "add",
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
    HWND edit = CreateWindowExA(
        0,
        "BUTTON",
        "edit",
        WS_CHILD | WS_VISIBLE,
        10,
        200,
        100,
        100,
        hwnd,
        (HMENU)BUTTON_ID_EDIT,
        hInstance,
        nullptr  
    );   
    HWND del = CreateWindowExA(
        0,
        "BUTTON",
        "delete",
        WS_CHILD | WS_VISIBLE,
        10,
        100,
        100,
        100,
        hwnd,
        (HMENU)BUTTON_ID_DELETE,
        hInstance,
        nullptr  
    );
    HWND save = CreateWindowExA(
        0,
        "BUTTON",
        "save",
        WS_CHILD | WS_VISIBLE,
        10,
        300,
        100,
        100,
        hwnd,
        (HMENU)BUTTON_ID_SAVE,
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