// Sem5Lab06.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sem5Lab06.h"

#define MAX_LOADSTRING 100

#define IDB_START     10001
#define IDB_STOP      10002

struct ThreadInfo
{
    volatile int posX;
    volatile int posY;
    int speedX;
    int speedY;
    COLORREF color;
    HANDLE handle;
};

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND mainWnd;
bool isRunning = false;
ThreadInfo infos[] = {
    { 600, 400, -1, 0, RGB( 255, 0, 0 ), nullptr },
    { 600, 400, 1, -1, RGB( 0, 255, 0 ), nullptr },
    { 600, 400, 1, 1, RGB( 0, 0, 255 ), nullptr }
};
constexpr int squareSize = 50;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI        ThreadProc( LPVOID pParam );

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // Инициализация ресурсов
    infos[0].handle = CreateThread( NULL, 0, ThreadProc, (LPVOID)(infos + 0), CREATE_SUSPENDED, NULL );
    infos[1].handle = CreateThread( NULL, 0, ThreadProc, (LPVOID)(infos + 1), CREATE_SUSPENDED, NULL );
    infos[2].handle = CreateThread( NULL, 0, ThreadProc, (LPVOID)(infos + 2), CREATE_SUSPENDED, NULL );

    // Инициализация глобальных строк
    LoadStringW( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadStringW( hInstance, IDC_SEM5LAB06, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Выполнить инициализацию приложения:
    if (infos[0].handle && infos[1].handle && infos[2].handle && !InitInstance( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_SEM5LAB06 ) );

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
    TerminateThread( infos[2].handle, 0 );
    TerminateThread( infos[1].handle, 0 );
    TerminateThread( infos[0].handle, 0 );
    CloseHandle( infos[2].handle );
    CloseHandle( infos[1].handle );
    CloseHandle( infos[0].handle );

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
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SEM5LAB06 ) );
    wcex.hCursor        = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_SEM5LAB06 );
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

    mainWnd = hWnd;

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
        CreateWindowW( L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE, 50, 50, 100, 50, hWnd, (HMENU)IDB_START, hInst, nullptr );
        CreateWindowW( L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE, 150, 50, 100, 50, hWnd, (HMENU)IDB_STOP, hInst, nullptr );
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD( wParam );
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDB_START:
            if (!isRunning)
            {
                isRunning = true;
                for (int i = 0; i < 3; i++)
                {
                    ResumeThread( infos[i].handle );
                }
            }
            break;
        case IDB_STOP:
            if (isRunning)
            {
                isRunning = false;
                for (int i = 0; i < 3; i++)
                {
                    SuspendThread( infos[i].handle );
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
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint( hWnd, &ps );

        for (int i = 0; i < 3; i++)
        {
            HBRUSH brush = CreateSolidBrush( infos[i].color );
            int x = infos[i].posX;
            int y = infos[i].posY;
            RECT rect { x - squareSize / 2, y - squareSize / 2, x + squareSize / 2, y + squareSize / 2 };
            FillRect( hdc, &rect, brush );
            DeleteObject( brush );
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

DWORD WINAPI ThreadProc( LPVOID pParam )
{
    ThreadInfo& info = *reinterpret_cast<ThreadInfo*>(pParam);
    while (true)
    {
        int x = info.posX;
        int y = info.posY;
        RECT oldRect { x - squareSize / 2, y - squareSize / 2, x + squareSize / 2, y + squareSize / 2 };
        InvalidateRect( mainWnd, &oldRect, TRUE );
        x += info.speedX;
        y += info.speedY;
        RECT newRect { x - squareSize / 2, y - squareSize / 2, x + squareSize / 2, y + squareSize / 2 };
        InvalidateRect( mainWnd, &newRect, FALSE );
        info.posX = x;
        info.posY = y;
        Sleep( 15 );
    }
    return 0;
}
