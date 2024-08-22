#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include "resource.h"
#include "Tetris.h"
#include "Gdi.h"


#define GAME_SCREEN_L         (SCREEN_X)//��Ļ����
#define GAME_SCREEN_H         (SCREEN_Y)//��Ļ���

#define GAME_POINT_SIZE       (MAX_PNT_SIZE)//���С����λ:���أ���Ϊ������
#define GAME_POINT_L          (GAME_POINT_SIZE)
#define GAME_POINT_H          (GAME_POINT_SIZE)

#define GAME_MAX_X            ((GAME_SCREEN_L) / (GAME_POINT_L))//�ƶ�����
#define GAME_MAX_Y            ((GAME_SCREEN_H) / (GAME_POINT_H))//�ƶ�����

#define MAX_BSC_SHAPE_CNT     7//7�ֻ�����״
#define MAX_EXT_SHAPE_CNT     4//4�ֱ任��״
#define MAX_SHAPE_CNT         (MAX_BSC_SHAPE_CNT*MAX_EXT_SHAPE_CNT)//�ܹ���״����
#define SHAPE_PNT_CNT         4//ÿ����״���ĸ��㹹��

#define GAME_MAX_SPEED        10//�����Ϸ�ٶ�
#define GAME_MIN_SPEED        1 //��С��Ϸ�ٶ�
#define GAME_SPEED_STEP       1//ÿ��10����Ϸ�ٶ�+1

typedef struct game_point_s //��������
{
    int       x;
    int       y;
    int       col;
} game_point_t;

typedef struct game_shape_s //������״
{
    int           index;
    game_point_t    point;
} game_shape_t;

typedef enum game_property_e    //������״������
{
    PR_MIN = 0,
    PR_NULL = 0,
    PR_III, 
    PR_JJJ, 
    PR_LLL, 
    PR_OOO, 
    PR_SSS, 
    PR_TTT, 
    PR_ZZZ, 
    PR_MAX
} game_property_t; 

typedef struct warmap_s
{
    int col;
    int pr;
}game_map_t;

game_shape_t        glGameShape; 
int              glGameSCore;   //�÷�
int              glGameSpeed;   //�ٶ�
tetris_life_t       glGameLife;     //����ֵ

/*����˹���鹲��7�ֻ�����״��ÿ�ֻ�����״����ת4�Σ�ÿ����״
��4��С������ɡ���������ÿ�ֻ�����״����һ��4x4�ľ����
��һ������ȥ��¼ÿ��С��������ھ������Ͻǵ�λ��*/
game_point_t GAME_SHAPE_BOX[MAX_SHAPE_CNT][SHAPE_PNT_CNT] =
{
    //I///
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 2, 0, TRUE }, { 3, 0, TRUE } },//I0
    { { 0, 0, TRUE }, { 0, 1, TRUE }, { 0, 2, TRUE }, { 0, 3, TRUE } },//I1
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 2, 0, TRUE }, { 3, 0, TRUE } },//I2
    { { 0, 0, TRUE }, { 0, 1, TRUE }, { 0, 2, TRUE }, { 0, 3, TRUE } }, //I3
    ///J///
    { { 0, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 2, 1, TRUE } },//J0
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 0, 1, TRUE }, { 0, 2, TRUE } },//J1
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 2, 0, TRUE }, { 2, 1, TRUE } },//J2
    { { 1, 0, TRUE }, { 1, 1, TRUE }, { 0, 2, TRUE }, { 1, 2, TRUE } }, //J3
    ///L///
    { { 2, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 2, 1, TRUE } },//L0
    { { 0, 0, TRUE }, { 0, 1, TRUE }, { 0, 2, TRUE }, { 1, 2, TRUE } },//L1
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 2, 0, TRUE }, { 0, 1, TRUE } },//L2
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 1, 1, TRUE }, { 1, 2, TRUE } }, //L3
    ///O///
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE } },//O0
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE } },//O1
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE } },//O2
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE } }, //O3
    ///S
    { { 1, 0, TRUE }, { 2, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE } },//S0
    { { 0, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 1, 2, TRUE } },//S1
    { { 1, 0, TRUE }, { 2, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE } },//S2
    { { 0, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 1, 2, TRUE } }, //S3
    ///T
    { { 1, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 2, 1, TRUE } },//T0
    { { 0, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 0, 2, TRUE } },//T1
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 2, 0, TRUE }, { 1, 1, TRUE } },//T2
    { { 1, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 1, 2, TRUE } }, //T3
    ///Z
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 1, 1, TRUE }, { 2, 1, TRUE } },//Z0
    { { 1, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 0, 2, TRUE } },//Z1
    { { 0, 0, TRUE }, { 1, 0, TRUE }, { 1, 1, TRUE }, { 2, 1, TRUE } },//Z2
    { { 1, 0, TRUE }, { 0, 1, TRUE }, { 1, 1, TRUE }, { 0, 2, TRUE } }  //Z3

};

