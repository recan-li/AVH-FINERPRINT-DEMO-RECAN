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
  
  This file contains function related to the matching of fingerprints 

########################################################################*/

#if (CFG_MATCHING_V1)

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#if (CFG_ARM_MATH_ENABLE)
#include "arm_math.h"
#else
#include <math.h>
#endif

#include "matching.h"


/* TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO */
FvsError_t MatchingCompareImages
(
    /*@unused@*/const FvsImage_t image1,
    /*@unused@*/const FvsImage_t image2,
    /*@unused@*/FvsInt_t* pgoodness
)
{
    /* original code from Shivang Patel tries to make this */
    /* that is an image based matching, Shivang also extracts
    the minutia from the images and makes a comparison of these
    features */
    
    /* I would rather drop completely that code and only retain the
    good ideas in it and implement a cross-corelation algorithm
    without any feature extraction */
    
    return FvsOK;
}

    
/* TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO */
FvsError_t MatchingCompareMinutiaSets
(
    /*@unused@*/const FvsMinutiaSet_t set1,
    /*@unused@*/const FvsMinutiaSet_t set2,
    /*@unused@*/FvsInt_t* pgoodness
)
{
    /* I already provided functions to extract the minutia from a
    fingerprint image. What we have to implement here is a minutia
    based matching algorithm */
    
    return FvsOK;
}

#endif
