#include "stdafx.h"
#include "Memory.h"
#include "FileNode.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

char *disk_Start_Address;

//fat��
int fat[block_number];

//���п����
int unused_block_num;

//�ļ��ڵ��
FileNode *fcb;


// �������̿ռ�
void create_memory()
{
	// 1GB�ռ�
	disk_Start_Address = (char *)malloc(disk_size * sizeof(char *));

	// ��ʼ��FAT��
	for (int i = 0; i < block_number; i++)
		fat[i] = -1;
	//��ʼ���ļ��ڵ��
	fcb = NULL;
	unused_block_num = block_number;
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

// �ڴ��̿ռ䴴�����ļ�
void create_file(char file_name[], int file_size)
{
	using namespace std;
	
	ifstream fin;
	ofstream fout;

	int number_of_block = ceil(file_size / block_size);
	
	// �ռ䲻�� ����
	if (unused_block_num < number_of_block)
	{
		printf("Memory is not enough!!!\n");
		return;
	}

	unused_block_num -= number_of_block;

	// ����FAT��հ׿�
	int last_block_num = -1;
	for (int i = 0; i < block_number; i++)
	{
		if (fat[i] == -1)
		{
			number_of_block -= 1;
			if (last_block_num != -1)
				fat[last_block_num] = i;
			last_block_num = i;
			if (number_of_block == 0)
				break;
		}
	}
	fat[last_block_num] = 0;
	
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
