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
  
  This file contains functions to read and write BMP files

########################################################################*/

#include <string.h>
#include <stdlib.h>
#if (CFG_ARM_MATH_ENABLE)
#include "arm_math.h"
#else
#include <math.h>
#endif

#include "minutia.h"


typedef struct iFvsMinutiaSet_t
{
    FvsInt_t     nbminutia; /* number of minutia set in the table */
    FvsInt_t     tablesize; /* max number of minutia that can be stored in the table */
    FvsMinutia_t ptable[1]; /* the minutia table starts here */
} iFvsMinutiaSet_t;




FvsMinutiaSet_t MinutiaSetCreate(const FvsInt_t size)
{
    iFvsMinutiaSet_t* p = NULL;
    p = (iFvsMinutiaSet_t*)malloc(sizeof(iFvsMinutiaSet_t)+size*sizeof(FvsMinutia_t));
    if (p!=NULL)
    {   /* no minutia in table yet */
        p->nbminutia = 0;
        p->tablesize = size;
    }    
    return (FvsMinutiaSet_t)p;
}


void MinutiaSetDestroy(FvsMinutiaSet_t minutia)
{
    iFvsMinutiaSet_t* p = NULL;
    if (minutia==NULL)
        return;
    p = minutia;
    free(p);
}


/* returns the maximum number of minutia the table can contain */
FvsInt_t MinutiaSetGetSize(const FvsMinutiaSet_t min)
{
    const iFvsMinutiaSet_t* minutia = (iFvsMinutiaSet_t*)min;
    FvsInt_t nret = 0;
    
    if (minutia!=NULL)
        nret = minutia->tablesize;

    return nret;
}


/* returns the number of minutia in the set */
FvsInt_t MinutiaSetGetCount(const FvsMinutiaSet_t min)
{
    const iFvsMinutiaSet_t* minutia = (iFvsMinutiaSet_t*)min;
    FvsInt_t nret = 0;
    
    if (minutia!=NULL)
        nret = minutia->nbminutia;

    return nret;
}


/* returns a pointer to the table of minutia */
FvsMinutia_t* MinutiaSetGetBuffer(FvsMinutiaSet_t min)
{
    iFvsMinutiaSet_t* minutia = (iFvsMinutiaSet_t*)min;
    FvsMinutia_t* pret = NULL;

    if (minutia!=NULL)
        pret = minutia->ptable;

    return pret;
}


/* empty the minutia set */
FvsError_t MinutiaSetEmpty(FvsMinutiaSet_t min)
{
    iFvsMinutiaSet_t* minutia = (iFvsMinutiaSet_t*)min;
    FvsError_t nRet = FvsOK;

    if (minutia!=NULL)
        minutia->nbminutia = 0;
    else
        nRet = FvsMemory;

    return nRet;
}


FvsError_t MinutiaSetAdd(FvsMinutiaSet_t min,
       const FvsFloat_t x, const FvsFloat_t y,
       const FvsMinutiaType_t type, const FvsFloat_t angle)
{
    iFvsMinutiaSet_t* minutia = (iFvsMinutiaSet_t*)min;
    FvsError_t nRet = FvsOK;

    if (minutia->nbminutia < minutia->tablesize)
    {
        minutia->ptable[minutia->nbminutia].x       = x;
        minutia->ptable[minutia->nbminutia].y       = y;
        minutia->ptable[minutia->nbminutia].type    = type;
        minutia->ptable[minutia->nbminutia].angle   = angle;
        minutia->nbminutia++;
    }
    else
        /* no place left in table */
        nRet = FvsMemory;

    return nRet;
}


static FvsError_t MinutiaSetCheckClean(FvsMinutiaSet_t min)
{
    iFvsMinutiaSet_t* minutia = (iFvsMinutiaSet_t*)min;
    FvsError_t    nRet = FvsOK;
    FvsFloat_t    tx, ty, ta; /* tolerances */
    FvsInt_t      i, j;
    FvsMinutia_t* mi, *mj;

    tx = 4.0;
    ty = 4.0;
    ta = 0.5;

    if (minutia!=NULL && minutia->nbminutia > 1)
    {
        /* test if a minutia with these coordinates, type and
           angle is already in the table */
        for (j = 0;   j < minutia->nbminutia; j++)
        for (i = j+1; i < minutia->nbminutia; i++)
        {
            mi = minutia->ptable + i;
            mj = minutia->ptable + j;

            /* compare minutia i and j in position/angle */

            /* RULE 1: Similar minutia that are near each other -> Remove 1 */
            if ( (fabs(mi->x     - mj->x    ) < tx) &&
                 (fabs(mi->y     - mj->y    ) < ty) &&
                 (fabs(mi->angle - mj->angle) < ta)
               )
            {
                minutia->nbminutia--;
                *mi = minutia->ptable[minutia->nbminutia];
            }

            /* RULE 2: Minutia that are near each other and opposite directions -> Remove both */
        }
    }
    else
        /* no place left in table */
        nRet = FvsMemory;

    return nRet;
}


#define P(x,y)      p[(x)+(y)*pitch]