int GAME_SHAPE_SIZE[MAX_SHAPE_CNT][2] =
{
    {4, 1}, {1, 4}, {4, 1}, {1, 4}, //I
    {3, 2}, {2, 3}, {3, 2}, {2, 3}, //J
    {3, 2}, {2, 3}, {3, 2}, {2, 3}, //L
    {2, 2}, {2, 2}, {2, 2}, {2, 2}, //O
    {3, 2}, {2, 3}, {3, 2}, {2, 3}, //S
    {3, 2}, {2, 3}, {3, 2}, {2, 3}, //T
    {3, 2}, {2, 3}, {3, 2}, {2, 3}  //Z
};

const int TETRIS_PR_COLOUR[PR_MAX] =    //�������ɫ
{
    GDI_RGB_BACKGROUND,     //PR_NULL = 0,      //��ɫ
    GDI_RGB_FOREGROUND,     //PR_III            //��ɫ
    GDI_RGB_PURPLE,         //PR_JJJ            //��ɫ
    GDI_RGB_GREEN,          //PR_LLL            //��ɫ
    GDI_RGB_YELLOW,         //PR_OOO            //��ɫ
    GDI_RGB_RED,            //PR_SSS            //��ɫ
    GDI_RGB_BLUE,           //PR_TTT            //��ɫ
    GDI_RGB_CYAN            //PR_ZZZ            //��ɫ
};

static game_map_t TETRIS_GAME_MAP[GAME_MAX_X][GAME_MAX_Y] = { 0 };//��Ϸ��ͼ,�൱����ʾ����

void game_debug_out(void)
{
#define DEBUG_TEXT_SIZE       (20)
    int kill = 0, life = 0, enemy = 0, bomb = 0, super = 0;
    TCHAR debugStr[200] = { 0 };
    PTCHAR lifeStr = NULL;

    lifeStr = (tetris_get_life() != LF_LIVE) ? (TEXT("����")) : (TEXT("����"));
    wsprintf(debugStr, TEXT("״̬[%s], �ٶ�[%02d], ����[%03d]"),
        lifeStr, tetris_get_speed(), tetris_get_score());

    gdi_textout(0, GAME_SCREEN_H - DEBUG_TEXT_SIZE - 1, DEBUG_TEXT_SIZE, GDI_RGB_FOREGROUND, 0, debugStr);
    gdi_update();
}

#if 0
int game_draw_point(const game_point_t* const s_point)//ʹ�����Ź����DPI����һ�������,�ɹ�����0��ʧ�ܷ���1
{
    int x = 0, y = 0, col = 0;
#ifdef GAME_MIRROR_XY
    int yt = 0;
#endif

    if (s_point == RTN_NULL)
        return RTN_ERR;

    x = s_point->x;
    y = s_point->y;
    col = s_point->col;

    if ((x < 0) || (x >= GAME_MAX_X) ||
        (y < 0) || (y >= GAME_MAX_Y))
    {
        return RTN_ERR;
    }

#ifdef GAME_MIRROR_XY
    yt = y;
    y = x * GAME_POINT_L;
    x = yt * GAME_POINT_H;

    x = ((GAME_MAX_Y - 1) - x);
#else
    x = x * GAME_POINT_L;
    y = y * GAME_POINT_H;
#endif

    if (GAME_POINT_SIZE != 1) //��һ�����δ������Ź���ĵ�
        gdi_rectangle(x, y, x + GAME_POINT_L - 1, y + GAME_POINT_H - 1, col, 1);
    else//Ϊ�˼ӿ��ٶȣ����Ϊ1�����ţ�ֱ�ӻ���
        gdi_set_point(x, y, col);

    return RTN_OK;
}

