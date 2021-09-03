#ifndef Flash_sector_memory_h		//헤더파일이 여러번 중복 호출 되지 않도록 한다.
#define Flash_sector_memory_h

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define S_SIZE 32		//블록 당 섹터의 개수
#define D_SIZE 512		//섹터의 크기
#define B_OF_MB 64		//MB당 블록의 개수

typedef struct block{		//블록을 구성하기 위한 섹터 구조체
	char sector[D_SIZE];
}BLOCK;

BLOCK* memory;			//1MB를 할당 하기 위한 섹터 구조체 포인터
int* table;				//맵핑 테이블
int* num_write;			//쓰기 횟수
int* num_erase;			//지우기 횟수
char num_buffer[256];	//테이블 버퍼

int start();			//이미 존재하는 파일을 확인하고 새로 만들지 그대로 쓸지 결정하는 함수
void Init(int num);		//입력 받은 크기만큼 메모리를 생성하는 함수
void write(int num, char *data);	//원하는 위치의 섹터에 데이터를 입력하는 함수
int overwrite(int num);	//이미 데이터가 입력되어 있는 섹터를 구별하여 예외처리하는 함수
void read(int num);		//원하는 위치의 섹터 데이터를 출력하는 함수
void erase(int num);	//원하는 위치의 섹터를 블록단위로 지우는 함수
void end(int num);		//프로그램 종료 함수

void set_table(int num);	//테이블 생성 함수
void f_read(int num);		//FTL READ 함수
void f_write(int num, char data[]);	//FTL WRITE 함수
void printf_table(int num);		//테이블 출력 함수

#endif /* Flash_sector_memory_h */
#pragma once


