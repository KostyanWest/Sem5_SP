// MySender.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "MySender.h"

#define MAX_LOADSTRING 100

#define IDG_COLOR    10001
#define IDR_RED      10002
#define IDR_GREEN    10003
#define IDR_BLUE     10004
#define IDG_FIGURE   10005
#define IDR_RHOMBUS  10006
#define IDR_SQUARE   10007
#define IDR_CIRCLE   10008
#define IDR_STAR     10009
#define IDC_DRAW     10010

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

UINT myColorMsg;
UINT myFigureMsg;
UINT myStateMsg;

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

    myColorMsg = RegisterWindowMessageW( L"MyColorMsg" );
    myFigureMsg = RegisterWindowMessageW( L"MyFigureMsg" );
    myStateMsg = RegisterWindowMessageW( L"MyStateMsg" );

    // Инициализация глобальных строк
    LoadStringW( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadStringW( hInstance, IDC_MYSENDER, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Выполнить инициализацию приложения:
    if (!InitInstance( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_MYSENDER ) );

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
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_MYSENDER ) );
    wcex.hCursor        = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_MYSENDER );
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
        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Color:", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10, 10, 350, 100, hWnd, (HMENU)IDG_COLOR, hInst, nullptr );
        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Red", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
            20, 30, 300, 20, hWnd, (HMENU)IDR_RED, hInst, nullptr );
        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Green", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
            20, 55, 300, 20, hWnd, (HMENU)IDR_GREEN, hInst, nullptr );
        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Blue", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
            20, 80, 300, 20, hWnd, (HMENU)IDR_BLUE, hInst, nullptr );

        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Figure:", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10, 150, 350, 125, hWnd, (HMENU)IDG_FIGURE, hInst, nullptr );
        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Rhombus", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
            20, 170, 300, 20, hWnd, (HMENU)IDR_RHOMBUS, hInst, nullptr );
        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Square", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
            20, 195, 300, 20, hWnd, (HMENU)IDR_SQUARE, hInst, nullptr );
        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Circle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
            20, 220, 300, 20, hWnd, (HMENU)IDR_CIRCLE, hInst, nullptr );
        CreateWindowEx( WS_EX_WINDOWEDGE, L"BUTTON", L"Star", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON,
            20, 245, 300, 20, hWnd, (HMENU)IDR_STAR, hInst, nullptr );

        CreateWindowW( L"BUTTON", L"Draw", WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
            10, 300, 100, 20, hWnd, (HMENU)IDC_DRAW, hInst, nullptr );

        SendMessageW( GetDlgItem( hWnd, IDR_RED ), BM_SETCHECK, BST_CHECKED, 0 );
        SendMessageW( GetDlgItem( hWnd, IDR_RHOMBUS ), BM_SETCHECK, BST_CHECKED, 0 );
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD( wParam );
        HWND hReceiver = FindWindowW( nullptr, L"MyReceiver" );
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDR_RED:
        case IDR_GREEN:
        case IDR_BLUE:
            if (hReceiver)
            {
                SendMessageW( GetDlgItem( hWnd, IDR_RED ), BM_SETCHECK, BST_UNCHECKED, 0 );
                SendMessageW( GetDlgItem( hWnd, IDR_GREEN ), BM_SETCHECK, BST_UNCHECKED, 0 );
                SendMessageW( GetDlgItem( hWnd, IDR_BLUE ), BM_SETCHECK, BST_UNCHECKED, 0 );
                SendMessageW( (HWND)lParam, BM_SETCHECK, BST_CHECKED, 0 );
                PostMessageW( hReceiver, myColorMsg, (WPARAM)hWnd, wmId - IDR_RED );
            }
            break;
        case IDR_RHOMBUS:
        case IDR_SQUARE:
        case IDR_CIRCLE:
        case IDR_STAR:
            if (hReceiver)
            {
                SendMessageW( GetDlgItem( hWnd, IDR_RHOMBUS ), BM_SETCHECK, BST_UNCHECKED, 0 );
                SendMessageW( GetDlgItem( hWnd, IDR_SQUARE ), BM_SETCHECK, BST_UNCHECKED, 0 );
                SendMessageW( GetDlgItem( hWnd, IDR_CIRCLE ), BM_SETCHECK, BST_UNCHECKED, 0 );
                SendMessageW( GetDlgItem( hWnd, IDR_STAR ), BM_SETCHECK, BST_UNCHECKED, 0 );
                SendMessageW( (HWND)lParam, BM_SETCHECK, BST_CHECKED, 0 );
                PostMessageW( hReceiver, myFigureMsg, (WPARAM)hWnd, wmId - IDR_RHOMBUS );
            }
            break;
        case IDC_DRAW:
            if (hReceiver)
            {
                LRESULT state = !SendMessageW( (HWND)lParam, BM_GETCHECK, 0, 0 );
                SendMessageW( (HWND)lParam, BM_SETCHECK, state, 0 );
                PostMessageW( hReceiver, myStateMsg, (WPARAM)hWnd, state );
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
