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
	//初始化构造函数
	FileNode();
	~FileNode();

	//初始化构造函数
	//参数列表依次为：文件名 后缀名 创建日期 文件大小 首块编号 文件夹编号 父级文件夹编号
	FileNode(char [], char [], uint64_t, int32_t, int32_t, int32_t, int32_t);
		
	//按照1970-01-01 00:00:00 格式打印时间
	void show_time();

	//重载==运算符 当且仅当文件名与后缀名都相等时返回true
	bool operator == (const FileNode &x) const;

	//获取file_size接口
	int32_t get_file_size();
	
	//打印文件名接口
	void show_file_name();

	//获取首块编号
	int32_t get_first_block_number();

	//获取文件夹编号
	int32_t get_folder_number();
	
	//获取父级文件夹编号
	int32_t get_belong_folder_number();

	//获取创建时间
	uint64_t get_create_Date();

	//获取修改时间
	uint64_t get_change_Date();

	//获得文件名
	char * get_file_name();

	//获得文件后缀
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
