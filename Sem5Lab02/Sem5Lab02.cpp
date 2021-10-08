// Sem5Lab02.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sem5Lab02.h"

#define MAX_LOADSTRING 100

#define IDC_LISTBOX1 10001
#define IDC_LISTBOX2 10002
#define IDC_EDIT     10003
#define IDB_ADD      10004
#define IDB_DELETE   10005
#define IDB_TORIGHT  10006
#define IDB_CLEAR    10007

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hListbox1;
HWND hListbox2;
HWND hEdit;

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
    LoadStringW( hInstance, IDC_SEM5LAB02, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Выполнить инициализацию приложения:
    if (!InitInstance( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_SEM5LAB02 ) );

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
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SEM5LAB02 ) );
    wcex.hCursor        = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_SEM5LAB02 );
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
        hListbox1 = CreateWindowExW( WS_EX_CLIENTEDGE, L"LISTBOX", L"Listbox1",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            20, 20, 300, 200, hWnd, (HMENU)IDC_LISTBOX1, hInst, nullptr );
        hListbox2 = CreateWindowExW( WS_EX_CLIENTEDGE, L"LISTBOX", L"Listbox2",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            340, 20, 300, 200, hWnd, (HMENU)IDC_LISTBOX2, hInst, nullptr );
        hEdit = CreateWindowExW( WS_EX_CLIENTEDGE, L"EDIT", L"Edit",
            WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            20, 230, 300, 30, hWnd, (HMENU)IDC_EDIT, hInst, nullptr );
        CreateWindowW( L"BUTTON", L"Add", WS_CHILD | WS_VISIBLE, 20, 270, 75, 30, hWnd, (HMENU)IDB_ADD, hInst, nullptr );
        CreateWindowW( L"BUTTON", L"Delete", WS_CHILD | WS_VISIBLE, 95, 270, 75, 30, hWnd, (HMENU)IDB_DELETE, hInst, nullptr );
        CreateWindowW( L"BUTTON", L"ToRight", WS_CHILD | WS_VISIBLE, 170, 270, 75, 30, hWnd, (HMENU)IDB_TORIGHT, hInst, nullptr );
        CreateWindowW( L"BUTTON", L"Clear", WS_CHILD | WS_VISIBLE, 245, 270, 75, 30, hWnd, (HMENU)IDB_CLEAR, hInst, nullptr );
        SendMessageW( hEdit, EM_LIMITTEXT, MAX_LOADSTRING - 1, 0 );
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD( wParam );
        switch (wmId)
        {
        case IDB_ADD:
        {
            int length = GetWindowTextLengthW( hEdit );
            if (length > 0)
            {
                TCHAR buffer[MAX_LOADSTRING];
                GetWindowTextW( hEdit, buffer, sizeof( buffer ) / sizeof( buffer[0] ) );
                int index = SendMessageW( hListbox1, LB_FINDSTRINGEXACT, -1, (LPARAM)buffer );
                if (index == LB_ERR)
                {
                    SendMessageW( hListbox1, LB_ADDSTRING, 0, (LPARAM)buffer );
                    SetWindowTextW( hEdit, L"" );
                }
            }
        }
        break;
        case IDB_DELETE:
        {
            int index = SendMessageW( hListbox1, LB_GETCURSEL, 0, 0 );
            if (index != LB_ERR)
            {
                SendMessageW( hListbox1, LB_DELETESTRING, index, 0 );
            }
        }
        break;
        case IDB_TORIGHT:
        {
            int index1 = SendMessageW( hListbox1, LB_GETCURSEL, 0, 0 );
            if (index1 != LB_ERR)
            {
                TCHAR buffer[MAX_LOADSTRING];
                SendMessageW( hListbox1, LB_GETTEXT, index1, (LPARAM)buffer );
                int index2 = SendMessageW( hListbox2, LB_FINDSTRINGEXACT, -1, (LPARAM)buffer );
                if (index2 == LB_ERR)
                {
                    SendMessageW( hListbox2, LB_ADDSTRING, 0, (LPARAM)buffer );
                }
            }
        }
        break;
        case IDB_CLEAR:
        {
            SendMessageW( hListbox1, LB_RESETCONTENT, 0, 0 );
            SendMessageW( hListbox2, LB_RESETCONTENT, 0, 0 );
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
