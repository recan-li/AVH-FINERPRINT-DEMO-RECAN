/*########################################################################

  Demo program that shows how to use the FVS library to improve
  fingerprint images and extract the minutia.
  At the moment the library supports the import
  of bitmap images. The result is exported as a bitmap image as well.

  Link this sample with the FVS library (libfvs)

########################################################################*/


#include "fvs.h"


int main(int argc, char *argv[])
{
    FvsImage_t mask;
    FvsImage_t image;
    FvsFloatField_t direction;
    FvsFloatField_t frequency;
    FvsMinutiaSet_t minutia;

    /* check if all parameters are there */
    if (argc!=3)
    {
        printf("Usage:\n %s input_image output_image\n", argv[0]);
        return -1;
    }

    direction = FloatFieldCreate();
    frequency = FloatFieldCreate();
    mask      = ImageCreate();
    image     = ImageCreate();
    minutia   = MinutiaSetCreate(5000);

    if (direction!=NULL && frequency!=NULL &&
        mask!=NULL && image!=NULL && minutia!=NULL)
    {
        (void)FvsImageImport(image, argv[1]);
	    (void)ImageSoftenMean(image, 3);

        fprintf(stdout, "1/8 Determining the ridge direction\n");
        (void)FingerprintGetDirection(image, direction, 5, 8);

        fprintf(stdout, "2/8 Determining the ridge frequency\n");
        (void)FingerprintGetFrequency(image, direction, frequency);

        fprintf(stdout, "3/8 Creating the mask\n");
        (void)FingerprintGetMask(image, direction, frequency, mask);

        fprintf(stdout, "4/8 Enhancing the fingerprint image\n");
        (void)ImageEnhanceGabor(image, direction, frequency, mask, 4.0);

        fprintf(stdout, "5/8 Binarize\n");
        (void)ImageBinarize(image, (FvsByte_t)0x80);

        fprintf(stdout, "6/8 Thinning\n");
        (void)ImageThinHitMiss(image);

        fprintf(stdout, "7/8 Detecting minutia\n");
        (void)MinutiaSetExtract(minutia, image, direction, mask);

        fprintf(stdout, "8/8 Drawing minutia\n");
        (void)ImageClear(image);
        (void)MinutiaSetDraw(minutia, image);

        (void)FvsImageExport(image, argv[2]);
    }
    fprintf(stdout, "Cleaning up and exiting...\n");
    MinutiaSetDestroy(minutia);
    ImageDestroy(image);
    ImageDestroy(mask);
    FloatFieldDestroy(frequency);
    FloatFieldDestroy(direction);

    return 0;
}


