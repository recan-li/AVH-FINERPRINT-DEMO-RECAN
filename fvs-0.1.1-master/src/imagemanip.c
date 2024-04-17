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

  Copyright(C) 2002. All Rights Reserved.

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

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

/* {{{ Local stretching */

/* helper macro */
#define PIJKL p[i+k + (j+l)*nSizeX]

/* local stretch */
FvsError_t ImageLocalStretch(FvsImage_t image, const FvsInt_t size, const FvsInt_t tolerance)
{
    /* define a bunch of variables */
    int nSizeX = ImageGetWidth(image)  - size + 1;
    int nSizeY = ImageGetHeight(image) - size + 1;
    FvsInt_t i, j, t, l;
    FvsInt_t sum, denom;
    FvsByte_t a = 0;
    FvsInt_t k = 0;
    FvsByte_t b = 255;
    int hist[256];
    FvsByte_t* p = ImageGetBuffer(image);
    if (p==NULL)
       return FvsMemory;
    for (j=0; j < nSizeY; j+=size)
    {
        for (i=0; i < nSizeX; i+=size)
        {
            /* compute local histogram */
            memset(hist, 0, 256*sizeof(int));
            for (l = 0; l<size; l++)
                for (k = 0; k<size; k++)
                    hist[PIJKL]++;
            
            /* stretch locally */
            for (k=0,   sum=0;   k <256; k++)
            {
                sum+=hist[k];
                a = (FvsByte_t)k;
                if (sum>tolerance) break;
            }
            
            for (k=255, sum=0; k >= 0; k--)
            {
                sum+=hist[k];
                b = (FvsByte_t)k;
                if (sum>tolerance) break;
            }

            denom = (FvsInt_t)(b-a);
            if (denom!=0)
            {
                for (l = 0; l<size; l++)
                {
                    for (k = 0; k<size; k++)
                    {
                        if (PIJKL<a) PIJKL = a;
                        if (PIJKL>b) PIJKL = b;
                        t = (FvsInt_t)((((PIJKL)-a)*255)/denom);
                        PIJKL = (FvsByte_t)(t);
                    }
                }
            }
        }
    }

    return FvsOK;
}
/* }}} */
/* {{{ Fingerprint orientation field */

#define P(x,y)      ((int32_t)p[(x)+(y)*pitch])

/*
** In this step, we estimate the ridge orientation field.
** Given a normalized image G, the main steps of the algorithm are as
** follows:
**
** 1 - Divide G into blocks of w x w - (15 x 15)
**
** 2 - Compute the gradients dx(i,j) and dy(i,j) at each pixel (i,j),
**     depending on the computational requirement, the gradient operator
**     may vary from the single Sobel operator to the more complex Marr-
**     Hildreth operator.
**
** 3 - Estimate the local orientation of each block centered at pixel
**     (i,j), using the following operations:
**
**               i+w/2 j+w/2
**               ---   --- 
**               \     \
**     Nx(i,j) =  --    -- 2 dx(u,v) dy(u,v)
**               /     /
**               ---   ---
**            u=i-w/2 v=j-w/2
**
**               i+w/2 j+w/2
**               ---   --- 
**               \     \
**     Ny(i,j) =  --    -- dx²(u,v) - dy²(u,v)
**               /     /
**               ---   ---
**            u=i-w/2 v=j-w/2
**
**                  1    -1  / Nx(i,j) \
**     Theta(i,j) = - tan   |  -------  |
**                  2        \ Ny(i,j) /
**
**     where Theta(i,j) is the least square estimate of the local ridge
**     orientation at the block centered at pixel (i,j). Mathematically,
**     it represents the direction that is orthogonal to the dominant
**     direction of the Fourier spectrum of the w x w window.
**
** 4 - Due to the presence of noise, corrupted ridge and furrow structures,
**     minutiae, etc. in the input image,the estimated local ridge
**     orientation may not always be a correct estimate. Since local ridge
**     orientation varies slowly in a local neighbourhood, where no
**     singular point appears, a low pass filter can be used to modify the
**     incorrect local ridge orientation. In order to perform the low-pass
**     filtering, the orientation image needs to be converted into a
**     continuous vector field, which is defined as follows:
**       Phi_x(i,j) = cos( 2 x theta(i,j) )
**       Phi_y(i,j) = sin( 2 x theta(i,j) )
**     With the resulting vector field, the low-pass filtering can then
**     be performed with a convolution as follows:
**       Phi2_x(i,j) = (W @ Phi_x) (i,j)
**       Phi2_y(i,j) = (W @ Phi_y) (i,j)
**     where W is a 2D low-pass filter.
**
** 5 - Compute the local ridge orientation at (i,j) with
**
**              1    -1  / Phi2_y(i,j) \
**     O(i,j) = - tan   |  -----------  |
**              2        \ Phi2_x(i,j) /
**
** With this algorithm, a fairly smooth orientatin field estimate can be
** obtained.
**
*/

