// Sem5Lab03.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sem5Lab03.h"

#define MAX_LOADSTRING 100

#define IDB_DRAW     10001
#define IDB_ERASE    10002

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

bool isDrawing = false;
constexpr int imageX = 175;
constexpr int imageY = 400;

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

    // Инициализация глобальных строк
    LoadStringW( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadStringW( hInstance, IDC_SEM5LAB03, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Выполнить инициализацию приложения:
    if (!InitInstance( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_SEM5LAB03 ) );

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
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SEM5LAB03 ) );
    wcex.hCursor        = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_SEM5LAB03 );
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
        CreateWindowW( L"BUTTON", L"Draw", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 50, 50, 100, 50, hWnd, (HMENU)IDB_DRAW, hInst, nullptr );
        CreateWindowW( L"BUTTON", L"Erase", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 200, 50, 100, 50, hWnd, (HMENU)IDB_ERASE, hInst, nullptr );
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD( wParam );
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDB_DRAW:
            if (!isDrawing)
            {
                isDrawing = true;
                RECT rect { imageX - 150, imageY - 250, imageX + 150, imageY + 250 };
                InvalidateRect( hWnd, &rect, FALSE );
            }
            break;
        case IDB_ERASE:
            if (isDrawing)
            {
                isDrawing = false;
                RECT rect { imageX - 150, imageY - 250, imageX + 150, imageY + 250 };
                InvalidateRect( hWnd, &rect, TRUE );
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
    case WM_DRAWITEM:
    {
        DRAWITEMSTRUCT* pItem = reinterpret_cast<DRAWITEMSTRUCT*>(lParam);

        switch (wParam)
        {
        case IDB_DRAW:
        case IDB_ERASE:
        {
            COLORREF color;
            if (pItem->itemState & ODS_SELECTED)
                color = RGB( 255, 0, 255 );
            else
                color = RGB( 0, 0, 0 );

            HPEN hPen = CreatePen( PS_SOLID, 6, color );
            auto hOldPen = SelectObject( pItem->hDC, hPen );

            HBRUSH hBrush;
            if (pItem->itemState & ODS_GRAYED)
                hBrush = CreateSolidBrush( RGB( 200, 200, 200 ) );
            else
                hBrush = CreateSolidBrush( RGB( 255, 255, 255 ) );
            auto hOldBrush = SelectObject( pItem->hDC, hBrush );

            RoundRect( pItem->hDC, pItem->rcItem.left, pItem->rcItem.top, pItem->rcItem.right, pItem->rcItem.bottom, 32, 32 );
            WCHAR buffer[MAX_LOADSTRING];
            GetWindowTextW( pItem->hwndItem, buffer, MAX_LOADSTRING );
            auto oldColor = GetTextColor( pItem->hDC );
            SetTextColor( pItem->hDC, color );
            DrawTextW( pItem->hDC, buffer, -1, &(pItem->rcItem), DT_SINGLELINE | DT_CENTER | DT_VCENTER );

            SetTextColor( pItem->hDC, oldColor );
            SelectObject( pItem->hDC, hOldBrush );
            SelectObject( pItem->hDC, hOldPen );
            DeleteObject( hBrush );
            DeleteObject( hOldPen );
            return TRUE;
        }
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint( hWnd, &ps );
        
        if (isDrawing)
        {
            HPEN hBrownPen = CreatePen( PS_SOLID, 5, RGB( 800, 40, 0 ) );
            HBRUSH hBrownBrush = CreateSolidBrush( RGB( 130, 65, 0 ) );

            auto hOldPen = SelectObject( hdc, hBrownPen );
            auto hOldBrush = SelectObject( hdc, hBrownBrush );

            int x = imageX;
            int y = imageY;
            Rectangle( hdc, x - 10, y + 100, x + 10, y + 200 );
            DeleteObject( hBrownPen );
            DeleteObject( hBrownBrush );


            HPEN hGreenPen = CreatePen( PS_SOLID, 5, RGB( 0, 255, 0 ) );
            SelectObject( hdc, hGreenPen );
            HBRUSH hGreenBrush = CreateSolidBrush( RGB( 0, 200, 0 ) );
            SelectObject( hdc, hGreenBrush );

            POINT points[] = {
                //bottom
                {x,       y      },
                {x - 100, y + 100},
                {x + 100, y + 100},
                // center
                {x,       y - 100},
                {x - 100, y      },
                {x + 100, y      },
                // top
                {x,       y - 200},
                {x - 100, y - 100},
                {x + 100, y - 100}
            };
            Polygon( hdc, points, 3 );     // bottom
            Polygon( hdc, points + 3, 3 ); // center
            Polygon( hdc, points + 6, 3 ); // top

            DeleteObject( hGreenPen );
            DeleteObject( hGreenBrush );


            HPEN hNullPen = CreatePen( PS_NULL, 0, 0 );
            SelectObject( hdc, hNullPen );
            HBRUSH hRedBrush = CreateSolidBrush( RGB( 255, 0, 0 ) );
            SelectObject( hdc, hRedBrush );

            Ellipse( hdc, x - 30, y - 170, x - 10, y - 150 );
            Ellipse( hdc, x + 30, y - 40, x + 50, y - 20 );
            Ellipse( hdc, x - 60, y + 70, x - 40, y + 90 );

            DeleteObject( hNullPen );


            HPEN hYellowPen = CreatePen( PS_SOLID, 5, RGB( 255, 180, 0 ) );
            SelectObject( hdc, hYellowPen );

            Rectangle( hdc, x + 50, y + 150, x + 100, y + 200 );
            MoveToEx( hdc, x + 75, y + 150 + 2, nullptr );
            LineTo( hdc, x +75, y + 200 - 2 );
            MoveToEx( hdc, x + 50 + 2, y + 175, nullptr );
            LineTo( hdc, x + 100 - 2, y + 175 );

            DeleteObject( hYellowPen );
            DeleteObject( hRedBrush );


            HPEN hWhitePen = CreatePen( PS_SOLID, 2, RGB( 255, 255, 255 ) );
            SelectObject( hdc, hWhitePen );

            Arc( hdc, x - 30 + 3, y - 170 + 3, x - 10 - 3, y - 150 - 3,
                x - 20, y - 170, x - 30, y - 160 );
            Arc( hdc, x + 30 + 3, y - 40 + 3, x + 50 - 3, y - 20 - 3,
                x + 40, y - 40, x + 30, y - 30 );
            Arc( hdc, x - 60 + 3, y + 70 + 3, x - 40 - 3, y + 90 - 3,
                x - 50, y + 70, x - 60, y + 80 );

            DeleteObject( hWhitePen );


            SelectObject( hdc, hOldBrush );
            SelectObject( hdc, hOldPen );
        }

        EndPaint( hWnd, &ps );
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
