// Sem5Lab01.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sem5Lab01.h"

#define MAX_LOADSTRING 100

#define IDM_START 10001
#define IDM_STOP  10002
#define IDT_TIMER 10003

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Cursor AND mask 
BYTE ANDmaskCursor[] =
{
    0b1111'1111, 0b1111'1111,   // line 1
    0b0011'1111, 0b1111'1111,   // line 2
    0b0001'1101, 0b1111'1111,   // line 3
    0b0001'1100, 0b1111'1111,   // line 4

    0b0001'1100, 0b0111'1111,   // line 5
    0b0001'1100, 0b0111'1111,   // line 6
    0b1111'1101, 0b1111'1111,   // line 7
    0b1111'1111, 0b1101'1111,   // line 8

    0b0111'1111, 0b1100'1111,   // line 9
    0b0001'1111, 0b0000'0111,   // line 10
    0b0001'1111, 0b1111'1111,   // line 11
    0b0000'1111, 0b1111'1111,   // line 12

    0b0000'1110, 0b0110'1111,   // line 13
    0b0000'1110, 0b0000'0111,   // line 14
    0b0000'1110, 0b0000'0111,   // line 15
    0b0000'1000, 0b0000'0111,   // line 16

    0b0000'1100, 0b0011'1111,   // line 17
    0b0001'1100, 0b0011'1111,   // line 18
    0b0011'1110, 0b0001'1111,   // line 19
    0b1111'1110, 0b0001'1111,   // line 20
    0b1111'1111, 0b0011'1111,   // line 21
};

// Cursor XOR mask 
BYTE XORmaskCursor[] =
{
    0b0000'0000, 0b0000'0000,   // line 1
    0b0000'0000, 0b0000'0000,   // line 2
    0b0100'0000, 0b0000'0000,   // line 3
    0b0111'0110, 0b0000'0000,   // line 4

    0b0111'0111, 0b0000'0000,   // line 5
    0b0111'0111, 0b0000'0000,   // line 6
    0b0111'0111, 0b0000'0000,   // line 7
    0b0001'0110, 0b0000'0000,   // line 8

    0b0010'0000, 0b1110'0000,   // line 9
    0b0111'0001, 0b1111'0000,   // line 10
    0b0111'0011, 0b1111'0000,   // line 11
    0b0111'1011, 0b1000'0000,   // line 12

    0b0111'1011, 0b1110'0000,   // line 13
    0b0111'1011, 0b1111'0000,   // line 14
    0b0111'1011, 0b0000'0000,   // line 15
    0b0111'0011, 0b0000'0000,   // line 16

    0b0110'0001, 0b1000'0000,   // line 17
    0b0100'0001, 0b1000'0000,   // line 18
    0b0000'0000, 0b1100'0000,   // line 19
    0b0000'0000, 0b1100'0000,   // line 20
    0b0000'0000, 0b0000'0000,   // line 21
};

constexpr int cursorWidth = 16;
constexpr int cursorHeight = 21;
static_assert(sizeof( ANDmaskCursor ) == sizeof( XORmaskCursor ), "Cursor masks have a different size");
static_assert(cursorWidth * cursorHeight == sizeof( ANDmaskCursor ) * 8, "Invalid cursor size");

HCURSOR hCursor;
HICON hIcon;
HICON hIconSmall;

// Переменные текста
WCHAR myText[] = L"Здесь могла быть ваша реклама...";
constexpr int myTextCount = sizeof( myText ) / sizeof( myText[0] ) - 1;
RECT MTRect { 0, 0, 0, 0 };
HFONT hMTFont;
int MTOffset = -50;
int MTSpeed = 0;


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

    // Инициализация ресурсов
    //hCursor = static_cast<HCURSOR>(LoadImage( nullptr, L"cursor.cur", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE ));
    hCursor = CreateCursor( hInstance, 0, 0, cursorWidth, cursorHeight, ANDmaskCursor, XORmaskCursor );
    hIcon = static_cast<HICON>(LoadImage( nullptr, L"Sem5Lab01.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE ));
    hIconSmall = static_cast<HICON>(LoadImage( nullptr, L"small.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE ));
    LOGFONTW lf {
            40,
            0,
            0,
            0,
            FW_NORMAL,
            TRUE,
            FALSE,
            FALSE,
            RUSSIAN_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_ROMAN,
            L"Times New Roman"
    };
    hMTFont = CreateFontIndirectW( &lf );

    // Инициализация глобальных строк
    LoadStringW( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadStringW( hInstance, IDC_SEM5LAB01, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Выполнить инициализацию приложения:
    if (!InitInstance( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_SEM5LAB01 ) );

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

    // Освобождение ресурсов
    DeleteObject( hMTFont );
    DestroyIcon( hIconSmall );
    DestroyIcon( hIcon );
    DestroyCursor( hCursor );

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
    wcex.hIcon          = hIcon;
    wcex.hCursor        = hCursor;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_SEM5LAB01 );
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = hIconSmall;

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
        HMENU hMenu = GetMenu( hWnd );
        HMENU hSubMenu = CreateMenu();
        AppendMenuW( hSubMenu, MF_STRING, IDM_START, L"Start" );
        AppendMenuW( hSubMenu, MF_STRING, IDM_STOP, L"Stop" );
        InsertMenuW( hMenu, 1, MF_BYPOSITION | MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu), L"Command" );
    }
    break;
    case WM_SETCURSOR:
    {
        UNREFERENCED_PARAMETER( wParam );
        int htId = LOWORD( lParam );
        switch (htId)
        {
        case HTCAPTION:
        case HTCLIENT:
        case HTCLOSE:
        case HTHSCROLL:
        case HTMENU:
        case HTMAXBUTTON:
        case HTMINBUTTON:
        case HTSYSMENU:
        case HTVSCROLL:
            SetCursor( hCursor );
            return TRUE;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
        }
    }
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
        case IDM_START:
            SetTimer( hWnd, IDT_TIMER, 30, nullptr );
            break;
        case IDM_STOP:
            KillTimer( hWnd, IDT_TIMER );
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

        auto hFont = SelectObject( hdc, hMTFont );
        auto color = GetTextColor( hdc );
        SetTextColor( hdc, RGB( 255, 0, 255 ) );
        DrawTextW( hdc, myText, myTextCount, &MTRect, 0 );
        SetTextColor( hdc, color );
        SelectObject( hdc, hFont );

        EndPaint( hWnd, &ps );
    }
    break;
    case WM_TIMER:
    {
        switch (wParam)
        {
        case IDT_TIMER:
        {
            UNREFERENCED_PARAMETER( lParam );
            if (MTOffset < 50)
                MTSpeed += 1;
            else
                MTSpeed = -MTSpeed;
            MTOffset += MTSpeed;

            InvalidateRect( hWnd, &MTRect, TRUE );

            RECT trect { 0, 0, 0, 0 };
            HDC hdc = GetWindowDC( hWnd );
            auto hFont = SelectObject( hdc, hMTFont );
            DrawTextW( hdc, myText, myTextCount, &trect, DT_CALCRECT );
            SelectObject( hdc, hFont );
            ReleaseDC( hWnd, hdc );

            RECT crect;
            GetClientRect( hWnd, &crect );
            int baseX = (crect.right - trect.right) / 2;
            int baseY = (crect.bottom - trect.bottom) / 2;

            MTRect.left = baseX;
            MTRect.top = baseY + MTOffset;
            MTRect.right = MTRect.left + trect.right;
            MTRect.bottom = MTRect.top + trect.bottom;
            InvalidateRect( hWnd, &MTRect, FALSE );
        }
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
