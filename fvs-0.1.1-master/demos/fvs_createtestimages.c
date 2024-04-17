/*########################################################################

  The contents of this file are subject to the Mozilla Public License
  Version 1.0(the "License");   You  may  NOT  use this file except in
  compliance with the License. You may obtain a copy of the License at
                http:// www.mozilla.org/MPL/
  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
  the License for the specific language governing rights and limitations
  under the License.

  The Initial Developer of the Original Code is Jaap de Haan.

  Copyright(C) 2002-2003. All Rights Reserved.

  Authors: Jaap de Haan(jdh)
  
  This file contains all the main functions of the program.
  This is where everything is called from.

########################################################################*/

#include "fvs.h"

#if (CFG_ARM_MATH_ENABLE)
#include "arm_math.h"
#else
#include <math.h>
#endif

#define IMG_WIDTH_SIZE 256
#define IMG_HEIGHT_SIZE 360

/*
** Source code to generate test images 256x256, gray level bitmaps
**
*/

#define TEST_PATH "test/"

static void GenerateLineImage(FvsImage_t img, FvsInt_t spacing, FvsInt_t angle)
{
    FvsInt_t x, y, w, h;
    char name[1024];
    FvsFloat_t dir   = angle*M_PI/180.0;
    FvsFloat_t piom, s, c;
    FvsByte_t* p     = ImageGetBuffer(img);
    FvsInt_t pitch   = ImageGetPitch(img);

    if (p==NULL)
    {
        printf("Error, NULL pointer in function GenerateTestImage\n");
        return;
    }

    w = ImageGetWidth(img);
    h = ImageGetHeight(img);
    s = sin(dir);
    c = cos(dir);

    printf("Generating a test image with spacing %3i and angle %3i\n", spacing, angle);
    (void)ImageClear(img);
    piom = 2*M_PI / (FvsFloat_t)(spacing);

    for (y = 0; y < h; y++)
    for (x = 0; x < w; x++)
    {
        p[x+y*pitch] = (uint8_t)(127.5*(1+cos(piom*(-x*s + y*c))));
    }

    (void)snprintf(name, (size_t)1000, TEST_PATH "testimg%03i%03i.bmp", spacing, angle);
    (void)FvsImageExport(img, name);
}

static void GenerateCircleImage(FvsImage_t img, FvsInt_t spacing)
{
    FvsInt_t x, y, w, h;
    char name[1024];
    FvsFloat_t piom, d;
    FvsByte_t* p     = ImageGetBuffer(img);
    FvsInt_t pitch   = ImageGetPitch(img);

    if (p==NULL)
    {
        printf("Error, NULL pointer in function GenerateCircleImage\n");
        return;
    }

    w = ImageGetWidth(img);
    h = ImageGetHeight(img);

    printf("Generating a circle test image with spacing %3i\n", spacing);
    (void)ImageClear(img);
    piom = 2*M_PI / (FvsFloat_t)(spacing);

    for (y = 0; y < h; y++)
    for (x = 0; x < w; x++)
    {
        d = sqrt((h/2-y)*(h/2-y) + (w/2-x)*(w/2-x));
        p[x+y*pitch] = (uint8_t)(127.5*(1+cos(piom*d)));
    }

    (void)snprintf(name, (size_t)1000, TEST_PATH "testimgcircle%03i.bmp", spacing);
    (void)FvsImageExport(img, name);
}

int main(/*@unused@*/ int argc, /*@unused@*/ char *argv[])
{
    FvsImage_t img = ImageCreate();
    FvsInt_t   spacing, angle;
    FvsError_t nRet;

    printf("Creating a new image\n");    
    if (img==NULL) return -1;

    printf("Setting the size\n");
    nRet = ImageSetSize(img, IMG_WIDTH_SIZE, IMG_HEIGHT_SIZE);
    if (nRet==FvsOK)
    {
        for (spacing=4; spacing <= 16; spacing+=4)
        {
            for (angle=0; angle < 180; angle+=15)
                GenerateLineImage(img, spacing, angle);
            GenerateCircleImage(img, spacing);
        }
    }

    printf("Cleaning up\n");
    ImageDestroy(img);
    return 0;
}


