#include "stdafx.h"
#include "Memory.h"
#include "FileNode.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

char *disk_Start_Address;

//fat表
int fat[block_number];

//空闲块个数
int unused_block_num;

//文件节点表
FileNode *fcb;


// 创建磁盘空间
void create_memory()
{
	// 1GB空间
	disk_Start_Address = (char *)malloc(disk_size * sizeof(char *));

	// 初始化FAT表
	for (int i = 0; i < block_number; i++)
		fat[i] = -1;
	//初始化文件节点表
	fcb = NULL;
	unused_block_num = block_number;
}

// 获得第k块首地址
char *get_kth_address(int k)
{
	return disk_Start_Address + k * block_size;
}

// 根据首地址获得第k块编号
int get_kth_number(char *p)
{
	return (p - disk_Start_Address) / block_size;
}

// 删除磁盘空间
void delete_memory()
{
	free(disk_Start_Address);
}

// 在磁盘空间创建新文件
void create_file(char file_name[], int file_size)
{
	using namespace std;
	
	ifstream fin;
	ofstream fout;

	int number_of_block = ceil(file_size / block_size);
	
	// 空间不足 报错
	if (unused_block_num < number_of_block)
	{
		printf("Memory is not enough!!!\n");
		return;
	}

	unused_block_num -= number_of_block;

	// 遍历FAT表空白块
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


// 读取文件内容
void read_file(char file_name[])
{
	using namespace std;
	ifstream fin;
	fin.open(file_name);
	
	// 打不开则报错
	if (!fin.is_open()) //	if (fin.fail())
	{
		printf("Open file filed!!!\n");
		fin.close();
		return;
	}
	
	//一字符一字符的读取文件
	char ch;
	while (fin.get(ch))
	{
		printf("%c", ch);
	}
	fin.close();
	return;
}

//清空fat表
void clean_fat(int x)
{
	while (fat[x] != -1)
	{
		clean_fat(fat[x]);
	}
	fat[x] = -1;
	return;
}

//删除文件
void delete_file(char file_name[])
{
	
}
