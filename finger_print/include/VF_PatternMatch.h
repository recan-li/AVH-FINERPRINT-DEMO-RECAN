#ifndef	__VFPATTERNMATCH_H__
#define __VFPATTERNMATCH_H__

#include "VF_Type.h"
#include "VF_Global.h"
#include "VF_Function.h"
#include "VF_AlignMatch.h"

#if !(CFG_USE_C_ONLY)

#include <vector>
using namespace std;

#endif

#if (CFG_USE_C_ONLY)

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} Vector;

void vector_init(Vector *vec) {
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

void vector_push_back(Vector *vec, int value) {
    if (vec->size >= vec->capacity) {
        vec->capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        vec->data = (int *)realloc(vec->data, vec->capacity * sizeof(int));
    }

    vec->data[vec->size] = value;
    vec->size++;
}

int vector_at(Vector *vec, int index) {
    if (index < 0 || index >= vec->size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(1);
    }

    return vec->data[index];
}

void vector_free(Vector *vec) {
    free(vec->data);
}

int main999() {
    Vector vec;
    vector_init(&vec);

    vector_push_back(&vec, 10);
    vector_push_back(&vec, 20);
    vector_push_back(&vec, 30);

    for (int i = 0; i < vec.size; i++) {
        printf("%d\n", vector_at(&vec, i));
    }

    vector_free(&vec);

    return 0;
}

#endif

#define CENTRALRADIUS 60

void coreMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, 
				  PMATCHRESULT lpMatchResult, VF_FLAG matchMode, int n, int m)
{
	MATCHRESULT alignMax;	// ���ƶ����ıȶԽ��
	MATCHRESULT globalMatchResult;	// �ȶԽ��
	sint32  agate = 8;		// �������˽ṹ�Ƕ����
	sint32  num = 0;
	// ��ʼ����õıȶԽ��
	alignMax.Similarity = 0;
	alignMax.MMCount = 0;
	alignMax.Rotation = 0;
	alignMax.TransX = 0;
	alignMax.TransY = 0;

	FEATURE	alignFeature;	// ������ָ������

	// λ��ƫ��
	int transx = (lpTemplate->MinutiaArr[n].x - lpFeature->MinutiaArr[m].x);
	int transy = (lpTemplate->MinutiaArr[n].y - lpFeature->MinutiaArr[m].y);


	for(int i = 0; i < lpFeature->MinutiaNum; i++)
	{
		for(int j = 0; j < lpTemplate->MinutiaNum; j++)
		{
			alignFeature.MinutiaNum = 0;
									
			if (lpFeature->MinutiaArr[i].Type == VF_MINUTIA_CORE || lpTemplate->MinutiaArr[j].Type == VF_MINUTIA_CORE) continue;
			if (lpFeature->MinutiaArr[i].Type == VF_MINUTIA_DELTA || lpTemplate->MinutiaArr[j].Type == VF_MINUTIA_DELTA) continue;
			
			int rotation = GetAngle(lpTemplate->MinutiaArr[j].x, lpTemplate->MinutiaArr[j].y, 
				lpFeature->MinutiaArr[i].x, lpFeature->MinutiaArr[i].y);
			
			align(lpFeature, &alignFeature, &lpFeature->MinutiaArr[i], rotation, transx, transy);			


				// �����������ָ���������бȶ�
			alignMatch(&alignFeature, lpTemplate, &globalMatchResult, matchMode);
			// ����ȶԽ������õıȶԽ�����ã������alignMax
			if(globalMatchResult.Similarity > alignMax.Similarity)
			{
				alignMax.MMCount = globalMatchResult.MMCount;
				alignMax.Similarity = globalMatchResult.Similarity;
				alignMax.Rotation = rotation;
				alignMax.TransX = transx;
				alignMax.TransY = transy;
				
				// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
				if(matchMode == VF_MATCHMODE_IDENTIFY && alignMax.MMCount >= 8 )
				{
					if(alignMax.Similarity > 100)
					{
						*lpMatchResult = alignMax;
						return;
					}
				}
			}
		}
	}
	

	//���ձȶԽ��
	*lpMatchResult = alignMax;


}


