
#ifndef __WIN_2_LINUX_H__
#define __WIN_2_LINUX_H__

#include <stdint.h>
#include <stdbool.h>

#define CFG_FEATURE_V1 	1
#define CFG_FEATURE_V2 	0

#define CFG_MATCH_SCORE_LEVEL 50

#define sint32 int32_t
//#define sint8  int8_t

#define __stdcall

#define BOOL  	bool
#define FALSE 	false
#define TRUE 	  true

typedef unsigned char BYTE;
 
typedef unsigned short WORD;
 
typedef unsigned long DWORD;

typedef unsigned long LONG;

typedef const char * LPCSTR;

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

//µãÀàÐÍ£¬×ø±êÖµÎª¸¡µã
typedef  struct DblPoint
{
	double x;
	double y;
} DBLPOINT;

//µãÀàÐÍ£¬×ø±êÖµÎª³¤ÕûÊý
/*typedef struct tagPOINT
{
long  x;
long  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;
*/

//typedef POINT *PPOINT;

#define	MAX_SINGULARYNUM	30
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#if (CFG_USE_BMP_H)
#include "finger_print.h"
#endif

#endif
