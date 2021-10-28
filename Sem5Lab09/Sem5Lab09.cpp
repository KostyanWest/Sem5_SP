// Sem5Lab09.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Sem5Lab09.h"
#include <regex>

#define MAX_LOADSTRING 100

#define IDC_LISTBOX  10001
#define IDC_EDIT     10002
#define IDB_SEARCH   10003

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hListbox;
HWND hEdit;
HWND hSearch;

struct Value
{
    std::shared_ptr<std::wstring> path;
    std::wstring name;
    std::wstring data;
};
std::vector<Value> regValues {};

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void QueryKey( HKEY hKey, std::shared_ptr<std::wstring> fullPath );

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
    LoadStringW( hInstance, IDC_SEM5LAB09, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    // Выполнить инициализацию приложения:
    if (!InitInstance( hInstance, nCmdShow ))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_SEM5LAB09 ) );

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
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SEM5LAB09 ) );
    wcex.hCursor        = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW( IDC_SEM5LAB09 );
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
        hListbox = CreateWindowExW( WS_EX_CLIENTEDGE, L"LISTBOX", L"Listbox",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            20, 20, 1200, 600, hWnd, (HMENU)IDC_LISTBOX, hInst, nullptr );
        hEdit = CreateWindowExW( WS_EX_CLIENTEDGE, L"EDIT", L"Key word",
            WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            20, 630, 460, 30, hWnd, (HMENU)IDC_EDIT, hInst, nullptr );
        hSearch = CreateWindowW( L"BUTTON", L"Поиск", WS_CHILD | WS_VISIBLE, 500, 630, 120, 30, hWnd, (HMENU)IDB_SEARCH, hInst, nullptr );
        QueryKey( HKEY_CURRENT_USER, std::make_shared<std::wstring>( L"HKEY_CURRENT_USER" ) );
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD( wParam );
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDB_SEARCH:
        {
            SendMessageW( hListbox, LB_RESETCONTENT, 0, 0 );
            SetWindowTextW( hSearch, L"Поиск..." );

            WCHAR buffer[500];
            GetWindowTextW( hEdit, buffer, sizeof( buffer ) / sizeof( buffer[0] ) );
          
            GetWindowTextW( hEdit, buffer, sizeof( buffer ) / sizeof( buffer[0] ) );
            for (auto& val : regValues)
            {
                if (val.path->find( buffer ) != std::string::npos ||
                    val.name.find( buffer ) != std::string::npos ||
                    val.data.find( buffer ) != std::string::npos)
                {
                    std::wstring str = *(val.path) + L": " + val.name + L" = " + val.data;
                    SendMessageW( hListbox, LB_ADDSTRING, 0, (LPARAM)str.c_str() );
                }
            }
            SetWindowTextW( hSearch, L"Поиск" );
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

void QueryKey(HKEY hKey, std::shared_ptr<std::wstring> fullPath)
{
    static WCHAR nameBuffer[16383];
    static BYTE dataBuffer[1024 * 8];
    int valueType;

    for (int i = 0,
        nameBufferLength = sizeof( nameBuffer ) / sizeof( nameBuffer[0] ),
        dataBufferLength = sizeof( dataBuffer ) / sizeof( dataBuffer[0] );
        RegEnumValueW( hKey, i, nameBuffer, (LPDWORD)&nameBufferLength,
            NULL, (LPDWORD)&valueType, dataBuffer, (LPDWORD)&dataBufferLength ) == ERROR_SUCCESS;
        i++,
        nameBufferLength = sizeof( nameBuffer ) / sizeof( nameBuffer[0] ),
        dataBufferLength = sizeof( dataBuffer ) / sizeof( dataBuffer[0] ))
    {
        nameBuffer[nameBufferLength] = L'\0';
        switch (valueType)
        {
        case REG_DWORD:
        {
            std::wstring name( nameBuffer );
            std::wstring data = std::to_wstring( *((UINT32*)dataBuffer) );
            Value val { fullPath, name, data };
            regValues.push_back( val );
        }
        break;
        case REG_QWORD:
        {
            std::wstring name( nameBuffer );
            std::wstring data = std::to_wstring( *((UINT64*)dataBuffer) );
            Value val { fullPath, name, data };
            regValues.push_back( val );
        }
        break;
        case REG_SZ:
        {
            std::wstring name( nameBuffer );
            dataBuffer[dataBufferLength] = L'\0';
            std::wstring data( (WCHAR*)dataBuffer );
            Value val { fullPath, name, data };
            regValues.push_back( val );
        }
        break;
        default:
            break;
        }
    }

    int kek;
    for (int i = 0,
        nameBufferLength = sizeof( nameBuffer ) / sizeof( nameBuffer[0] ),
        dataBufferLength = sizeof( dataBuffer ) / sizeof( dataBuffer[0] );
        (kek = RegEnumKeyExW( hKey, i, nameBuffer, (LPDWORD)&nameBufferLength,
            NULL, NULL, NULL, NULL )) == ERROR_SUCCESS;
        i++,
        nameBufferLength = sizeof( nameBuffer ) / sizeof( nameBuffer[0] ),
        dataBufferLength = sizeof( dataBuffer ) / sizeof( dataBuffer[0] ))
    {
        nameBuffer[nameBufferLength] = L'\0';
        auto subPath = std::make_shared<std::wstring>( *fullPath + L"\\" + nameBuffer );
        HKEY hSubKey;
        if (RegOpenKeyExW( hKey, nameBuffer, 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &hSubKey ) ==
            ERROR_SUCCESS)
        {
            QueryKey( hSubKey, subPath );
            RegCloseKey( hSubKey );
        }
    }
}
