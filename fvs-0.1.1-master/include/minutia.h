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

#if !defined FVS__MINUTIA_HEADER__INCLUDED__
#define FVS__MINUTIA_HEADER__INCLUDED__


/* basic type definitions */
#include "fvstypes.h"
#include "image.h"
#include "floatfield.h"


/*!
  Definitions for the different minutia types.
*/
typedef enum FvsMinutiaType_t
{
    FvsMinutiaTypeEnding      = 0, /*!< line ending */
    FvsMinutiaTypeBranching   = 1, /*!< line branch */
    FvsMinutiaTypeCore        = 2, /*!< core point */
    FvsMinutiaTypeDelta       = 3, /*!< delta point */
} FvsMinutiaType_t;


/*!
  Minutia structure. We use here the FBI model, we will try to detect
  as well as possible the position, type and orientation of a minutia.
*/
typedef struct FvsMinutia_t
{
    /*! descibes the type of minutia */
    FvsMinutiaType_t type;
    /*! x position */
    FvsFloat_t    x;
    /*! y position */
    FvsFloat_t    y;
    /*! local minutia direction */
    FvsFloat_t    angle;
} FvsMinutia_t;


/*!
  The implementation of the object is private and must not be known
  by the user. Use this handle to manipulate the file through the
  functions provided hereunder.
*/
typedef /*@mutable@*/ FvsHandle_t FvsMinutiaSet_t;


/*!
  Create a new minutia set that may contain at most size minutia.
  \param size    maximum number of minutia the set may contain
  \return        NULL if allocation failed, otherwise a new object handle.
*/
/*@only@*/ /*@null@*/ FvsMinutiaSet_t MinutiaSetCreate (const FvsInt_t size);

/*!
  Delete the memory allocated for the minutia set. After having been
  destroyed, the minutia set obviously cannot be used anymore in function
  calls before a new set gets allocated again.
  \param   minutia the minutia set to destroy
  \return  nothing
*/
void  MinutiaSetDestroy(/*@only@*/ /*@out@*/ /*@null@*/ FvsMinutiaSet_t minutia);

/*!
  Returns the maximum number of minutia the set may contain.
  \param minutia  The minutia set
  \return         Get the maximum number of minutia that can be stored in the set
*/
FvsInt_t MinutiaSetGetSize(const FvsMinutiaSet_t minutia);


/*!
  Returns the number of minutia in the set.
  \param minutia  The minutia set
  \return         The number of minutia in the set
*/
FvsInt_t MinutiaSetGetCount(const FvsMinutiaSet_t minutia);


/*!
  Returns a pointer to the table of minutia.
  \param minutia  The minutia set
  \return         A pointer to the minutia set
*/
/*@exposed@*/ /*@null@*/ FvsMinutia_t* MinutiaSetGetBuffer(FvsMinutiaSet_t minutia);


/*!
  Empty the minutia set.
  \param minutia  The minutia set to modify
  \return         An error code
*/
FvsError_t MinutiaSetEmpty(FvsMinutiaSet_t minutia);


/*!
  Add a minutia to the set, returns an error if the set is full.
  \param minutia  The minutia set
  \param x        The position in the x direction of the minutia
  \param y        The position in the y direction of the minutia
  \param type     The minutia type
  \param angle    The minutia direction in radians
  \return         An error code
*/
FvsError_t MinutiaSetAdd
    (
    FvsMinutiaSet_t        minutia,
    const FvsFloat_t       x,
    const FvsFloat_t       y,
    const FvsMinutiaType_t type,
    const FvsFloat_t       angle
    );


/*!
  Extract minutia from a thinned image and store them into the set.
  The allocated set has to be big enough to contain all minutias. If it is too
  small then detection will stop after the set being filled.
  \param minutia   The minutia set to fill in
  \param image     The thinned image to extract the minutia from (position and type)
  \param direction The orientation field used to compute the direction
  \param mask      A mask telling where valid fingerprint data is present
  \return          An error code
*/
FvsError_t MinutiaSetExtract
    (
    FvsMinutiaSet_t       minutia,
    const FvsImage_t      image,
    const FvsFloatField_t direction,
    const FvsImage_t      mask
    );


/*!
  Draw the minutia set into the image. The background is not changed, so that
  the minutia are drawed as an overlay.
  \todo allow changes in colors and appearance would be nice...
  \param minutia  The minutia set to draw into the image
  \param image    The image onto which the minutia should be drawn
  \return         An error code
*/
FvsError_t MinutiaSetDraw
    (
    const FvsMinutiaSet_t minutia,
    FvsImage_t image
    );


#endif /* FVS__MINUTIA_HEADER__INCLUDED__ */

