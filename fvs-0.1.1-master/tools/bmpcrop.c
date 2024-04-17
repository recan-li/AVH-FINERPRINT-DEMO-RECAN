
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#pragma pack(1) // 禁用结构体成员之间的对齐

// 定义 BMP 文件的文件头和图像信息头
typedef struct {
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPFileHeader;

typedef struct {
    unsigned int size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    int xResolution;
    int yResolution;
    unsigned int colorsUsed;
    unsigned int colorsImportant;
} BMPInfoHeader;

// 定义调色板结构
typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char reserved;
} BMPColorPalette;

#define CROP_WIDTH      160
#define CROP_HEIGHT     160
#define BMP_SIZE_STR    "160x160"

#if 0
static 
void cropBMP(const char *inputFilePath, const char *outputFilePath, int cropWidth, int cropHeight) {
    // 打开输入文件
    FILE *inputFile = fopen(inputFilePath, "rb");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // 读取文件头和信息头
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    fread(&fileHeader, sizeof(BMPFileHeader), 1, inputFile);
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, inputFile);

    // 检查是否为8位BMP
    if (infoHeader.bitsPerPixel != 8 || infoHeader.width != 256 || infoHeader.height != 256) {
        fclose(inputFile);
        printf("Input file is not a 256x256 8-bit BMP.\n");
        return;
    }

    // 读取调色板
    BMPColorPalette *colorPalette = (BMPColorPalette *)malloc(infoHeader.colorsUsed * sizeof(BMPColorPalette));
    fread(colorPalette, sizeof(BMPColorPalette), infoHeader.colorsUsed, inputFile);

    // 计算裁剪的起始坐标
    int startX = (infoHeader.width - cropWidth) / 2;
    int startY = (infoHeader.height - cropHeight) / 2;

    // 更新信息头
    infoHeader.width = cropWidth;
    infoHeader.height = cropHeight;
    infoHeader.imageSize = cropWidth * cropHeight;

    // 更新文件头的大小
    fileHeader.size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + (infoHeader.colorsUsed * sizeof(BMPColorPalette)) + infoHeader.imageSize;

    // 创建输出文件
    FILE *outputFile = fopen(outputFilePath, "wb");
    if (outputFile == NULL) {
        fclose(inputFile);
        perror("Error opening output file");
        return;
    }

    // 写入文件头和信息头
    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, outputFile);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outputFile);
    // 写入调色板
    fwrite(colorPalette, sizeof(BMPColorPalette), infoHeader.colorsUsed, outputFile);

    // 跳过原始图像数据到裁剪区域的起始位置
    fseek(inputFile, startY * infoHeader.width + startX, SEEK_SET);

    // 读取并写入裁剪后的像素数据
    for (int y = 0; y < cropHeight; y++) {
        for (int x = 0; x < cropWidth; x++) {
            unsigned char pixel;
            fread(&pixel, sizeof(unsigned char), 1, inputFile);
            fwrite(&pixel, sizeof(unsigned char), 1, outputFile);
        }
        // 跳过原始图像的剩余部分
        fseek(inputFile, (infoHeader.width - cropWidth), SEEK_CUR);
    }

    // 释放内存
    free(colorPalette);

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);
}
#else
#include<stdio.h>
 
