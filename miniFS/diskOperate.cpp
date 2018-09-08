#include "stdafx.h"
#include "Memory.h"
#include "FileNode.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <malloc.h>

char *disk_Start_Address;

//fat��
int fat[block_number];
bool bitmap[block_number];

//���п����
int unused_block_num;

//�ļ��ڵ��
FileNode *fcb;


// �������̿ռ�
void create_memory()
{
	using namespace std;
	// 1GB�ռ�
	fstream disk_file;
	disk_file.open("1.txt", ios::out);
	if (!disk_file.is_open())
	{
		printf("����ʧ�ܣ�����\n");
		return;
	}
	disk_file.seekg(disk_size-1, ios::cur);
	disk_file << '\0';
	disk_file.close();
	
	// ��ʼ��FAT��
	for (int i = 0; i < block_number; i++)
		fat[i] = -1;
	//��ʼ���ļ��ڵ��
	fcb = NULL;
	unused_block_num = block_number;
	memset(bitmap, 0, sizeof bitmap);
}

// ��õ�k���׵�ַ
char *get_kth_address(int k)
{
	return disk_Start_Address + k * block_size;
}

// �����׵�ַ��õ�k����
int get_kth_number(char *p)
{
	return (p - disk_Start_Address) / block_size;
}

// ɾ�����̿ռ�
void delete_memory()
{
	free(disk_Start_Address);
}

// Ϊ�ļ�����հ׿�
void set_block(int file_size)
{
	int number_of_block = ceil(file_size / block_size);
	int last_block_num = -1;
	
	// ����FAT��հ׿�
	for (int i = 0; i < block_number; i++)
	{
		if (!bitmap[i])
		{
			bitmap[i] = 1;
			if (last_block_num != -1)
				fat[last_block_num] = i;
			last_block_num = i;
			number_of_block -= 1;
			if (!number_of_block)
				break;
		}
	}
	return;
}

// �ڴ��̿ռ䴴�����ļ�
void create_file(char file_name[], int file_size)
{
	using namespace std;

	// ��ô����ļ�ʱ��ʱ��
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	FILETIME filetime;
	SystemTimeToFileTime(&systime, &filetime);
	uint64_t create_file_time = filetime.dwHighDateTime << 32 | filetime.dwLowDateTime;


	// �ļ�����洢���� ����ȡ��
	int number_of_block = ceil(file_size / block_size);
	
	// �ռ䲻�� ����
	if (unused_block_num < number_of_block)
	{
		printf("Memory is not enough!!!\n");
		return;
	}


	// �ռ��㹻 ����հ׿�
	unused_block_num -= number_of_block;
	set_block(file_size);
	
	// �����µ��ļ��ڵ�FileNode
	FileNode *new_file = new FileNode;
	
}


// ��ȡ�ļ�����
void read_file(char file_name[])
{
	using namespace std;
	ifstream fin;
	fin.open(file_name);
	
	// �򲻿��򱨴�
	if (!fin.is_open()) //	if (fin.fail())
	{
		printf("Open file filed!!!\n");
		fin.close();
		return;
	}
	
	//һ�ַ�һ�ַ��Ķ�ȡ�ļ�
	char ch;
	while (fin.get(ch))
	{
		printf("%c", ch);
	}
	fin.close();
	return;
}

//���fat��
void clean_fat(int x)
{
	while (fat[x] != -1)
	{
		clean_fat(fat[x]);
	}
	fat[x] = -1;
	return;
}

//ɾ���ļ�
void delete_file(char file_name[])
{
	
}