int game_get_point(const game_point_t* const s_point)//��ȡ��Ļ��ĳ�������ɫ
{
    int x = 0, y = 0;
#ifdef GAME_MIRROR_XY
    int yt = 0;
#endif

    if (s_point == RTN_NULL)
        return RTN_ERR;

    x = s_point->x;
    y = s_point->y;

    if ((x < 0) || (x >= GAME_MAX_X) ||
        (y < 0) || (y >= GAME_MAX_Y))
    {
        return FALSE;
    }

#ifdef GAME_MIRROR_XY
    yt = y;
    y = x * GAME_POINT_L;
    x = yt * GAME_POINT_H;

    x = ((GAME_MAX_Y - 1) - x);
#else
    x = x * GAME_POINT_L;
    y = y * GAME_POINT_H;
#endif

    return((gdi_get_point(x, y)) ? TRUE : FALSE);
}

void game_clear_screen(int col)//ָ����ɫ�����Ļ
{
    gdi_clear(col);
}

#else

//��Ϸ��ͼϵͳ

//����GDI����һ����״
int map_draw_point(int x, int y, int col, int pr)
{

    if ((x < 0) || (x >= GAME_MAX_X) ||
        (y < 0) || (y >= GAME_MAX_Y))
    {
        return RTN_ERR;
    }

    x = x * GAME_POINT_L;
    y = y * GAME_POINT_H;

    pr = ((pr >= PR_MAX) ? PR_NULL : ((pr <= PR_NULL) ? PR_NULL : pr));

    if (col)
    {
        col = TETRIS_PR_COLOUR[pr];
    }
    else
    {
        col = TETRIS_PR_COLOUR[PR_NULL];
    }

    if (GAME_POINT_SIZE != 1) //��һ�����δ������Ź���ĵ�
    {
        gdi_rectangle(x, y, x + GAME_POINT_L - 1, y + GAME_POINT_H - 1, col, 1);
    }
    else//Ϊ�˼ӿ��ٶȣ����Ϊ1�����ţ�ֱ�ӻ���
    {
        gdi_set_point(x, y, col);
    }

    return RTN_OK;
}

int game_draw_point(const game_point_t* const s_point)//����ͼ�����һ����״���ɹ�����0��ʧ�ܷ���1
{
    //pr�������ڿ�����ʾ����״
    int pr = PR_NULL;
    int col = FALSE;
    int x = 0, y = 0;

    if (s_point == NULL)
        return RTN_ERR;

    x = s_point->x;
    y = s_point->y;

    if ((x < 0) || (x >= GAME_MAX_X) || (y < 0) || (y >= GAME_MAX_Y))
    {
        return RTN_ERR;
    }

    if (s_point->col != FALSE)
    {
        col = TRUE;
        pr = s_point->col;
    }
    else
    {
        col = FALSE;
        pr = PR_NULL;
    }

    pr = ((pr >= PR_MAX) ? PR_NULL : ((pr <= PR_NULL) ? PR_NULL : pr));

    TETRIS_GAME_MAP[x][y].col = col;
    TETRIS_GAME_MAP[x][y].pr = pr;

    return RTN_OK;
}

int game_get_point(const game_point_t* const s_point)//��ȡ��ͼ��ĳ�������ɫ������ָ���ĵ����ɫ(1��0)
{
    int x = 0, y = 0;
    int col = 0, pr = 0;
    int ret = PR_NULL;

    if (s_point == RTN_NULL)
    {
        return RTN_ERR;
    }

    x = s_point->x;
    y = s_point->y;

    if ((x < 0) || (x >= GAME_MAX_X) ||
        (y < 0) || (y >= GAME_MAX_Y))
    {
        return FALSE;
    }

    return TETRIS_GAME_MAP[x][y].col;
}

