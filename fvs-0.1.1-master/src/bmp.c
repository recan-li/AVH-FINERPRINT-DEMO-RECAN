
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

#if (CFG_USE_BMP_H)
#include <string.h>
#include <stdint.h>
#include "finger_print.h"

extern int bmp_get_index(int m, int n);
#endif

//
//	loadBitmap: ´ÓBMPÎÄ¼þÖÐ¶ÁÈ¡Í¼ÏñÊý¾Ý
sint32	FvsLoadBitmap(sint8 *lpszFileName, uint8 *lpBitmap, sint32 *lpWidth, sint32 *lpHeight)
{
/////////////////////////////////////////////////////////////////////
//  lpszFileName: [in] BMPÎÄ¼þÃû
//	lpBitmap: [out] ¶Á³öµÄÍ¼ÏñÊý¾Ý
//  lpWidth: [out] Í¼Ïñ¿í¶È
//	lpHeight: [out] Í¼Ïñ¸ß¶È
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
	int m, n;
	char *c = strrchr(lpszFileName, '/');
	c++;
	m = atoi(c);
	c = strchr(c, '_');
	c++;
	n = atoi(c);	
	int bmp_index = bmp_get_index(m, n);
	uint8 *p = (uint8 *)(g_finger_print_bmp_data + (bmp_index - 1) * BMP_IMAGE_SIZE);
	uint8 *p0 = p;

	//printf("%d %d bmp_index: %d %d\n", m, n, bmp_index, (bmp_index - 1) * BMP_IMAGE_SIZE);

	memcpy((uint8 *)&bf, p, sizeof(BITMAPFILEHEADER));
	p += sizeof(BITMAPFILEHEADER);
	memcpy((uint8 *)&bi, p, sizeof(BITMAPINFOHEADER));
	p += sizeof(BITMAPINFOHEADER);

	// ¼ì²éÊÇ·ñÎªBMPÎÄ¼þ
	if(bf.bfType != 19778)
	{
		printf("[%s:%d] re %d\n", __func__, __LINE__, 2);
		return 2;
	}
	// ¼ì²éÊÇ·ñÎª256É«»Ò¶ÈÍ¼Ïñ
	if(bi.biBitCount != 8 && bi.biClrUsed != 256)
	{
		printf("[%s:%d] re %d %d %d\n", __func__, __LINE__, 3, bi.biBitCount, bi.biClrUsed);
		return 3;
	}
	// µÃµ½Í¼Ïñ´óÐ¡
	Height = bi.biHeight;
	Width = bi.biWidth;
	//printf("%d %d\n", Height, Width);
	
	// »º³åÇøÖ¸Õë²»Îª¿ÕÔò¶ÁÈ¡Í¼ÏñÊý¾Ýµ½»º³åÇø
	if(lpBitmap != NULL)
	{
		lineBytes = (sint32)WIDTHBYTES(Width);
		Size = lineBytes * Height * (bi.biBitCount / 8);
		memcpy((uint8 *)palatte, p, 1024);
		p += 1024;
		// °´ÐÐ¶ÁÈ¡£¬Ã¿ÐÐÖ»¶ÁÈ¡Width¸ö×Ö½Ú
		for(i = 0; i < Height; i++)
		{
			p = p0 + 1078 + lineBytes * i;
			memcpy((uint8 *)(lpBitmap+i*Width), p, Width);
			char *pp = (void *)(lpBitmap+i*Width);
			//debug_buffer_data(pp, 16);
		}
	}

#else

	// ´ò¿ªÎÄ¼þ
	fp = fopen(lpszFileName, "rb");
	if(fp == NULL)
	{
		printf("[%s:%d] re %d (%s)\n", __func__, __LINE__, 1, lpszFileName);
		return 1;
	}
	// ¶ÁÎÄ¼þÍ·
	fread((void *)&bf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread((void *)&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	// ¼ì²éÊÇ·ñÎªBMPÎÄ¼þ
	if(bf.bfType != 19778)
	{
		fclose(fp);
		printf("[%s:%d] re %d\n", __func__, __LINE__, 2);
		return 2;
	}
	// ¼ì²éÊÇ·ñÎª256É«»Ò¶ÈÍ¼Ïñ
	if(bi.biBitCount != 8 && bi.biClrUsed != 256)
	{
#if 1
		fclose(fp);
		printf("[%s:%d] re %d %d %d\n", __func__, __LINE__, 3, bi.biBitCount, bi.biClrUsed);
		return 3;
#endif
	}
	// µÃµ½Í¼Ïñ´óÐ¡
	Height = bi.biHeight;
	Width = bi.biWidth;
	
	// »º³åÇøÖ¸Õë²»Îª¿ÕÔò¶ÁÈ¡Í¼ÏñÊý¾Ýµ½»º³åÇø
	if(lpBitmap != NULL)
	{
		lineBytes = (sint32)WIDTHBYTES(Width);
		Size = lineBytes*Height * (bi.biBitCount / 8);
		
		fread(palatte, 1, 1024, fp);
		// °´ÐÐ¶ÁÈ¡£¬Ã¿ÐÐÖ»¶ÁÈ¡Width¸ö×Ö½Ú
		for(i = 0; i < Height; i++)
		{			
			fseek(fp, 1078+lineBytes*i, SEEK_SET);
			fread((void *)(lpBitmap+i*Width), 1, Width, fp);
			char *pp = (void *)(lpBitmap+i*Width);
			//debug_buffer_data(pp, 16);
		}
	}
	fclose(fp);

#endif

	*lpWidth = Width;
	*lpHeight = Height;

	return 0;
}

//
//	saveBitmap: ½«Í¼ÏñÊý¾Ý±£´æ³ÉBMPÎÄ¼þ
sint32	FvsSaveBitmap(sint8 *lpszFileName, uint8 *lpBitmap, sint32 Width, sint32 Height)
{
/////////////////////////////////////////////////////////////////////
//	lpBitmap: [in] Òª±£´æµÄÍ¼ÏñÊý¾Ý
//  Width: [in] Í¼Ïñ¿í¶È
//	Height: [in] Í¼Ïñ¸ß¶È
//  lpszFileName: [in] BMPÎÄ¼þÃû
/////////////////////////////////////////////////////////////////////
   	FILE               *fp = NULL;
	uint8				palatte[1024];
	BITMAPFILEHEADER   bf;
	BITMAPINFOHEADER   bi;
	sint32             lineBytes = 0;
	sint32             i;
	
	lineBytes = (sint32)WIDTHBYTES(Width);
	
	//¹¹ÔìbmpÎÄ¼þÍ·ÐÅÏ¢
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
	//¹¹Ôìµ÷É«°åÊý¾Ý
	for(i=0;i<256;i++){
		palatte[i*4]=(unsigned char)i;
		palatte[i*4+1]=(unsigned char)i;
		palatte[i*4+2]=(unsigned char)i;
		palatte[i*4+3]=0;
	}	

#if (CFG_USE_BMP_H)
	// TODO
#else
	// ´´½¨ÎÄ¼þ
	fp = fopen(lpszFileName, "wb");
	if(fp == NULL)
	{
		return 1;
	}
	// Ð´ÎÄ¼þÍ·
	fwrite((char *)&bf,sizeof(BITMAPFILEHEADER), 1, fp); 
	// Ð´BMPÍ¼ÏñÐÅÏ¢Í·
	fwrite((char *)&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	// Ð´µ÷É«°å
	fwrite((char *)palatte, sizeof(palatte), 1, fp);
	// °´ÐÐÐ´£¬Ã¿ÐÐÐ´WIDTHBYTES(Width)¸ö×Ö½Ú
	for(i = 0; i < Height; i++)
		fwrite((char *)(lpBitmap+i*Width), lineBytes, 1, fp);

	fclose(fp);	
#endif

	return 0;
}