static FvsError_t FingerprintDirectionLowPass(FvsFloat_t* theta, FvsFloat_t* out,
            FvsInt_t nFilterSize, FvsInt_t w, FvsInt_t h)
{
    FvsError_t nRet = FvsOK;
    FvsFloat_t* filter = NULL;
    FvsFloat_t* phix   = NULL;
    FvsFloat_t* phiy   = NULL;
    FvsFloat_t* phi2x  = NULL;
    FvsFloat_t* phi2y  = NULL;
    FvsInt_t fsize  = nFilterSize*2+1;
    size_t nbytes = (size_t)(w*h*sizeof(FvsFloat_t));
    FvsFloat_t nx, ny;
    FvsInt_t val;
    FvsInt_t i, j, x, y;

    //printf("__HEAP_BASE: %p ~ %p\n", Image$$ARM_LIB_HEAP$$ZI$$Base, Image$$ARM_LIB_HEAP$$ZI$$Base + 0x372000);

    filter= (FvsFloat_t*)malloc((size_t)fsize*fsize*sizeof(FvsFloat_t));
    phix  = (FvsFloat_t*)malloc(nbytes);
    phiy  = (FvsFloat_t*)malloc(nbytes);
    phi2x = (FvsFloat_t*)malloc(nbytes);
    phi2y = (FvsFloat_t*)malloc(nbytes);

    //printf("%s:%d %d %d\n", __func__, __LINE__, (size_t)fsize*fsize*sizeof(FvsFloat_t), nbytes);
    //printf("%s:%d %p %p %p %p %p\n", __func__, __LINE__, filter, phix, phiy, phi2x, phi2y);

    if (filter==NULL || phi2x==NULL || phi2y==NULL || phix==NULL || phiy==NULL) {
        printf("%s:%d\n", __func__, __LINE__);
        nRet = FvsMemory;
    }
    else
    {
        /* reset all fields to 0 */
        memset(filter, 0, (size_t)fsize*fsize*sizeof(FvsFloat_t));
        memset(phix,   0, nbytes);
        memset(phiy,   0, nbytes);
        memset(phi2x,  0, nbytes);
        memset(phi2y,  0, nbytes);

        /* 4 - Compute a continuous field from theta */
        for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
            val = x+y*w;
            phix[val] = cos(theta[val]);
            phiy[val] = sin(theta[val]);
        }
        /* build the low-pass filter */
        nx = 0.0;
        for (j = 0; j < fsize; j++)
        for (i = 0; i < fsize; i++)
        {
            filter[j*fsize+i] = 1.0;
/*
            filter[j*fsize+i] = (FvsFloat_t)(fsize - (abs(nFilterSize-i)+abs(nFilterSize-j)));
*/
            nx += filter[j*fsize+i]; /* sum of coefficients */
        }
        if (nx>1.0)
        {
            for (j = 0; j < fsize; j++)
            for (i = 0; i < fsize; i++)
                /* normalize the result */
                filter[j*fsize+i] /= nx;
        }
        /* low-pass on the result arrays getting phi2 */
        for (y = 0; y < h-fsize; y++)
        for (x = 0; x < w-fsize; x++)
        {
            nx = 0.0;
            ny = 0.0;
            for (j = 0; j < fsize; j++)
            for (i = 0; i < fsize; i++)
            {
                val = (x+i)+(j+y)*w;
                nx += filter[j*fsize+i]*phix[val];
                ny += filter[j*fsize+i]*phiy[val];
            }
            val = x+y*w;
            phi2x[val] = nx;
            phi2y[val] = ny;
        }
        /* we do not need phix, phiy anymore, delete them */
        if (phix!=NULL) { free(phix); phix=NULL; }
        if (phiy!=NULL) { free(phiy); phiy=NULL; }

        /* 5 - local ridge orientation -> theta */
        for (y = 0; y < h-fsize; y++)
        for (x = 0; x < w-fsize; x++)
        {
            val = x+y*w;
            out[val] = atan2(phi2y[val], phi2x[val])*0.5;
        }
    }

    if (phix!=NULL)  free(phix);
    if (phiy!=NULL)  free(phiy);
    if (phi2x!=NULL) free(phi2x);
    if (phi2y!=NULL) free(phi2y);
    if (filter!=NULL)free(filter);

    return nRet;
}


