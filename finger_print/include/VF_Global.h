#ifndef	__VFGLOBAL_H__
#define __VFGLOBAL_H__

#include "VF_Type.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "math.h"

#define		MAXIMGW				640		// ����ͼ�������
#define		MAXIMGH				480		// ����ͼ�����߶�
#define		MAXIMGSIZE			(MAXIMGW*MAXIMGH) // ����ͼ������С

sint32  	IMGW = 256;		// ��ǰ����ͼ��Ŀ��
sint32      IMGH = 360;		// ��ǰ����ͼ��ĸ߶�
//sint32      IMGSIZE = (IMGW*IMGH);	// ��ǰ����ͼ��Ĵ�С
sint32      IMGSIZE = 92160; //(IMGW*IMGH)

#define BMP_IMAGE_SIZE 	93238

#if !(CFG_ENABLE_LOWER_RAM)
uint8		g_OrgFinger[MAXIMGSIZE];	// ����ͼ�����ݻ�����
uint8		g_Orient[MAXIMGSIZE];		// �������ݻ�����
uint8		g_Divide[MAXIMGSIZE];		// �ָ�ͼ���ݻ�����
uint8		g_Temp[MAXIMGSIZE];			// ��ʱ���ݻ�����

uint8		*g_lpOrgFinger = &g_OrgFinger[0];	// ����ͼ�����ݻ������׵�ַ
uint8		*g_lpOrient = &g_Orient[0];			// �������ݻ������׵�ַ
uint8		*g_lpDivide = &g_Divide[0];			// �ָ�ͼ���ݻ������׵�ַ
uint8		*g_lpTemp = &g_Temp[0];				// ��ʱ���ݻ������׵�ַ

#else

uint8		*g_lpOrgFinger = NULL;
uint8		*g_lpOrient = NULL;
uint8		*g_lpDivide = NULL;
uint8		*g_lpTemp = NULL;

#endif

#define     MAXMINUTIANUM		72
// �����������
#define     MAXRAWMINUTIANUM        100			// �������������ĳ�ʼ�����������Ŀ

sint32      g_MinutiaNum = 0;					// ��������֮��

#define  PI          3.141593
#define  EPI         57.29578

// ��ĳ��Ϊ���ĵ�12������������7����λ�õĺ���ƫ��
static const sint32  g_DSite[12][7][2] = {
	-3, 0,  -2, 0,  -1, 0,   0, 0,   1, 0,   2, 0,   3, 0,
	-3,-1,  -2,-1,  -1, 0,   0, 0,   1, 0,   2, 1,   3, 1,
	-3,-2,  -2,-1,  -1,-1,   0, 0,   1, 1,   2, 1,   3, 2,
	-3,-3,  -2,-2,  -1,-1,   0, 0,   1, 1,   2, 2,   3, 3,
	-2,-3,  -1,-2,  -1,-1,   0, 0,   1, 1,   1, 2,   2, 3,
	-1,-3,  -1,-2,   0,-1,   0, 0,   0, 1,   1, 2,   1, 3,
	 0,-3,   0,-2,   0,-1,   0, 0,   0, 1,   0, 2,   0, 3,
	-1, 3,  -1, 2,   0, 1,   0, 0,   0,-1,   1,-2,   1,-3,
	-2, 3,  -1, 2,  -1, 1,   0, 0,   1,-1,   1,-2,   2,-3,
	-3, 3,  -2, 2,  -1, 1,   0, 0,   1,-1,   2,-2,   3,-3,
	-3, 2,  -2, 1,  -1, 1,   0, 0,   1,-1,   2,-1,   3,-2,
	-3, 1,  -2, 1,  -1, 0,   0, 0,   1, 0,   2,-1,   3,-1
};

// ������ṹ
typedef struct tagMinutiae { 
  sint32    x;				// ������
  sint32    y;				// ������
  sint32    Direction;		// ����
  sint32	Triangle[3];	// ������Ϊ�������Բ�뾶Ϊ��ֵ������������������ķ���
  sint32    Type;			// ����
} MINUTIA, *MINUTIAPTR; 

// ָ������(ģ��)�ṹ
typedef struct tagFeature{
	sint32		MinutiaNum;					// ��������
	MINUTIA		MinutiaArr[MAXMINUTIANUM];	// ����������
} FEATURE, *FEATUREPTR;


FEATURE    g_Feature;		// ��ǰָ�Ƶ�ָ������

#define SAFE_FREE(x) { if (x != NULL) free((void *)x); x = NULL; }

#define WIDTHBYTES(i)    ((i+3)/4*4)

#endif