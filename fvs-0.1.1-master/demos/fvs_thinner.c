/*########################################################################

  Demo program that shows how to use the FVS library to improve
  fingerprint images. At the moment the library supports the import
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
	(void)ImageSoftenMean(image,3);

        fprintf(stdout, "1/6 Determining the ridge direction\n");
        (void)FingerprintGetDirection(image, direction, 5, 8);

        fprintf(stdout, "2/6 Determining the ridge frequency\n");
        (void)FingerprintGetFrequency(image, direction, frequency);

        fprintf(stdout, "3/6 Creating the mask\n");
        (void)FingerprintGetMask(image, direction, frequency, mask);

        fprintf(stdout, "4/6 Enhancing the fingerprint image\n");
        (void)ImageEnhanceGabor(image, direction, frequency, mask, 4.0);

        fprintf(stdout, "5/6 Binarize\n");
        (void)ImageBinarize(image, (FvsByte_t)0x80);

        fprintf(stdout, "6/6 Thinning\n");
        (void)ImageThinHitMiss(image);
        /* an alternative: */
        /*(void)ImageThinConnectivity(image);*/

        (void)FvsImageExport(image, argv[2]);
    }
    fprintf(stdout, "Cleaning up and exiting...\n");
    ImageDestroy(image);
    ImageDestroy(mask);
    FloatFieldDestroy(direction);
    FloatFieldDestroy(frequency);

    return 0;
}