FvsError_t FingerprintGetDirection(const FvsImage_t image, FvsFloatField_t field,
            const FvsInt_t nBlockSize, const FvsInt_t nFilterSize)
{
    /* width & height of the input image */
    FvsInt_t w       = ImageGetWidth (image);
    FvsInt_t h       = ImageGetHeight(image);
    FvsInt_t pitch   = ImageGetPitch (image);
    FvsByte_t* p     = ImageGetBuffer(image);
    FvsInt_t i, j, u, v, x, y;
    FvsInt_t s = nBlockSize*2+1;
    FvsFloat_t dx[s*s];
    FvsFloat_t dy[s*s];
    FvsFloat_t nx, ny;
    FvsFloat_t* out;
    FvsFloat_t* theta  = NULL;
    FvsError_t nRet = FvsOK;

    /* (re-)allocate the output image */
    nRet = FloatFieldSetSize(field, w, h);
    if (nRet!=FvsOK) {
        printf("%s:%d\n", __func__, __LINE__);
        return nRet;
    }
    nRet = FloatFieldClear(field);
    if (nRet!=FvsOK) {
        printf("%s:%d\n", __func__, __LINE__);
        return nRet;
    }
    out = FloatFieldGetBuffer(field);

    /* allocate memory for the orientation values */
    if (nFilterSize>0)
    {
        theta = (FvsFloat_t*)malloc(w * h * sizeof(FvsFloat_t));
        if (theta!=NULL)
            memset(theta, 0, (w * h * sizeof(FvsFloat_t)));
    }

    /* detect any allocation error */
    if (out==NULL || (nFilterSize>0 && theta==NULL)) {
        printf("%s:%d\n", __func__, __LINE__);
        nRet = FvsMemory;
    }
    else
    {
        /* 1 - divide the image in blocks */
        for (y = nBlockSize+1; y < h-nBlockSize-1; y++)
        for (x = nBlockSize+1; x < w-nBlockSize-1; x++)
        {
            /* 2 - for the block centered at x,y compute the gradient */
            for (j = 0; j < s; j++)
            for (i = 0; i < s; i++)
            {
                dx[i*s+j] = (FvsFloat_t)
                           (P(x+i-nBlockSize,   y+j-nBlockSize) -
                            P(x+i-nBlockSize-1, y+j-nBlockSize));
                dy[i*s+j] = (FvsFloat_t)
                           (P(x+i-nBlockSize,   y+j-nBlockSize) -
                            P(x+i-nBlockSize,   y+j-nBlockSize-1));
            }

            /* 3 - compute orientation */
            nx = 0.0;
            ny = 0.0;
            for (v = 0; v < s; v++)
            for (u = 0; u < s; u++)
            {
                nx += 2 * dx[u*s+v] * dy[u*s+v];
                ny += dx[u*s+v]*dx[u*s+v] - dy[u*s+v]*dy[u*s+v];
            }
            /* compute angle (-pi/2 .. pi/2) */
            if (nFilterSize>0)
                theta[x+y*w] = atan2(nx, ny);
            else
                out[x+y*w] = atan2(nx, ny)*0.5;
        }
        if (nFilterSize>0) {
            nRet = FingerprintDirectionLowPass(theta, out, nFilterSize, w, h);
            //printf("%s:%d\n", __func__, __LINE__);
        }
    }

    if (theta!=NULL) free(theta);
    return nRet;
}

