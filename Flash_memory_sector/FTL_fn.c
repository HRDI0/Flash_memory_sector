#include "Flash_sector_memory.h"

void f_read(int num) {											//ftl_read
	read(*(table + num));
	printf("PSN : %d\n", *(table + num));
}
void f_write(int num, char data[]) {							//ftl_write
	write(*(table + num), data);
	printf("PSN : %d\n", *(table + num));
}
void printf_table(int num) {
	for (int i = 0; i < num; i++) {								//���̺� ���
		printf("LSN : %d  PSN : %d\n", i,*(table + i));
	}
}
void set_table(int num) {										//���̺� ����
	FILE* fp;
	fopen_s(&fp, "table.txt", "r");								//���� ���̺� ���� Ȯ��
	table = (int*)malloc(sizeof(int) * B_OF_MB * S_SIZE * num); //�� ���ͼ���ŭ ���̺��� �����Ѵ�.
	if (table == NULL) {
		printf("�޸� �Ҵ� ����!");							//�޸� �Ҵ� Ȯ��
		return;
	}
	if(fp != NULL) {											//���� ���̺� ������ �ҷ�����
		int i = 0;
		while (!feof(fp))
		{
			fgets(num_buffer, sizeof(num_buffer), fp);
			*(table+i) = atoi(num_buffer);
			i++;
		}
		
		fclose(fp);
	}
	else {
		for (int i = 0; i < (B_OF_MB * S_SIZE * num); i++) {	//�ʱ� ���̺� �ʱ�ȭ
			*(table + i) = i;
		}
	}
}