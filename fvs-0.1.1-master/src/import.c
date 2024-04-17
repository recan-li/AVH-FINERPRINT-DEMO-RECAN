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
                  to import files from more than 40 formats.

########################################################################*/


#include "import.h"

#include <stdio.h>

#if (CFG_NON_MAGICK_API)

#include "bmp.h"

/* imports a fingeprint image from a file */
FvsError_t FvsImageImport(FvsImage_t image, const FvsString_t filename)
{
    FvsError_t ret = FvsOK;
    uint8 *lpBitmap;
    sint32 width = 0;
    sint32 height = 0;

    ret = FvsLoadBitmap((sint8 *)filename, NULL, &width, &height);

    ret = ImageSetSize(image, (FvsInt_t)width, (FvsInt_t)height);
    lpBitmap = (uint8 *)ImageGetBuffer(image);

    FvsLoadBitmap((sint8 *)filename, lpBitmap, &width, &height);

    return ret;
}

#else

#include <magick/api.h>

/* imports a fingeprint image from a file */
FvsError_t FvsImageImport(FvsImage_t image, const FvsString_t filename)
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
    
    /* Init Magick environment */
    InitializeMagick(".");
    GetExceptionInfo(&exception);

    /* Create an empty imageinfo */
    magicinfo = CloneImageInfo((ImageInfo*)NULL);

    /* set the filename */
    (void)strcpy(magicinfo->filename, filename);

    /* read images */
    magicimage = ReadImage(magicinfo, &exception);
    if (exception.severity!=UndefinedException)
      CatchException(&exception);
    if (magicimage!=(Image*)NULL)
    {
	/* dump info for debugging purposes at the moment */
	/* DescribeImage(magicimage, stdout, 0); */

	/* allocate the image for Fvs */
	ret = ImageSetSize(image,
	    (FvsInt_t)magicimage->columns,
	    (FvsInt_t)magicimage->rows);
	if (ret==FvsOK)
	{
	    /* get buffer and pitch */
	    buffer = ImageGetBuffer(image);
	    pitch  = ImageGetPitch(image);
	    height = ImageGetHeight(image);
	    width  = ImageGetWidth(image);
	    
	    /* set image type and normalize */
	    NormalizeImage(magicimage);
	
	    /* copy the data */
	    for (i=0; i<height; i++)
	    {
		ExportImagePixels(magicimage, 0, i, width, 1, "I", CharPixel,
			buffer+i*pitch, &exception);
	    }
	}
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

#endif