/* }}} */
/* {{{ Fingerprint frequency field */

/* 
** In this step, we estimate the ridge frequency. In a local neighbour-
** hood where no minutiae and singular points appear, the gray levels
** along ridges and furrows can be modelled as a sinusoidal-shaped wave
** along a direction normal to the local ridge orientation. Therefore,
** local ridge frequency is another intrinsic property of a fingerprint
** image. Let G be a normalized image G, and O be the orientation image
** (computed at step B). Then the steps involved in local ridge 
** frequency estimation are as follows:
**
** 1 - Divide G into blocks of w x w - (16 x 16)
**
** 2 - For each block centered at pixel (i,j), compute an oriented
**     window of size l x w (32 x 16) that is defined in the ridge
**     coordinates system.
**
** 3 - For each block centered at pixel (i,j), compute the x-signature
**     X[0], X[1], ... X[l-1] of the ridges and furrows within the
**     oriented window where:
**
**              --- w-1
**            1 \
**     X[k] = -  --  G (u, v), k = 0, 1, ..., l-1
**            w /
**              --- d=0
**
**     u = i + (d - w/2).cos O(i,j) + (k - l/2).sin O(i,j)
**
**     v = j + (d - w/2).sin O(i,j) - (k - l/2).cos O(i,j)
**
**     If no minutiae and singular points appear in the oriented window,
**     the x-signature forms a discrete sinusoidal-shape wave, which has
**     the same frequency as that of the ridges and furrows in the
**     oriented window. Therefore, the frequency of ridges and furrows
**     can be estimated from the x-signature. Let T(i,j) be the average
**     number of pixels between two consecutive peaks in the x-signature,
**     then the frequency, OHM(i,j) is computed as OHM(i,j) = 1 / T(i,j).
**
**     If no consecutive peaks can be detected from the x-signature, then
**     the frequency is assigned a value of -1, to differenciate it from
**     the valid frequency values.
**
** 4 - For a fingerprint image scanned at a fixed resolution, the value
**     of the frequency of the ridges and furrows in a local neighbour-
**     hood lies in a certain range. For a 500 dpi image, this range is
**     [1/3, 1/25]. Therefore, if the estimated value of the frequency
**     is out of this range, then the frequency is assigned a value of
**     -1 to indicate that an valid frequency cannot be obtained.
**
** 5 - The blocks in which minutiae and/or singular points appear and/or
**     ridges and furrows are corrupted do not form a well defined
**     sinusoidal-shaped wave. The frequency value for these blocks need
**     to be interpolated from the frequency of the neighbouring blocks
**     which have a well-defined frequency. (Ex: Gaussian kernel mean 0,
**     and variance 9 and of size 7).
**
** 6 - Inter-ridges distance change slowly in a local neighbourhood. A
**     low-pass filter can be used to remove the outliers.
**
*/

/* width */
#define BLOCK_W     16
#define BLOCK_W2     8

/* length */
#define BLOCK_L     32
#define BLOCK_L2    16

#define EPSILON     0.0001
#define LPSIZE      3

#define LPFACTOR    (1.0/((LPSIZE*2+1)*(LPSIZE*2+1)))


