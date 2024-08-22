#include "Main.h"

typedef struct game_ctrl_s
{
    BOOL            run;
    int           debug;
}game_ctrl_t;

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
HWND hWnd;                                      // 当前窗口
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

game_ctrl_t GAME_CTRL = { 0 };

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Readme(HWND, UINT, WPARAM, LPARAM);
DWORD   WINAPI      TetrisRun(LPVOID lpProgram);
void                DebugOpen(void);
void                DebugClose(void);
void                GameCtrlInit(void);
void                GamePlaySound(int32 sid);

////****Winmain函数，程序入口点函数**************************************  
int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HACCEL hAccelTable;
    MSG msg;
    DWORD   dwTetrisRunTId;
    HANDLE  hTetrisRunT;

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMETETRIS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    DebugOpen();

    gdi_init(hWnd);//显示初始化

    gdi_set_start_screen_image(TEXT("background0.bmp"));

    // 设置随机数种子为当前时间
    srand(time(0));

    int randnum = rand() % 3;
    if(randnum == 0) gdi_set_background_image(TEXT("background1.bmp"));
    else if(randnum == 1) gdi_set_background_image(TEXT("background2.bmp"));
	else gdi_set_background_image(TEXT("background3.bmp")); 

    gdi_clear(GDI_RGB_BACKGROUND);//显示初始化
    GameCtrlInit();
    GamePlaySound(IDR_WAVE_BACKG);

    hTetrisRunT = CreateThread(NULL, 0, TetrisRun, NULL, 0, &dwTetrisRunTId);
    //创建线程失败
    if (hTetrisRunT == NULL)
    {
        ExitProcess(0);//主程序退出
    }

    //快捷键
    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMETETRIS));

    // 显示开场画面
    gdi_show_start_screen();

    // 主消息循环: 
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            //TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//****设计一个窗口类，类似填空题，使用窗口结构体*************************
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    HBRUSH hWindowBrush;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW; 
    wcex.lpfnWndProc = WndProc;   
    wcex.cbClsExtra = 0;  
    wcex.cbWndExtra = 0;  
    wcex.hInstance = hInstance;  
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMETETRIS));  
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW); 
    hWindowBrush = CreateSolidBrush((COLORREF)GDI_RGB_BACKGROUND); 
    wcex.hbrBackground = hWindowBrush;
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GAMETETRIS); 
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); 

    return RegisterClassExW(&wcex); 
}

//****初始化函数*************************************
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; 

    DWORD dwStyle =
        WS_OVERLAPPEDWINDOW |
        WS_CLIPCHILDREN |
        WS_CLIPSIBLINGS |
        WS_POPUPWINDOW;
    dwStyle &= (~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX));

    RECT wndRect;
    wndRect.left = 0;
    wndRect.top = 0;

#if 0
    wndRect.right = MAX_WND_WIDTH + GetSystemMetrics(SM_CXFRAME);
    wndRect.bottom = MAX_WND_HEIGHT + GetSystemMetrics(SM_CYCAPTION) +
        GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CYMENU);
    AdjustWindowRect(&wndRect, WS_CAPTION, FALSE);
#else
    wndRect.right = MAX_WND_WIDTH;
    wndRect.bottom = MAX_WND_HEIGHT;
    AdjustWindowRect(&wndRect, dwStyle, TRUE);
