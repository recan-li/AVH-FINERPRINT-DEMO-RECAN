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

#if !defined FVS__HISTOGRAM_HEADER__INCLUDED__
#define FVS__HISTOGRAM_HEADER__INCLUDED__

/* basic type definitions */
#include "fvstypes.h"


#include "image.h"

/*!
  The implementation of the object is private and must not be known
  by the user. Use this handle to manipulate the file through the
  functions provided hereunder.
*/
typedef /*@abstract@*/ /*@mutable@*/ FvsHandle_t FvsHistogram_t;


/*!
  Create a new histogram object.
  \return      NULL if allocation failed, otherwise a new object handle.
*/
/*@only@*/ /*@null@*/ FvsHistogram_t HistogramCreate(void);


/*!
  Destroy an existing histogram object.
  \param histogram  pointer to the object to be deleted.
  \return           An error code
*/
void HistogramDestroy(/*@only@*/ /*@out@*/ /*@null@*/ FvsHistogram_t histogram);


/*!
  Reset an histogram to 0.
  \param histogram  Histogram object to modify
  \return           An error code
*/
FvsError_t HistogramReset(FvsHistogram_t histogram);


/*!
  Compute the histogram of an 8bit image.
  \param histogram  Histogram object to modify
  \param image      Image grom which the histogram will be computed
  \return           An error code
*/
FvsError_t HistogramCompute(FvsHistogram_t histogram, const FvsImage_t image);


/*!
  Get the mean of an histogram.
  \param histogram  Histogram object
  \return           The mean gray value
*/
FvsByte_t HistogramGetMean(const FvsHistogram_t histogram);


/*!
  Get the variance of an histogram.
  \param histogram  Histogram object
  \return           The histogram standard deviation
*/
FvsUint_t HistogramGetVariance(const FvsHistogram_t histogram);


#endif /* FVS__HISTOGRAM_HEADER__INCLUDED__ */

