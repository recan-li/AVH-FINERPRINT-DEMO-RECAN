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

#if !defined FVS__IMAGEMANIP_HEADER__INCLUDED__
#define FVS__IMAGEMANIP_HEADER__INCLUDED__

/* include basic image manipulation functions */
#include "img_base.h"
#include "floatfield.h"


/*!
  Get a floating point field that contains the ridge direction of the
  fingerprint image.
  The algorithm used here is described in many scientific papers. The
  result is better if the image has a high contrast and is normalized.
  The values representing the direction vary between -PI/2 and PI/2
  radians the ridge direction is not oriented.
  The higher the block size, the better is the analysis but it will
  take even longer to perform the computations. The low pass filter
  size allows to filter out errors as the ridge direction varies slowly
  in the fingerprint.
  \param image       Image to extract the ridge direction from
  \param field       Ridge directions (floating point values)
  \param nBlockSize  Size of the blocks into the image is divided into
  \param nFilterSize Size of the low pass filter (set to 0 to deactivate)
  \return        An error code.
*/
FvsError_t FingerprintGetDirection(const FvsImage_t image, FvsFloatField_t field,
            const FvsInt_t nBlockSize, const FvsInt_t nFilterSize);

/*!
  Get a floating point field that contains the ridge frequency.
  \param image       Image to extract the ridge frequency direction from
  \param direction   Ridge directions (floating point values)
  \param frequency   Ridge frequency 
  \return        An error code.
*/
FvsError_t FingerprintGetFrequency(const FvsImage_t image, const FvsFloatField_t direction,
            FvsFloatField_t frequency);

/*!
  Get a mask that tells where are the parts of the fingerprint that contain
  useful information for further processing. The mask should be set to 0 where
  the information is not usable: on borders, background parts, where the
  fingerprint image is of very bad quality, ... The mask is set to 255 where
  the fingerprint data can be used.
  \param image       Image to extract the ridge frequency direction from
  \param direction   Ridge directions (floating point values)
  \param frequency   Ridge frequency 
  \param mask        The output mask
  \return        An error code.
*/
FvsError_t FingerprintGetMask(const FvsImage_t image, const FvsFloatField_t direction,
            const FvsFloatField_t frequency, FvsImage_t mask);

/*!
  Thin the image using connectivity information.
  The image must be binarized (i.e. contain only 0x00 or 0xFF values)
  This algorithm looks at the immediate neighbourhood of a pixel and
  checks how many transitions from black to white are present. According
  to the result, the pixel is removed or not.
  \param image   A binarized image to modify
  \return        An error code.
*/
FvsError_t ImageThinConnectivity(FvsImage_t image);


/*!
  Thin the image using "Hit and Miss" structuring elements.
  The image must be binarized (i.e. contain only 0x00 or 0xFF values)
  This algo has the disadvantage to produce spurious lines resulting
  from the skeletonization. These may be eliminated by another algorithm.
  postprocessing is then still needed afterwards.
  \param image   A binarized image to modify
  \return        An error code.
*/
FvsError_t ImageThinHitMiss(FvsImage_t image);


/*!
  Local contrast stretching operator.
  \param image     Image to stretch
  \param size      Size of the blocks onto which stretching should be applied
  \param tolerance value used to eliminate the border of the local histogram
  \return          An error code
*/
FvsError_t ImageLocalStretch(FvsImage_t image, const FvsInt_t size, const FvsInt_t tolerance);


/*!
  Use a structural operator to dilate the binary image.
  \param image    image to dilate
  \return An error code
*/
FvsError_t ImageDilate(FvsImage_t image);


/*!
  Use a structural operator to erode the binary image.
  \param image    image to dilate
  \return An error code
*/
FvsError_t ImageErode(FvsImage_t image);


/*!
  Enhance a fingerprint image.
  The algorithm used here is complex to describe but the postprocessing is
  based on Gabor filters whose parameters are computed dynamically.
  The image is change in-place so you will have to make a copy to preserve the
  original image.
  \param image     Image to modify
  \param direction A ridge direction field previously computed with FingerprintGetDirection for example
  \param frequency A ridge frequency field previously computed with FingerprintGetFrequency for example
  \param mask      Indicating which parts of the image are relevant
  \param radius    Action radius of the filter. 4.0 should be OK in most cases. The higher this value,
                   the resistenter the algorithm is to noise but then it produces more spurious lines.
  \return          An error code.
*/
FvsError_t ImageEnhanceGabor(FvsImage_t image, const FvsFloatField_t direction,
             const FvsFloatField_t frequency, const FvsImage_t mask, const FvsFloat_t radius);


#endif /* FVS__IMAGEMANIP_HEADER__INCLUDED__ */

