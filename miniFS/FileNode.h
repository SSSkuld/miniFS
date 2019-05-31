//#ifndef FILENODE.H
//#define FILENODE.H

#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include <string.h>
#include <cstdlib>
#include <Windows.h>

class FileNode
{
public:
	//��ʼ�����캯��
	FileNode();
	~FileNode();

	//��ʼ�����캯��
	//�����б�����Ϊ���ļ��� ��׺�� �������� �ļ���С �׿��� �ļ��б�� �����ļ��б��
	FileNode(char [], char [], uint64_t, int32_t, int32_t, int32_t, int32_t);
		
	//����1970-01-01 00:00:00 ��ʽ��ӡʱ��
	void show_time();

	//����==����� ���ҽ����ļ������׺�������ʱ����true
	bool operator == (const FileNode &x) const;

	//��ȡfile_size�ӿ�
	int32_t get_file_size();
	
	//��ӡ�ļ����ӿ�
	void show_file_name();

	//��ȡ�׿���
	int32_t get_first_block_number();

	//��ȡ�ļ��б��
	int32_t get_folder_number();
	
	//��ȡ�����ļ��б��
	int32_t get_belong_folder_number();

	//��ȡ����ʱ��
	uint64_t get_create_Date();

	//��ȡ�޸�ʱ��
	uint64_t get_change_Date();

	//����ļ���
	char * get_file_name();

	//����ļ���׺
	char * get_file_suffix();

	void rename_name(char []);

	void rename_suffix(char[]);

	void change_file_size(int32_t);

private:
	char file_Name[9];
	char file_Suffix[4];
	uint64_t create_Date;
	uint64_t change_Date;
	int32_t file_Size;
	int32_t first_Block_Num;
	int32_t folder_number;
	int32_t belong_folder_number;
};

//#endif // !FILENODE.H