#endif

    hWnd = CreateWindowW(szWindowClass,szTitle,dwStyle,CW_USEDEFAULT,  
        CW_USEDEFAULT, wndRect.right - wndRect.left,wndRect.bottom - wndRect.top,NULL, 
        (HMENU)LoadMenu(hInst, MAKEINTRESOURCE(IDC_GAMETETRIS)), hInstance, NULL); 
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//****消息处理函数**********************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId;
    PAINTSTRUCT ps;
    HDC hdc;
    switch (message)
    {
    case WM_COMMAND:
    {
        wmId = LOWORD(wParam);
        // 分析菜单选择: 
        switch (wmId)
        {
        case IDM_ABOUT: //关于
            GAME_CTRL.run = FALSE;//点击对话框的时候暂停
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_README: //说明
            GAME_CTRL.run = FALSE;//点击对话框的时候暂停
            DialogBox(hInst, MAKEINTRESOURCE(IDD_READMEBOX), hWnd, Readme);
            break;
        case IDM_EXIT://退出
            DestroyWindow(hWnd);
            break;
        case IDM_STOP://暂停
            GAME_CTRL.run = FALSE;
            break;
        case IDM_CONTINUE://继续
            GAME_CTRL.run = TRUE;
            break;
        case IDM_ADDSPEED://加速
            tetris_add_speed();
            break;
        case IDM_SUBSPEED://减速
            tetris_sub_speed();
            break;
        case IDM_TETRISFIRE://变形
            tetris_shape_deform();
            break;
        case IDM_DEBUG_ON:
            GAME_CTRL.debug = TRUE;
            break;
        case IDM_DEBUG_OFF:
            GAME_CTRL.debug = FALSE;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        gdi_update();
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        gdi_dinit();
        GamePlaySound(0);
        DebugClose();
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (g_gameState == GAME_STATE_START_SCREEN)
        {
            g_gameState = GAME_STATE_PLAYING;
            GAME_CTRL.run = TRUE;  // 启动游戏
        }
        else
        {
            switch (wParam)
            {
            case VK_UP://上
                break;
            case VK_DOWN://下
                tetris_shape_move(DR_DOWN);
                break;
            case VK_LEFT://左
                tetris_shape_move(DR_LEFT);
                break;
            case VK_RIGHT://右
                tetris_shape_move(DR_RIGHT);
                break;
            case VK_RETURN://回车键变形
                tetris_shape_deform();
                break;
            case VK_F1://加速
                tetris_add_speed();
                break;
            case VK_F2://减速
                tetris_sub_speed();
                break;
            case VK_F5://
                GAME_CTRL.debug = TRUE;
                break;
            case VK_F6://
                GAME_CTRL.debug = FALSE;
                break;
            case VK_SPACE://暂停/开始
                if (GAME_CTRL.run == FALSE)
                {
                    GAME_CTRL.run = TRUE;
                }
                else
                {
                    GAME_CTRL.run = FALSE;
                }
                break;
            default:
                break;
            }
            break;
        }
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)// “关于”框的消息处理程序。
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            GAME_CTRL.run = TRUE;//点击对话框的时候暂停
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Readme(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)  //响应用户在对话框中的操作
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            GAME_CTRL.run = TRUE;//点击对话框的时候暂停
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

DWORD WINAPI TetrisRun(LPVOID lpProgram)  //运行游戏，多线程的运用
{
    tetris_game_init();

    while (TRUE)
    {
        if (g_gameState == GAME_STATE_PLAYING)
        {
            if (GAME_CTRL.run)
            {
                if (tetris_game_run(GAME_CTRL.debug) != LF_LIVE)
                {
                    MessageBox(hWnd, TEXT("继续战斗吧!"), TEXT("你输了!"), MB_OK);
                    GameCtrlInit();
                    tetris_game_init();
                }
                Sleep(1100 - (tetris_get_speed() * 100));
            }
        }
    }
    return 0;
}

void GameCtrlInit(void)   //设置游戏运行状态和调试状态
{
    GAME_CTRL.run = TRUE;
    GAME_CTRL.debug = TRUE;
}

void GamePlaySound(int32 sid)
{
    if (sid)
    {//start
        PlaySound(MAKEINTRESOURCE(sid), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
    }
    else
    {//stop
        PlaySound(NULL, 0, 0);
    }
}

void DebugOpen(void)    //输出额外的调试信息
{
#ifdef __DEBUG__
    FILE* stream;
    AllocConsole();
    SetConsoleTitle(_T("Tank_Debug"));
    freopen_s(&stream, "CONOUT$", "w+t", stdout);
    freopen_s(&stream, "CONIN$", "r+t", stdin);
#endif
}

void DebugClose(void)
{
#ifdef __DEBUG__
    FreeConsole();
#endif
}