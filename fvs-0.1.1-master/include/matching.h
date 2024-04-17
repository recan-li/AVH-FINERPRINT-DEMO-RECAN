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

#if !defined FVS__MATCHING_HEADER__INCLUDED__
#define FVS__MATCHING_HEADER__INCLUDED__

#include "image.h"
#include "minutia.h"


/*!
  \todo Document and implement, remove local functions
  \param image1    First image to compare
  \param image2    Second image that wil be compared to image1
  \param pgoodness An index that indicates how good the match is, the higher, the better
  \return          An error code
*/
FvsError_t MatchingCompareImages(const FvsImage_t image1,
                                 const FvsImage_t image2,
                                 FvsInt_t* pgoodness);

/*!
  \todo Document and implement, remove local functions
  \param minutia1  First minutia set to compare
  \param minutia2  Second minutia set that wil be compared to image1
  \param pgoodness An index that indicates how good the match is, the higher, the better
  \return          An error code
*/
FvsError_t MatchingCompareMinutiaSets(const FvsMinutiaSet_t minutia1,
                                      const FvsMinutiaSet_t minutia2,
                                      FvsInt_t* pgoodness);


#endif /* __MATCHING_HEADER__INCLUDED__ */

