#include "Gdi.h"

HPEN		hGdiPen = NULL;	        //画笔
HBRUSH		hGdiBrush = NULL;	    //画刷
HDC			mdc;	            //内存设备(双缓冲技术)
HDC         hdc;                //硬件设备
HWND        hGdiWnd;                //窗口
RECT        hGdiWndRect;            //窗口客户区大小

HBITMAP     mGdiBmp;
HBITMAP     mGdiBmpOld;

HBITMAP hBackgroundBmp = NULL;
HBITMAP hStartScreenBmp = NULL;

GameState g_gameState = GAME_STATE_START_SCREEN;

#define maxX SCREEN_X
#define maxY SCREEN_Y

static void _gdi_clr_pencol(HPEN _hGdiPen)
{
    DeleteObject(_hGdiPen);//释放资源
    SelectObject(mdc, hGdiPen);//恢复初始画刷
}

static HPEN _gdi_set_pencol(int color)
{
    HPEN _hGdiPen;

    COLORREF color_t = (COLORREF)color;
    _hGdiPen = CreatePen(PS_SOLID, 1, color_t);//画笔

    hGdiPen = static_cast<HPEN>(SelectObject(mdc, _hGdiPen));//为缓存DC选择画笔
    return _hGdiPen;
}

static void _gdi_clr_brushcol(HBRUSH _hGdiBrush)
{
    DeleteObject(_hGdiBrush);//释放资源
    SelectObject(mdc, hGdiBrush);//恢复初始画刷
}

static HBRUSH _gdi_set_brushcol(int color)
{
    HBRUSH _hGdiBrush;

    COLORREF color_t = (COLORREF)color;
    _hGdiBrush = CreateSolidBrush(color_t);//画刷

    hGdiBrush = static_cast<HBRUSH>(SelectObject(mdc, _hGdiBrush));//为缓存DC选择画刷
    return _hGdiBrush;
}