#if (CFG_USE_C_ONLY)
void deltaMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, 
				  PMATCHRESULT lpMatchResult, VF_FLAG matchMode, 
				  Vector *n_delta, Vector *m_delta)
#else
void deltaMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, 
				  PMATCHRESULT lpMatchResult, VF_FLAG matchMode, 
				  vector<int>& n_delta, vector<int>& m_delta)
#endif
{


	MATCHRESULT alignMax;	// ���ƶ����ıȶԽ��
	MATCHRESULT globalMatchResult;	// �ȶԽ��
	sint32  agate = 8;		// �������˽ṹ�Ƕ����
	sint32  num = 0;
	// ��ʼ����õıȶԽ��
	alignMax.Similarity = 0;
	alignMax.MMCount = 0;
	alignMax.Rotation = 0;
	alignMax.TransX = 0;
	alignMax.TransY = 0;

	FEATURE	alignFeature;	// ������ָ������
	int n, m;

#if (CFG_USE_C_ONLY)
	for(int nn = 0; nn < n_delta->size; nn++)
		for(int mm = 0; mm< m_delta->size; mm++)
		{
	
			n = (int)n_delta->data[nn];
			m = (int)m_delta->data[mm];
#else
	for(int nn = 0; nn <n_delta.size(); nn++)
		for(int mm = 0; mm< m_delta.size(); mm++)
		{
	
			n = (int)n_delta[nn];
			m = (int)m_delta[mm];
#endif
			// λ��ƫ��
			int transx = (lpTemplate->MinutiaArr[n].x - lpFeature->MinutiaArr[m].x);
			int transy = (lpTemplate->MinutiaArr[n].y - lpFeature->MinutiaArr[m].y);


			for(int i = 0; i < lpFeature->MinutiaNum; i++)
			{
				for(int j = 0; j < lpTemplate->MinutiaNum; j++)
				{
					alignFeature.MinutiaNum = 0;
											
					if (lpFeature->MinutiaArr[i].Type == VF_MINUTIA_CORE || lpTemplate->MinutiaArr[j].Type == VF_MINUTIA_CORE) continue;
					if (lpFeature->MinutiaArr[i].Type == VF_MINUTIA_DELTA || lpTemplate->MinutiaArr[j].Type == VF_MINUTIA_DELTA) continue;

					
					int rotation = GetAngle(lpFeature->MinutiaArr[i].x, lpFeature->MinutiaArr[i].y, 
								lpTemplate->MinutiaArr[j].x, lpTemplate->MinutiaArr[j].y);
					
					align(lpFeature, &alignFeature, &lpFeature->MinutiaArr[i], rotation, transx, transy);			


						// �����������ָ���������бȶ�
					alignMatch(&alignFeature, lpTemplate, &globalMatchResult, matchMode);
					// ����ȶԽ������õıȶԽ�����ã������alignMax
					if(globalMatchResult.Similarity > alignMax.Similarity)
					{
						alignMax.MMCount = globalMatchResult.MMCount;
						alignMax.Similarity = globalMatchResult.Similarity;
						alignMax.Rotation = rotation;
						alignMax.TransX = transx;
						alignMax.TransY = transy;
						
						// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
						if(matchMode == VF_MATCHMODE_IDENTIFY && alignMax.MMCount >= 8 )
						{
							if(alignMax.Similarity > 100)
							{
								*lpMatchResult = alignMax;
								return;
							}
						}
					}
				}
			}
		}
	

	//���ձȶԽ��
	*lpMatchResult = alignMax;

}


int dist(int x0, int y0, int x1, int y1)
{
	return (int)sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
}

void centralMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{
	/////////////////////////////////////////////////////////////////////////////
//	lpFeature: [in] Ҫ�ȶԵĵ�һ��ָ������ָ��
//	lpTemplate: [in] Ҫ�ȶԵĵڶ���ָ������ָ��
//	lpMatchResult: [out] �ȶԽ��ָ��
//	matchMode: [in] �ȶ�ģʽ
/////////////////////////////////////////////////////////////////////////////
	sint32	m, n, a1, a2;
	sint32	rotation;		// ��ת�Ƕ�
	sint32	transx, transy;	// λ��ƫ��
	FEATURE	alignFeature;	// ������ָ������
	MATCHRESULT alignMax;	// ���ƶ����ıȶԽ��
	MATCHRESULT globalMatchResult;	// �ȶԽ��
	sint32  agate = 8;		// �������˽ṹ�Ƕ����
	sint32  num = 0;
	// ��ʼ����õıȶԽ��
	alignMax.Similarity = 0;
	alignMax.MMCount = 0;
	alignMax.Rotation = 0;
	alignMax.TransX = 0;
	alignMax.TransY = 0;

	int nx = 0, ny = 0;
	for(n = 0; n < lpTemplate->MinutiaNum; n++) 
	{
		nx += lpTemplate->MinutiaArr[n].x;
		ny += lpTemplate->MinutiaArr[n].y;
	}

	nx = nx / lpTemplate->MinutiaNum;
	ny = ny / lpTemplate->MinutiaNum;


	int mx = 0, my = 0;
	for(m = 0; m < lpFeature->MinutiaNum; m++)
	{
		mx += lpFeature->MinutiaArr[m].x;
		my += lpFeature->MinutiaArr[m].y;
	}

	mx = mx / lpFeature->MinutiaNum;
	my = my / lpFeature->MinutiaNum;

	int Counter = 0;


	// ����ͬ���͵�ָ������������Ϊͬһ��ָ���������ж���ȶ�
	for(n = 0; n < lpTemplate->MinutiaNum; n++)
	{
		if(dist(nx, ny, lpTemplate->MinutiaArr[n].x, lpTemplate->MinutiaArr[n].y) > CENTRALRADIUS) continue;

		for(m = 0; m < lpFeature->MinutiaNum; m++)
		{
			// ��ͬ�����򲻱ȶ�
			if(lpFeature->MinutiaArr[m].Type != lpTemplate->MinutiaArr[n].Type)
				continue;

		
			if(dist(mx, my, lpFeature->MinutiaArr[m].x, lpFeature->MinutiaArr[m].y) > CENTRALRADIUS) continue;

			Counter++;

			if(matchMode == VF_MATCHMODE_IDENTIFY)
			{
				// �������������˽ṹ�ȶԣ������������׼
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Direction % 180);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Direction % 180);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[1] != 255 && lpTemplate->MinutiaArr[n].Triangle[1] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Triangle[1]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Triangle[1]);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[2] != 255 && lpTemplate->MinutiaArr[n].Triangle[2] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[1] != 255 && lpTemplate->MinutiaArr[n].Triangle[1] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[1], lpFeature->MinutiaArr[m].Triangle[2]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[1], lpTemplate->MinutiaArr[n].Triangle[2]);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[2] != 255 && lpTemplate->MinutiaArr[n].Triangle[2] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Triangle[2]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Triangle[2]);
					if(abs(a1-a2)>agate)
						continue;
				}
			}
			alignFeature.MinutiaNum = 0;
			// ��ת�Ƕ�
			rotation = GetAngleDis(lpFeature->MinutiaArr[m].Direction, 
									lpTemplate->MinutiaArr[n].Direction);
			// λ��ƫ��
			transx = (lpTemplate->MinutiaArr[n].x - lpFeature->MinutiaArr[m].x);
			transy = (lpTemplate->MinutiaArr[n].y - lpFeature->MinutiaArr[m].y);
			
			// ��lpFeature��lpTemplate����
			align(lpFeature, &alignFeature, &lpFeature->MinutiaArr[m],
					rotation, transx, transy);
			// �����������ָ���������бȶ�
			alignMatch(&alignFeature, lpTemplate, &globalMatchResult, matchMode);
			// ����ȶԽ������õıȶԽ�����ã������alignMax
			if(globalMatchResult.Similarity > alignMax.Similarity)
			{
				alignMax.MMCount = globalMatchResult.MMCount;
				alignMax.Similarity = globalMatchResult.Similarity;
				alignMax.Rotation = rotation;
				alignMax.TransX = transx;
				alignMax.TransY = transy;
				
				// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
				if(matchMode == VF_MATCHMODE_IDENTIFY && alignMax.MMCount >= 8 )
				{
					if(alignMax.Similarity > 100)
					{
						*lpMatchResult = alignMax;
						return;
					}
				}
			}
		}
	}
	
	//���ձȶԽ��
	*lpMatchResult = alignMax;
}


