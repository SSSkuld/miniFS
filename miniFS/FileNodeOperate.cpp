#include"FileNode.h"
#include<Windows.h>
#include<time.h>

// 初始化构造函数 无传参
FileNode::FileNode()
{
	memset(file_Name, '\0', sizeof file_Name);
	memset(file_Suffix, '\0', sizeof file_Suffix);
	create_Date = 0;
	file_Size = 0;
	first_Block_Num = -1;
}

// 初始化构造函数 有传参
FileNode::FileNode(char _file_Name[], char _file_Suffix[], int _create_Date, int _file_Size, int _first_Block_Num)
{
	int len_file_name = strlen(_file_Name);
	for (int i = 0; i < len_file_name; i++)
		file_Name[i] = _file_Name[i];

	int len_file_suffix = strlen(_file_Suffix);
	for (int i = 0; i < len_file_suffix; i++)
		file_Suffix[i] = _file_Suffix[i];

	create_Date = _create_Date;
	file_Size = _file_Size;
	first_Block_Num = _first_Block_Num;
}

// 屏幕输出文件属性
void FileNode::att(FileNode x)
{
	int len_file_name = strlen(x.file_Name);
	int len_file_suffix = strlen(x.file_Suffix);

	//打印文件名
	for (int i = 0; i < len_file_name; i++)
		printf("%c", x.file_Name[i]);
	printf(".");
	for (int i = 0; i < len_file_suffix; i++)
		printf("%c", x.file_Suffix[i]);
	printf("\n");

	//打印文件大小


	return;
}

// 将uint64保存的时间以year-month-day hour:minute:second格式屏幕打印
void FileNode::show_time()
{
	FILETIME filetime;
	SYSTEMTIME systemtime;
	filetime.dwHighDateTime = create_Date >> 32;
	filetime.dwLowDateTime = create_Date & 0xffffffff;
	FileTimeToSystemTime(&filetime, &systemtime);
	printf("%4d-%2d-%2d ", systemtime.wYear, systemtime.wMonth, systemtime.wDay);
	printf("%2d:%2d:%2d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond);
}

// 文件名及后缀名均相等时返回true
bool FileNode :: operator == (const FileNode &x) const
{
	int len_file_name = strlen(x.file_Name);
	int len_file_suffix = strlen(x.file_Suffix);

	for (int i = 0; i < len_file_name; i++)
		if (x.file_Name[i] != file_Name[i])
			return false;
	for (int i = 0; i < len_file_suffix; i++)
		if (x.file_Suffix[i] != file_Suffix[i])
			return false;

	return true;
}