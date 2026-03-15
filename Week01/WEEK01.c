#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

int main() {
	double height, weight;
	int age, i;
	FILE* fp1,
		* fp2; 
	fp1 = fopen("C:\\Users\\82103\\OneDrive\\바탕 화면\\input.dat", "r"); 
	fp2 = fopen("C:\\Users\\82103\\OneDrive\\바탕 화면\\output.dat", "w");

	if (fp1 == NULL) {
		printf("input.dat 열기 실패\n");
		return 1;
	}

	if (fp2 == NULL) {
		printf("output.dat 열기 실패\n");
		return 1;
	}

	for (i = 0; i < 3; i++) {
	fscanf(fp1, "%lf %lf %d", &height, &weight, &age); // 입력 받기
	fprintf(fp2, "%.2f %.2f %d\n", height, weight, age); //출력하기
	}
	fclose(fp1); // input.dat 파일닫기
	fclose(fp2); // output.dat 파일닫기
	return 0;
}

//pjs4661@sju.ac.kr