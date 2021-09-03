#include "Flash_sector_memory.h"

int start() {
	FILE* fp;
	fopen_s(&fp, "memory.txt", "r");					//memory.txt 파일을 읽기 모드로 열기
	if (fp != NULL) {									//파일이 존재한다면
		char choice;
		while (1) {
			printf("Use last file? y/n : ");			//기존 파일을 그대로 사용할건지 새로 만들건지 결정
			scanf_s("%c", &choice,2);
			while (getchar() != '\n'); 
			switch (choice) {

			case 'y': {
				fclose(fp);
				return 0;
				break;
			}
			case 'n': {
				fclose(fp);
				return 1;
				break;
			}
			default:
				printf("\nPlease use only y or n\n");	//y와n 이외의 입력값은 예외처리한다.
				break;
			}
		}
	}
	else return 1;
	fclose(fp);	
}
void Init(int num) {
	printf("Flash memory size : ");
	
	FILE* fp;
	fopen_s(&fp,"memory.txt", "w+");					//memory.txt 파일을 쓰기모드로 열기/생성
	for (int i = 0; i < (num* B_OF_MB * S_SIZE * D_SIZE); i++) {
		fprintf(fp, " ");								//입력받은 크기만큼 메모장에 메모리 입력
	}
	fclose(fp);

	printf("%d byte memory\n", num * B_OF_MB * S_SIZE * D_SIZE );
	printf("Success\n");
}
void write(int num, char *data) {
	int location = num * D_SIZE;						//섹터의 위치
	if (overwrite(num) == 1) {							//입력받은 섹터번호의 위치에 데이터 유무 확인
		FILE* fp;
		fopen_s(&fp, "memory.txt", "r+");				//메모리 파일을 '읽기'/쓰기 모드로 열기
		fseek(fp, location, SEEK_SET);					//입력받은 섹터번호의 위치로 이동 
		fprintf(fp, "%s", data);						//해당 섹터에 데이터 입력
		fclose(fp);

		printf("write Sector %d success\n", num);
	}
	else {												//Overwrite
		char *d_buffer[S_SIZE];							//블록이 들어갈 버퍼
		for (int s = 0; s < S_SIZE; s++) {
			d_buffer[s] = (char*)malloc(D_SIZE);
		}
		printf("%d", _msize(*(d_buffer+31)));
		int num_Block = (num / S_SIZE);					//해당 섹터가 소속된 블록의 위치
		int f_sector = num_Block * D_SIZE;				//해당 블록의 첫번째 섹터
		if (f_sector == 0) f_sector++;
		FILE* fp;
		fopen_s(&fp, "memory.txt", "r+");				//메모리 파일을 '읽기'/쓰기 모드로 열기
		fseek(fp, f_sector, SEEK_SET);					//해당 섹터의 소속 블록의 첫 번째 섹터로 이동
		for (int i = 0; i < S_SIZE; i++) {				//블록 데이터 백업
				fgets(d_buffer[i],D_SIZE, fp);
		}
		for (int size = 0; size < D_SIZE; size++) {		//백업된 데이터에서 지워야할 섹터 제거
			d_buffer[num][size] = 32;
		}
		//strcpy_s(d_buffer[num],D_SIZE, data);
		fclose(fp);

		erase(num);										//해당 블록 지우기

		fopen_s(&fp, "memory.txt", "r+");
		fseek(fp, f_sector, SEEK_SET);
		for (int i = 0; i < S_SIZE; i++) {
			fprintf(fp,"%s",d_buffer[i]);				//해당 블록에 데이터 입력
		}

		printf("write Sector %d success\n", num);
		fclose(fp);
	}
}
int overwrite(int num) {								//입력받은 위치에 섹터에 데이터 유무 확인
	FILE* fp;
	fopen_s(&fp, "memory.txt", "r");					//메모리 파일을 '읽기'/쓰기 모드로 열기
	char buffer[D_SIZE];
	fseek(fp, (num * D_SIZE), SEEK_SET);
	fread(buffer, sizeof(buffer), 1, fp);
	if (buffer[0] != 32) {								//만약 해당 섹터에 데이터가 존재한다면 0 리턴
		fclose(fp);
		return 0;
	}
	else {
		fclose(fp);
		return 1;										//해당 섹터가 비어 있다면 1 리턴
	}
}

void read(int num) {									//입력받은 섹터번호의 섹터 데이터 출력
	FILE* fp;
	char buffer[D_SIZE];
	fopen_s(&fp, "memory.txt", "r");					//메모리 파일을 읽기모드로 열기
	fseek(fp, (num * D_SIZE), SEEK_SET);				//입력받은 섹터번호의 위치로 이동
	fread(buffer, 1, D_SIZE, fp);						//해당 섹터의 데이터 읽기
	printf("%.*s",30, buffer);							//해당 섹터의 데이터 출력
	fclose(fp);
}
void erase(int num) {									//입력받은 섹터 번호의 섹터를 블록 단위로 지우기
	int num_Block = (num / S_SIZE);						//해당 섹터가 소속된 블록의 위치
	int f_sector = num_Block * D_SIZE;					//해당 블록의 첫번째 섹터
	if (f_sector == 0) f_sector++;
	FILE* fp;
	char data[D_SIZE] = { 0, };							//지우기 처리를 위한 빈 데이터

	for(int j = 0; j <D_SIZE-1; j++){
		data[j] = 32;
	}
	fopen_s(&fp, "memory.txt", "r+");					//메모리 파일을 '읽기'/쓰기 모드로 열기
	fseek(fp, f_sector, SEEK_SET);						//해당 섹터의 소속 블록의 첫 번째 섹터로 이동
	for (int i = 0; i < S_SIZE; i++) {
		fprintf(fp,"%s", data);							//첫 번째 섹터부터 32번째 섹터까지 지우기 처리
	}
	fclose(fp);
}
void end(int num) {
	FILE* fp;
	fopen_s(&fp, "table.txt", "w");						//테이블 저장
	for (int i = 0; i < num; i++) {
		fprintf(fp, "%d\n", *(table+i));
	}
	fclose(fp);
	exit(0);											//프로그램 종료
}
