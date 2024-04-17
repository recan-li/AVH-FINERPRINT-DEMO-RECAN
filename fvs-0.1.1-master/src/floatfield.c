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

#include "floatfield.h"

/*!
  A fingerprint floating point field structure.
*/
typedef struct iFvsFloatField_t
{
    FvsFloat_t      *pimg;          /* floating point field array */
    FvsInt_t         w;             /* width of field */
    FvsInt_t         h;             /* height of field */
    FvsInt_t         pitch;         /* pitch */
} iFvsFloatField_t;


FvsFloatField_t FloatFieldCreate()
{
    iFvsFloatField_t* p = NULL;
    p = (FvsFloatField_t)malloc(sizeof(iFvsFloatField_t));

    if (p!=NULL)
    {
        p->h        = 0;
        p->w        = 0;
        p->pitch    = 0;
        p->pimg     = NULL;
    }

    return (FvsFloatField_t)p;
}


void FloatFieldDestroy(FvsFloatField_t field)
{
    iFvsFloatField_t* p = NULL;

    if (field==NULL)
        return;

    p = field;
    (void)FloatFieldSetSize(field, 0, 0);
    free(p);
}


FvsError_t FloatFieldSetSize(FvsFloatField_t img, const FvsInt_t width, const FvsInt_t height)
{
    iFvsFloatField_t* field = (iFvsFloatField_t*)img;
    FvsError_t nRet = FvsOK;
    FvsInt_t newsize = (FvsInt_t)(width*height*sizeof(FvsFloat_t));

    /* special case for zero size */
    if (newsize==0)
    {
        if (field->pimg!=NULL)
        {
            free(field->pimg);
            field->pimg = NULL;
            field->w = 0;
            field->h = 0;
            field->pitch = 0;
        }
        return FvsOK;
    }

    if ((FvsInt_t)(field->h*field->w*sizeof(FvsFloat_t)) != newsize)
    {
        free(field->pimg);
        field->w = 0;
        field->h = 0;
        field->pitch = 0;
        /* This allocates the amount of memory need for the field structure */
        field->pimg = (FvsFloat_t*)malloc((size_t)newsize);
    }

    if (field->pimg == NULL)
        nRet = FvsMemory;
    else
    {
        field->h = height;
        field->w = width;
        field->pitch = width;
    }
    return nRet;
}


FvsError_t FloatFieldCopy(FvsFloatField_t destination, const FvsFloatField_t source)
{
    iFvsFloatField_t* dest = (iFvsFloatField_t*)destination;
    iFvsFloatField_t* src  = (iFvsFloatField_t*)source;
    FvsError_t nRet = FvsOK;

    nRet = FloatFieldSetSize(dest, src->w, src->h);
    
    if (nRet==FvsOK)
        memcpy(dest->pimg, src->pimg, src->h*src->w*sizeof(FvsFloat_t));

    return nRet;
}


FvsError_t FloatFieldClear(FvsFloatField_t img)
{
    return FloatFieldFlood(img, 0.0);
}


FvsError_t FloatFieldFlood(FvsFloatField_t img, const FvsFloat_t value)
{
    iFvsFloatField_t* field = (iFvsFloatField_t*)img;
    FvsError_t nRet = FvsOK;
    FvsInt_t i;
    if (field->pimg!=NULL)
    {
        for (i=0; i<field->h*field->w; i++)
            field->pimg[i] = value;
    }
    return nRet;
}


/* set a pixel value in the picture */
void FloatFieldSetValue(FvsFloatField_t img, const FvsInt_t x, const FvsInt_t y, const FvsFloat_t val)
{
    iFvsFloatField_t* field = (iFvsFloatField_t*)img;
    int address = y * field->w + x;
    field->pimg[address] = val;
}


/* This function returns the pixel for the x and y value */
FvsFloat_t FloatFieldGetValue(FvsFloatField_t img, const FvsInt_t x, const FvsInt_t y)
{
    iFvsFloatField_t* field = (iFvsFloatField_t*)img;
    /* position in array */
    int address = y * field->pitch + x;
    return field->pimg[address];
}


/* returns a pointer to the field buffer */
FvsFloat_t* FloatFieldGetBuffer(FvsFloatField_t img)
{
    iFvsFloatField_t* field = (iFvsFloatField_t*)img;
    return field->pimg;
}


/* retrieve width and height */
FvsInt_t FloatFieldGetWidth(const FvsFloatField_t img)
{
    iFvsFloatField_t* field = (iFvsFloatField_t*)img;
    return field->w;
}


FvsInt_t FloatFieldGetHeight(const FvsFloatField_t img)
{
    iFvsFloatField_t* field = (iFvsFloatField_t*)img;
    return field->h;
}


/* get the pitch pixel(x,y) pos is at x + y * pitch */
FvsInt_t FloatFieldGetPitch(const FvsFloatField_t img)
{
    iFvsFloatField_t* field = (iFvsFloatField_t*)img;
    return field->pitch;
}


