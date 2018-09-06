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
	�Ȳ������ļ�����
	ֻд�ļ���
	����ȫΪ private ����
	��Ҫ�������б�д public���ʽӿ�

	��������:
		�����ô�д�շ�
		���ڱ������� _ �ָ�� ���׵���Сд�����൥������ĸ����д
		���ں��� _�ָ�� ��Сд
*/
class FileNode
{
public:
	//��ʼ�����캯��
	FileNode();
	~FileNode();

	//��ʼ�����캯��
	//�����б�����Ϊ���ļ��� ��׺�� �������� �ļ���С �׿���
	FileNode(char [], char [], uint64_t, int, int);
		
	void show_time(FileNode x);

	//����==����� ���ҽ����ļ������׺�������ʱ����true
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

//��uint64�����ʱ����year-month-day hour:minute:second��ʽ��Ļ��ӡ
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
