// MyReceiver.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "MyReceiver.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

UINT myColorMsg;
UINT myFigureMsg;
UINT myStateMsg;

LPARAM colorSelect = 0;
LPARAM figureSelect = 0;
LPARAM stateSelect = 0;

POINT pos { -300, -300 };

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
    LoadStringW( hInstance, IDC_MYRECEIVER, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Выполнить инициализацию приложения:
    if (!InitInstance( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_MYRECEIVER ) );

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
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_MYRECEIVER ) );
    wcex.hCursor        = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_MYRECEIVER );
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
    case WM_LBUTTONDOWN:
    {
        if (stateSelect)
        {
            pos.x = ((int)(short)LOWORD( lParam )); // GET_X_LPARAM
            pos.y = ((int)(short)HIWORD( lParam )); // GET_Y_LPARAM
            RECT rect { pos.x - 128, pos.y - 128, pos.x + 128, pos.y + 128 };
            InvalidateRect( hWnd, &rect, FALSE );
        }
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD( wParam );
        // Разобрать выбор в меню:
        switch (wmId)
        {
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
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint( hWnd, &ps );
        
        COLORREF color;
        switch (colorSelect)
        {
        case 0:
            color = RGB( 255, 0, 0 );
            break;
        case 1:
            color = RGB( 0, 255, 0 );
            break;
        case 2:
            color = RGB( 0, 0, 255 );
            break;
        default:
            color = RGB( 0, 0, 0 );
        }

        HPEN hPen = CreatePen( PS_SOLID, 6, color );
        auto hOldPen = SelectObject( hdc, hPen );
        HBRUSH hBrush = CreateSolidBrush( color );
        auto hOldBrush = SelectObject( hdc, hBrush );

        switch (figureSelect)
        {
        case 0:
        {
            POINT points[] = {
                {pos.x,       pos.y - 100},
                {pos.x - 100, pos.y      },
                {pos.x,       pos.y + 100},
                {pos.x + 100, pos.y       }
            };
            Polygon( hdc, points, sizeof( points ) / sizeof( points[0] ) );
        }
        break;
        case 1:
        {
            Rectangle( hdc, pos.x - 100, pos.y - 100, pos.x + 100, pos.y + 100 );
        }
        break;
        case 2:
        {
            Ellipse( hdc, pos.x - 100, pos.y - 100, pos.x + 100, pos.y + 100 );
        }
        break;
        case 3:
        {
            POINT points[] = {
                {pos.x,       pos.y - 100},
                {pos.x - 25,  pos.y - 25 },
                {pos.x - 100, pos.y - 25 },
                {pos.x - 40,  pos.y + 20 },
                {pos.x - 60,  pos.y + 100},
                {pos.x,       pos.y + 50 },
                {pos.x + 60,  pos.y + 100},
                {pos.x + 40,  pos.y + 20 },
                {pos.x + 100, pos.y - 25 },
                {pos.x + 25,  pos.y - 25 },
            };
            Polygon( hdc, points, sizeof( points ) / sizeof( points[0] ) ); 
        }
        break;
        default:
        {
            RECT rect { pos.x - 50, pos.y - 100, pos.x + 50, pos.y + 100 };
            FillRect( hdc, &rect, hBrush );
        }
        }

        SelectObject( hdc, hOldBrush );
        SelectObject( hdc, hOldPen );
        DeleteObject( hBrush );
        DeleteObject( hPen );

        EndPaint( hWnd, &ps );
    }
    break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    default:
        if (message == myColorMsg)
            colorSelect = lParam;
        else if (message == myFigureMsg)
            figureSelect = lParam;
        else if (message == myStateMsg)
            stateSelect = lParam;
        else
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
