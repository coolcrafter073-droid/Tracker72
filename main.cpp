#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include "sqlite3.h"
const int BUTTON_ID = 1;
const int BUTTON_ID_SHOW = 2;
const int BUTTON_ID_DELETE = 3;
const int BUTTON_ID_EDIT = 4;
const int BUTTON_ID_SAVE = 5; 
const int BUTTON_ID_EXPORT = 6;
sqlite3* db;
HWND txt;
HWND edt;
HWND txt_mass;
HWND edt_sec;
HWND lst;
HWND hwnd;
struct Entry{
    int id;
    std::string title;
    std::string description;
};

std::vector<Entry> V = {};
void log(std::string s){
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);
    int year = local_time->tm_year + 1900; 
    int month = local_time->tm_mon + 1;    
    int day = local_time->tm_mday;       
    int hour = local_time->tm_hour;      
    int min = local_time->tm_min;        
    int sec = local_time->tm_sec;           
    std::ofstream f;
    f.open("tracker.log", std::ios::app);
    if (f.is_open()){       
        f << day << "." << month << "." << year << " " << hour << ":" << min << ":" << sec << ":" << s + "\n";
        f.close();  
    }
    else{
         MessageBoxA(hwnd, "Critical Error: Cant open log files", "U got msg", MB_OK);
    }

}
void check(int rc){
    if(rc != SQLITE_OK && rc != SQLITE_ROW && rc != SQLITE_DONE){
        std::string s = sqlite3_errmsg(db); 
        log("SQLITE_ERROR: " + s);
        MessageBoxA(hwnd, s.c_str(), "U got msg", MB_OK);
    }
    
}
int callback(void* data, int argc, char** argv, char** azColName){
        std::vector<Entry>* vec = static_cast<std::vector<Entry>*>(data);
        std::string s = "";
        Entry test_entry;
        test_entry.id = std::stoi(argv[0]);
        test_entry.title = argv[1];
        test_entry.description = argv[2];
        vec->push_back(test_entry);
        for (int i = 0; i < argc; i++){
            s += argv[i];
        }   
        return 0;
    }
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
                    log("add btn pressed");
                    char buff[1024] = {};
                    GetWindowTextA(edt , buff ,1024);
                    char buff2[1024] = {};
                    GetWindowTextA(edt_sec , buff2 ,1024);
                    sqlite3_stmt* stmt = nullptr;
                    int id = 0;
                    int rc = sqlite3_prepare_v2(
                        db,
                        "INSERT INTO Entries (title, description) VALUES (?,?);",
                        -1,
                        &stmt,
                        nullptr
                    ); 
                    check(rc);
                    rc = sqlite3_bind_text(stmt, 1, buff, -1 , SQLITE_TRANSIENT );
                    check(rc);
                    rc = sqlite3_bind_text(stmt, 2, buff, -1 , SQLITE_TRANSIENT );
                    check(rc);
                    rc = sqlite3_step(stmt);
                    check(rc);
                    if (rc == SQLITE_DONE){
                        id = sqlite3_last_insert_rowid(db);
                    }
                    rc = sqlite3_finalize(stmt);
                    check(rc);
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
                        
                        Entry entry = {id, buff, buff2};
                        V.push_back(entry);
                        SendMessageA(lst, LB_ADDSTRING, 0 , (LPARAM) entry.title.c_str());
                        std::string inf_display = "Current Entries:\n";
                        for (int i = 0; i < V.size(); i++){
                            inf_display += "Entry_ID: " + std::to_string(V[i].id) + "; " + V[i].title + "\n" + V[i].description + "\n";
                        };
                        std::string count_display = "Total Entries:\n " + std::to_string(V.size());
                        SetWindowTextA(txt_mass, inf_display.c_str());
                        SetWindowTextA(edt, "");
                        SetWindowTextA(edt_sec, "");
                        SetWindowTextA(txt,count_display.c_str());
                        std::cout << count_display;
                    };
                    

                }
                if(LOWORD(wParam) == BUTTON_ID_SHOW){
                   log("show btn pressed"); 
                   int index = SendMessage(lst, LB_GETCURSEL, 0 , 0);
                   if (index == LB_ERR){
                    MessageBoxA(hwnd, "Maaaan , pls select an Entry_title", "U got msg", MB_OK);
                   }
                   else {
                    std::string dsp = std::to_string(V[index].id) + ") "+ V[index].description;
                    SetWindowTextA(txt_mass, dsp.c_str()); 
                   }
                   
                   
                }
                if(LOWORD(wParam) == BUTTON_ID_DELETE){
                   log("del btn pressed");
                   int index = SendMessage(lst, LB_GETCURSEL, 0 , 0);
                   if (index == LB_ERR){
                    MessageBoxA(hwnd, "Maaaan , pls select an Entry_title", "U got msg", MB_OK);
                   }
                   else {
                    SendMessage(lst, LB_DELETESTRING, index , 0);
                    sqlite3_stmt* stmt = nullptr;
                    int rc = sqlite3_prepare_v2(
                        db,
                        "DELETE FROM Entries WHERE id = ?",
                        -1,
                        &stmt,
                        nullptr
                    );
                    check(rc);  
                    rc = sqlite3_bind_int(stmt, 1, V[index].id);
                    check(rc);
                    rc = sqlite3_step(stmt);
                    check(rc);
                    rc = sqlite3_finalize(stmt);
                    check(rc);
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
                   log("edt btn pressed");
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
                   log("save btn pressed");
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
                        sqlite3_stmt* stmt = nullptr;
                        int rc = sqlite3_prepare_v2(
                            db,
                            "UPDATE Entries SET title = ? , description = ? WHERE id = ?",
                            -1,
                            &stmt,
                            nullptr
                        );
                        check(rc);  
                        rc = sqlite3_bind_text(stmt, 1, new_title, -1 , SQLITE_TRANSIENT );
                        check(rc);
                        rc = sqlite3_bind_text(stmt, 2, new_description, -1 , SQLITE_TRANSIENT );
                        check(rc);
                        rc = sqlite3_bind_int(stmt, 3, V[index].id);
                        check(rc);
                        rc = sqlite3_step(stmt);
                        check(rc);
                        rc = sqlite3_finalize(stmt);
                        check(rc);
                        SendMessageA(lst, LB_INSERTSTRING, index , (LPARAM) V[index].title.c_str());
                        std::string inf_display = "Current Entries:\n";
                        for (int i = 0; i < V.size(); i++){
                            inf_display += "* " + V[i].title + "\n" + V[i].description + "\n";
                        };
                        SetWindowTextA(txt_mass, inf_display.c_str());
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
    log("Application started: ");
    const char CLASS_NAME[] = "MyTrackerWindow";
    int callback(void*, int, char**, char**);
    
    WNDCLASSA wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "I am still alive",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1000,
        800,
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
    HWND expbtn = CreateWindowExA(
        0,
        "BUTTON",
        "export",
        WS_CHILD | WS_VISIBLE,
        10,
        500,
        100,
        100,
        hwnd,
        (HMENU)BUTTON_ID_EXPORT,
        hInstance,
        nullptr  
    );
    txt = CreateWindowExA(
        0,
        "STATIC",
        "No Entries Currently",
        WS_CHILD | WS_VISIBLE,  
        170,
        350,
        200,
        120,
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
        170,
        hwnd,
        nullptr,
        hInstance,
        nullptr 
    );
    int rc = sqlite3_open("tracker.db", &db);
    if (rc != SQLITE_OK){
        std::string s = sqlite3_errmsg(db);
        MessageBoxA(hwnd, s.c_str(), "U got msg", MB_OK);
    }
    char *sql = "CREATE TABLE IF NOT EXISTS Entries(id INTEGER PRIMARY KEY, title TEXT, description TEXT); SELECT * FROM Entries;";
    rc = sqlite3_exec(db, sql , callback, &V, 0);
    
    if (rc != SQLITE_OK){
        std::string s = sqlite3_errmsg(db);
        log(s);
        MessageBoxA(hwnd, s.c_str(), "U got msg", MB_OK);
    }
    for (int i = 0; i < V.size(); i++){
        SendMessageA(lst, LB_ADDSTRING, 0 , (LPARAM) V[i].title.c_str()); 
    };
        
    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg{};

    while (GetMessageA(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    sqlite3_close(db);
    log("application closed");
    return 0;
}