
#ifndef __BMP_H__
#define __BMP_H__

#include "config.h"

#define BOOL  	bool
#define FALSE 	false
#define TRUE 	  true

#if 0
/* Basic Types */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef short sint16;
typedef unsigned long uint32;

//typedef long sint32;

#endif
typedef char sint8;
typedef int sint32;

typedef unsigned char BYTE;
 
typedef unsigned short WORD;
 
typedef unsigned long DWORD;

typedef unsigned long LONG;

typedef const char * LPCSTR;

#define WIDTHBYTES(i)    ((i+3)/4*4)

#if 0

#pragma pack(1)

typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

#else

#pragma pack(1)

typedef struct {
    short 	bfType;
    int 	bfSize;
    short 	bfReserved1;
    short 	bfReserved2;
    int 	bfOffBits;
} BMPFileHeader, BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct {
    int biSize;
    int biWidth;
    int biHeight;
    short biPlanes;
    short biBitCount;
    int biCompression;
    int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
} BMPInfoHeader, BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

#endif

sint32  FvsLoadBitmap(sint8 *lpszFileName, uint8 *lpBitmap, sint32 *lpWidth, sint32 *lpHeight);
sint32  FvsSaveBitmap(sint8 *lpszFileName, uint8 *lpBitmap, sint32 Width, sint32 Height);

#endif