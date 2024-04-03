#ifndef	__VFFUNCTION_H__
#define __VFFUNCTION_H__

#include "VF_Type.h"
#include "VF_Global.h"

//
//	IsFork: �ж�ĳ���Ƿ�Ϊ���
//
bool   IsFork(uint8 *lpNow)
{
/////////////////////////////////////////////////////////////////////////////
//	lpNow : [in] ��ǰ��ĵ�ַ
/////////////////////////////////////////////////////////////////////////////
	sint32 i, sum;
	// ĳ����Χ8����ĵ�ַƫ��
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	// 8������������������Ĳ�ľ���ֵ�ĺ����Ϊ6*255��Ϊ���
	sum = 0;
	for(i = 0; i < 8; i++)
	{
		sum += abs(*(lpNow + SiteD8[(i+1)%8]) - *(lpNow + SiteD8[i]));
	}
	if(sum == 255*6)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//
//	IsEnd: �ж�ĳ���Ƿ�Ϊ�˵�
//
bool   IsEnd(uint8 *lpNow)
{
/////////////////////////////////////////////////////////////////////////////
//	lpNow : [in] ��ǰ��ĵ�ַ
/////////////////////////////////////////////////////////////////////////////
	sint32 i, sum;
	// ĳ����Χ8����ĵ�ַƫ��
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	// 8������������������Ĳ�ľ���ֵ�ĺ����Ϊ2*255��Ϊ�˵�
	sum = 0;
	for(i = 0; i < 8; i++)
	{
		sum += abs(*(lpNow + SiteD8[(i+1)%8]) - *(lpNow + SiteD8[i]));
	}
	if(sum == 255*2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//
//	GetNext: �������ϸ��ݵ�ǰ���ǰ�����ַ�ҵ���һ����ĵ�ַ
//
sint32  GetNext(uint8 *lpNow, uint8 *lpLast, uint8 **lppNext)
{
/////////////////////////////////////////////////////////////////////////////
//	lpNow : [in] ��ǰ��ĵ�ַ
//	lpLast : [in] ǰ����ĵ�ַ
//	lppNext : [out] ��һ����ĵ�ַ��ָ��
/////////////////////////////////////////////////////////////////////////////
	sint32  i;
	uint8   *lpSrc = NULL;
	sint32  n;
	uint8   *Temp[8];	// ĳ����Χ8����ĵ�ַ
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	*lppNext = NULL;
	
	// �ҵ���ǰ����Χ���Ǻڵ㲢�Ҳ���ǰ����ĺڵ㣬���浽����
	n = 0;
	for(i = 0; i < 8; i++)
	{
		lpSrc = lpNow + SiteD8[i];
		if(*lpSrc == 0 && lpSrc != lpLast)
		{
			Temp[n] = lpSrc;
			n++;
		}
	}
	
	if(n == 0)	// û���ҵ������ڵ����ʾû�к�̵�
	{
		*lppNext = NULL;
		return 1;
	}
	else if(n == 1) // �ҵ���һ��������Ϊ��̵�
	{
		*lppNext = Temp[0];
		return 0;
	}
	else // �ҵ�������򷵻ش���
	{
		*lppNext = NULL;
		return 1;
	}
	
	return 0;
}

//
//	GetByDis: ���������ҵ����뵱ǰ�˵�Ϊd�ĵ�ĵ�ַ
//
sint32  GetByDis(uint8 *lpEnd, uint8 **lppPos, sint32 d)
{
/////////////////////////////////////////////////////////////////////////////
//	lpEnd : [in] ��ǰ�˵�ĵ�ַ
//	lppPos : [out] Ŀ�ĵ�ĵ�ַ(����ھ���d���ж˵���߲�㣬�򷵻����ǵ��ڵ�)
//	d : [in] ���루������
/////////////////////////////////////////////////////////////////////////////
	sint32  i, n;
	uint8   *lpNow = NULL;
	uint8   *lpLast = NULL;
	uint8   *lpNext = NULL;
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	*lppPos = NULL;
	
	// �ҵ���ǰ����Χ���Ǻڵ㲢�Ҳ���ǰ����ĺڵ㣬���浽����
	n = 0;
	for(i = 0; i < 8; i++)
	{
		if(*(lpEnd + SiteD8[i]) == 0)
		{
			lpNow = lpEnd + SiteD8[i];
			n++;
		}
	}
	if(n != 1) // ��Ŀ�������1�򷵻ش���
	{
		return 1;
	}
	// �ҵ��ĺڵ��ַΪlpNow
	// �˵���Ϊǰ����,��ַ���浽lpLast
	lpLast = lpEnd; 
	// ѭ����d�κ�̵�
	for(i = 0; i < d; i++)
	{
		// ����ǲ��������ѭ��
		if(IsFork(lpNow))
		{
			break;
		}
		// �ɹ��ҵ���һ��������ĵ�ǰ���ǰ����
		if(GetNext(lpNow, lpLast, &lpNext) == 0)
		{
			lpLast = lpNow;
			lpNow = lpNext;
		}
		else
		{
			break;
		}

	}
	// ����ҵ��ĵ�Ϊ��㣬�򷵻���ǰ����
	if(IsFork(lpNow))
	{
		*lppPos = lpLast;
	}	
	else	// ���򷵻ص�ǰ��
	{
		*lppPos = lpNow;
	}

	return 0;
}

//
//	PX: ���ݵ�ַ�õ���ǰ��ĺ�����
//
sint32  PX(uint8 *lpPos)
{
/////////////////////////////////////////////////////////////////////////
//	lpPos: [in] ��ǰ���ַ
/////////////////////////////////////////////////////////////////////////
	return ((lpPos - g_lpOrgFinger) % IMGW);
}

//
//	PY: ���ݵ�ַ�õ���ǰ���������
//
sint32  PY(uint8 *lpPos)
{
/////////////////////////////////////////////////////////////////////////
//	lpPos: [in] ��ǰ���ַ
/////////////////////////////////////////////////////////////////////////
	return ((lpPos - g_lpOrgFinger) / IMGW);
}

//
//	GetJiajiao: �������Ƕȵļн�(0 - 90)
//
sint32  GetJiajiao(sint32 angle1, sint32 angle2)
{
/////////////////////////////////////////////////////////////////////////
//	angle1: [in] �Ƕ�һ ��0 - 180��
//	angle2: [in] �Ƕȶ� ��0 - 180��
/////////////////////////////////////////////////////////////////////////
	sint32  a;

	a = abs(angle1-angle2);
	if(a > 90)
	{
		a = 180-a;
	}
	return a;
}

//
//	AngleAbs360: �������Ƕȵļн�(0 - 180)
//
sint32  AngleAbs360(sint32 angle1, sint32 angle2)
{
/////////////////////////////////////////////////////////////////////////
//	angle1: [in] �Ƕ�һ ��0 - 360��
//	angle2: [in] �Ƕȶ� ��0 - 360��
/////////////////////////////////////////////////////////////////////////
	sint32 a;
	a = abs(angle1 - angle2);
	if(a > 180)
	{
		return (360 - a);
	}
	else
	{
		return a;
	}
}

//
//	GetAngleDis�� �����Ƕȼ�ľ���(0-360)  (��ʱ�뷽��)
//
sint32  GetAngleDis(sint32 angleBegin, sint32 angleEnd)
{
/////////////////////////////////////////////////////////////////////////
//	angleBegin: [in] ��ʼ�Ƕ� ��0 - 360��
//	angleEnd: [in] �ս�Ƕ� ��0 - 360��
/////////////////////////////////////////////////////////////////////////
	sint32  a;

	a = angleEnd-angleBegin;
	if(a < 0)
	{
		a += 360;
	}
	return a;
}

//
//	GetAngle�� ������������x�᷽��ĽǶ�
//
sint32  GetAngle(sint32 x0, sint32 y0, sint32 x1, sint32 y1)
{
/////////////////////////////////////////////////////////////////////////
//	x0: [in] ��һ���������
//	y0: [in] ��һ����������
//	x1: [in] �ڶ����������
//	y1: [in] �ڶ�����������
/////////////////////////////////////////////////////////////////////////
	double angle;

	angle = atan2((double)(y1-y0), (double)(x1-x0));
	// ����ת����0 - 2*PI
	if(angle < 0)
	{
		angle += 2*PI;
	}
	// ����ת��Ϊ�Ƕ�
	return (sint32)(angle*EPI + 0.5);
}

//
//	DIndex: �ԽǶȽ����������õ�������������(0-12)
//
sint32  DIndex(sint32 angle)
{
/////////////////////////////////////////////////////////////////////////
//	angle: [in] �Ƕ� ��0 - 180��
/////////////////////////////////////////////////////////////////////////
	if(angle >= 173 || angle < 8)
	{
		return 0;
	}
	else
	{
		return ((angle-8)/15 + 1);
	}
}


#endif