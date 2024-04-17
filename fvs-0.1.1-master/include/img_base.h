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

#if !defined FVS__IMAGE_BASE_HEADER__INCLUDED__
#define FVS__IMAGE_BASE_HEADER__INCLUDED__

#include "image.h"

typedef enum FvsLogical_t
{
    FvsLogicalOr   = 1,
    FvsLogicalAnd  = 2,
    FvsLogicalXor  = 3,
    FvsLogicalNAnd = 4,
    FvsLogicalNOr  = 5,
    FvsLogicalNXor = 6,
} FvsLogical_t;

/*!
  Transform a gray image into a binary image with either the value 0
  or 255 as pixel color.
  \param image   Image to binarize
  \param limit   Threshold value
  \return        An error code
*/
FvsError_t ImageBinarize(FvsImage_t image, const FvsByte_t limit);


/*!
  Inverts the pixel colors of an image.
  \param image   Image to binarize
  \return        An error code
*/
FvsError_t ImageInvert(FvsImage_t image);


/*!
  Compute the average of 2 images overwrites the result in the first image.
  \param image1  Image that will contain the result
  \param image2  Image to combine image1 with
  \return        An error code
*/
FvsError_t ImageAverage(FvsImage_t image1, const FvsImage_t image2);


/*!
  Compute the average of 2 images overwrites the result in the first image.
  This function makes a circular average computation. 0 and 255 will result
  in 0 as an average instead of 127 as with the precedent function.
  \param image1  Image that will contain the result
  \param image2  Image to combine image1 with
  \return        An error code
*/
FvsError_t ImageAverageModulo(FvsImage_t image1, const FvsImage_t image2);


/*!
  Compute a logical combination of two images.
  \param image1  Image that will contain the result
  \param image2  Image to combine image1 with
  \return        An error code
*/
FvsError_t ImageLogical(FvsImage_t image1,  const FvsImage_t image2,  const FvsLogical_t operation);


/*!
  Translate an image given a vector.
  \todo Implement the function
  \param image   Image to translate
  \param vx      x vector
  \param vy      y vector
  \return        An error code
*/
FvsError_t ImageTranslate(FvsImage_t image, const FvsInt_t vx, const FvsInt_t vy);


/*!
  Create a test image composed of stripes.
  \param image      Image to modify
  \param horizontal horizontal or vertical stripes
  \return           An error code
*/
FvsError_t ImageStripes(FvsImage_t image, const FvsBool_t horizontal);


/*!
  Normalize an image so that it gets specified mean and variance
  \param image      Image to modify
  \param mean       Mean value the resulting image must have
  \param variance   Standard deviation the resulting image must have
  \return           An error code
*/
FvsError_t ImageNormalize(FvsImage_t image, const FvsByte_t mean, const FvsUint_t variance);


/*!
  Change the luminosity of an image argument ranging [-255..255] 
  \param image      Image to modify
  \param luminosity Relative luminosity factor
  \return           An error code
*/
FvsError_t ImageLuminosity(FvsImage_t image, const FvsInt_t luminosity);


/*!
  Change the contrast of an image argument ranging [-127..127]
  \param image      Image to modify
  \param contrast   Relative contrast factor
  \return           An error code
*/
FvsError_t ImageContrast(FvsImage_t image, const FvsInt_t contrast);


/*!
  Soften the image by computing a mean value.
  \param image      Image to modify
  \param size       Size of the computation window
  \return           An error code
*/
FvsError_t ImageSoftenMean(FvsImage_t image, const FvsInt_t size);


#endif /* FVS__IMAGE_BASE_HEADER__INCLUDED__ */