int game_get_pntpr(const game_point_t* const s_point)
{
    int x = 0, y = 0;
    int col = 0, pr = 0;
    int ret = PR_NULL;

    if (s_point == RTN_NULL)
    {
        return RTN_ERR;
    }

    x = s_point->x;
    y = s_point->y;

    if ((x < 0) || (x >= GAME_MAX_X) ||
        (y < 0) || (y >= GAME_MAX_Y))
    {
        return PR_NULL;
    }

    return TETRIS_GAME_MAP[x][y].pr;
}

void game_clear_screen(int col)//ָ����ɫ�����Ļ
{
    int x = 0, y = 0;

    for (x = 0; x < GAME_MAX_X; x++)
    {
        for (y = 0; y < GAME_MAX_Y; y++)
        {
            TETRIS_GAME_MAP[x][y].col = col;
            TETRIS_GAME_MAP[x][y].pr = PR_NULL;
        }
    }
}

//�ѵ�ͼ���Ƶ���Ļ������
int game_update_screen(void)
{
    int x = 0, y = 0;

    gdi_clear(TETRIS_PR_COLOUR[PR_NULL]);

    for (x = 0; x < GAME_MAX_X; x++)
    {
        for (y = 0; y < GAME_MAX_Y; y++)
        {
            if(TETRIS_GAME_MAP[x][y].col) map_draw_point(x, y, TETRIS_GAME_MAP[x][y].col, TETRIS_GAME_MAP[x][y].pr);
        }
    }

    gdi_update();
    return RTN_OK;
}
#endif