FvsError_t FingerprintGetFrequency(const FvsImage_t image, const FvsFloatField_t direction,
           FvsFloatField_t frequency)
{
    /* width & height of the input image */
    FvsError_t nRet = FvsOK;
    FvsInt_t w      = ImageGetWidth (image);
    FvsInt_t h      = ImageGetHeight(image);
    FvsInt_t pitchi = ImageGetPitch (image);
    FvsByte_t* p    = ImageGetBuffer(image);
    FvsFloat_t* out;
    FvsFloat_t* freq;
    FvsFloat_t* orientation = FloatFieldGetBuffer(direction);

    FvsInt_t x, y, u, v, d, k;
    size_t size;

    if (p==NULL)
        return FvsMemory;

    /* (re-)allocate the output image */
    nRet = FloatFieldSetSize(frequency, w, h);
    if (nRet!=FvsOK) return nRet;
    (void)FloatFieldClear(frequency);
    freq = FloatFieldGetBuffer(frequency);
    if (freq==NULL)
        return FvsMemory;

    /* allocate memory for the output */
    size = w*h*sizeof(FvsFloat_t);
    out  = (FvsFloat_t*)malloc(size);
    if (out!=NULL)
    {
        FvsFloat_t dir = 0.0;
        FvsFloat_t cosdir = 0.0;
        FvsFloat_t sindir = 0.0;

        FvsInt_t peak_pos[BLOCK_L]; /* peak positions */
        FvsInt_t peak_cnt;          /* peak count     */
        FvsFloat_t peak_freq;         /* peak frequence */
        FvsFloat_t Xsig[BLOCK_L];     /* x signature    */
        FvsFloat_t pmin, pmax;

        memset(out,  0, size);
        memset(freq, 0, size);

        /* 1 - Divide G into blocks of BLOCK_W x BLOCK_W - (16 x 16) */
        for (y = BLOCK_L2; y < h-BLOCK_L2; y++)
        for (x = BLOCK_L2; x < w-BLOCK_L2; x++)
        {
            /* 2 - oriented window of size l x w (32 x 16) in the ridge dir */
            dir = orientation[(x+BLOCK_W2) + (y+BLOCK_W2)*w];
            cosdir = -sin(dir);  /* ever > 0 */
            sindir = cos(dir);   /* -1 ... 1 */

            /* 3 - compute the x-signature X[0], X[1], ... X[l-1] */
            for (k = 0; k < BLOCK_L; k++)
            {
                Xsig[k] = 0.0;
                for (d = 0; d < BLOCK_W; d++)
                {
                    u = (FvsInt_t)(x + (d-BLOCK_W2)*cosdir + (k-BLOCK_L2)*sindir);
                    v = (FvsInt_t)(y + (d-BLOCK_W2)*sindir - (k-BLOCK_L2)*cosdir);
                    /* clipping */
                    if (u<0) u = 0; else if (u>w-1) u = w-1;
                    if (v<0) v = 0; else if (v>h-1) v = h-1;
                    Xsig[k] += p[u + (v*pitchi)];
                }
                Xsig[k] /= BLOCK_W;
            }

            /* Let T(i,j) be the avg number of pixels between 2 peaks */
            /* find peaks in the x signature */
            peak_cnt = 0;
            /* test if the max - min or peak to peak value too small is,
            then we ignore this point */
            pmax = pmin = Xsig[0];
            for (k = 1; k < BLOCK_L; k++)
            {
                if (pmin>Xsig[k]) pmin = Xsig[k];
                if (pmax<Xsig[k]) pmax = Xsig[k];
            }
            if ((pmax - pmin)>64.0)
            {
                for (k = 1; k < BLOCK_L-1; k++)
                if ((Xsig[k-1] < Xsig[k]) && (Xsig[k] >= Xsig[k+1]))
                {
                    peak_pos[peak_cnt++] = k;
                }
            }
            /* compute mean value */
            peak_freq = 0.0;
            if (peak_cnt>=2)
            {
                for (k = 0; k < peak_cnt-1; k++)
                    peak_freq += peak_pos[k+1]-peak_pos[k];
                peak_freq /= peak_cnt-1;
            }
            /* 4 - must lie in a certain range [1/25-1/3] */
            /*     changed to range [1/30-1/2] */
            if (peak_freq > 30.0)
                out[x+y*w] = 0.0;
            else if (peak_freq < 2.0)
                out[x+y*w] = 0.0;
            else
                out[x+y*w] = 1.0/peak_freq;
        }
        /* 5 - interpolated ridge period for the unknown points */
        for (y = BLOCK_L2; y < h-BLOCK_L2; y++)
        for (x = BLOCK_L2; x < w-BLOCK_L2; x++)
        {
            if (out[x+y*w]<EPSILON)
            {
                if (out[x+(y-1)*w]>EPSILON)
                {
                    out[x+(y*w)] = out[x+(y-1)*w];
                }
                else
                {
                    if (out[x-1+(y*w)]>EPSILON)
                        out[x+(y*w)] = out[x-1+(y*w)];
                }
            }
        }
        /* 6 - Inter-ridges distance change slowly in a local neighbourhood */
        for (y = BLOCK_L2; y < h-BLOCK_L2; y++)
        for (x = BLOCK_L2; x < w-BLOCK_L2; x++)
        {
            k = x + y*w;
            peak_freq = 0.0;
            for ( v = -LPSIZE; v <= LPSIZE; v++)
            for ( u = -LPSIZE; u <= LPSIZE; u++)
                peak_freq += out[(x+u)+(y+v)*w];
            freq[k] = peak_freq*LPFACTOR;
        }
        free(out);
    }
    return nRet;
}

