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

#include <string.h>


#define P(x,y)      p[(x)+(y)*pitch]

/* Use a structural operator to dilate the image 
**    X
**  X X X
**    X
*/
FvsError_t ImageDilate(FvsImage_t image)
{
    FvsInt_t w      = ImageGetWidth (image);
    FvsInt_t h      = ImageGetHeight(image);
    FvsInt_t pitch  = ImageGetPitch (image);
    FvsInt_t size   = ImageGetSize  (image);
    FvsByte_t* p    = ImageGetBuffer(image);
    FvsInt_t x,y;

    if (p==NULL)
        return FvsMemory;
    
    for (y=1; y<h-1; y++)
    for (x=1; x<w-1; x++)
    {
        if (P(x,y)==0xFF)
        {
            P(x-1, y) |= 0x80;
            P(x+1, y) |= 0x80;
            P(x, y-1) |= 0x80;
            P(x, y+1) |= 0x80;
        }
    }

    for (y=0; y<size; y++)
        if (p[y])
            p[y] = 0xFF;

    return FvsOK;
}

FvsError_t ImageErode(FvsImage_t image)
{
    FvsInt_t w      = ImageGetWidth (image);
    FvsInt_t h      = ImageGetHeight(image);
    FvsInt_t pitch  = ImageGetPitch (image);
    FvsInt_t size   = ImageGetSize  (image);
    FvsByte_t* p    = ImageGetBuffer(image);
    FvsInt_t x,y;
    
    if (p==NULL)
        return FvsMemory;
    
    for (y=1; y<h-1; y++)
    for (x=1; x<w-1; x++)
    {
        if (P(x,y)==0x0)
        {
            P(x-1, y) &= 0x80;
            P(x+1, y) &= 0x80;
            P(x, y-1) &= 0x80;
            P(x, y+1) &= 0x80;
        }
    }

    for (y=0; y<size; y++)
        if (p[y]!=0xFF)
            p[y] = 0x0;

    return FvsOK;
}


