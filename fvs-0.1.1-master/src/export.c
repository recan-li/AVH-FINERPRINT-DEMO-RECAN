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

  Changes: jdh -> Added support for ImageMagick that enables
                  to export files to more than 40 formats.

########################################################################*/


#include "import.h"

#include <stdio.h>

#if (CFG_NON_MAGICK_API)

#include "bmp.h"

/* export a fingeprint image from a file */
FvsError_t FvsImageExport(const FvsImage_t image, const FvsString_t filename)
{
    uint8 *lpBitmap = (uint8 *)ImageGetBuffer(image);
    sint32 Width = ImageGetWidth(image);
    sint32 Height = ImageGetHeight(image);

    FvsSaveBitmap((sint8 *)filename, lpBitmap, Width, Height);

    return FvsOK;
}

#else

#include <magick/api.h>


/* imports a fingeprint image from a file */
FvsError_t FvsImageExport(const FvsImage_t image, const FvsString_t filename)
{
    ExceptionInfo exception;
    Image*        magicimage;
    ImageInfo*    magicinfo;

    FvsError_t ret = FvsOK;
    FvsByte_t*    buffer;
    FvsInt_t      pitch;
    FvsInt_t      height;
    FvsInt_t      width;
    FvsInt_t i;

    /* get buffer, size and pitch from the input image */
    buffer = ImageGetBuffer(image);
    pitch  = ImageGetPitch(image);
    height = ImageGetHeight(image);
    width  = ImageGetWidth(image);

    /* Init Magick environment */
    InitializeMagick(".");
    GetExceptionInfo(&exception);

    /* Create an empty imageinfo */
    magicinfo = CloneImageInfo((ImageInfo*)NULL);
    magicinfo->depth = 8;

    /* create image */
    magicimage = ConstituteImage(width, height, "I", CharPixel, buffer, &exception);
    if (exception.severity!=UndefinedException)
      CatchException(&exception);
    if (magicimage!=(Image*)NULL)
    {
    	/* copy the data */
    	for (i=0; i<height; i++)
    	{
    		ImportImagePixels(magicimage, 0, i, width, 1, "I", CharPixel,
    			buffer+i*pitch);
    	}
    	/* save the image to file */
    	(void)strcpy(magicimage->filename, filename);
    	magicimage->colorspace = GRAYColorspace;
    	magicimage->depth      = 8;
    	WriteImage(magicinfo, magicimage);

    	/* dump info for debugging purposes */
    	/* DescribeImage(magicimage, stdout, 0); */

        DestroyImage(magicimage);
    }
    else
        ret = FvsFailure;

    /* do cleanup */
    DestroyImageInfo(magicinfo);
    DestroyExceptionInfo(&exception);
    DestroyMagick();

    return ret;
}


#if 0

/* The WAND interface is pretty buggy... use the old API */

/* exports a fingeprint image to a file */
FvsError_t FvsImageExport(const FvsImage_t image, const FvsString_t filename)
{
    MagickWand*   wand;
    FvsByte_t*    buffer;
    FvsByte_t*    out;
    FvsInt_t      pitch;
    FvsInt_t      height;
    FvsInt_t      width;
    FvsError_t ret = FvsOK;
    FvsInt_t i;

    /* Init Magick */
    wand = NewMagickWand();
    if (wand!=NULL)
    {
	/* extract parameters */
	buffer = ImageGetBuffer(image);
	width  = ImageGetWidth(image);
	height = ImageGetHeight(image);
	pitch  = ImageGetPitch(image);
	
	/* allocate a new buffer for the pixel data */
	out = (FvsByte_t*)malloc(width*height);
	if (out!=NULL)
	{
	    for (i=0; i<height; i++)
		memcpy(out+i*width, buffer+i*pitch, width);

	    /* out now contains the picture data in a contiguous buffer */
	    MagickSetSize(wand, width, height);
	    MagickSetImagePixels(wand, 0, 0, width, height, "I", CharPixel, out);
	    
	    /* write data */    
	    MagickWriteImage(wand, filename);
	    
	    free(out);
	}
	else
	    ret = FvsMemory;

	/* do cleanup */	
	DestroyMagickWand(wand);    
    }
    else
	ret = FvsMemory;
    
    return ret;
}

#endif
#endif
