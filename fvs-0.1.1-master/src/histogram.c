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
  
########################################################################*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "histogram.h"

/*
** A histogram allows faster computing of some statistical image properties
** such as the mean value, variance,...
** This histogram specializes itself for 8 bit gray level images.
*/
typedef struct iFvsHistogram_t
{
    FvsUint_t       ptable[256];    /* 8 bit image histogram */
    FvsInt_t        ncount;         /* number of points in the histogram */
    FvsInt_t        nmean;          /* -1 = not computed yet */
    FvsInt_t        nvariance;      /* -1 = not computed yet */
} iFvsHistogram_t;



FvsHistogram_t HistogramCreate()
{
    iFvsHistogram_t* p = NULL;
    p = (FvsHistogram_t)malloc(sizeof(iFvsHistogram_t));

    if (p!=NULL)
    {
        /* reset the table */
        HistogramReset(p);
    }
    return (FvsHistogram_t)p;
}


void HistogramDestroy(FvsHistogram_t histogram)
{
    iFvsHistogram_t* p = NULL;
    if (histogram==NULL)
        return;
    p = histogram;
    free(p);
}


FvsError_t HistogramReset(FvsHistogram_t hist)
{
    iFvsHistogram_t* histogram = (iFvsHistogram_t*)hist;
    int i;
    for (i = 0; i < 256; i++)
        histogram->ptable[i] = 0;
    histogram->ncount    = 0;
    histogram->nmean     = -1;
    histogram->nvariance = -1;
    return FvsOK;
}


FvsError_t HistogramCompute(FvsHistogram_t hist, const FvsImage_t image)
{
    iFvsHistogram_t* histogram = (iFvsHistogram_t*)hist;
    FvsError_t nRet = FvsOK;
    FvsInt_t w      = ImageGetWidth(image);
    FvsInt_t h      = ImageGetHeight(image);
    FvsInt_t pitch  = ImageGetPitch(image);
    uint8_t* p      = ImageGetBuffer(image);
    FvsInt_t x, y;

    if (histogram==NULL || p==NULL)
        return FvsMemory;

    /* reset the histogram first */
    nRet = HistogramReset(hist);
    /* now compute the histogram */
    if (nRet==FvsOK)
    {
        FvsInt_t pos;
        for (y=0; y<h; y++)
        {
            pos = pitch*y;
            for (x=0; x<w; x++)
            {
                histogram->ptable[p[pos++]]++;
            }
        }
        histogram->ncount = w*h;
    }

    return nRet;
}


/* Get the mean of an histogram */
FvsByte_t HistogramGetMean(const FvsHistogram_t hist)
{
    iFvsHistogram_t* histogram = (iFvsHistogram_t*)hist;
    FvsInt_t val, i;

    val = histogram->nmean;
    if (val==-1)
    {
        val = 0;
        for (i = 1; i < 255; i++)
            val += i*histogram->ptable[i];

        i = histogram->ncount;
        if (i>0)
            val = val/i;
        else
            val = 0;

        histogram->nmean = val;
    }
    return (uint8_t)val;
}


/* Get the variance of an histogram */
FvsUint_t HistogramGetVariance(const FvsHistogram_t hist)
{
    iFvsHistogram_t* histogram = (iFvsHistogram_t*)hist;
    FvsInt_t val;
    FvsInt_t i;
    uint8_t mean;

    val = histogram->nvariance;
    if (val==-1)
    {
        /* we need the mean value */
        mean = HistogramGetMean(hist);
        val  = 0;
        for (i = 0; i < 255; i++)
            val += histogram->ptable[i]*(i - mean)*(i - mean);

        i = histogram->ncount;
        if (i>0)
            val = val/i;
        else
            val = 0;

        histogram->nvariance = val;
    }
    return (FvsUint_t)val;
}



