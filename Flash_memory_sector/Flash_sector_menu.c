#include "Flash_sector_memory.h"


int main() {
	int num = 0;				//섹터 번호, 생성한 메모리 크기를 입력받는 변수
	int m_size = 0;				//기존 파일의 크기를 저장하는 변수
	char command[10];			//커맨드를 입력받는 변수
	char data[D_SIZE];			//입력할 데이터를 입력받는 변수

	if (start()==1) {			//start 함수를 실행하여 기존 파일 유무와 기존파일 사용여부를 결정한다.
		printf("Flash_memory_Simulator \n");
		printf("Please 'init' or 'Init' memory size \n");
		printf("Command : ");
		scanf_s("%s %d", command, 10, &num);
		while (getchar() != '\n');

		if (strcmp(command, "init") == 0|| strcmp(command, "Init") == 0) Init(num);	//init 실행 조건
		else {
			printf("error code 1 : (wrong command)");		//init이외에 커맨드 입력시 오류처리
			return 0;
		}
	}
	FILE* fp;
	fopen_s(&fp, "memory.txt", "r");	//메모리 파일을 읽기모드로 연다.
	if (fp != NULL) {					//파일이 존재한다면 파일의 크기를 MB단위로 구한다.
		fseek(fp, 0, SEEK_END);
		m_size = ((ftell(fp) / D_SIZE) / S_SIZE) / B_OF_MB;	//파일 크기를 구한다.
	}
	fclose(fp);
	set_table(m_size);					//매핑 테이블 생성

	while (1) {							//쓰기, 읽기, 지우기, 프로그램 종료 메뉴
		printf("\nFlash_memory_Simulator \n");
		printf("write (ex/ w 0 A)   read (ex/ r 0)   print (ex/ print)   exit\n");
		printf("Command : ");
		scanf_s("%s", command, 10);

		if (strcmp(command, "w") == 0|| strcmp(command, "W") == 0) {		//쓰기 실행 조건
			scanf_s("%d %s",&num, data, D_SIZE);
			if (num < 0 || num >= B_OF_MB * S_SIZE * m_size) {	//만약 파일 크기를 초과하거나 0보다 작은 번호 입력시 오류처리
				printf("\nerror code 3 : (number_over/under flow)\n");
				continue;
			}
			f_write(num,data);
		}
		else if (strcmp(command, "r") == 0|| strcmp(command, "R") == 0) {	//읽기 실행 조건
			scanf_s("%d", &num);
			if (num < 0 || num >= B_OF_MB * S_SIZE * m_size) {
				printf("\nerror code 3 : (number_over/under flow)\n");
				continue;
			}
			f_read(num);
		}
		else if (strcmp(command, "print") == 0|| strcmp(command, "Print") == 0) {	//테이블 출력 실행 조건
			printf_table(B_OF_MB * S_SIZE * m_size);
		}
		else if (strcmp(command, "exit") == 0|| strcmp(command, "Exit") == 0) {	//프로그램 종료 실행 조건
			end(B_OF_MB * S_SIZE * m_size);
		}
		else {
			printf("\nerror code 2 : (wrong command 2)\n");		//지정된 커맨드 이외에 입력시 오류처리
		}
		while (getchar() != '\n');								//버퍼에 남아있는 데이터를 비운다.
	}

	free(table);
	return 0;
}