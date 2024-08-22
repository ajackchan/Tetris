#include "Gdi.h"

HPEN		hGdiPen = NULL;	        //����
HBRUSH		hGdiBrush = NULL;	    //��ˢ
HDC			mdc;	            //�ڴ��豸(˫���弼��)
HDC         hdc;                //Ӳ���豸
HWND        hGdiWnd;                //����
RECT        hGdiWndRect;            //���ڿͻ�����С

HBITMAP     mGdiBmp;
HBITMAP     mGdiBmpOld;

HBITMAP hBackgroundBmp = NULL;
HBITMAP hStartScreenBmp = NULL;

GameState g_gameState = GAME_STATE_START_SCREEN;

#define maxX SCREEN_X
#define maxY SCREEN_Y

static void _gdi_clr_pencol(HPEN _hGdiPen)
{
    DeleteObject(_hGdiPen);//�ͷ���Դ
    SelectObject(mdc, hGdiPen);//�ָ���ʼ��ˢ
}

static HPEN _gdi_set_pencol(int color)
{
    HPEN _hGdiPen;

    COLORREF color_t = (COLORREF)color;
    _hGdiPen = CreatePen(PS_SOLID, 1, color_t);//����

    hGdiPen = static_cast<HPEN>(SelectObject(mdc, _hGdiPen));//Ϊ����DCѡ�񻭱�
    return _hGdiPen;
}

static void _gdi_clr_brushcol(HBRUSH _hGdiBrush)
{
    DeleteObject(_hGdiBrush);//�ͷ���Դ
    SelectObject(mdc, hGdiBrush);//�ָ���ʼ��ˢ
}

static HBRUSH _gdi_set_brushcol(int color)
{
    HBRUSH _hGdiBrush;

    COLORREF color_t = (COLORREF)color;
    _hGdiBrush = CreateSolidBrush(color_t);//��ˢ

    hGdiBrush = static_cast<HBRUSH>(SelectObject(mdc, _hGdiBrush));//Ϊ����DCѡ��ˢ
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

    //��������
    hFont = CreateFont(size, 0, 0, 0, cWeight, bItalic, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("΢���ź�"));

    //ѡ������
    SelectObject(mdc, hFont);

    SetTextColor(mdc, colour);//����������ɫ
    //SetBkColor(mdc, GDI_RGB_BACKGROUND);//�������屳��ɫ
    SetBkMode(mdc, TRANSPARENT);//͸������
    SetTextAlign(mdc, TA_LEFT);//�����

    TextOut(mdc, x, y, text, lstrlen(text));

    DeleteObject(hFont);//ɾ������
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
    SelectObject(mdc, mGdiBmpOld); // �ָ�ԭʼλͼ
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
    int hGdiWndWidth = 0;//���ڿͻ������
    int hGdiWndHeight = 0;//���ڿͻ����߶�
    hGdiWndWidth = hGdiWndRect.right - hGdiWndRect.left;
    hGdiWndHeight = hGdiWndRect.bottom - hGdiWndRect.top;
    //������豸�ϵ����ݿ�����Ӳ���豸��
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