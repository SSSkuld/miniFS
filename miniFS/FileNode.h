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

//#endif // !FILENODE.H
