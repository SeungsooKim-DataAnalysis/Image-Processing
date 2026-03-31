#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

int main()
{
    BITMAPFILEHEADER bmpFile1, bmpFile2;
    BITMAPINFOHEADER bmpInfo1, bmpInfo2;

    FILE* inputFile1 = fopen("Input\\orgY.bmp", "rb");
    FILE* inputFile2 = fopen("Input\\testY.bmp", "rb");

    if (!inputFile1 || !inputFile2) {
        printf("파일 열기 실패\n");
        return -1;
    }

    fread(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, inputFile1);
    fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, inputFile1);
    fread(&bmpFile2, sizeof(BITMAPFILEHEADER), 1, inputFile2);
    fread(&bmpInfo2, sizeof(BITMAPINFOHEADER), 1, inputFile2);

    int size = bmpInfo1.biSizeImage;
    int width = bmpInfo1.biWidth;
    int height = bmpInfo1.biHeight;
    int bitCnt = bmpInfo1.biBitCount;

    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    unsigned char* inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
    unsigned char* inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
    unsigned char* outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

    fread(inputImg1, sizeof(unsigned char), size, inputFile1);
    fread(inputImg2, sizeof(unsigned char), size, inputFile2);

    // 2차원 배열
    double** Y1 = (double**)malloc(sizeof(double*) * height);
    double** Y2 = (double**)malloc(sizeof(double*) * height);
    double** SID = (double**)malloc(sizeof(double*) * height);

    for (int i = 0; i < height; i++) {
        Y1[i] = (double*)malloc(sizeof(double) * width);
        Y2[i] = (double*)malloc(sizeof(double) * width);
        SID[i] = (double*)malloc(sizeof(double) * width);
    }

    // 값 저장 + 밝기 증가
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Y1[i][j] = inputImg2[i * stride + 3 * j]; // test
            Y2[i][j] = inputImg1[i * stride + 3 * j]; // org
            SID[i][j] = Y1[i][j] + 40;
            if (SID[i][j] > 255) SID[i][j] = 255;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outputImg[i * stride + 3 * j + 0] = SID[i][j];
            outputImg[i * stride + 3 * j + 1] = SID[i][j];
            outputImg[i * stride + 3 * j + 2] = SID[i][j];
        }
    }
    double mse1 = 0, mse2 = 0;
    double psnr1, psnr2;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mse1 += (Y2[i][j] - Y1[i][j]) * (Y2[i][j] - Y1[i][j]); // org vs test
            mse2 += (Y2[i][j] - SID[i][j]) * (Y2[i][j] - SID[i][j]); // org vs +40
        }
    }

    mse1 /= (width * height);
    mse2 /= (width * height);

    psnr1 = mse1 != 0.0 ? 10.0 * log10(255 * 255 / mse1) : 99.99;
    psnr2 = mse2 != 0.0 ? 10.0 * log10(255 * 255 / mse2) : 99.99;

    printf("org와 test 비교: MSE = %.2lf  PSNR = %.2lf dB\n", mse1, psnr1);
    printf("org와 22011890 비교: MSE = %.2lf  PSNR = %.2lf dB\n", mse2, psnr2);

    // 결과 이미지 저장
    FILE* outputFile = fopen("..\\Output\\22011890.bmp", "wb");

    fwrite(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    // 메모리 해제
    for (int i = 0; i < height; i++) {
        free(Y1[i]);
        free(Y2[i]);
        free(SID[i]);
    }

    free(Y1);
    free(Y2);
    free(SID);

    free(inputImg1);
    free(inputImg2);
    free(outputImg);

    fclose(inputFile1);
    fclose(inputFile2);
    fclose(outputFile);

    return 0;
}
