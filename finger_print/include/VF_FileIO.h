//#define _CRT_SECURE_NO_WARNINGS

#ifndef	__VFFILEIO_H__
#define __VFFILEIO_H__

#include "VF_Type.h"
#include "VF_Global.h"

#if (CFG_USE_BMP_H)
#include <string.h>
#endif

//
//	loadBitmap: ��BMP�ļ��ж�ȡͼ������
sint32	loadBitmap(sint8 *lpszFileName, uint8 *lpBitmap, sint32 *lpWidth, sint32 *lpHeight)
{
/////////////////////////////////////////////////////////////////////
//  lpszFileName: [in] BMP�ļ���
//	lpBitmap: [out] ������ͼ������
//  lpWidth: [out] ͼ����
//	lpHeight: [out] ͼ��߶�
/////////////////////////////////////////////////////////////////////
   	FILE               *fp = NULL;
	uint8			   palatte[1024];
	BITMAPFILEHEADER   bf;
	BITMAPINFOHEADER   bi;
	sint32			   i;
	sint32             lineBytes = 0;
	sint32             Width;
	sint32             Height;
	sint32			   Size = 0;
	
#if (CFG_USE_BMP_H)
	char *c = strrchr(lpszFileName, '-');
	int bmp_index = atoi(c + 1);	
	uint8 *p = (uint8 *)(g_finger_print_bmp_data + (bmp_index - 1) * BMP_IMAGE_SIZE);
	uint8 *p0 = p;

	//printf("bmp_index: %d %d\n", bmp_index, (bmp_index - 1) * BMP_IMAGE_SIZE);

	memcpy((uint8 *)&bf, p, sizeof(BITMAPFILEHEADER));
	p += sizeof(BITMAPFILEHEADER);
	memcpy((uint8 *)&bi, p, sizeof(BITMAPINFOHEADER));
	p += sizeof(BITMAPINFOHEADER);

	// ����Ƿ�ΪBMP�ļ�
	if(bf.bfType != 19778)
	{
		printf("[%s:%d] re %d\n", __func__, __LINE__, 2);
		return 2;
	}
	// ����Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	if(bi.biBitCount != 8 && bi.biClrUsed != 256)
	{
		printf("[%s:%d] re %d %d %d\n", __func__, __LINE__, 3, bi.biBitCount, bi.biClrUsed);
		return 3;
	}
	// �õ�ͼ���С
	Height = bi.biHeight;
	Width = bi.biWidth;
	
	// ������ָ�벻Ϊ�����ȡͼ�����ݵ�������
	if(lpBitmap != NULL)
	{
		lineBytes = (sint32)WIDTHBYTES(Width);
		Size = lineBytes*Height;
		
		memcpy((uint8 *)palatte, p, 1024);
		p += 1024;
		// ���ж�ȡ��ÿ��ֻ��ȡWidth���ֽ�
		for(i = 0; i < Height; i++)
		{
			p = p0 + 1078 + lineBytes * i;
			memcpy((uint8 *)(lpBitmap+i*Width), p, Width);
		}
	}

#else

	// ���ļ�
	fp = fopen(lpszFileName, "rb");
	if(fp == NULL)
	{
		printf("[%s:%d] re %d (%s)\n", __func__, __LINE__, 1, lpszFileName);
		return 1;
	}
	// ���ļ�ͷ
	fread((void *)&bf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread((void *)&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	// ����Ƿ�ΪBMP�ļ�
	if(bf.bfType != 19778)
	{
		fclose(fp);
		printf("[%s:%d] re %d\n", __func__, __LINE__, 2);
		return 2;
	}
	// ����Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	if(bi.biBitCount != 8 && bi.biClrUsed != 256)
	{
		fclose(fp);
		printf("[%s:%d] re %d %d %d\n", __func__, __LINE__, 3, bi.biBitCount, bi.biClrUsed);
		return 3;
	}
	// �õ�ͼ���С
	Height = bi.biHeight;
	Width = bi.biWidth;
	
	// ������ָ�벻Ϊ�����ȡͼ�����ݵ�������
	if(lpBitmap != NULL)
	{
		lineBytes = (sint32)WIDTHBYTES(Width);
		Size = lineBytes*Height;
		
		fread(palatte, 1, 1024, fp);
		// ���ж�ȡ��ÿ��ֻ��ȡWidth���ֽ�
		for(i = 0; i < Height; i++)
		{
			fseek(fp, 1078+lineBytes*i, SEEK_SET);
			fread((void *)(lpBitmap+i*Width), 1, Width, fp);
		}
	}
	fclose(fp);

#endif

	*lpWidth = Width;
	*lpHeight = Height;

	return 0;
}

//
//	saveBitmap: ��ͼ�����ݱ����BMP�ļ�
sint32	saveBitmap(uint8 *lpBitmap, sint32 Width, sint32 Height, sint8 *lpszFileName)
{
/////////////////////////////////////////////////////////////////////
//	lpBitmap: [in] Ҫ�����ͼ������
//  Width: [in] ͼ����
//	Height: [in] ͼ��߶�
//  lpszFileName: [in] BMP�ļ���
/////////////////////////////////////////////////////////////////////
   	FILE               *fp = NULL;
	uint8				palatte[1024];
	BITMAPFILEHEADER   bf;
	BITMAPINFOHEADER   bi;
	sint32             lineBytes = 0;
	sint32             i;
	
	lineBytes = (sint32)WIDTHBYTES(Width);
	
	//����bmp�ļ�ͷ��Ϣ
	bf.bfType=19778;
	bf.bfSize = lineBytes*Height+1078;
	bf.bfReserved1=0;
	bf.bfReserved2=0;
	bf.bfOffBits=1078;
	bi.biSize=40;
	bi.biWidth = Width;
	bi.biHeight = Height;
	bi.biPlanes=1;
	bi.biBitCount=8;
	bi.biCompression=0;
	bi.biSizeImage= lineBytes*Height;
	bi.biXPelsPerMeter=0;
	bi.biYPelsPerMeter=0;
	bi.biClrUsed=256;

	bi.biClrImportant=0;
	//�����ɫ������
	for(i=0;i<256;i++){
		palatte[i*4]=(unsigned char)i;
		palatte[i*4+1]=(unsigned char)i;
		palatte[i*4+2]=(unsigned char)i;
		palatte[i*4+3]=0;
	}	

#if (CFG_USE_BMP_H)
	// TODO
#else
	// �����ļ�
	fp = fopen(lpszFileName, "wb");
	if(fp == NULL)
	{
		return 1;
	}
	// д�ļ�ͷ
	fwrite((char *)&bf,sizeof(BITMAPFILEHEADER), 1, fp); 
	// дBMPͼ����Ϣͷ
	fwrite((char *)&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	// д��ɫ��
	fwrite((char *)palatte, sizeof(palatte), 1, fp);
	// ����д��ÿ��дWIDTHBYTES(Width)���ֽ�
	for(i = 0; i < Height; i++)
		fwrite((char *)(lpBitmap+i*Width), lineBytes, 1, fp);

	fclose(fp);	
#endif

	return 0;
}

#endif