/* }}} */
/* {{{ Fingerprint mask */

FvsError_t FingerprintGetMask(const FvsImage_t image, /*@unused@*/ const FvsFloatField_t direction,
           const FvsFloatField_t frequency, FvsImage_t mask)
{
    FvsError_t nRet = FvsOK;
    FvsFloat_t freqmin = 1.0 / 25;
    FvsFloat_t freqmax = 1.0 / 3;

    /* width & height of the input image */
    FvsInt_t w      = ImageGetWidth (image);
    FvsInt_t h      = ImageGetHeight(image);
    FvsByte_t* out;
    FvsInt_t pitchout;
    FvsInt_t pos, posout, x, y;
    FvsFloat_t* freq = FloatFieldGetBuffer(frequency);

    if (freq==NULL)
        return FvsMemory;

    /* TODO: add sanity checks for the direction and mask */
    nRet = ImageSetSize(mask, w, h);
    if (nRet==FvsOK)
        nRet = ImageClear(mask);
    out = ImageGetBuffer(mask);
    if (out==NULL)
        return FvsMemory;
    if (nRet==FvsOK)
    {
    pitchout = ImageGetPitch(mask);

    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
            pos    = x + y * w;
            posout = x + y * pitchout;
            out[posout] = 0;
            if (freq[pos] >= freqmin && freq[pos] <= freqmax)
            {
/*              out[posout] = (uint8_t)(10.0/freq[pos]);*/
                out[posout] = 255;
            }
        }
    /* fill in the holes */
    for (y = 0; y < 4; y++)
        (void)ImageDilate(mask);
    /* remove borders */
    for (y = 0; y < 12; y++)
        (void)ImageErode(mask);
    }
    return nRet;
}


/* }}} */

/* {{{ Thinning algorithms */
/* {{{ -> Thin: Using connectivity */

#undef P
#define P(x,y)      ((x)+(y)*pitch)
#define REMOVE_P    { p[P(x,y)]=0x80; changed = FvsTrue; }


/*
From     : Nadeem Ahmed (umahmed@cc.umanitoba.ca)
Subject  : Thinning Algorithm
Newsgroup: borland.public.cppbuilder.graphics
Date :1998/02/23 

Here is that thinning algorithm:
                            9 2 3
                            8 1 4
                            7 5 6
For each pixel(#1, above), we have 8 neighbors (#'s 2-8).

Step 1:
a) Make sure pixel 1, has 2 to 6 (inclusive) neighbors
b) starting from 2, go clockwise until 9, and count the
   number of 0 to 1 transitions.  This should be equal to 1.
c) 2*4*6=0  (ie either 2,4 ,or 6 is off)
d) 4*6*8=0

if these conditions hold, remove pixel 1.
Do this for the entire image.

Step 2
a) same as above
b) same as above
c) 2*6*8=0
d) 2*4*8=0

if these hold remove pixel 1.
*/
/* defines to facilitate reading */
#define P1  p[P(x  ,y  )]
#define P2  p[P(x  ,y-1)]
#define P3  p[P(x+1,y-1)]
#define P4  p[P(x+1,y  )]
#define P5  p[P(x+1,y+1)]
#define P6  p[P(x  ,y+1)]
#define P7  p[P(x-1,y+1)]
#define P8  p[P(x-1,y  )]
#define P9  p[P(x-1,y-1)]


