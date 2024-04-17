/*########################################################################

  Demo program that shows how to use the FVS library to improve
  fingerprint images. At the moment the library supports the import
  of bitmap images. The result is exported as a bitmap image as well.

  Link this sample with the FVS library (libfvs)

########################################################################*/

#include "fvs.h"
#include <stdlib.h>
#include "matching.h"

#if 0
#define BMP_TEST_PATH           "./fingerprint_pictures/"
#define MAX_BMP_NUM_i           2
#define MAX_BMP_NUM_j           10
#define FILE_LINK_SYMBOL        "-"
#define BMP_BITS                ""
#else
#define BMP_TEST_PATH           "./fingerprint_bitmaps/"
#define MAX_BMP_NUM_i           2
#define MAX_BMP_NUM_j           4
#define FILE_LINK_SYMBOL        "_"
#define BMP_BITS                FILE_LINK_SYMBOL "8b"
#endif

#define CFG_MATCH_SCORE_LEVEL   50

#define MAX_MINUTIA_SET_CNT     1200

#define CFG_SAVE_MINUTIA_IMAGE  0

#if (CFG_USE_BMP_H)
int bmp_get_index(int m, int n)
{
    // 1_1 -> 0
    // 1_1 -> 1
    return (m - 1) * MAX_BMP_NUM_j + n;
}
#endif

void debug_buffer_data(uint8_t *data, uint32_t size)
{
    int i = 0;
    for (i = 0; i < size; i++) {
        printf("%02X", data[i]);
    }
    printf("\n");
}

static inline const char *get_result_string(FvsError_t ret1, FvsError_t ret2)
{
    //printf("%d %d\n", ret1, ret2);
    return (ret1 == FvsOK && ret2 == FvsOK) ? "[ OK ]" : "[FAIL]";
}

