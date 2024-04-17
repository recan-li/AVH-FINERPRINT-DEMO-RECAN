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
#include <stdbool.h>

#if (CFG_ARM_MATH_ENABLE)
#include "arm_math.h"
#else
#include <math.h>
#endif

#include "imagemanip.h"

#define bool_t bool



#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

#define NOT_BK(pos) (image[pos]!=0)
#define IS_BK(pos)  (image[pos]==0)

/* This function looks all around the current pixel
 to determine if it needs to be removed or if its the
 skeleton we are looking for in Thin() */
bool_t MatchPattern(uint8_t image[], int x, int y, int w, int h)
{
    bool_t nRet = false;
    
    /* make sure we don't go beyond our bounds before calling
       this function 
    */
    int lhe = y * w;        /* line here */
    int lup = lhe - w;      /* line up   */
    int ldo = lhe + w;      /* line down */

    int tl = lup + x - 1;   /* top left */
    int tc = lup + x;       /* top center */
    int tr = lup + x + 1;   /* top right */
    int hl = lhe + x - 1;   /* here left */
    int hr = lhe + x + 1;   /* here right */
    int bl = ldo + x - 1;   /* bottom left */
    int bc = ldo + x;       /* bottom center */
    int br = ldo + x + 1;   /* bottom right */

  /*
    now to check the first 5 if returns as true(see defines in
    top of this file because the pixel is of the skeleton we are
    looking for, other wise(the final else statement) this
    function returns false, so it should be removed 
  */
    /* first pattern
        ? ? ? one not 0
        0 1 0
        ? ? ? one not 0
    */
    if  ( image[hr]==0  &&  image[hl]==0  &&
        ((image[tl]!=0) || (image[tc]!=0) || (image[tr]!=0))&&
        ((image[bl]!=0) || (image[bc]!=0) || (image[br]!=0))
        )
    {
        nRet = true;
    }
    /* same but turned 90 deg
        ? 0 ?
        ? 1 ?
        ? 0 ?
    */
    else
    if  ( image[tc]==0  &&  image[bc]==0  &&
        ((image[bl]!=0) || (image[hl]!=0) || (image[tl]!=0))&&
        ((image[br]!=0) || (image[hr]!=0) || (image[tr]!=0))
        )
    {
        nRet = true;
    }
    /*
        ? ? ?
        ? 1 0
        ? 0 1
    */
    else
    if
        (image[br]==0xFF     &&  image[hr]==0  &&  image[bc]==0  &&
        (image[tr]!=0   ||  image[tc]!=0   ||
         image[tl]!=0   ||  image[hl]!=0   || image[bl]!=0)
         ) 
    {
        nRet = true;
    }
    /*
        ? ? ?
        0 1 ?
        1 0 ?
    */
    else
    if
        (image[bl]==0xFF     &&  image[hl]==0  &&  image[bc]==0   &&
        (image[br]!=0   ||  image[hr]!=0  ||
         image[tr]!=0   ||  image[tc]!=0  ||  image[tl]!=0))
    {
        nRet = true;
    }
    /*
        1 0 ?
        0 1 ?
        ? ? ?
    */
    else
    if
        (image[tl]==0xFF     &&  image[tc]==0  &&  image[hl]==0   &&
        (image[bl]!=0   ||  image[bc]!=0  ||
         image[br]!=0   ||  image[hr]!=0  ||  image[tr]!=0))
    {
        nRet = true;
    }
    /*
        ? 0 1
        ? 1 0
        ? ? ?
    */
    else
    if
        (image[tr]==0xFF     &&  image[hr]==0  &&  image[tc]==0   &&
        (image[tl]!=0   ||  image[hl]!=0  ||
         image[bl]!=0   ||  image[bc]!=0  ||  image[br]!=0))
    {
        nRet = true;
    }
 
    image[y*w + x] = (nRet==true)?0xFF:0x00;

    return nRet;
}


/* This function Thins the image so that it can be futher processed
   The I rewrote this function to run faster, before I used GetPixel
   but since it needed to be called over and over again it slowed
   down the process so I used image array instead */

FvsError_t ImageThin3(FvsImage_t imgf)
{
    bool_t Remain;
    int temp;
    uint8_t* image = ImageGetBuffer(imgf);
    register int x, y;
    int w = ImageGetWidth(imgf);  /* image width */
    int h = ImageGetHeight(imgf); /* image height */
    int tmp;
    int row;

    /* jdh, improved thinning speed with one heuristic */
    int _lastY;
    int _newY;
    
    /* initialization */
    _lastY = _newY = 1;

    /* Do until we are completely finished */
    Remain = true;
    while (Remain)
    {
        /* decrement one line avoids starting over the whole frame */
        /* again and again (heuristic) */
        /*_lastY = _newY - 1;
        if (_lastY<1)*/
            _lastY = 1;

        /* the new coordinates are set to max possible value */
        /* they will contain the minimal position at which the last change */
        /* in the picture was made */
        _newY = h;

        /* so far, no points have been modified */
        Remain = false;
        fprintf(stderr, ".");

        /* j = 0 <- */
        temp   = false;
        for (y = _lastY; y < h-1; y++)
            for (x = 1; x < w-1; x++)
            {
                row = y*w;
                tmp = image[row +(x + 1)]; /* <- */
                if (image[row + x] == 0xFF && tmp == 0
                    && MatchPattern(image, x, y, w, h) == false)
                    if (temp==false)
                    {
                        _newY  = min(_newY, y);
                        Remain = true;
                        temp   = true;
                    }
            } /* for y */

        for (x = w*_lastY; x < w*h; x++)
            if (image[x] == 0x00)
                image[x] = 0;
        
        /* j = 1 */
        temp   = false;
        for (y = _lastY; y < h-1; y++)
            for (x = 1; x < w-1; x++)
            {
                row = y*w;                    /* ^ */
                tmp = image[(y - 1) * w + x]; /* | */
                if (image[row + x] == 0xFF && tmp == 0
                    && MatchPattern(image, x, y, w, h)==false)
                    if (temp==false)
                    {
                        _newY  = min(_newY, y);
                        Remain = true;
                        temp   = true;
                    }
            } /* for y */
        
        for (x = w*_lastY; x < w*h; x++)
            if (image[x] == 0x00)
                image[x] = 0;

        /* j = 2 -> */
        temp   = false;
        for (y = _lastY; y < h-1; y++)
            for (x = 1; x < w-1; x++)
            {
                row = y*w;
                tmp = image[row +(x - 1)]; /* -> */
                if (image[row + x] == 0xFF && tmp == 0
                    && MatchPattern(image, x, y, w, h)==false)
                    if (temp==false)
                    {
                        _newY  = min(_newY, y);
                        Remain = true;
                        temp   = true;
                    }
            } /* for y */
        
        for (x = w*_lastY; x < w*h; x++)
            if (image[x] == 0x00)
                image[x] = 0;

        /* j = 3 */
        temp   = false;
        for (y = _lastY; y < h-1; y++)
            for (x = 1; x < w-1; x++)
            {
                row = y*w;                    /* | */
                tmp = image[(y + 1) * w + x]; /* v */
                if (image[row + x] == 0xFF && tmp == 0
                    && MatchPattern(image, x, y, w, h)==false)
                    if (temp==false)
                    {
                        _newY  = min(_newY, y);
                        Remain = true;
                        temp   = true;
                    }
            } /* for y */
        
        for (x = w*_lastY; x < w*h; x++)
            if (image[x] == 0x00)
                image[x] = 0;

    } /* while */

    return FvsOK;
}


