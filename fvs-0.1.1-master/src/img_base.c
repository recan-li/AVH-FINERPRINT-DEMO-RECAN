/*########################################################################

  The contents of this file are subject to the Mozilla Public License
  Version 1.0(the "License");   You  may  NOT  use this file except in
  compliance with the License. You may obtain a copy of the License at
                http:// www.mozilla.org/MPL/
  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
  the License for the specific language governing rights and limitations
  under the License.

  The Initial Developer of the Original Code is Shivang Patel.

  Copyright(C) 2002-2003. All Rights Reserved.

  Authors: Shivang Patel
           Jaap de Haan(jdh)
  
  This file contains functions that manipulate the image

########################################################################*/


#include "img_base.h"

#include "histogram.h"

#if (CFG_ARM_MATH_ENABLE)
#include "arm_math.h"
#else
#include <math.h>
#endif

#include <stdlib.h>
#include <string.h>


/* Transform the gray image into a black & white binary image */
FvsError_t ImageBinarize(FvsImage_t image, const FvsByte_t limit)
{
    FvsInt_t n;
    FvsByte_t *pimg = ImageGetBuffer(image);
    FvsInt_t size = ImageGetSize(image);
    if (pimg==NULL)
        return FvsMemory;
    /* loop through each pixel */
    for (n = 0; n < size; n++, pimg++)
    {
        /* now a do some math to decided if its white or black */
        *pimg = (*pimg < limit)?(FvsByte_t)0xFF:(FvsByte_t)0x00;
    }
    return ImageSetFlag(image, FvsImageBinarized);
}


/* Invert image pixel values */
FvsError_t ImageInvert(FvsImage_t image)
{
    FvsByte_t* pimg = ImageGetBuffer(image);
    FvsInt_t size = ImageGetSize(image);
    FvsInt_t n;
    if (pimg==NULL)
        return FvsMemory;
    for (n = 0; n < size; n++, pimg++)
    {
        *pimg = 0xFF - *pimg;
    }
    return FvsOK;
}


/* compute the average of 2 images */
FvsError_t ImageAverage(FvsImage_t image1, const FvsImage_t image2)
{
    FvsByte_t* p1 = ImageGetBuffer(image1);
    FvsByte_t* p2 = ImageGetBuffer(image2);
    FvsInt_t size1 = ImageGetSize(image1);
    FvsInt_t size2 = ImageGetSize(image2);
    FvsInt_t i;

    if (p1==NULL || p2==NULL)
        return FvsMemory;
    if (size1!=size2)
        return FvsBadParameter;

    for (i = 0; i < size1; i++, p1++)
    {
        *p1 = (*p1+*p2++)>>1;
    }
    return FvsOK;
}


/* compute a logical operation */
FvsError_t ImageLogical
    (
    FvsImage_t image1,
    const FvsImage_t image2,
    const FvsLogical_t operation
    )
{
    FvsByte_t* p1 = ImageGetBuffer(image1);
    FvsByte_t* p2 = ImageGetBuffer(image2);
    FvsInt_t size1 = ImageGetSize(image1);
    FvsInt_t i;

    if (p1==NULL || p2==NULL)
        return FvsMemory;
    if (ImageCompareSize(image1, image2)==FvsFalse)
        return FvsBadParameter;

    switch (operation)
    {
    case FvsLogicalOr:
        for (i = 0; i < size1; i++, p1++)
            *p1 = (*p1) | (*p2++);                    
        break;
    case FvsLogicalAnd:
        for (i = 0; i < size1; i++, p1++)
            *p1 = (*p1) & (*p2++);
        break;
    case FvsLogicalXor:
        for (i = 0; i < size1; i++, p1++)
            *p1 = (*p1) ^ (*p2++);
        break;
    case FvsLogicalNAnd:
        for (i = 0; i < size1; i++, p1++)
            *p1 = ~((*p1) & (*p2++));
        break;
    case FvsLogicalNOr:
        for (i = 0; i < size1; i++, p1++)
            *p1 = ~((*p1) | (*p2++));
        break;
    case FvsLogicalNXor:
        for (i = 0; i < size1; i++, p1++)
            *p1 = ~((*p1) ^ (*p2++));
        break;
    }
    return FvsOK;
}


