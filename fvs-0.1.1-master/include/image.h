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

#if !defined FVS__IMAGE_HEADER__INCLUDED__
#define FVS__IMAGE_HEADER__INCLUDED__

/* basic type definitions */
#include "fvstypes.h"


/*!
 The implementation of the object is private and must not be known
 by the user. Use this handle to manipulate the file through the
 functions provided hereunder.
*/
typedef /*@mutable@*/ FvsHandle_t FvsImage_t;


/*!
  Flags that define what for properties the image has. These flags are
  managed automatically by the library on some function calls. You may
  also set them by yourself if you know what you are doing. These flags
  will empeach some functions that only work with thinned images to end
  with a crash or incoherent results.
  \todo implement...
*/
typedef enum
{
    FvsImageGray      = 0, /*!< no special properties, 8 bit gray level */
    FvsImageBinarized = 1, /*!< marks a binarized image */
    FvsImageThinned   = 2, /*!< marks a thinned image */
} FvsImageFlag_t;


/*!
  Create a new image object.
  \return      NULL if allocation failed, otherwise a new object handle.
*/ 
/*@only@*/ /*@null@*/ FvsImage_t ImageCreate(void);


/*!
  Destroy an image object.
  \param image   pointer to an image object
  \return        nothing.
*/ 
void ImageDestroy(/*@only@*/ /*@out@*/ /*@null@*/ FvsImage_t image);


/*!
  Set the size if an image.
  The memory allocation is done automatically when needed and the function
  returns an error if it failed.
  \param image   an image object
  \param width   width in pixels
  \param height  height in pixels
  \return        an error code.
*/ 
FvsError_t ImageSetSize(FvsImage_t image, const FvsInt_t width, const FvsInt_t height);


/*!
  Set the current image flag. It should only be used in special cases, the flag
  will mostly be set automatically by the library.
  \param image   An image object
  \return        An error code
*/
FvsError_t ImageSetFlag(FvsImage_t image, const FvsImageFlag_t flag);


/*!
  Retrieve the current image flag.
  \param  image   An image object
  \return         The flag associated with the image
*/
FvsImageFlag_t ImageGetFlag(const FvsImage_t image);


/*!
  Copy a source image into a destination image.
  The memory allocation and resizing is done automatically when needed.
  \param destination  a destination image object
  \param source       a source image object
  \return             an error code.
*/
FvsError_t ImageCopy(FvsImage_t destination, const FvsImage_t source);


/*!
  Clear an image.
  Resets the contents of an image to zero.
  \param image  an image object
  \return       an error code.
*/
FvsError_t ImageClear(FvsImage_t image);


/*!
  Set all pixels in the image to a specific value.
  \param image  an image object
  \param value  the value of every pixel after the call
  \return       an error code.
*/
FvsError_t ImageFlood(FvsImage_t image, const FvsByte_t value);


/*!
  Set a pixel value in the picture.
  \param image  an image object
  \param x      x-coordinate
  \param y      y-coordinate
  \param val    value to set
  \return       nothing
*/
void ImageSetPixel(FvsImage_t image, const FvsInt_t x, const FvsInt_t y, const FvsByte_t val);


/*!
  This function returns the pixel for the x and y value.
  \param image  an image object
  \param x      x-coordinate
  \param y      y-coordinate
  \return       the pixel value
*/
FvsByte_t ImageGetPixel(const FvsImage_t image, const FvsInt_t x, const FvsInt_t y);


/*!
  Returns a pointer to the image buffer.
  \param image  an image object
  \return       a pointer to the beginning of the memory buffer
*/
/*@exposed@*/ /*@null@*/ FvsByte_t* ImageGetBuffer(FvsImage_t image);


/*!
  Retrieve the image width.
  \param image  an image object
  \return       the width in pixels
*/
FvsInt_t ImageGetWidth(const FvsImage_t image);


/*!
  Retrieve the image height.
  \param image  an image object
  \return       the heigth in pixels
*/
FvsInt_t ImageGetHeight(const FvsImage_t image);


/*!
  Get the pitch. The pitch of an image is not necessary the width of
  the image. In the image, pixel(x,y) position in the buffer is at
  x + y * pitch.
  \param image  an image object
  \return       the pitch in bytes
*/
FvsInt_t ImageGetPitch(const FvsImage_t image);


/*!
  Gets the number of bytes in the image buffer.
  \param image  an image object
  \return       the number of bytes
*/
FvsInt_t ImageGetSize(const FvsImage_t image);

/*!
  Compares the sizes of 2 images and returns true if they are the same.
  \param image1  an image object
  \param image2  an image object
  \return a boolean value telling if the images are of the same size
*/
FvsBool_t ImageCompareSize(const FvsImage_t image1, const FvsImage_t image2);


/*!
  Sets the number of dots per inch the captured image has.
  \param image  an image object
  \param dpi    the number of dots per inch
  \return       an error code.
*/
FvsError_t ImageSetDPI(FvsImage_t image, const FvsInt_t dpi);


/*!
  Gets the number of dots per inch the captured image has.
  \param image  an image object
  \return       the number of dots per inch
*/
FvsInt_t ImageGetDPI(const FvsImage_t image);


#endif /* FVS__IMAGE_HEADER__INCLUDED__ */

