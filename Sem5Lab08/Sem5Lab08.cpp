// Sem5Lab08.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sem5Lab08.h"
#include <tlhelp32.h>

#define MAX_LOADSTRING 100

#define IDC_LISTBOX1 10001
#define IDC_LISTBOX2 10002
#define IDB_REFRESH  10003
#define IDM_IDLE    (10100 + IDLE_PRIORITY_CLASS)
#define IDM_BELOW   (10100 + BELOW_NORMAL_PRIORITY_CLASS)
#define IDM_NORMAL  (10100 + NORMAL_PRIORITY_CLASS)
#define IDM_ABOVE   (10100 + ABOVE_NORMAL_PRIORITY_CLASS)
#define IDM_HIGH    (10100 + HIGH_PRIORITY_CLASS)
#define IDM_REAL    (10100 + REALTIME_PRIORITY_CLASS)

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hListbox1;
HWND hListbox2;
HMENU hPopupMenu;
int lastCheckedMenu;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadStringW( hInstance, IDC_SEM5LAB08, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Выполнить инициализацию приложения:
    if (!InitInstance( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_SEM5LAB08 ) );

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage( &msg, nullptr, 0, 0 ))
    {
        if (!TranslateAccelerator( msg.hwnd, hAccelTable, &msg ))
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
    WNDCLASSEXW wcex;

    wcex.cbSize         = sizeof( WNDCLASSEX );

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SEM5LAB08 ) );
    wcex.hCursor        = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_SEM5LAB08 );
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

    return RegisterClassExW( &wcex );
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr );

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message)
    {
    case WM_CREATE:
    {
        hListbox1 = CreateWindowW( L"LISTBOX", L"Listbox1",
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_SORT | LBS_NOTIFY,
            20, 20, 300, 500, hWnd, (HMENU)IDC_LISTBOX1, hInst, nullptr );
        hListbox2 = CreateWindowW( L"LISTBOX", L"Listbox2",
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_SORT,
            340, 20, 300, 500, hWnd, (HMENU)IDC_LISTBOX2, hInst, nullptr );
        CreateWindowW( L"BUTTON", L"Обновить", WS_CHILD | WS_VISIBLE, 20, 530, 75, 30, hWnd, (HMENU)IDB_REFRESH, hInst, nullptr );
        hPopupMenu = CreatePopupMenu();
        AppendMenuW( hPopupMenu, MF_STRING, IDM_REAL, L"Реального времени" );
        AppendMenuW( hPopupMenu, MF_STRING, IDM_HIGH, L"Высокий" );
        AppendMenuW( hPopupMenu, MF_STRING, IDM_ABOVE, L"Выше среднего" );
        AppendMenuW( hPopupMenu, MF_STRING, IDM_NORMAL, L"Обычный" );
        AppendMenuW( hPopupMenu, MF_STRING, IDM_BELOW, L"Ниже среднего" );
        AppendMenuW( hPopupMenu, MF_CHECKED | MF_STRING, IDM_IDLE, L"Низкий" );
        lastCheckedMenu = IDM_IDLE;
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD( wParam );
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDB_REFRESH:
        {
            SendMessageW( hListbox1, LB_RESETCONTENT, 0, 0 );
            SendMessageW( hListbox2, LB_RESETCONTENT, 0, 0 );
            HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
            if (hSnapshot != INVALID_HANDLE_VALUE)
            {
                PROCESSENTRY32W entry;
                entry.dwSize = sizeof( entry );
                if (Process32FirstW( hSnapshot, &entry ))
                {
                    do
                    {
                        int index = SendMessageW( hListbox1, LB_ADDSTRING, 0, (LPARAM)(entry.szExeFile) );
                        SendMessageW( hListbox1, LB_SETITEMDATA, index, entry.th32ProcessID );
                    }
                    while (Process32NextW( hSnapshot, &entry ));
                }
                CloseHandle( hSnapshot );
            }
        }
        break;
        case IDC_LISTBOX1:
        {
            switch (HIWORD(wParam))
            {
            case LBN_SELCHANGE:
            {
                SendMessageW( hListbox2, LB_RESETCONTENT, 0, 0 );
                int index = SendMessageW( hListbox1, LB_GETCURSEL, 0, 0 );
                if (index != LB_ERR)
                {
                    int processID = SendMessageW( hListbox1, LB_GETITEMDATA, index, 0 );
                    HANDLE hSnapshot;
                    // повторяем, пока не получим значение, отличное от ERROR_BAD_LENGTH
                    do
                    {
                        hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE, processID );
                    }
                    while (hSnapshot == (HANDLE)ERROR_BAD_LENGTH);

                    if (hSnapshot != INVALID_HANDLE_VALUE)
                    {
                        MODULEENTRY32W entry;
                        entry.dwSize = sizeof( entry );
                        if (Module32FirstW( hSnapshot, &entry ))
                        {
                            do
                            {
                                SendMessageW( hListbox2, LB_ADDSTRING, 0, (LPARAM)(entry.szModule) );
                            }
                            while (Module32NextW( hSnapshot, &entry ));
                        }
                        CloseHandle( hSnapshot );
                    }
                }
            }
            break;
            case LBN_SELCANCEL:
                SendMessageW( hListbox2, LB_RESETCONTENT, 0, 0 );
                break;
            default:
                return DefWindowProc( hWnd, message, wParam, lParam );
            }
        }
        break;
        case IDM_ABOUT:
            DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
            break;
        case IDM_EXIT:
            DestroyWindow( hWnd );
            break;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
        }
    }
    break;
    case WM_CONTEXTMENU:
    {
        if ((HWND)wParam == hListbox1)
        {
            int index = SendMessageW( hListbox1, LB_GETCURSEL, 0, 0 );
            if (index != LB_ERR)
            {
                int processID = SendMessageW( hListbox1, LB_GETITEMDATA, index, 0 );
                HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, processID );
                if (hProcess != NULL)
                {
                    int priority = GetPriorityClass( hProcess );
                    CheckMenuItem( hPopupMenu, lastCheckedMenu, MF_BYCOMMAND | MF_UNCHECKED );
                    lastCheckedMenu = 10100 + priority;
                    CheckMenuItem( hPopupMenu, lastCheckedMenu, MF_BYCOMMAND | MF_CHECKED );
                    CloseHandle( hProcess );
                }

                hProcess = OpenProcess( PROCESS_SET_INFORMATION, FALSE, processID );
                for (int i = 0; i < 6; i++)
                {
                    EnableMenuItem( hPopupMenu, i, MF_BYPOSITION | (hProcess != NULL ? MF_ENABLED : MF_GRAYED) );
                }

                int result = TrackPopupMenu( hPopupMenu,
                    TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION,
                    ((int)(short)LOWORD( lParam )),
                    ((int)(short)HIWORD( lParam )),
                    0, hWnd, NULL );

                if (hProcess != NULL)
                {
                    if (result != 0)
                    {
                        SetPriorityClass( hProcess, result - 10100 );
                    }
                    CloseHandle( hProcess );
                }
            }
        }
        else
        {
            return DefWindowProc( hWnd, message, wParam, lParam );
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL)
        {
            EndDialog( hDlg, LOWORD( wParam ) );
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