/* compute the average of 2 images modulo 256 */
FvsError_t ImageAverageModulo(FvsImage_t image1, const FvsImage_t image2)
{
    FvsByte_t* p1 = ImageGetBuffer(image1);
    FvsByte_t* p2 = ImageGetBuffer(image2);
    FvsInt_t size1 = ImageGetSize(image1);
    FvsInt_t size2 = ImageGetSize(image2);
    FvsInt_t i;
    FvsByte_t v1, v2;

    if (size1!=size2)
        return FvsBadParameter;

    if (p1==NULL || p2==NULL)
        return FvsMemory;

    for (i = 0; i < size1; i++)
    {
        v1 = *p1;
        v2 = *p2;
        if (v1<128) v1+=256;
        if (v2<128) v2+=256;
        v1 += v2;
        v1 >>=1;
        v1 = v1%256;
        *p1++ = (uint8_t)v1;
    }
    return FvsOK;
}


FvsError_t ImageTranslate(/*@unused@*/FvsImage_t image, /*@unused@*/const FvsInt_t vx, /*@unused@*/const FvsInt_t vy)
{
/* TODO: Implement
    uint8_t* p = ImageGetBuffer(image);
    int32_t w     = ImageGetWidth (image);
    int32_t h     = ImageGetHeight(image);
    int32_t pitch = ImageGetPitch (image);
    int32_t x, y, w2, h2;
*/
    return FvsOK;
}


#define P(x,y)      p[((x)+(y)*pitch)]


/* create a test image composed of stripes */
FvsError_t ImageStripes(FvsImage_t image, const FvsBool_t horizontal)
{
    FvsByte_t* p = ImageGetBuffer(image);
    FvsInt_t w     = ImageGetWidth (image);
    FvsInt_t h     = ImageGetHeight(image);
    FvsInt_t pitch = ImageGetPitch (image);
    FvsInt_t x,y;
    if (p==NULL)
        return FvsMemory;
    if (horizontal==FvsFalse)
    {
        for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
            P(x,y) = (FvsByte_t)x%256;
    }
    else
    {
        for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
            P(x,y) = (FvsByte_t)y%256;
    }
    return FvsOK;
}


/* change the luminosity of an image argument ranging [-255..255] */
FvsError_t ImageLuminosity(FvsImage_t image, const FvsInt_t luminosity)
{
    FvsByte_t* p = ImageGetBuffer(image);
    FvsInt_t  w = ImageGetWidth (image);
    FvsInt_t  h = ImageGetHeight(image);
    FvsInt_t pitch = ImageGetPitch (image);
    FvsInt_t x,y;
    FvsFloat_t fgray, a, b;
    if (p==NULL)
        return FvsMemory;
    if (luminosity>0)
    {
        a = (255.0 - abs(luminosity)) / 255.0;
        b = (FvsFloat_t)luminosity;
    }
    else
    {
        a = (255.0 - abs(luminosity)) / 255.0;
        b = 0.0;
    }
    for (y = 0; y < h; y++)
    for (x = 0; x < w; x++)
    {
        fgray = (FvsFloat_t)P(x,y);
        fgray = b + a*fgray;
        if (fgray < 0.0)    fgray = 0.0;
        if (fgray > 255.0)  fgray = 255.0;
        P(x,y)= (uint8_t)fgray;
    }
    return FvsOK;
}


