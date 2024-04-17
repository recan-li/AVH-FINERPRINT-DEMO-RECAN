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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#if (CFG_ARM_MATH_ENABLE)
#include "arm_math.h"
#else
#include <math.h>
#endif

#include "imagemanip.h"


/*
** jdh: image enhancement part. This enhancement algorithm is specialized
** on fingerprint images. It marks regions that are not to be used with a
** special color in the mask. Other pixels are improved to that ridges can
** be clearly separated using a threshold value.
** The algorithm produces a ridges direction field image and a mask that
** masks out the unusable areas or areas that are likely to contain no
** fingerprint as well as border of fingerprint areas.
**
** Ideas have been found in small articles:
** 1 - Fingerprint Enhancement: Lin Hong, Anil Jain, Sharathcha Pankanti,
**     and Ruud Bolle. [Hong96]
** 2 - Fingerprint Image Enhancement, Algorithm and Performance Evaluation:
**     Lin Hong, Yifei Wan and Anil Jain. [Hong98]
**
** The enhancement is performed using several steps as detailed in (2)
**  A - Normalization
**  B - Compute Orientation
**  C - Compute Frequency
**  D - Compute Region Mask
**  E - Filter
**
*/

#define P(x,y)      ((int32_t)p[(x)+(y)*pitch])

/* {{{ Filter the parts and improve the ridges */

/* 
** We apply a Gabor filter over the image using the direction and the
** frequency computer in the steps before. The even symetric gabor function
** used is the following
**
**                    / 1|x'²   y'²|\
** h(x,y:phi,f) = exp|- -|--- + ---| |.cos(2.PI.f.x')
**                    \ 2|dx²   dy²|/
**
** x' =  x.cos(phi) + y.sin(phi)
** y' = -x.sin(phi) + y.cos(phi)
**
** Its value is based on empirical data, we choose to set it to 4.0 at first.
** The bigger the value is, mre resistant to noise becomes the algorithm,
** but more likely will he procude spurious ridges.
**
** Let:
**  G be the normalized image
**  O the orientation image
**  F the frequency image
**  R the removable mask image
**  E the resulting enhanced image
**  Wg the size of the gabor filter
**
**          / 255                                          if R(i,j) = 0
**         |
**         |  Wg/2    Wg/2 
**         |  ---     ---
** E(i,j)= |  \       \
**         |   --      --  h(u,v:O(i,j),F(i,j)).G(i-u,j-v) otherwise
**         |  /       /
**          \ ---     ---
**            u=-Wg/2 v=-Wg/2
**
*/
/* helper function computing the gabor filter factor r2 is r*r */
FvsFloat_t EnhanceGabor(FvsFloat_t x, FvsFloat_t y, FvsFloat_t phi, FvsFloat_t f, FvsFloat_t r2)
{
    FvsFloat_t dy2 = 1.0/r2;
    FvsFloat_t dx2 = 1.0/r2;
    FvsFloat_t x2, y2;
    phi += M_PI/2;
    x2 = -x*sin(phi) + y*cos(phi);
    y2 =  x*cos(phi) + y*sin(phi);
    return exp(-0.5*(x2*x2*dx2 + y2*y2*dy2))*cos(2*M_PI*x2*f);
}

static FvsError_t ImageEnhanceFilter
    (
    FvsImage_t        normalized,
    const FvsImage_t  mask,
    const FvsFloat_t* orientation,
    const FvsFloat_t* frequence,
    FvsFloat_t        radius
    )
{
    FvsInt_t Wg2 = 8; /* from -5 to 5 are 11 */
    FvsInt_t i,j, u,v;
    FvsError_t nRet  = FvsOK;
    FvsImage_t enhanced = NULL;

    FvsInt_t w        = ImageGetWidth (normalized);
    FvsInt_t h        = ImageGetHeight(normalized);
    FvsInt_t pitchG   = ImageGetPitch (normalized);
    FvsByte_t* pG     = ImageGetBuffer(normalized);
    FvsFloat_t sum, f, o;
    //printf("%s:%d ... %d\n", __func__, __LINE__, 1);
    /* take square */
    radius = radius*radius;

    enhanced = ImageCreate();
    if (enhanced==NULL || pG==NULL)
        return FvsMemory;
    if (nRet==FvsOK)
        nRet = ImageSetSize(enhanced, w, h);
    
    //printf("%s:%d ... %d\n", __func__, __LINE__, 1);
    if (nRet==FvsOK)
    {
        FvsInt_t pitchE  = ImageGetPitch (enhanced);
        FvsByte_t* pE    = ImageGetBuffer(enhanced);
        if (pE==NULL)
            return FvsMemory;
        
        //printf("%s:%d ... %d\n", __func__, __LINE__, 1);
        (void)ImageClear(enhanced);
        for (j = Wg2; j < h-Wg2; j++) {
            for (i = Wg2; i < w-Wg2; i++)
            {
                //printf("%s:%d ... %d\n", __func__, __LINE__, 1);
                if (mask==NULL || ImageGetPixel(mask, i, j)!=0)
                {
                    sum = 0.0;
                    o = orientation[i+j*w];
                    f = frequence[i+j*w];
                    for (v = -Wg2; v <= Wg2; v++) {
                        for (u = -Wg2; u <= Wg2; u++)
                        {
                            //printf("%s:%d ... %d\n", __func__, __LINE__, 1);
                            sum += EnhanceGabor((FvsFloat_t)u, (FvsFloat_t)v, o,f,radius) * pG[(i-u)+(j-v)*pitchG];
                        }
                    }

                    /* printf("%6.1f ", sum);*/
                    if (sum>255.0) sum = 255.0;
                    if (sum<0.0)   sum = 0.0;
                    pE[i+j*pitchE] = (uint8_t)sum;
                }
            }
        }
        //printf("%s:%d ... %d\n", __func__, __LINE__, 1);
        nRet = ImageCopy(normalized, enhanced);
    }
    (void)ImageDestroy(enhanced);
    //printf("%s:%d ... %d\n", __func__, __LINE__, 1);
    return nRet;
}

/* }}} */

/* Enhance a fingerprint image */
FvsError_t ImageEnhanceGabor(FvsImage_t image, const FvsFloatField_t direction,
            const FvsFloatField_t frequency, const FvsImage_t mask, const FvsFloat_t radius)
{
    FvsError_t nRet = FvsOK;
    FvsFloat_t * image_orientation = FloatFieldGetBuffer(direction);
    FvsFloat_t * image_frequence   = FloatFieldGetBuffer(frequency);

    if (image_orientation==NULL || image_frequence==NULL) {
        return FvsMemory;
    }

    nRet = ImageEnhanceFilter(image, mask, image_orientation, image_frequence, radius);
    return nRet;
}



