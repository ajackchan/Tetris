#ifndef __GDI_H_
#define __GDI_H_

#include <Windows.h>
#include "type.h"

// Size
#define MAX_PNT_SIZE        (15)
#define MAX_WND_WIDTH       (MAX_PNT_SIZE * 20)
#define MAX_WND_HEIGHT      (MAX_PNT_SIZE * 40)
#define	SCREEN_X		    MAX_WND_WIDTH
#define	SCREEN_Y		    MAX_WND_HEIGHT

#define GDI_RGB_RED         ((int)(RGB(255, 0, 0)))//红色
#define GDI_RGB_GREEN       ((int)(RGB(0, 255, 0)))//绿色
#define GDI_RGB_BLUE        ((int)(RGB(0, 0, 255)))//蓝色
#define GDI_RGB_YELLOW      ((int)(RGB(255, 255, 0)))//黄色
#define GDI_RGB_PURPLE      ((int)(RGB(255, 0, 255)))//紫色
#define GDI_RGB_CYAN        ((int)(RGB(0, 255, 255)))//青色
#define GDI_RGB_BLACK       ((int)(RGB(0, 0, 0)))//黑色
#define GDI_RGB_WHITE       ((int)(RGB(255, 255, 255)))//白色
#define GDI_RGB_GARY        ((int)(RGB(192, 192, 192)))//灰色

#define GDI_RGB_FOREGROUND  (GDI_RGB_WHITE)	//前景色
#define GDI_RGB_BACKGROUND  (GDI_RGB_GARY)	//背景色

#define GDI_TMODE_NULL      (0x00)
#define GDI_TMODE_BOLD      (0x01)//粗体
#define GDI_TMODE_ITALIC    (0x02)//斜体
#define GDI_TMODE_UNDLINE   (0x04)//下划线

typedef enum {
    GAME_STATE_START_SCREEN,
    GAME_STATE_PLAYING
} GameState;

extern GameState g_gameState;

extern void   gdi_get_screen_size(int *x, int *y);
extern void   gdi_set_point(int  x, int  y, int colour);
extern int  gdi_get_point(int x, int y);
extern void   gdi_line(int x0, int y0, int x1, int y1, int colour);
extern void   gdi_triangle(int x1, int y1, int x2, int y2, int colour, int filled);
extern void   gdi_rhombus(int x1, int y1, int x2, int y2, int colour, int filled);
extern void   gdi_rectangle(int x1, int y1, int x2, int y2, int colour, int filled);
extern void   gdi_ellipse(int x1, int y1, int x2, int y2, int colour, int filled);
extern void   gdi_circle(int  x, int  y, int  r, int colour, int filled);
extern void   gdi_textout(int x, int y, int size, int colour, int mode, PTCHAR text);
extern void   gdi_clear(int colour);
extern int  gdi_init(HWND hWnd);
extern int  gdi_dinit(void);
extern int  gdi_update(void);
extern void   gdi_set_background_image(const TCHAR* imagePath);
extern void   gdi_set_start_screen_image(const TCHAR* imagePath);
extern void   gdi_show_start_screen(void);



#endif