/* draw the mintuia set into the image */
FvsError_t MinutiaSetDraw(const FvsMinutiaSet_t min, FvsImage_t image)
{
    FvsInt_t w       = ImageGetWidth(image);
    FvsInt_t h       = ImageGetHeight(image);
    FvsInt_t pitch   = ImageGetPitch(image);
    FvsByte_t* p     = ImageGetBuffer(image);

    FvsInt_t n, x, y;
    FvsFloat_t fx, fy;
    FvsMinutia_t* minutia = MinutiaSetGetBuffer(min);
    FvsInt_t nbminutia    = MinutiaSetGetCount(min);

    if (minutia==NULL || p==NULL)
        return FvsMemory;

    /* draw each minutia */
    for (n = 0; n < nbminutia; n++)
    {
        x = (FvsInt_t)minutia[n].x;
        y = (FvsInt_t)minutia[n].y;
        if (x<w-5 && x>4)
        {
            if (y<h-5 && y>4)
            {
                switch (minutia[n].type)
                {
                case FvsMinutiaTypeEnding:
                    P(x,y)    = 0xFF;
                    P(x-1, y) = 0xA0;
                    P(x+1, y) = 0xA0;
                    P(x, y-1) = 0xA0;
                    P(x, y+1) = 0xA0;
                    break;
                case FvsMinutiaTypeBranching:
                    P(x,y)    = 0xFF;
                    P(x-1, y-1) = 0xA0;
                    P(x+1, y-1) = 0xA0;
                    P(x-1, y+1) = 0xA0;
                    P(x+1, y+1) = 0xA0;
                    break;
                default:
                    continue;
                }
                fx = sin(minutia[n].angle);
                fy = -cos(minutia[n].angle);
                P(x+(int32_t)(fx)    ,y+(int32_t)(fy)    ) = 0xFF;
                P(x+(int32_t)(fx*2.0),y+(int32_t)(fy*2.0)) = 0xFF;
                P(x+(int32_t)(fx*3.0),y+(int32_t)(fy*3.0)) = 0xFF;
                P(x+(int32_t)(fx*4.0),y+(int32_t)(fy*4.0)) = 0xFF;
                P(x+(int32_t)(fx*5.0),y+(int32_t)(fy*5.0)) = 0xFF;
            }
        }
    }
    return FvsOK;
}


/* defines to facilitate reading */
#define P1  P(x  ,y-1)
#define P2  P(x+1,y-1)
#define P3  P(x+1,y  )
#define P4  P(x+1,y+1)
#define P5  P(x  ,y+1)
#define P6  P(x-1,y+1)
#define P7  P(x-1,y  )
#define P8  P(x-1,y-1)


/*
** This function locates the minutia and compute their properties
*/
FvsError_t MinutiaSetExtract
    (
    FvsMinutiaSet_t       minutia,
    const FvsImage_t      image,
    const FvsFloatField_t direction,
    const FvsImage_t      mask
    )
{
    FvsInt_t w      = ImageGetWidth(image);
    FvsInt_t h      = ImageGetHeight(image);
    FvsInt_t pitch  = ImageGetPitch(image);
    FvsInt_t pitchm = ImageGetPitch(mask);
    FvsByte_t* p    = ImageGetBuffer(image);
    FvsByte_t* m    = ImageGetBuffer(mask);
    FvsInt_t   x, y;
    FvsFloat_t angle = 0.0;
    FvsInt_t   whitecount;

    if (m==NULL || p==NULL)
        return FvsMemory;

    (void)MinutiaSetEmpty(minutia);

    /* loop through the image and find the minutas */
    for (y = 1; y < h-1; y++)
    for (x = 1; x < w-1; x++)
    {
        if (m[x+y*pitchm]==0)
             continue;
        if (P(x,y)==0xFF)
        {
            whitecount = 0;
            if (P1!=0) whitecount++;
            if (P2!=0) whitecount++;
            if (P3!=0) whitecount++;
            if (P4!=0) whitecount++;
            if (P5!=0) whitecount++;
            if (P6!=0) whitecount++;
            if (P7!=0) whitecount++;
            if (P8!=0) whitecount++;

            switch(whitecount)
            {
            case 0:
                /* isolated point, ignore it */
                break;
            case 1:
                /* detect angle */
  	            angle = FloatFieldGetValue(direction, x, y);
                (void)MinutiaSetAdd(minutia, (FvsFloat_t)x, (FvsFloat_t)y,
                              FvsMinutiaTypeEnding, (FvsFloat_t)angle);
                break;
            case 2:
                break;
            default:
                /* normal line or branching */
                /* it may be a branching but we must make sure */
/*
                if ((P(x-1, y+1) && P(x-1, y-1)) ||
                    (P(x-1, y-1) && P(x+1, y-1)) ||
                    (P(x+1, y-1) && P(x+1, y+1)) ||
                    (P(x+1, y+1) && P(x-1, y+1)))
*/
                {
     	            angle = FloatFieldGetValue(direction, x, y);
                    (void)MinutiaSetAdd(minutia, (FvsFloat_t)x, (FvsFloat_t)y,
                                  FvsMinutiaTypeBranching, (FvsFloat_t)angle);
                }
                /* is branch double check */
                break;
            }
        }
    }
    (void)MinutiaSetCheckClean(minutia);
    
    return FvsOK;
}