/* change the contrast of an image argument ranging [-127..127] */
FvsError_t ImageContrast(FvsImage_t image, const FvsInt_t contrast)
{
    FvsByte_t* p = ImageGetBuffer(image);
    FvsInt_t  w = ImageGetWidth (image);
    FvsInt_t  h = ImageGetHeight(image);
    FvsInt_t pitch = ImageGetPitch (image);
    FvsInt_t x,y;
    FvsFloat_t fgray, a, b;
    if (p==NULL)
        return FvsMemory;
    a = (FvsFloat_t)((127.0 + contrast) / 127.0);
    b = (FvsFloat_t)(-contrast);
    for (y = 0; y < h; y++)
    for (x = 0; x < w; x++)
    {
        fgray = (FvsFloat_t)P(x,y);
        fgray = b + a*fgray;
        if (fgray < 0.0)    fgray = 0.0;
        if (fgray > 255.0)  fgray = 255.0;
        P(x,y)= (uint8_t)fgray;
    }
    return FvsOK;
}


FvsError_t ImageSoftenMean(FvsImage_t image, const FvsInt_t size)
{
    FvsByte_t* p1  = ImageGetBuffer(image);
    FvsByte_t* p2;
    FvsInt_t   w   = ImageGetWidth (image);
    FvsInt_t   h   = ImageGetHeight(image);
    FvsInt_t pitch = ImageGetPitch (image);
    FvsInt_t pitch2;
    FvsInt_t x,y,s,p,q,a,c;
    FvsImage_t im2;

    im2 = ImageCreate();
    
    if (im2==NULL || p1==NULL)
        return FvsMemory;

    s = size/2;		/* size */
    a = size*size;	/* area */    
    if (a==0)
	return FvsBadParameter;
	
    /* copy image to make the computation */
    ImageCopy(im2, image);
    p2 = ImageGetBuffer(im2);
    if (p2==NULL)
    {
	ImageDestroy(im2);
	return FvsMemory;
    }
    pitch2 = ImageGetPitch (im2);
    
    for (y = s; y < h-s; y++)
    for (x = s; x < w-s; x++)
    {
	c = 0;
	for (q=-s;q<=s;q++)
	for (p=-s;p<=s;p++)
	{
    	    c += p2[(x+p)+(y+q)*pitch2];
	}
        p1[x+y*pitch] = c/a;
    }
    
    ImageDestroy(im2);
    return FvsOK;
}


/* normalize an image so that it gets specified mean and variance */
FvsError_t ImageNormalize(FvsImage_t image, const FvsByte_t mean, const FvsUint_t variance)
{
    FvsByte_t* p = ImageGetBuffer(image);
    FvsInt_t   w = ImageGetWidth (image);
    FvsInt_t   h = ImageGetHeight(image);
    FvsInt_t   pitch = ImageGetPitch (image);
    FvsInt_t   x,y;
    FvsFloat_t fmean, fsigma, fmean0, fsigma0, fgray;
    FvsFloat_t fcoeff = 0.0;

    FvsHistogram_t histogram = NULL;
    FvsError_t nRet;

    if (p==NULL)
        return FvsMemory;
    histogram = HistogramCreate();
    if (histogram!=NULL)
    {
        /* compute histogram */
        nRet = HistogramCompute(histogram, image);
        if (nRet==FvsOK)
        {
            /* first compute sigma and mean */
            fmean   = (FvsFloat_t)HistogramGetMean(histogram);
            fsigma  = sqrt((FvsFloat_t)HistogramGetVariance(histogram));

            fmean0  = (FvsFloat_t)mean;
            fsigma0 = sqrt((FvsFloat_t)variance);
            if (fsigma>0.0)
                fcoeff = fsigma0/fsigma;
            for (y = 0; y < h; y++)
            for (x = 0; x < w; x++)
            {
                fgray = (FvsFloat_t)P(x,y);
                fgray = fmean0 + fcoeff*(fgray - mean);
                if (fgray < 0.0)    fgray = 0.0;
                if (fgray > 255.0)  fgray = 255.0;
                P(x,y)= (uint8_t)fgray;
            }
        }
        HistogramDestroy(histogram);
    }
    return nRet;
}


