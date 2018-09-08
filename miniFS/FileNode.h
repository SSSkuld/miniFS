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

//#endif // !FILENODE.H
