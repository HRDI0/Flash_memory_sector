#include "Flash_sector_memory.h"


int main() {
	int num = 0;				//���� ��ȣ, ������ �޸� ũ�⸦ �Է¹޴� ����
	int m_size = 0;				//���� ������ ũ�⸦ �����ϴ� ����
	char command[10];			//Ŀ�ǵ带 �Է¹޴� ����
	char data[D_SIZE];			//�Է��� �����͸� �Է¹޴� ����

	if (start()==1) {			//start �Լ��� �����Ͽ� ���� ���� ������ �������� ��뿩�θ� �����Ѵ�.
		printf("Flash_memory_Simulator \n");
		printf("Please 'init' or 'Init' memory size \n");
		printf("Command : ");
		scanf_s("%s %d", command, 10, &num);
		while (getchar() != '\n');

		if (strcmp(command, "init") == 0|| strcmp(command, "Init") == 0) Init(num);	//init ���� ����
		else {
			printf("error code 1 : (wrong command)");		//init�̿ܿ� Ŀ�ǵ� �Է½� ����ó��
			return 0;
		}
	}
	FILE* fp;
	fopen_s(&fp, "memory.txt", "r");	//�޸� ������ �б���� ����.
	if (fp != NULL) {					//������ �����Ѵٸ� ������ ũ�⸦ MB������ ���Ѵ�.
		fseek(fp, 0, SEEK_END);
		m_size = ((ftell(fp) / D_SIZE) / S_SIZE) / B_OF_MB;	//���� ũ�⸦ ���Ѵ�.
	}
	fclose(fp);
	set_table(m_size);					//���� ���̺� ����

	while (1) {							//����, �б�, �����, ���α׷� ���� �޴�
		printf("\nFlash_memory_Simulator \n");
		printf("write (ex/ w 0 A)   read (ex/ r 0)   print (ex/ print)   exit\n");
		printf("Command : ");
		scanf_s("%s", command, 10);

		if (strcmp(command, "w") == 0|| strcmp(command, "W") == 0) {		//���� ���� ����
			scanf_s("%d %s",&num, data, D_SIZE);
			if (num < 0 || num >= B_OF_MB * S_SIZE * m_size) {	//���� ���� ũ�⸦ �ʰ��ϰų� 0���� ���� ��ȣ �Է½� ����ó��
				printf("\nerror code 3 : (number_over/under flow)\n");
				continue;
			}
			f_write(num,data);
		}
		else if (strcmp(command, "r") == 0|| strcmp(command, "R") == 0) {	//�б� ���� ����
			scanf_s("%d", &num);
			if (num < 0 || num >= B_OF_MB * S_SIZE * m_size) {
				printf("\nerror code 3 : (number_over/under flow)\n");
				continue;
			}
			f_read(num);
		}
		else if (strcmp(command, "print") == 0|| strcmp(command, "Print") == 0) {	//���̺� ��� ���� ����
			printf_table(B_OF_MB * S_SIZE * m_size);
		}
		else if (strcmp(command, "exit") == 0|| strcmp(command, "Exit") == 0) {	//���α׷� ���� ���� ����
			end(B_OF_MB * S_SIZE * m_size);
		}
		else {
			printf("\nerror code 2 : (wrong command 2)\n");		//������ Ŀ�ǵ� �̿ܿ� �Է½� ����ó��
		}
		while (getchar() != '\n');								//���ۿ� �����ִ� �����͸� ����.
	}

	free(table);
	return 0;
}