FvsError_t ImageRemoveSpurs(FvsImage_t image)
{
    FvsInt_t w       = ImageGetWidth(image);
    FvsInt_t h       = ImageGetHeight(image);
    FvsInt_t pitch   = ImageGetPitch(image);
    FvsByte_t* p     = ImageGetBuffer(image);
    FvsInt_t x, y, n, t, c;

    /* Thanks to Tony Xu for contributing this section that improves the quality
     of the binarized image by getting rid of the extra pixels (spurs)

    jdh: improvment by using 2 steps, the previous algorithm had the bad
    habbit to remove completely some lines oriented in a special way... */
    c = 0;
    
    do 
    {
	n = 0;    
	for (y=1; y<h-1; y++)
        for (x=1; x<w-1; x++)
        {
            if( p[P(x,y)]==0xFF)
            {
                t=0;
                if (P3==0 && P2!=0 && P4==0) t++;
                if (P5==0 && P4!=0 && P6==0) t++;
                if (P7==0 && P6!=0 && P8==0) t++;
                if (P9==0 && P8!=0 && P2==0) t++;
                if (P3!=0 && P4==0) t++;
                if (P5!=0 && P6==0) t++;
                if (P7!=0 && P8==0) t++;
                if (P9!=0 && P2==0) t++;
                if (t==1)
		{
                    p[P(x,y)] = 0x80;
		    n++;	    
		}
 
            }
        }
	for (y=1; y<h-1; y++)
        for (x=1; x<w-1; x++)
        {
            if( p[P(x,y)]==0x80)
	        p[P(x,y)] = 0;
	}
    
    } while (n>0 && ++c < 5);
        
    return FvsOK;
}


/* a) Make sure it has 2 to 6 neighbours */
#define STEP_A  n = 0; /* number of neighbours */ \
                if (P2!=0) n++; if (P3!=0) n++; if (P4!=0) n++; if (P5!=0) n++; \
                if (P6!=0) n++; if (P7!=0) n++; if (P8!=0) n++; if (P9!=0) n++; \
                if (n>=2 && n<=6)

/* b) count number 0 to 1 transsitions */
#define STEP_B  t = 0; /* number of transitions */ \
                if (P9==0 && P2!=0) t++; if (P2==0 && P3!=0) t++; \
                if (P3==0 && P4!=0) t++; if (P4==0 && P5!=0) t++; \
                if (P5==0 && P6!=0) t++; if (P6==0 && P7!=0) t++; \
                if (P7==0 && P8!=0) t++; if (P8==0 && P9!=0) t++; \
                if (t==1)
 
FvsError_t ImageThinConnectivity(FvsImage_t image)
{
    FvsInt_t w       = ImageGetWidth(image);
    FvsInt_t h       = ImageGetHeight(image);
    FvsInt_t pitch   = ImageGetPitch(image);
    FvsByte_t* p     = ImageGetBuffer(image);
    FvsInt_t x, y, n, t;
    FvsBool_t changed = FvsTrue;

    if (p==NULL)
        return FvsMemory;
    if (ImageGetFlag(image)!=FvsImageBinarized)
        return FvsBadParameter;

    while (changed==FvsTrue)
    {
        changed = FvsFalse;
        for (y=1; y<h-1; y++)
        for (x=1; x<w-1; x++)
        {
            if (p[P(x,y)]==0xFF)
            {
                STEP_A
                {
                    STEP_B
                    {
                        /*
                        c) 2*4*6=0  (ie either 2,4 ,or 6 is off)
                        d) 4*6*8=0
                        */
                        if (P2*P4*P6==0 && P4*P6*P8==0)
                            REMOVE_P;

                    }
                }
            }
        }

        for (y=1; y<h-1; y++)
        for (x=1; x<w-1; x++)
            if (p[P(x,y)]==0x80)
                p[P(x,y)] = 0;

        for (y=1; y<h-1; y++)
        for (x=1; x<w-1; x++)
        {
            if (p[P(x,y)]==0xFF)
            {
                STEP_A
                {
                    STEP_B
                    {
                        /*
                        c) 2*6*8=0
                        d) 2*4*8=0
                        */
                        if (P2*P6*P8==0 && P2*P4*P8==0)
                            REMOVE_P;

                    }
                }
            }
        }

        for (y=1; y<h-1; y++)
        for (x=1; x<w-1; x++)
            if (p[P(x,y)]==0x80)
                p[P(x,y)] = 0;
    }

    ImageRemoveSpurs(image);
    
    return ImageSetFlag(image, FvsImageThinned);
}

 
	