#if (CFG_DISABLE_MAIN_FUNCTION)
int main_entry(int argc, const char *argv[])
#else
int main(int argc, const char *argv[])
#endif
{
    FvsError_t ret = FvsOK;
    FvsError_t ret1 = FvsOK;
    FvsError_t ret2 = FvsOK;
    FvsImage_t mask1;
    FvsImage_t image1;
    FvsFloatField_t direction1;
    FvsFloatField_t frequency1;
    FvsMinutiaSet_t minutia1;
    FvsImage_t mask2;
    FvsImage_t image2;
    FvsFloatField_t direction2;
    FvsFloatField_t frequency2;
    FvsMinutiaSet_t minutia2;
    FvsInt_t goodness = 100;
    FvsInt_t goodness_tmp = 0;
    char bmp1[100] = {0};
    char bmp2[100] = {0};
    char bmp1_1[100] = {0};
    char bmp2_2[100] = {0};
    char *p1;
    char *p2;
    int i = 0;
    int j = 0;
    int k = 0;
    int matched_ok = 0;
    int matched_fail = 0;

#if 0
    /* check if all parameters are there */
    if (argc < 3 || argc > 4)
    {
        printf("Usage:\n %s image1 image2 [goodness]\n", argv[0]);
        return -1;
    }

    if (argc == 4) {
       goodness = atoi(argv[3]);
    }
#endif

    printf("\n\nBegin to match %d finger print pictures ...\n", MAX_BMP_NUM_i * MAX_BMP_NUM_j);
    printf("-----------------------------------------------------------\n");
    printf("[%10s] vs [%10s] -> score, match result      \n", "bmp file 1", "bmp file 2");
    printf("-----------------------------------------------------------\n\n");

    // get bmp1
    for (i = 1; i <= MAX_BMP_NUM_i; i++) {        
        for (j = 1; j <= (MAX_BMP_NUM_j - 1); j++) {
            snprintf(bmp1, sizeof(bmp1), BMP_TEST_PATH"%d%s%d%s.bmp", i, FILE_LINK_SYMBOL, j, BMP_BITS);
            snprintf(bmp1_1, sizeof(bmp1_1), BMP_TEST_PATH"%d%s%d%stmp.bmp", i, FILE_LINK_SYMBOL, j, FILE_LINK_SYMBOL);
            p1 = strrchr(bmp1, '/')+1;

            // get bmp2
            for (k = j + 1; k <= MAX_BMP_NUM_j; k++) {          
                snprintf(bmp2, sizeof(bmp2), BMP_TEST_PATH"%d%s%d%s.bmp", i, FILE_LINK_SYMBOL, k, BMP_BITS);
                snprintf(bmp2_2, sizeof(bmp2_2), BMP_TEST_PATH"%d%s%d%stmp.bmp", i, FILE_LINK_SYMBOL, k, FILE_LINK_SYMBOL);
                p2 = strrchr(bmp2, '/')+1;

                //fprintf(stderr, "[%10s] vs [%10s] -> \n", p1, p2);
                //fflush(stderr);
                //continue;

                direction1 = FloatFieldCreate();
                frequency1 = FloatFieldCreate();
                mask1      = ImageCreate();
                image1     = ImageCreate();
                minutia1   = MinutiaSetCreate(MAX_MINUTIA_SET_CNT);

                direction2 = FloatFieldCreate();
                frequency2 = FloatFieldCreate();
                mask2      = ImageCreate();
                image2     = ImageCreate();
                minutia2   = MinutiaSetCreate(MAX_MINUTIA_SET_CNT);

                //fprintf(stderr, "0/9 (%p %p %p %p %p) (%p %p %p %p %p)\n", \
                //    direction1, frequency1, mask1, image1, minutia1, \
                //    direction2, frequency2, mask2, image2, minutia2);

                (void)FvsImageImport(image1, bmp1);
                (void)ImageSoftenMean(image1, 3);
                (void)FvsImageImport(image2, bmp2);
                (void)ImageSoftenMean(image2, 3);
                fprintf(stderr, "-> Matching [%s] vs [%s]\n", bmp1, bmp2);

                ImageNormalize(image1, 100, 10000);
                ImageNormalize(image2, 100, 10000);

                fprintf(stderr, "1/9 Determining the ridge direction ...\n");
                fflush(stderr);
                ret1 = FingerprintGetDirection(image1, direction1, 5, 8);
                ret2 = FingerprintGetDirection(image2, direction2, 5, 8);
                fprintf(stderr, "1/9 Determining the ridge direction %s\n", get_result_string(ret1, ret2));
                fflush(stderr);

                fprintf(stderr, "2/9 Determining the ridge frequency ...\n");
                fflush(stderr);
                ret1 = FingerprintGetFrequency(image1, direction1, frequency1);
                ret2 = FingerprintGetFrequency(image2, direction2, frequency2);
                fprintf(stderr, "2/9 Determining the ridge frequency %s\n", get_result_string(ret1, ret2));
                fflush(stderr);

                fprintf(stderr, "3/9 Creating the mask ...\n");
                ret1 = FingerprintGetMask(image1, direction1, frequency1, mask1);
                ret2 = FingerprintGetMask(image2, direction2, frequency2, mask2);
                fprintf(stderr, "3/9 Creating the mask %s\n", get_result_string(ret1, ret2));
                fflush(stderr);

                fprintf(stderr, "4/9 Enhancing the fingerprint image ...\n");
                fflush(stderr);
                #if (CFG_ENHANCEGABOR_DISABLE)
                ret1 = FvsOK;
                ret2 = FvsOK;
                #else
                ret1 = ImageEnhanceGabor(image1, direction1, frequency1, mask1, 4.0);
                ret2 = ImageEnhanceGabor(image2, direction2, frequency2, mask2, 4.0);
                #endif
                fprintf(stderr, "4/9 Enhancing the fingerprint image %s\n", get_result_string(ret1, ret2));
                fflush(stderr);

                fprintf(stderr, "5/9 Binarize ...\n");
                fflush(stderr);
                ret1 = ImageBinarize(image1, (FvsByte_t)0x80);
                ret2 = ImageBinarize(image2, (FvsByte_t)0x80);
                fprintf(stderr, "5/9 Binarize %s\n", get_result_string(ret1, ret2));
                fflush(stderr);

                fprintf(stderr, "6/9 Thinning ...\n");
                fflush(stderr);
                ret1 = ImageThinHitMiss(image1);
                ret2 = ImageThinHitMiss(image2);
                fprintf(stderr, "6/9 Thinning %s\n", get_result_string(ret1, ret2));
                fflush(stderr);

                fprintf(stderr, "7/9 Detecting minutia ...\n");
                fflush(stderr);
                ret1 = MinutiaSetExtract(minutia1, image1, direction1, mask1);
                ret2 = MinutiaSetExtract(minutia2, image2, direction2, mask2);
                fprintf(stderr, "7/9 Detecting minutia %s\n", get_result_string(ret1, ret2));
                fflush(stderr);

                fprintf(stderr, "8/9 Drawing minutia ...\n");
                fflush(stderr);
                (void)ImageClear(image1);
                (void)MinutiaSetDraw(minutia1, image1);
                (void)ImageClear(image2);
                (void)MinutiaSetDraw(minutia2, image2);
                fprintf(stderr, "8/9 Drawing minutia %s\n", get_result_string(ret1, ret2)); 
                fflush(stderr);          

                #if (CFG_SAVE_MINUTIA_IMAGE)
                (void)FvsImageExport(image1, bmp1_1);
                (void)FvsImageExport(image2, bmp2_2);
                #endif

                fprintf(stderr, "9/9 Matching minutia ...\n");
                fflush(stderr);
                goodness_tmp = 0;
                ret = MatchingCompareMinutiaSets(minutia1, minutia2, &goodness_tmp);
                fprintf(stderr, "9/9 Matching minutia %s\n", get_result_string(ret1, ret2));
                fflush(stderr);

                //fprintf(stderr, "Cleaning up and exiting...\n");
                MinutiaSetDestroy(minutia1);
                ImageDestroy(image1);
                ImageDestroy(mask1);
                FloatFieldDestroy(frequency1);
                FloatFieldDestroy(direction1);
                MinutiaSetDestroy(minutia2);
                ImageDestroy(image2);
                ImageDestroy(mask2);
                FloatFieldDestroy(frequency2);
                FloatFieldDestroy(direction2);

                if (goodness_tmp >= CFG_MATCH_SCORE_LEVEL)
                {
                    matched_ok++;
                    fprintf(stderr, "Checking match result ... %3d, Same Fingerprint!     \n", goodness_tmp);
                }
                else
                {
                    matched_fail++;
                    fprintf(stderr, "Checking match result ... %3d, Different Fingerprint!\n", goodness_tmp);
                }
                fprintf(stderr, "\n");
                fflush(stderr);
            }
        }
    }

    printf("-----------------------------------------------------------\n");
    printf("---                END OF MATCH RESULT                  ---\n");
    printf("---                  OK: %2d FAIL: %2d                    ---\n", matched_ok, matched_fail);
    printf("-----------------------------------------------------------\n");

    return 0;
}