void  patternMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{
#if (CFG_USE_C_ONLY)

	Vector n_core;
	Vector m_core;

	Vector n_delta;
	Vector m_delta;
	
	int n, m;

	vector_init(&n_core);
	vector_init(&m_core);
	vector_init(&n_delta);
	vector_init(&m_delta);

	for(n = 0; n < lpFeature->MinutiaNum; n++) {
		if (lpFeature->MinutiaArr[n].Type == VF_MINUTIA_CORE) {
			vector_push_back(&n_core, n);
		}

		/*if (lpFeature->MinutiaArr[n].Type == VF_MINUTIA_DELTA) {
			n_delta.push_back(n);
		}*/
	}

	for(m = 0; m < lpTemplate->MinutiaNum; m++) {
		if (lpTemplate->MinutiaArr[m].Type == VF_MINUTIA_CORE) {
			vector_push_back(&m_core, m);
		}

		/*if (lpTemplate->MinutiaArr[m].Type == VF_MINUTIA_DELTA) {
			m_delta.push_back(m);
		}*/
	}

	
	

	if (n_core.size > 0 && m_core.size > 0) 
	{
		for(int i = 0; i< n_core.size; i++)
			for(int j = 0; j< m_core.size; j++)
			{
				n = (int)n_core.data[i];

				m = (int)m_core.data[j];

				coreMatch(lpFeature, lpTemplate, lpMatchResult, matchMode, n, m);

				// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
				if(matchMode == VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount >= 8)
				{
					if(lpMatchResult->Similarity > 100)
					{				
						return;
					}
				}
			}
	}

	if (n_delta.size > 0 && m_delta.size > 0) 
	{
		deltaMatch(lpFeature, lpTemplate, lpMatchResult, matchMode, &n_delta, &m_delta);

		// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
		if(matchMode == VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount >= 8)
		{
			if(lpMatchResult->Similarity > 100)
			{				
				return;
			}
		}
	}

	vector_free(&n_core);
	vector_free(&m_core);
	vector_free(&n_delta);
	vector_free(&m_delta);

#else
	vector<int> n_core;
	vector<int> m_core;

	vector<int> n_delta;
	vector<int> m_delta;
	
	int n, m;

	for(n = 0; n < lpFeature->MinutiaNum; n++) {
		if (lpFeature->MinutiaArr[n].Type == VF_MINUTIA_CORE) {
			n_core.push_back(n);
		}

		/*if (lpFeature->MinutiaArr[n].Type == VF_MINUTIA_DELTA) {
			n_delta.push_back(n);
		}*/
	}

	for(m = 0; m < lpTemplate->MinutiaNum; m++) {
		if (lpTemplate->MinutiaArr[m].Type == VF_MINUTIA_CORE) {
			m_core.push_back(m);
		}

		/*if (lpTemplate->MinutiaArr[m].Type == VF_MINUTIA_DELTA) {
			m_delta.push_back(m);
		}*/
	}

	
	

	if (n_core.size() > 0 && m_core.size() > 0) 
	{
		for(int i = 0; i< n_core.size(); i++)
			for(int j = 0; j< m_core.size(); j++)
			{
				n = (int)n_core[i];

				m = (int)m_core[j];

				coreMatch(lpFeature, lpTemplate, lpMatchResult, matchMode, n, m);

				// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
				if(matchMode == VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount >= 8)
				{
					if(lpMatchResult->Similarity > 100)
					{				
						return;
					}
				}
			}
	}

	if (n_delta.size() > 0 && m_delta.size() > 0) 
	{
		deltaMatch(lpFeature, lpTemplate, lpMatchResult, matchMode, n_delta, m_delta);

		// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
		if(matchMode == VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount >= 8)
		{
			if(lpMatchResult->Similarity > 100)
			{				
				return;
			}
		}
	} 

#endif

	centralMatch(lpFeature, lpTemplate, lpMatchResult, matchMode);
	if(matchMode == VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount >= 8)
	{
		if(lpMatchResult->Similarity > 100)
		{				
			return;
		}
	}

		
	globalMatch(lpFeature, lpTemplate, lpMatchResult, matchMode);
	
}

#endif 
