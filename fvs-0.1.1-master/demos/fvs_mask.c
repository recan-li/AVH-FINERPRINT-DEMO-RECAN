/*########################################################################

  Demo program that shows how to use the FVS library to create a mask
  within which the relevant fingerprint data is present.
  At the moment the library supports the import
  of bitmap images. The result is exported as a bitmap image as well.

  Link this sample with the FVS library (libfvs)

########################################################################*/


#include "fvs.h"


int main(int argc, char *argv[])
{
    FvsImage_t image;
    FvsImage_t mask;
    FvsFloatField_t direction;
    FvsFloatField_t frequency;

    /* check if all parameters are there */
    if (argc!=3)
    {
        printf("Usage:\n %s input_image output_image\n", argv[0]);
        return -1;
    }

    mask      = ImageCreate();
    image     = ImageCreate();
    direction = FloatFieldCreate();
    frequency = FloatFieldCreate();

    if (mask!=NULL && image!=NULL && direction!=NULL && frequency!=NULL)
    {
        (void)FvsImageImport(image, argv[1]);
	(void)ImageSoftenMean(image, 3);

        fprintf(stdout, "1/3 Determining the ridge direction\n");
        (void)FingerprintGetDirection(image, direction, 7, 8);

        fprintf(stdout, "2/3 Determining the ridge frequency\n");
        (void)FingerprintGetFrequency(image, direction, frequency);

        fprintf(stdout, "3/3 Creating the mask\n");
        (void)FingerprintGetMask(image, direction, frequency, mask);

        (void)FvsImageExport(mask, argv[2]);
    }
    fprintf(stdout, "Cleaning up and exiting...\n");
    FloatFieldDestroy(frequency);
    FloatFieldDestroy(direction);
    ImageDestroy(image);
    ImageDestroy(mask);
    return 0;
}