static 
void cropBMP(const char *inputFilePath, const char *outputFilePath, int cropWidth, int cropHeight)
{
    int bmpHeight;
    int bmpWidth;
    unsigned char* pBmpBuf;
    unsigned char* pBmpBuf2;
    BMPColorPalette* pColorTable;
    int biBitCount;
    int begin_x;
    int begin_y;
 
    //读取bmp文件
    FILE* fp = fopen(inputFilePath, "rb");
    if (fp == 0){
        return;
    }

    BMPFileHeader fileHead;
    fread(&fileHead, sizeof(BMPFileHeader), 1, fp);
 
    BMPInfoHeader head;
    fread(&head, 40, 1, fp);
    bmpHeight = head.height;
    bmpWidth = head.width;
    biBitCount = head.bitsPerPixel;

    begin_x = (bmpWidth - cropWidth) / 2;
    begin_y = (bmpHeight - cropHeight) / 2;
 
    pColorTable = (BMPColorPalette *)malloc( sizeof(BMPColorPalette) * 256);
    fread(pColorTable, sizeof(BMPColorPalette), 256, fp);
 
    int LineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//保证每一行字节数都为4的整数倍
    pBmpBuf = (unsigned char *)malloc(LineByte * bmpHeight);
    fread(pBmpBuf, LineByte * bmpHeight, 1, fp);
 
    fclose(fp);

    #if 0
       // 更新信息头
    infoHeader.width = cropWidth;
    infoHeader.height = cropHeight;
    infoHeader.imageSize = cropWidth * cropHeight;

    // 更新文件头的大小
    fileHeader.size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + (infoHeader.colorsUsed * sizeof(BMPColorPalette)) + infoHeader.imageSize;

    #endif
    
    FILE* fop = fopen(outputFilePath, "wb");
    head.height = cropHeight;//为截取文件重写位图高度
    head.width = cropWidth;//为截取文件重写位图宽度
    int LineByte2 = (head.width * biBitCount / 8 + 3) / 4 * 4;
    pBmpBuf2 = (unsigned char *)malloc(LineByte2 * head.height);
    head.imageSize = LineByte2 * head.height;
    fileHead.size = head.size + fileHead.offset + (head.colorsUsed * sizeof(BMPColorPalette));
 
    fwrite(&fileHead, sizeof(BMPFileHeader), 1, fop);
    fwrite(&head, sizeof(BMPInfoHeader), 1, fop);
    fwrite(pColorTable, sizeof(BMPColorPalette), 256, fop);
 
    for (int i = 0; i < cropHeight; ++i) {
        for (int j = 0; j < cropWidth; ++j)
        {            
            pBmpBuf2[i * cropWidth + j] = pBmpBuf[(begin_y + i) * bmpWidth + begin_x + j];
        }
    }

    fwrite(pBmpBuf2, LineByte2 * head.height, 1, fop);
 
    fclose(fop);

    free(pColorTable);
    free(pBmpBuf);
    free(pBmpBuf2);
}
#endif

static void listBmpFiles(const char *dirPath, int cropWidth, int cropHeight) 
{
    DIR *dir;
    struct dirent *entry;

    // 打开目录
    dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    // 遍历目录
    while ((entry = readdir(dir)) != NULL) {
        char *p = strstr(entry->d_name, "8b.bmp");
        // 检查文件名是否以 ".bmp" 结尾
        if (p != NULL && \
            strstr(entry->d_name, "_" BMP_SIZE_STR "_8b.bmp") == NULL && \
            strstr(entry->d_name, "tmp.bmp") == NULL) {
            //printf("Found BMP file: %s\n", entry->d_name);
            // 在这里，你可以添加处理 BMP 文件的代码
            char file1[1024] = {0};
            char file2[1024] = {0};

            snprintf(file1, sizeof(file1), "%s/%s", dirPath, entry->d_name);
            p = strstr(file1, ".bmp");
            memcpy(file2, file1, p - file1);
            strcat(file2, "_" BMP_SIZE_STR "_8b.bmp");
            //printf("%s %s\n", file1, file2);

            cropBMP(file1, file2, cropWidth, cropHeight);
        }
    }

    // 关闭目录
    closedir(dir);
}

int main(int argc, const char *argv[]) 
{
    FILE *inputFile, *outputFile;
    const char *inputFileName;
    const char *outputFileName;

    if (argc == 2) {
        listBmpFiles(argv[1], CROP_WIDTH, CROP_HEIGHT);
        return 0;
    }

    if (argc == 3) {
        inputFileName = argv[1];
        outputFileName = argv[2];
        cropBMP(inputFileName, outputFileName, CROP_WIDTH, CROP_HEIGHT);
        return 0;
    }

    return 0;
}