/* redefine REMOVE_P */
#undef REMOVE_P

/* }}} */
/* {{{ -> Thin: Hit and miss */

#define REMOVE_P    { p[P(x,y)]=0x00; changed = FvsTrue; }

/*
// jdh: Second thinning algorithm based on a Hit and Miss transformation
*/
FvsError_t ImageThinHitMiss(FvsImage_t image)
{
    FvsInt_t w      = ImageGetWidth(image);
    FvsInt_t h      = ImageGetHeight(image);
    FvsInt_t pitch  = ImageGetPitch(image);
    FvsByte_t* p    = ImageGetBuffer(image);

    /*
    // Hit and Miss structuring elements for thinning
    //
    // this algo has the disadvantage to produce spurious lines resulting
    // from the skeletonization. These may be eliminated by another algorithm.
    // postprocessing is then still needed afterwards.
    // 
    // 0 0 0      0 0 
    //   1      1 1 0
    // 1 1 1      1
    //
    */
    FvsInt_t x,y, t;
    FvsBool_t changed = FvsTrue;

    if (p==NULL)
        return FvsMemory;

    if (ImageGetFlag(image)!=FvsImageBinarized)
        return FvsBadParameter;

    while (changed==FvsTrue)
    {
        changed = FvsFalse;
        for (y=1; y<h-1; y++)
        for (x=1; x<w-1; x++)
        {
            if (p[P(x,y)]==0xFF)
            {
                /*
                // 0 0 0  0   1  1 1 1  1   0
                //   1    0 1 1    1    1 1 0
                // 1 1 1  0   1  0 0 0  1   0
                */
                if (p[P(x-1,y-1)]==0 && p[P(x,y-1)]==0 && p[P(x+1,y-1)]==0 &&
                    p[P(x-1,y+1)]!=0 && p[P(x,y+1)]!=0 && p[P(x+1,y+1)]!=0)
                    REMOVE_P;

                if (p[P(x-1,y-1)]!=0 && p[P(x,y-1)]!=0 && p[P(x+1,y-1)]!=0 &&
                    p[P(x-1,y+1)]==0 && p[P(x,y+1)]==0 && p[P(x+1,y+1)]==0)
                    REMOVE_P;

                if (p[P(x-1,y-1)]==0 && p[P(x-1,y)]==0 && p[P(x-1,y+1)]==0 &&
                    p[P(x+1,y-1)]!=0 && p[P(x+1,y)]!=0 && p[P(x+1,y+1)]!=0)
                    REMOVE_P;

                if (p[P(x-1,y-1)]!=0 && p[P(x-1,y)]!=0 && p[P(x-1,y+1)]!=0 &&
                    p[P(x+1,y-1)]==0 && p[P(x+1,y)]==0 && p[P(x+1,y+1)]==0)
                    REMOVE_P;

                /*
                //   0 0  0 0      1      1  
                // 1 1 0  0 1 1  0 1 1  1 1 0
                //   1      1    0 0      0 0
                */                
                if (p[P(x,y-1)]==0 && p[P(x+1,y-1)]==0 && p[P(x+1,y)]==0 &&
                    p[P(x-1,y)]!=0 && p[P(x,y+1)]!=0)
                    REMOVE_P;

                if (p[P(x-1,y-1)]==0 && p[P(x,y-1)]==0 && p[P(x-1,y)]==0 &&
                    p[P(x+1,y)]!=0 && p[P(x,y+1)]!=0)
                    REMOVE_P;

                if (p[P(x-1,y+1)]==0 && p[P(x-1,y)]==0 && p[P(x,y+1)]==0 &&
                    p[P(x+1,y)]!=0 && p[P(x,y-1)]!=0)
                    REMOVE_P;

                if (p[P(x+1,y+1)]==0 && p[P(x+1,y)]==0 && p[P(x,y+1)]==0 &&
                    p[P(x-1,y)]!=0 && p[P(x,y-1)]!=0)
                    REMOVE_P;                
            }
        }
    }
    
    ImageRemoveSpurs(image);

    return ImageSetFlag(image, FvsImageThinned);
}

/* }}} */
/* }}} */





