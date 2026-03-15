#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;
	inputFile = fopen("AICenter.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("w: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	unsigned char* tempImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			// tempImg에 조건에 따른 값을 담습니다.
			if (j < 100 && i < 100) {
				tempImg[j * stride + 3 * i + 0] = 0;
				tempImg[j * stride + 3 * i + 1] = 0;
				tempImg[j * stride + 3 * i + 2] = 0;
			}
			else if (j > 400 && i > 400) {
				tempImg[j * stride + 3 * i + 0] = 255;
				tempImg[j * stride + 3 * i + 1] = 255;
				tempImg[j * stride + 3 * i + 2] = 255;
			}
			else {
				// 조건 외 영역은 원본 복사
				tempImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
				tempImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
				tempImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
			}
		}
	}

	// 3. 작업이 끝난 tempImg를 다시 inputImg로 덮어쓰기 (교체)
	// 이렇게 하면 아래의 "Original Copy" 루프가 inputImg를 읽을 때 수정된 값이 들어갑니다.
	memcpy(inputImg, tempImg, size);
	free(tempImg);
	

	//Original Copy
	for (int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
		}
	}

	FILE* outputFile = fopen("Output.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);
}
