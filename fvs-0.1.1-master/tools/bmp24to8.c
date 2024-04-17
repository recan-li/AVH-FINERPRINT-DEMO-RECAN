
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

static void convert24To8(const char *inFileName, const char *outFileName) 
{
    // 打开24位 BMP 文件
    FILE *inFile = fopen(inFileName, "rb");
    if (inFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // 读取 BMP 文件头和信息头
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    fread(&fileHeader, sizeof(BMPFileHeader), 1, inFile);
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, inFile);

    // 确保是24位 BMP
    if (infoHeader.bitsPerPixel != 24) {
        fclose(inFile);
        printf("Input file is not a 24-bit BMP.\n");
        return;
    }

    // 创建8位 BMP 文件
    FILE *outFile = fopen(outFileName, "wb");
    if (outFile == NULL) {
        fclose(inFile);
        perror("Error opening output file");
        return;
    }

    // 更新信息头为8位 BMP
    infoHeader.bitsPerPixel = 8;
    infoHeader.colorsUsed = 256; // 8位 BMP 有 256 种颜色
    infoHeader.colorsImportant = 256;
    infoHeader.imageSize = (infoHeader.width * infoHeader.height * infoHeader.bitsPerPixel) / 8;

    // 更新文件头的大小
    fileHeader.size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + infoHeader.imageSize;

    // 写入8位 BMP 文件头和信息头
    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, outFile);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, outFile);

    // 创建并写入颜色表
    unsigned char colorTable[1024]; // 256 colors * 4 bytes (BGR + reserved)
    for (int i = 0; i < 256; i++) {
        colorTable[i * 4] = (unsigned char)i;       // Blue
        colorTable[i * 4 + 1] = (unsigned char)i;   // Green
        colorTable[i * 4 + 2] = (unsigned char)i;   // Red
        colorTable[i * 4 + 3] = 0;                  // Reserved
    }
    fwrite(colorTable, sizeof(colorTable), 1, outFile);

    // 读取并转换像素数据
    int padding = (4 - (infoHeader.width * 3) % 4) % 4; // 24位 BMP 的行填充
    unsigned char pixel[3];
    unsigned char gray;
    for (int i = 0; i < infoHeader.height; i++) {
        for (int j = 0; j < infoHeader.width; j++) {
            fread(pixel, sizeof(pixel), 1, inFile);
            // 计算灰度值
            gray = (unsigned char)((pixel[0] * 0.3) + (pixel[1] * 0.59) + (pixel[2] * 0.11));
            fwrite(&gray, sizeof(gray), 1, outFile);
        }
        // 跳过填充
        fseek(inFile, padding, SEEK_CUR);
        // 8位 BMP 不需要填充
    }

    // 关闭文件
    fclose(inFile);
    fclose(outFile);
}

static void listBmpFiles(const char *dirPath) 
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
        char *p = strstr(entry->d_name, ".bmp");
        // 检查文件名是否以 ".bmp" 结尾
        if (p != NULL && \
            strstr(entry->d_name, "8b.bmp") == NULL && \
            strstr(entry->d_name, "tmp.bmp") == NULL) {
            //printf("Found BMP file: %s\n", entry->d_name);
            // 在这里，你可以添加处理 BMP 文件的代码
            char file1[1024] = {0};
            char file2[1024] = {0};

            snprintf(file1, sizeof(file1), "%s/%s", dirPath, entry->d_name);
            p = strstr(file1, ".bmp");
            memcpy(file2, file1, p - file1);
            strcat(file2, "_8b.bmp");
            //printf("%s %s\n", file1, file2);

            convert24To8(file1, file2);
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
        listBmpFiles(argv[1]);
        return 0;
    }

    if (argc == 3) {
        inputFileName = argv[1];
        outputFileName = argv[2];
        convert24To8(inputFileName, outputFileName);
        return 0;
    }

    return 0;
}

