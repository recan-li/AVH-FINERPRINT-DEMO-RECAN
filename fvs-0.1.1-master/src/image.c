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
  
########################################################################*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "image.h"

/*!
  A fingerprint image structure, a fingerprint image is a 256 gray level
  image. This should be enough to do accurate processing. This image
  class defines a common format for all algorithms running in this
  suite. Images may be imported from different sources such as files,
  network data, databases,...
*/
typedef struct iFvsImage_t
{
    FvsByte_t      *pimg;          /* 8 bit image array */    
    FvsInt_t        w;             /* width of image */
    FvsInt_t        h;             /* height of image */
    FvsInt_t        pitch;         /* pitch */
    FvsImageFlag_t  flags;         /* flags */
    FvsInt_t        dpi;           /* dots per inch */
} iFvsImage_t;


FvsImage_t ImageCreate()
{
    iFvsImage_t* p = NULL;

    p = (FvsImage_t)malloc(sizeof(iFvsImage_t));
    if (p!=NULL)
    {
        p->h        = 0;
        p->w        = 0;
        p->pitch    = 0;
        p->pimg     = NULL;
        p->flags    = FvsImageGray; /* default flag */    
    }
    return (FvsImage_t)p;
}


void ImageDestroy(FvsImage_t image)
{
    iFvsImage_t* p = NULL;
    if (image==NULL)
        return;
    (void)ImageSetSize(image, 0, 0);
    p = image;
    free(p);
}


FvsError_t ImageSetFlag(FvsImage_t img, const FvsImageFlag_t flag)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    image->flags = flag; 
    return FvsOK;
}


FvsError_t ImageSetDPI(FvsImage_t img, const FvsInt_t dpi)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    image->dpi = dpi; 
    return FvsOK;
}


FvsInt_t ImageGetDPI(const FvsImage_t img)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    return image->dpi; 
}


FvsImageFlag_t ImageGetFlag(const FvsImage_t img)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    return image->flags; 
}


FvsError_t ImageSetSize(FvsImage_t img, const FvsInt_t width, const FvsInt_t height)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    FvsError_t nRet = FvsOK;
    FvsInt_t newsize = width*height;

    /* special case for zero size */
    if (newsize==0)
    {
        if (image->pimg!=NULL)
        {
            free(image->pimg);
            image->pimg = NULL;
            image->w = 0;
            image->h = 0;
            image->pitch = 0;
        }
        return FvsOK;
    }

    if (image->h*image->w != newsize)
    {
        free(image->pimg);
        image->w = 0;
        image->h = 0;
        image->pitch = 0;
        /* This allocates the amount of memory need for the image structure */
        image->pimg = (uint8_t*)malloc((size_t)newsize);
    }

    if (image->pimg == NULL)
        nRet = FvsMemory;
    else
    {
        image->h = height;
        image->w = width;
        image->pitch = width;
    }
    return nRet;
}


FvsError_t ImageCopy(FvsImage_t destination, const FvsImage_t source)
{
    iFvsImage_t* dest = (iFvsImage_t*)destination;
    iFvsImage_t* src  = (iFvsImage_t*)source;
    FvsError_t nRet = FvsOK;

    nRet = ImageSetSize(dest, src->w, src->h);
    
    if (nRet==FvsOK)
        memcpy(dest->pimg, src->pimg, (size_t)src->h*src->w);

    /* copy the flag */
    dest->flags = src->flags;

    return nRet;
}


FvsError_t ImageClear(FvsImage_t img)
{
    return ImageFlood(img, 0);
}


FvsError_t ImageFlood(FvsImage_t img, const FvsByte_t value)
{
    FvsError_t nRet = FvsOK;
    iFvsImage_t* image = (iFvsImage_t*)img;
    if (image==NULL) return FvsMemory;
    if (image->pimg!=NULL)
        memset(image->pimg, (int)value, (size_t)(image->h*image->w));
    return nRet;
}


/* set a pixel value in the picture */
void ImageSetPixel(FvsImage_t img, const FvsInt_t x, const FvsInt_t y, const FvsByte_t val)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    int address = y * image->w + x;
    image->pimg[address] = val;
}


/* This function returns the pixel for the x and y value */
FvsByte_t ImageGetPixel(const FvsImage_t img, const FvsInt_t x, const FvsInt_t y)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    /* position in array */
    int address = y * image->pitch + x;
    return image->pimg[address];
}


/* returns a pointer to the image buffer */
FvsByte_t* ImageGetBuffer(FvsImage_t img)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    if (image==NULL) return NULL;
    return image->pimg;
}


/* retrieve width and height */
FvsInt_t ImageGetWidth(const FvsImage_t img)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    if (image==NULL) return -1;
    return image->w;
}


FvsInt_t ImageGetHeight(const FvsImage_t img)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    if (image==NULL) return -1;
    return image->h;
}


FvsInt_t ImageGetSize(const FvsImage_t img)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    if (image==NULL) return 0;
    return image->h * image->w;
}

/* get the pitch pixel(x,y) pos is at x + y * pitch */
FvsInt_t ImageGetPitch(const FvsImage_t img)
{
    iFvsImage_t* image = (iFvsImage_t*)img;
    if (image==NULL) return -1;
    return image->pitch;
}

FvsBool_t ImageCompareSize(const FvsImage_t image1, const FvsImage_t image2)
{
    if (ImageGetWidth(image1)!=ImageGetWidth(image2))
        return FvsFalse;
    if (ImageGetHeight(image1)!=ImageGetHeight(image2))
        return FvsFalse;
    return FvsTrue;
}

