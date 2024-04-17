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

#if !defined FVS__FLOAT_FIELD_HEADER__INCLUDED__
#define FVS__FLOAT_FIELD_HEADER__INCLUDED__

/* basic type definitions */
#include "fvstypes.h"


/*!
 the implementation of the object is private and must not be known
 by the user. Use this handle to manipulate the floating point field
 through the functions provided hereunder.
*/
typedef /*@mutable@*/ FvsHandle_t FvsFloatField_t;


/*!
  Create a new floating point field object.
  \return      NULL if allocation failed, otherwise a new object handle.
*/ 
/*@only@*/ /*@null@*/ FvsFloatField_t FloatFieldCreate(void);


/*!
  Destroy an existing floating point field object.
  \param field   pointer to a floating point field object
  \return        nothing.
*/ 
void FloatFieldDestroy(/*@only@*/ /*@out@*/ /*@null@*/ FvsFloatField_t field);


/*!
  Set the size if a floating point field.
  The memory allocation is done automatically when needed and the function
  returns an error if it failed.
  \param field   a floating point field object
  \param width   width in pixels
  \param height  height in pixels
  \return        an error code.
*/ 
FvsError_t FloatFieldSetSize(FvsFloatField_t field, const FvsInt_t width, const FvsInt_t height);


/*!
  Copy a source image into a destination image.
  The memory allocation and resizing is done automatically when needed.
  \param destination  a destination floating point field object
  \param source       a source floating point field object
  \return             an error code.
*/
FvsError_t FloatFieldCopy(FvsFloatField_t destination, const FvsFloatField_t source);


/*!
  Clear an image.
  Resets the contents of a floating point field to zero.
  \param field  a floating point field object
  \return       an error code.
*/
FvsError_t FloatFieldClear(FvsFloatField_t field);


/*!
  Set all values in the floating point field to a specific value.
  \param field  a floating point field object
  \param value  the value of every pixel after the call
  \return       an error code.
*/
FvsError_t FloatFieldFlood(FvsFloatField_t field, const FvsFloat_t value);


/*!
  Set a floating point value in a floating point field at specified coordinates.
  \param field  a floating point field object
  \param x      x-coordinate
  \param y      y-coordinate
  \param val    value to set
  \return       nothing
*/
void FloatFieldSetValue(FvsFloatField_t field, const FvsInt_t x, const FvsInt_t y, const FvsFloat_t val);


/*!
  This function returns the value for the x and y coordinates.
  \param field  a floating point field object
  \param x      x-coordinate
  \param y      y-coordinate
  \return       the floating point value
*/
FvsFloat_t FloatFieldGetValue(const FvsFloatField_t field, const FvsInt_t x, const FvsInt_t y);


/*!
  Returns a pointer to the floating point field buffer.
  \param field  a floating point field object
  \return       a pointer to the beginning of the memory buffer
*/
/*@exposed@*/ /*@null@*/ FvsFloat_t* FloatFieldGetBuffer(FvsFloatField_t field);


/*!
  Retrieve the floating point field width.
  \param field  a floating point field object
  \return       the width in pixels
*/
FvsInt_t FloatFieldGetWidth(const FvsFloatField_t field);


/*!
  Retrieve the floating point field height.
  \param field  a floating point field object
  \return       the heigth in pixels
*/
FvsInt_t FloatFieldGetHeight(const FvsFloatField_t field);


/*!
  Get the pitch. The pitch of a floating point field is not necessary the width
  that was specified when setting the size. In the floating point field, the value
  at position (x,y) in the buffer is at address x + y * pitch.
  \param field  a floating point field object
  \return       the pitch in bytes
*/
FvsInt_t FloatFieldGetPitch(const FvsFloatField_t field);



#endif /* FVS__IMAGE_HEADER__INCLUDED__ */

