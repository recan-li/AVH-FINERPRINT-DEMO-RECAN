//#include "stdafx.h"
#include "VF_Api.h"
#include "SyntFinger.h"
#include "VF_Global.h"

int __stdcall Add(int numa, int numb)
{
	return (numa + numb);
}

// Extract: ��ָ��ͼ������ȡָ������
int __stdcall Analyze(BYTE *lpImage, int Width, int Height, BYTE *lpFeature, int *lpSize)
{
	///////////////////////////////////////////////////////////////////////
	//  Width:	[in] ָ��ͼ����
	//  Height:	[in] ָ��ͼ��߶�
	//  lpImage:    [in] ָ��ͼ������ָ��
	//  Resolution:	[in] ָ��ͼ��ֱ��ʣ�Ĭ��500
	//  lpFeature:	[out] ��ȡ��ָ����������ָ��
	//  lpSize:	[out] ָ���������ݴ�С
	///////////////////////////////////////////////////////////////////////

	// TODO: Add your implementation code here
	VF_RETURN	re;

	// ����ָ��ͼ������
	VF_ImportFinger(lpImage, Width, Height);

	// ����ָ��ͼ����ȡָ������
	re = VF_Process();
	if (re != VF_OK)
		return re;

	// ��ָ���������б���
	re = VF_FeatureEncode(&g_Feature, lpFeature, lpSize);
	if (re != VF_OK)
		return re;

	return 0;
}


// ExtractEx: ��ָ��ͼ��BMP�ļ�����ȡָ������
int __stdcall AnalyzeFromFile(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize)
{
	//  lpszFileName: [in] ָ��ͼ���ļ�·��
	//  Resolution: [in] ָ��ͼ��ֱ���
	//  lpFeature: [out] ��ȡ��ָ���������ݻ�����ָ��
	//  lpSize: [out] ��ȡ��ָ���������ݴ�С
	
	VF_RETURN	re;

	// ��ȡָ��ͼ������
	re = VF_LoadFinger(lpszFileName);
	if (re != VF_OK) {
		printf("re1: %d\n", (int)re);
		return re;
	}

	// ����ָ��ͼ����ȡָ������
	re = VF_Process();
	if (re != VF_OK) {
		printf("re2: %d\n", (int)re);
		return re;
	}

	// ��ָ���������б���
	re = VF_FeatureEncode(&g_Feature, lpFeature, lpSize);
	if (re != VF_OK) {
		printf("re3: %d\n", (int)re);
		return re;
	}

	return 0;
}

// VerifyMatch: ����ָ��������ıȶ�
int __stdcall PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore)
{
	//	lpFeature1:		[in] ��һ��ָ������
	//	lpFeature2:		[in] �ڶ���ָ������
	//	lpScore:		[out] �ȶԵ����ƶ�
	//	FastMode:		[in] �Ƿ���п���ģʽ�ȶ�
	VF_RETURN	re1,re2;
	MATCHRESULT mr;
	FEATURE		feat1, feat2;

	// ��һ��ָ�������Ľ���
	re1 = VF_FeatureDecode(lpFeature1, &feat1);
	if (re1 != VF_OK)
	{
		printf("ͼ��1����ʧ��\n");
		return 0;
		//return re1;
	}

	// �ڶ���ָ�������Ľ���
	re2 = VF_FeatureDecode(lpFeature2, &feat2);
	if (re2 != VF_OK)
	{
		printf("ͼ��2����ʧ��\n");
		return 0;
		//return re2;
	}

	*lpScore = 0;

	bool FastMode = true;

	if (FastMode)
	{
		// ����ģʽ�ıȶ�
		VF_VerifyMatch(&feat1, &feat2, &mr, VF_MATCHMODE_IDENTIFY);
	}
	else
	{
		// ��ȷģʽ�ıȶ�
		VF_VerifyMatch(&feat1, &feat2, &mr, VF_MATCHMODE_VERIFY);
	}

	// ƥ������ƶ�
	//*lpScore = mr.Similarity/10;
	*lpScore = mr.Similarity;
	/*if (mr.MMCount < 8)
	{
		*lpScore = 0;
	}
	else
	{
		*lpScore = mr.Similarity;
	}*/

	return 0;
}

#if 0
#define MAX_INDEX 	217
#define PIC_PATH 	"./input-finger-pictures"
#else
#define MAX_INDEX 	10
#define PIC_PATH 	"./finger_print_pictures"
#endif

#if (CFG_ENABLE_MAIN_FUNCTION)
int main(int argc, const char *argv[])
#else
int main_entry(int argc, const char *argv[])
#endif
{
	char ImagePathName1[100];
	char ImagePathName2[100];
	char *p1 = NULL;
	char *p2 = NULL;

	BYTE lpFeature1[500] = { 0 };
	BYTE lpFeature2[500] = { 0 };

	int lpSize1 = 0, lpSize2 = 0, score = 0;
	int iReturn = 0;
	int i,j;

#if (CFG_ENABLE_LOWER_RAM)
	g_lpOrgFinger = (uint8 *)malloc(MAXIMGSIZE);
	g_lpOrient = (uint8 *)malloc(MAXIMGSIZE);
	g_lpDivide = (uint8 *)malloc(MAXIMGSIZE);
	g_lpTemp = (uint8 *)malloc(MAXIMGSIZE);
	//printf("g_lpOrgFinger:  %p\n", g_lpOrgFinger);
	//printf("g_lpOrient:     %p\n", g_lpOrient);
	//printf("g_lpDivide:     %p\n", g_lpDivide);
	//printf("g_lpTemp:       %p\n", g_lpTemp);
#endif

	printf("\n\nBegin to match %d finger print pictures ...\n", MAX_INDEX);
	printf("-----------------------------------------------------------\n");
	printf("[%10s] vs [%10s] -> score, match result!     \n", "bmp file 1", "bmp file 2");
	printf("-----------------------------------------------------------\n");

	for (i = 1; i <= (MAX_INDEX-1); i++) {
		for (j = i+1; j <= MAX_INDEX; j++) {
			sprintf(ImagePathName1, PIC_PATH "/1-%d.bmp", i);
			sprintf(ImagePathName2, PIC_PATH "/1-%d.bmp", j);
			p1 = strrchr(ImagePathName1, '/')+1;
			p2 = strrchr(ImagePathName2, '/')+1;

			printf("[%10s] vs [%10s] -> ", p1, p2);

			iReturn = AnalyzeFromFile(ImagePathName1, lpFeature1, &lpSize1);
			if (iReturn != 0)
			{
				printf("open bmp1 [%10s] fail %d\n", p1, iReturn);
				break;
			}

			iReturn = AnalyzeFromFile(ImagePathName2, lpFeature2, &lpSize2);
			if (iReturn != 0)
			{
				printf("open bmp2 [%10s] fail %d\n", p2, iReturn);
				continue;
			}

			PatternMatch(lpFeature1, lpFeature2, &score);

			if (score >= CFG_MATCH_SCORE_LEVEL)
			{
				printf("%3d, Same Fingerprint!     \n", score);
			}
			else
			{
				printf("%3d, Different Fingerprint!\n", score);
			}
		}
	}
	printf("-----------------------------------------------------------\n");
	printf("---                END OF MATCH RESULT                  ---\n");
	printf("-----------------------------------------------------------\n");

#if (CFG_ENABLE_LOWER_RAM)
	free(g_lpOrgFinger);
	free(g_lpOrient);
	free(g_lpDivide);
	free(g_lpTemp);
#endif
	
	return 0;
}