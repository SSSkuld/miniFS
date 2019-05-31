#include "stdafx.h"
#include "FileNode.h"
#include <cstdio>
#include <cstring>
#include <iostream>

FileNode::FileNode()
{
	memset(file_Name, '\0', sizeof(file_Name));
	memset(file_Suffix, '\0', sizeof(file_Suffix));
	create_Date = 0;
	file_Size = 0;
	first_Block_Num = -1;
	folder_number = 0;
}

FileNode::FileNode(char _file_Name[], char _file_Suffix[], uint64_t _create_Date, int32_t _file_Size, int32_t _first_Block_Num, int32_t _folder_number, int32_t _belong_folder_number)
{
	memset(file_Name, '\0', sizeof file_Name);
	memset(file_Suffix, '\0', sizeof(file_Suffix));
	int len_file_name = strlen(_file_Name);
	for (int i = 0; i < len_file_name; i++)
		file_Name[i] = _file_Name[i];
	int len_file_suffix = strlen(_file_Suffix);
	for (int i = 0; i < len_file_suffix; i++)
		file_Suffix[i] = _file_Suffix[i];
	create_Date = _create_Date;
	file_Size = _file_Size;
	first_Block_Num = _first_Block_Num;
	folder_number = _folder_number;
	belong_folder_number = _belong_folder_number;
}

bool FileNode :: operator == (const FileNode &x) const
{
	int len_file_name = strlen(x.file_Name);
	for (int i = 0; i < len_file_name; i++)
		if (file_Name[i] != x.file_Name[i])
			return false;
	int len_file_suffix = strlen(x.file_Suffix);
	for (int i = 0; i < len_file_suffix; i++)
		if (file_Suffix[i] != x.file_Suffix[i])
			return false;
	return true;
}

//将uint64保存的时间以year-month-day hour:minute:second格式屏幕打印
void FileNode::show_time()
{
	SYSTEMTIME systemtime;
	FILETIME filetime;

	filetime.dwHighDateTime = (create_Date >> 32);
	filetime.dwLowDateTime = (create_Date & 0xffffffff);

//	std::cout << filetime.dwHighDateTime << " " << filetime.dwLowDateTime << std::endl;

	FileTimeToSystemTime(&filetime, &systemtime);

	printf("%04d-%02d-%02d ", systemtime.wYear, systemtime.wMonth, systemtime.wDay);
	printf("%02d:%02d:%02d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond);
//	std::cout << create_Date << std::endl;
}

int32_t FileNode::get_file_size()
{
	return file_Size;
}

void FileNode::show_file_name()
{
	int len_of_name = strlen(file_Name);
	int len_of_suffix = strlen(file_Suffix);
	for (int i = 0; i < len_of_name; i++)
		printf("%c", file_Name[i]);
	if (get_folder_number() == -1)
	{
		printf(".");
		for (int i = 0; i < len_of_suffix; i++)
			printf("%c", file_Suffix[i]);
	}
	printf("\n");
	return;
}

int32_t FileNode::get_first_block_number()
{
	return first_Block_Num;
}

int32_t FileNode::get_folder_number()
{
	return folder_number;
}

int32_t FileNode::get_belong_folder_number()
{
	return belong_folder_number;
}

uint64_t FileNode::get_create_Date()
{
	return create_Date;
}

uint64_t FileNode::get_change_Date()
{
	return change_Date;
}

char * FileNode::get_file_name()
{
	return file_Name;
}

char * FileNode::get_file_suffix()
{
	return file_Suffix;
}

void FileNode::rename_name(char new_name[])
{
	memset(file_Name, '\0', sizeof(file_Name));
	int len = strlen(new_name);
	for (int i = 0; i < len; i++)
		file_Name[i] = new_name[i];
	return;
}

void FileNode::rename_suffix(char new_suffix[])
{
	memset(file_Suffix, '\0', sizeof(file_Suffix));
	int len = strlen(new_suffix);
	for (int i = 0; i < len; i++)
		file_Suffix[i] = new_suffix[i];
	return;
}

void FileNode::change_file_size(int32_t x)
{
	file_Size = x;
}

FileNode::~FileNode()
{
}