void gdi_clear(int colour)
{
    if (g_gameState == GAME_STATE_START_SCREEN && hStartScreenBmp)
    {
        HDC hdcMem = CreateCompatibleDC(mdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hStartScreenBmp);

        BITMAP bmp;
        GetObject(hStartScreenBmp, sizeof(BITMAP), &bmp);

        SetStretchBltMode(mdc, HALFTONE);
        StretchBlt(mdc, 0, 0, maxX, maxY, hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
    }
    else if (hBackgroundBmp)
    {
        HDC hdcMem = CreateCompatibleDC(mdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hBackgroundBmp);

        BITMAP bmp;
        GetObject(hBackgroundBmp, sizeof(BITMAP), &bmp);

        SetStretchBltMode(mdc, HALFTONE);
        StretchBlt(mdc, 0, 0, maxX, maxY, hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
    }
    else
    {
        gdi_rectangle(0, 0, maxX, maxY, colour, TRUE);
    }
}

void gdi_get_screen_size(int* x, int* y)
{
    if (x != NULL)
    {
        *x = maxX;
    }
    if (y != NULL)
    {
        *y = maxY;
    }
}

void gdi_set_point(int x, int y, int colour)
{
    x = ((x < 0) ? 0 : ((x > (maxX - 1)) ? (maxX - 1) : x));
    y = ((y < 0) ? 0 : ((y > (maxY - 1)) ? (maxY - 1) : y));

    HPEN hPen = _gdi_set_pencol(colour);

    SetPixel(mdc, x, y, colour);

    _gdi_clr_pencol(hPen);
}

int gdi_get_point(int x, int y)
{
    x = ((x < 0) ? 0 : ((x > (maxX - 1)) ? (maxX - 1) : x));
    y = ((y < 0) ? 0 : ((y > (maxY - 1)) ? (maxY - 1) : y));

    COLORREF col = GetPixel(mdc, x, y);

    return (int)col;

}

void gdi_line(int x0, int y0, int x1, int y1, int colour)
{
    x0 = ((x0 < 0) ? 0 : ((x0 > (maxX - 1)) ? (maxX - 1) : x0));
    x1 = ((x1 < 0) ? 0 : ((x1 > (maxX - 1)) ? (maxX - 1) : x1));
    y0 = ((y0 < 0) ? 0 : ((y0 > (maxY - 1)) ? (maxY - 1) : y0));
    y1 = ((y1 < 0) ? 0 : ((y1 > (maxY - 1)) ? (maxY - 1) : y1));
    MoveToEx(mdc, x0, y0, NULL);

    HPEN _hPen = _gdi_set_pencol(colour);
    LineTo(mdc, x1, y1);
    _gdi_clr_pencol(_hPen);
}

void gdi_triangle(int x1, int y1, int x2, int y2, int colour, int filled)
{
    HPEN _hPen;
    HBRUSH _hBrush;
    POINT triangle[3] = { 0 };
    int halfx = 0;

    halfx = ((x2 - x1 + 1) / 2);

    triangle[0].x = x1 + halfx;
    triangle[0].y = y1;

    triangle[1].x = x1;
    triangle[1].y = y2;

    triangle[2].x = x2;
    triangle[2].y = y2;

    if (filled)
    {
        _hPen = _gdi_set_pencol(colour);
        _hBrush = _gdi_set_brushcol(colour);
        Polygon(mdc, triangle, 3);
        _gdi_clr_pencol(_hPen);
        _gdi_clr_brushcol(_hBrush);
    }
    else
    {
        _hPen = _gdi_set_pencol(colour);
        Polygon(mdc, triangle, 3);
        _gdi_clr_pencol(_hPen);
    }
}

void gdi_rhombus(int x1, int y1, int x2, int y2, int colour, int filled)
{
    HPEN _hPen;
    HBRUSH _hBrush;
    POINT rhombus[4] = { 0 };
    int halfx = 0, halfy = 0;

    halfx = ((x2 - x1 + 1) / 2);
    halfy = ((y2 - y1 + 1) / 2);

    rhombus[0].x = x1 + halfx;
    rhombus[0].y = y1;

    rhombus[1].x = x2;
    rhombus[1].y = y2 - halfy;

    rhombus[2].x = x2 - halfx;
    rhombus[2].y = y2;

    rhombus[3].x = x1;
    rhombus[3].y = y1 + halfx;

    if (filled)
    {
        _hPen = _gdi_set_pencol(colour);
        _hBrush = _gdi_set_brushcol(colour);
        Polygon(mdc, rhombus, 4);
        _gdi_clr_pencol(_hPen);
        _gdi_clr_brushcol(_hBrush);
    }
    else
    {
        _hPen = _gdi_set_pencol(colour);
        Polygon(mdc, rhombus, 4);
        _gdi_clr_pencol(_hPen);
    }
}

void gdi_rectangle(int x1, int y1, int x2, int y2, int colour, int filled)
{
    HPEN _hPen;
    HBRUSH _hBrush;
    x1 = ((x1 < 0) ? 0 : ((x1 > (maxX - 1)) ? (maxX - 1) : x1));
    y1 = ((y1 < 0) ? 0 : ((y1 > (maxY - 1)) ? (maxY - 1) : y1));
    x2 = ((x2 < 0) ? 0 : ((x2 > (maxX - 1)) ? (maxX - 1) : x2));
    y2 = ((y2 < 0) ? 0 : ((y2 > (maxY - 1)) ? (maxY - 1) : y2));

    if (filled)
    {
        _hPen = _gdi_set_pencol(colour);
        _hBrush = _gdi_set_brushcol(colour);
        Rectangle(mdc, x1, y1, x2, y2);
        _gdi_clr_pencol(_hPen);
        _gdi_clr_brushcol(_hBrush);
    }
    else
    {
        _hPen = _gdi_set_pencol(colour);
        Rectangle(mdc, x1, y1, x2, y2);
        _gdi_clr_pencol(_hPen);
    }
}

void gdi_ellipse(int x1, int y1, int x2, int y2, int colour, int filled)
{
    HPEN _hPen;
    HBRUSH _hBrush;
    x1 = ((x1 < 0) ? 0 : ((x1 > (maxX - 1)) ? (maxX - 1) : x1));
    y1 = ((y1 < 0) ? 0 : ((y1 > (maxY - 1)) ? (maxY - 1) : y1));
    x2 = ((x2 < 0) ? 0 : ((x2 > (maxX - 1)) ? (maxX - 1) : x2));
    y2 = ((y2 < 0) ? 0 : ((y2 > (maxY - 1)) ? (maxY - 1) : y2));

    if (filled)
    {
        _hPen = _gdi_set_pencol(colour);
        _hBrush = _gdi_set_brushcol(colour);
        Ellipse(mdc, x1, y1, x2, y2);
        _gdi_clr_pencol(_hPen);
        _gdi_clr_brushcol(_hBrush);
    }
    else
    {
        _hPen = _gdi_set_pencol(colour);
        Ellipse(mdc, x1, y1, x2, y2);
        _gdi_clr_pencol(_hPen);
    }
}

void gdi_circle(int x, int y, int r, int colour, int filled)
{
    gdi_ellipse(x - r, y - r, x + r, y + r, colour, filled);
}

void gdi_textout(int x, int y, int size, int colour, int mode, PTCHAR text)
{
    HFONT hFont;
    int cWeight = FW_NORMAL;
    DWORD bItalic = FALSE;
    DWORD bUnderline = FALSE;

    x = ((x < 0) ? 0 : ((x > (maxX - 1)) ? (maxX - 1) : x));
    y = ((y < 0) ? 0 : ((y > (maxY - 1)) ? (maxY - 1) : y));

    if (mode != GDI_TMODE_NULL)
    {
        if (mode & GDI_TMODE_BOLD)
        {
            cWeight = FW_BOLD;
        }
        if (mode & GDI_TMODE_ITALIC)
        {
            bItalic = TRUE;
        }
        if (mode & GDI_TMODE_UNDLINE)
        {
            bUnderline = TRUE;
        }
    } 

    //创建字体
    hFont = CreateFont(size, 0, 0, 0, cWeight, bItalic, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("微软雅黑"));

    //选择字体
    SelectObject(mdc, hFont);

    SetTextColor(mdc, colour);//设置字体颜色
    //SetBkColor(mdc, GDI_RGB_BACKGROUND);//设置字体背景色
    SetBkMode(mdc, TRANSPARENT);//透明背景
    SetTextAlign(mdc, TA_LEFT);//左对齐

    TextOut(mdc, x, y, text, lstrlen(text));

    DeleteObject(hFont);//删除字体
}

int gdi_init(HWND hWnd)
{
    hGdiWnd = hWnd;
    hdc = GetDC(hGdiWnd);
    mdc = CreateCompatibleDC(hdc);
    GetClientRect(hGdiWnd, &hGdiWndRect);

    int hGdiWndWidth = hGdiWndRect.right - hGdiWndRect.left;
    int hGdiWndHeight = hGdiWndRect.bottom - hGdiWndRect.top;

    mGdiBmp = CreateCompatibleBitmap(hdc, hGdiWndWidth, hGdiWndHeight);
    mGdiBmpOld = static_cast<HBITMAP>(SelectObject(mdc, mGdiBmp));

    return OK;
}

int gdi_dinit(void)
{
    DeleteObject(mGdiBmp);
    SelectObject(mdc, mGdiBmpOld); // 恢复原始位图
    DeleteDC(mdc);
    ReleaseDC(hGdiWnd, hdc);

    if (hBackgroundBmp)
    {
        DeleteObject(hBackgroundBmp);
    }

    if (hStartScreenBmp)
    {
        DeleteObject(hStartScreenBmp);
    }

    return OK;
}

int gdi_update(void)
{
    int hGdiWndWidth = 0;//窗口客户区宽度
    int hGdiWndHeight = 0;//窗口客户区高度
    hGdiWndWidth = hGdiWndRect.right - hGdiWndRect.left;
    hGdiWndHeight = hGdiWndRect.bottom - hGdiWndRect.top;
    //把软件设备上的内容拷贝到硬件设备上
    BitBlt(hdc, 0, 0, hGdiWndWidth, hGdiWndHeight, mdc, 0, 0, SRCCOPY);

    DEBUG_LOG("hGdiWndWidth[%d], hGdiWndHeight[%d]\r\n", hGdiWndWidth, hGdiWndHeight);

    return OK;
}

void gdi_set_background_image(const TCHAR* imagePath)
{
    if (hBackgroundBmp)
    {
        DeleteObject(hBackgroundBmp);
    }

    hBackgroundBmp = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void gdi_set_start_screen_image(const TCHAR* imagePath)
{
    if (hStartScreenBmp)
    {
        DeleteObject(hStartScreenBmp);
    }

    hStartScreenBmp = (HBITMAP)LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void gdi_show_start_screen(void)
{
    gdi_clear(GDI_RGB_BACKGROUND);
    gdi_textout(50, 50, 24, GDI_RGB_RED, GDI_TMODE_BOLD, TEXT("Press any key to start..."));
    gdi_update();
}