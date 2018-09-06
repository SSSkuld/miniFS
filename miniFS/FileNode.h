//#ifndef FILENODE.H
//#define FILENODE.H


#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include <string.h>
#include <cstdlib>
#include <Windows.h>

/*
	先不考虑文件夹类
	只写文件类
	变量全为 private 类型
	需要调用自行编写 public访问接口

	命名规则:
		类名用大写驼峰
		类内变量名用 _ 分割单词 除首单词小写外其余单词首字母均大写
		类内函数 _分割单词 均小写
*/
class FileNode
{
public:
	//初始化构造函数
	FileNode();
	~FileNode();

	//初始化构造函数
	//参数列表依次为：文件名 后缀名 创建日期 文件大小 首块编号
	FileNode(char [], char [], uint64_t, int, int);
		
	void show_time(FileNode x);

	//重载==运算符 当且仅当文件名与后缀名都相等时返回true
	bool operator == (const FileNode &x) const;

private:
	char file_Name[9];
	char file_Suffix[4];
	uint64_t create_Date;
	int file_Size;
	int first_Block_Num;
};

FileNode::FileNode()
{
	memset(file_Name, '\0', sizeof(file_Name));
	memset(file_Suffix, '\0', sizeof(file_Suffix));
	create_Date = 0;
	file_Size = 0;
	first_Block_Num = -1;
}

FileNode::FileNode(char _file_Name[], char _file_Suffix[], uint64_t _create_Date, int _file_Size, int _first_Block_Num)
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
void FileNode::show_time(FileNode x)
{
	SYSTEMTIME systemtime;
	FILETIME filetime;

	filetime.dwHighDateTime = x.create_Date >> 32;
	filetime.dwLowDateTime = x.create_Date & 0xffffffff;

	FileTimeToSystemTime(&filetime, &systemtime);

	printf("%4d-%2d-%2d ", systemtime.wYear, systemtime.wMonth, systemtime.wDay);
	printf("%2d:%2d:%2d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond);
}

FileNode::~FileNode()
{
}

//#endif // !FILENODE.H