//����Ա�
int game_point_cmp(game_point_t* s1, game_point_t* s2)
{
    if ((s1->x == s2->x) && (s1->y == s2->y) && (s1->col == s2->col))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//�������ϵͳ

//��ʾһ��shape
int game_show_shpe(game_shape_t* _shape)
{
    int i = 0;
    game_point_t shape_pnt;

    for (i = 0; i < SHAPE_PNT_CNT; i++)
    {
        shape_pnt.x = (GAME_SHAPE_BOX[_shape->index][i]).x + (_shape->point).x;
        shape_pnt.y = (GAME_SHAPE_BOX[_shape->index][i]).y + (_shape->point).y;
        shape_pnt.col = (((_shape->index) / MAX_EXT_SHAPE_CNT) + 1);

        if ((shape_pnt.x < 0) || (shape_pnt.x >= GAME_MAX_X) ||
            (shape_pnt.y < 0) || (shape_pnt.y >= GAME_MAX_Y))
        {
            continue;
        }

        if (game_draw_point(&shape_pnt) != RTN_OK)
        {
            continue;
        }
    }

    return RTN_OK;
}

//���ָ����shape
int game_clear_shpe(game_shape_t* _shape)
{
    int i = 0;
    game_point_t shape_pnt;

    for (i = 0; i < SHAPE_PNT_CNT; i++)
    {
        shape_pnt.x = (GAME_SHAPE_BOX[_shape->index][i]).x + (_shape->point).x;
        shape_pnt.y = (GAME_SHAPE_BOX[_shape->index][i]).y + (_shape->point).y;
        shape_pnt.col = FALSE;

        if ((shape_pnt.x < 0) || (shape_pnt.x >= GAME_MAX_X) ||
            (shape_pnt.y < 0) || (shape_pnt.y >= GAME_MAX_Y))
        {
            continue;
        }

        if (game_draw_point(&shape_pnt) != RTN_OK)
        {
            continue;
        }
    }

    return RTN_OK;
}

//���ָ����
int game_clear_line(int row)
{
    int x = 0;
    game_point_t pnt;

    if ((row < 0) || (row >= GAME_MAX_Y))
    {
        return RTN_ERR;
    }

    pnt.y = row;
    pnt.col = FALSE;

    for (x = 0; x < GAME_MAX_X; x++)
    {
        pnt.x = x;

        if (game_draw_point(&pnt) != RTN_OK)
        {
            continue;
        }
    }

    return RTN_OK;
}

//���һ������Ļ�����״(��shape box�е�����)
int game_get_rshape_idx(void)
{
    static uint32 sseed = 0;
    int index = 0;
    srand(sseed);
    index = rand() % (MAX_SHAPE_CNT - 1); //0-27
    sseed = index;
    return index;
}

//�ж�ĳһ���ǲ����Ѿ�����,TRUE:����, FALSE:δ��
int game_line_isfull(int row)
{
    int x = 0, row_cnt = 0;
    game_point_t pnt;

    if ((row < 0) || (row >= GAME_MAX_Y))
    {
        return FALSE;
    }

    pnt.y = row;
    pnt.col = TRUE;

    for (x = 0; x < GAME_MAX_X; x++)
    {
        pnt.x = x;

        if (game_get_point(&pnt) == TRUE)
            row_cnt++;
        else
            break;
    }

    DEBUG_LOG("");
    return ((row_cnt == GAME_MAX_X) ? TRUE : FALSE);
}

//�ж�һ�����Ƿ���shape��,TRUE:��, FALSE:����
int game_pnt_is_shape(game_point_t* pnt, game_shape_t* shape)
{
    int i = 0;
    game_point_t pnt_t = { 0 };

    for (i = 0; i < SHAPE_PNT_CNT; i++)
    {
        pnt_t.x = (shape->point).x + (GAME_SHAPE_BOX[shape->index][i]).x;
        pnt_t.y = (shape->point).y + (GAME_SHAPE_BOX[shape->index][i]).y;
        pnt_t.col = (GAME_SHAPE_BOX[shape->index][i]).col;

        if (game_point_cmp(pnt, &pnt_t) == TRUE)
        {
            return TRUE;
        }
    }

    return FALSE;
}

//��õ�ǰ���ѵ��߶�,���ʧ��,�򷵻�RTN_ERR
int game_get_maxh_stack(void)
{
    game_point_t pnt;
    pnt.col = TRUE;

    for (pnt.y = 0; pnt.y < GAME_MAX_Y; pnt.y++)
    {
        for (pnt.x = 0; pnt.x < GAME_MAX_X; pnt.x++)
        {
            //�������,�����ߵ��Ǹ��㲻Ϊ��,���������ǵ�ǰ���߶�
            if (game_get_point(&pnt) == TRUE)
            {
                return (GAME_MAX_Y - 1 - pnt.y);
            }
        }
    }

    return RTN_ERR;
}

//��ȡshape�Ĵ�С
int game_get_shape_size(game_shape_t* shape, int flag)
{
    int idx = ((flag == 0) ? 0 : 1);

    if ((shape == NULL) || (shape->index >= MAX_SHAPE_CNT))
    {
        return RTN_ERR;
    }

    return(GAME_SHAPE_SIZE[shape->index][idx]);
}

#if 0
//�ж�ĳһ�����Ƿ��ܹ�����,RTN_OK:������,RTN_ERR:��������
int game_point_can_down(game_point_t* pnt)
{
    game_point_t pnt_t;

    if ((pnt == NULL) ||
        (pnt->x < 0) || (pnt->x >= (GAME_MAX_X - 1)) ||
        (pnt->y < 0) || (pnt->y >= (GAME_MAX_Y - 1)))
    {
        return RTN_ERR;
    }


    pnt_t.x = pnt->x;
    pnt_t.y = pnt->y + 1;
    return ((game_get_point(&pnt_t) == FALSE) ? RTN_OK : RTN_ERR);
}
#endif

//�ж�ָ����shape�Ƿ��ܹ�����,����,�����ƶ�,TRUE:�����ƶ�, FALSE:�������ƶ�
int game_shape_can_move(game_shape_t* shape, tetris_dir_t dir)
{
    int i = 0, dx = 0, dy = 0;
    game_point_t pnt_t = { 0 };

    DEBUG_LOG("");

    switch (dir)
    {
    case DR_DOWN:
        dx = 0;
        dy = 1;
        break;

    case DR_LEFT:
        dx = -1;
        dy = 0;
        break;

    case DR_RIGHT:
        dx = 1;
        dy = 0;
        break;

    default:
        return FALSE;
    }

    for (i = 0; i < SHAPE_PNT_CNT; i++)
    {
        //����shape�е��ĸ���
        //shape����һ����
        pnt_t.x = (shape->point).x + (GAME_SHAPE_BOX[shape->index][i]).x + dx;
        pnt_t.y = (shape->point).y + (GAME_SHAPE_BOX[shape->index][i]).y + dy;
        pnt_t.col = (shape->point).col;

        if ((pnt_t.x < 0) || (pnt_t.x >= GAME_MAX_X) ||
            (pnt_t.y < 0) || (pnt_t.y >= GAME_MAX_Y))
        {
            return FALSE;
        }

        //���shape��ĳ�������һ���㲻�ǿյ�,���������û��shape��,
        //������Ѿ�û������Ŀռ�,�����������
        if ((game_get_point(&pnt_t) != FALSE) &&
            (game_pnt_is_shape(&pnt_t, shape) != TRUE))
        {
            DEBUG_LOG("pnt_t,i=%d, x=%d,y=%d,col=%d\n", i, pnt_t.x, pnt_t.y, pnt_t.col);
            return FALSE;
        }
    }

    DEBUG_LOG("");

    return TRUE;
}

//�ж�ȫ��shape�Ƿ��ܹ���������,�������ֱ����ʾ,��������������
int game_shape_down(game_shape_t* _shape)
{
    if (_shape == NULL)
    {
        return RTN_ERR;
    }

    DEBUG_LOG("idx=%d,x=%d,y=%d,col=%d\n",
        _shape->index, _shape->point.x,
        _shape->point.y, _shape->point.col);

    if (game_shape_can_move(_shape, DR_DOWN) != TRUE)
    {
        //�Ѿ�û������ռ�,ֱ����ʾ
        DEBUG_LOG("game_shape_down FALSE\n");

        game_show_shpe(_shape);//��ʾ
        return RTN_ERR;
    }
    else
    {
        //��������ռ�,�����������ʾ
        game_clear_shpe(_shape);
        _shape->point.y += 1;//��������
        game_show_shpe(_shape);
    }

    DEBUG_LOG("idx=%d,x=%d,y=%d,col=%d\n",
        _shape->index, _shape->point.x,
        _shape->point.y, _shape->point.col);

    return RTN_OK;
}

//��������һ��ȫ��shape
int game_shape_init(void)
{
    glGameShape.point.x = ((GAME_MAX_X - 1) / 2);
    glGameShape.point.y = 0;
    glGameShape.point.col = TRUE;
    glGameShape.index = game_get_rshape_idx();

    return RTN_OK;
}

//��Ϸ��ʼ��
int tetris_game_init(void)
{
    game_clear_screen(FALSE);
    glGameSCore = 0;
    glGameSpeed = 5;
    glGameLife = LF_LIVE;
    game_shape_init();
    game_show_shpe(&glGameShape);
    game_update_screen();
    return RTN_OK;
}


//��Ϸ��ʼ����,����������,����,����
int tetris_game_run(int debug)
{
    //shapeһֱ�½�,ֱ���޷��½�Ϊֹ
    int x = 0, y = 0, y1 = 0;
    int max_stackh = 0;
    int max_y = 0;
    game_point_t pnt;
    int oldcol = FALSE;

    if (game_shape_down(&glGameShape) != RTN_OK)
    {
        //��������������,�������,������һ���µ�shape,���¿�ʼ
        max_stackh = game_get_maxh_stack();
        DEBUG_LOG("max_stackh=%d\n", max_stackh);

        if (max_stackh >= (GAME_MAX_Y - 1))
        {
            glGameLife = LF_DIE;
            return glGameLife;
        }

        max_y = (GAME_MAX_Y - 1) - max_stackh;

        for (y = (GAME_MAX_Y - 1); y >= max_y; y--)
        {
            //Ѱ���Ƿ�������
            if (game_line_isfull(y) == TRUE)
            {
                //�����ǰ��������,�������,������ǰ��,����������
                DEBUG_LOG("line %d isfull\n", y);
                glGameSCore++;

                if ((glGameSCore % GAME_SPEED_STEP) == 0)
                {
                    if (glGameSpeed < GAME_MAX_SPEED)
                        glGameSpeed++;
                }

                game_clear_line(y);//��������

                //�����ǰ�в��ǵ�һ��,���ϲ����е�����
                if (y > 0)
                {
                    for (y1 = y - 1; y1 >= max_y; y1--)
                    {
                        for (x = 0; x < GAME_MAX_X; x++)
                        {
                            //�ѵ�ǰ������ֵ���Ƶ���һ����
                            pnt.x = x;
                            pnt.y = y1;

                            if (game_get_point(&pnt) == TRUE)
                            {
                                //��������ɫ
                                oldcol = game_get_pntpr(&pnt);
                                //�����ǰ�����
                                pnt.col = FALSE;
                                game_draw_point(&pnt);
                                //pnt.col = TRUE;
                                pnt.col = oldcol;
                                pnt.y = y1 + 1;
                                game_draw_point(&pnt);
                            }
                        }
                    }

                    //��Ϊ�ϲ������ж�������һ��,����Ҫ���������¶�λ����ǰ���ٴν����ж�
                    y += 1;

                }
            }
        }

        game_shape_init();//���²���һ��shape

        if ((game_get_shape_size(&glGameShape, 1) - 1) >
            (GAME_MAX_Y - 1 - max_stackh))//�Ѿ�û�пռ���
        {
            glGameLife = LF_DIE;
            return glGameLife;
        }
    }

    DEBUG_LOG("glGameShape,idx=%d,x=%d,y=%d,col=%d\n\n",
        glGameShape.index, glGameShape.point.x,
        glGameShape.point.y, glGameShape.point.col);
    DEBUG_LOG("GAME_MAX_X[%d],GAME_MAX_Y[%d]\r\n", GAME_MAX_X, GAME_MAX_Y);

    game_update_screen();

    if (debug)
    {
        game_debug_out();
    }

    return glGameLife;
}

//ȫ��shape����
int tetris_shape_deform(void)
{
    int shape_idx = 0;
    if ((game_shape_can_move(&glGameShape, DR_DOWN) != TRUE))
    {
        //����Ѿ���������,���ֹ����
        return RTN_ERR;
    }

    game_clear_shpe(&glGameShape);
    shape_idx = glGameShape.index;

    if ((glGameShape.index % 4) < (MAX_EXT_SHAPE_CNT - 1))
    {
        glGameShape.index += 1;
    }
    else
    {
        glGameShape.index = ((int)(glGameShape.index / 4) * 4);
    }

    game_show_shpe(&glGameShape);

    if ((game_shape_can_move(&glGameShape, DR_DOWN) != TRUE))
    {
        //�������֮��������,�����ʧ��,��ԭ
        game_clear_shpe(&glGameShape);
        glGameShape.index = shape_idx;
        game_show_shpe(&glGameShape);
        return RTN_ERR;
    }
    return RTN_OK;
}

//ʹȫ��shape��ָ�������ƶ�
int tetris_shape_move(tetris_dir_t dir)
{
    if (game_shape_can_move(&glGameShape, dir) != TRUE)
    {
        return RTN_ERR;
    }

    switch (dir)
    {
    case DR_DOWN:
        game_clear_shpe(&glGameShape);
        glGameShape.point.y += 1;
        break;

    case DR_LEFT:
        if (glGameShape.point.x > 0)
        {
            game_clear_shpe(&glGameShape);
            glGameShape.point.x -= 1;
        }

        break;

    case DR_RIGHT:
        if (glGameShape.point.x < (GAME_MAX_X - 1))
        {
            game_clear_shpe(&glGameShape);
            glGameShape.point.x += 1;
        }

        break;

    default:
        break;
    }

    game_show_shpe(&glGameShape);
    return RTN_OK;
}

//��ȡ��Ϸ����ֵ
int tetris_get_life(void)
{
    return glGameLife;
}

//�����Ϸ�÷�
int tetris_get_score(void)
{
    return glGameSCore;
}

//�����Ϸ�ٶ�
int tetris_get_speed(void)
{
    return glGameSpeed;
}

//��Ϸ�ٶȼӿ�
int tetris_add_speed(void)
{
    if (glGameSpeed < GAME_MAX_SPEED)
        glGameSpeed++;

    return glGameSpeed;
}

//��Ϸ�ٶȼ���
int tetris_sub_speed(void)
{
    if (glGameSpeed > GAME_MIN_SPEED)
        glGameSpeed--;

    return glGameSpeed;
}
