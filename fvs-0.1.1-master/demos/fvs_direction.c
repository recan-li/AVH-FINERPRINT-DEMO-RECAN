/*########################################################################

  Demo program that shows how to use the FVS library to get the ridge
  direction of a fingerprint image. At the moment the library supports
  the import of bitmap images. The result is exported as a bitmap image
  as well.

  Link this sample against the FVS library (libfvs)

########################################################################*/


#include "fvs.h"

#if (CFG_ARM_MATH_ENABLE)
#include "arm_math.h"
#else
#include <math.h>
#endif

static FvsError_t OverlayDirection(FvsImage_t image, const FvsFloatField_t field)
{
    FvsError_t nRet = FvsOK;
    FvsInt_t w      = ImageGetWidth (image);
    FvsInt_t h      = ImageGetHeight(image);
    FvsInt_t pitch, dirp;
    FvsFloat_t theta, c, s;
    FvsByte_t* p;
    FvsFloat_t* orientation;
    FvsInt_t x, y, size, i, j, l;

    size = 8; /* length in pixels of a vector */

    /* change luminosity */
    (void)ImageLuminosity(image, 168);

    pitch  = ImageGetPitch (image);
    p      = ImageGetBuffer(image);

    orientation = FloatFieldGetBuffer(field);
    dirp        = FloatFieldGetPitch(field);

    if (p==NULL || orientation==NULL)
        return FvsMemory;

    /* build vectors */
    for (y = size; y < h-size; y+=size-2)
    for (x = size; x < w-size; x+=size-2)
    {
        theta = orientation[x+y*dirp];
        c = cos(theta);
        s = sin(theta);
        /* draw a vector of length size in the direction theta */
        for (l = 0; l < size; l++)
        {
            i = (FvsInt_t)(x + size/2 - l*s);
            j = (FvsInt_t)(y + size/2 + l*c);
            p[i+j*pitch] = 0;
        }
    }
    return nRet;
}

int main(int argc, char *argv[])
{
    FvsImage_t      image = NULL;
    FvsFloatField_t field = NULL;

    /* check if all parameters are there */
    if (argc!=3)
    {
        printf("Usage:\n %s input_image output_image\n", argv[0]);
        return -1;
    }
    
    image = ImageCreate();
    field = FloatFieldCreate();

    if ( (image!=NULL) && (field!=NULL) )
    {
        fprintf(stdout, "Opening file %s...\n", argv[1]);
        (void)FvsImageImport(image, argv[1]);
	(void)ImageSoftenMean(image, 3);

        fprintf(stdout, "1/2 Determining the ridge direction\n");
        (void)FingerprintGetDirection(image, field, 7, 8);

        fprintf(stdout, "2/2 Creating a new image with the direction\n");
        (void)OverlayDirection(image, field);

        (void)FvsImageExport(image, argv[2]);
        fprintf(stdout, "Cleaning up and exiting...\n");
    }
    FloatFieldDestroy(field);
    ImageDestroy(image);
    return 0;
}


