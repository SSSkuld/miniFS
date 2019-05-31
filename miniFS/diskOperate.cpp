#include "stdafx.h"
#include "Memory.h"
#include "FileNode.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <malloc.h>

//fat表
int32_t fat[block_number];
bool bitmap[block_number];

//空闲块个数
int32_t unused_block_num = 0;

//文件节点表
std::vector<FileNode> folder[max_folder_number];
bool folder_map[max_folder_number];
int32_t fa_folder[max_folder_number];

//当前所在目录
int now_root_log;
char minifs_name[10];
bool is_create;

// 创建磁盘空间
void create_memory()
{
	using namespace std;
	// 1GB空间

	fstream disk_file;
	disk_file.open(const_address, ios::out);
	if (!disk_file.is_open())
	{
		printf("Create Fail!!!\n");
		return;
	}

//	/*
	// create ver 1.0
	disk_file.seekg(disk_size - 1, ios::cur);
	disk_file << '\0';
//	*/

	/*  create ver 2.0
	printf("Now creating...\n");
	disk_file.seekg(0, ios::cur);
	for (int i = 0; i < disk_size; i++)
	{
		disk_file << '$';
		printf("installation progress : %d / 100\%\r", (i / block_size) / 264);
	}
//	*/

	disk_file.close();
	printf("Created disk memory successfully!\n");
}

// 初始化
void fmt()
{
	// 初始化FAT表
	for (int i = 0; i < block_number; i++)
		fat[i] = -1;
	//初始化文件节点表
	for (int i = 0; i < max_folder_number; i++)
	{
		folder[i].resize(512);
		folder[i].clear();
	}
	unused_block_num = block_number;
	memset(bitmap, 0, sizeof bitmap);
	memset(folder_map, 0, sizeof(folder_map));
	folder_map[0] = 1;
	for (int i = 0; i < 6000; i++)
		bitmap[i] = 1;
	now_root_log = 0;
	memset(fa_folder, 0, sizeof(fa_folder));
	memset(minifs_name, '\0', sizeof(minifs_name));
	is_create = false;
}

// 安装空间
void mount_memory(char fs_name[])
{
	fmt();

	memset(minifs_name, '\0', sizeof(minifs_name));
	int len = strlen(fs_name);
	for (int i = 0; i < len; i++)
		minifs_name[i] = fs_name[i];
	
	is_create = true;

	printf("Installed successfully!\n");
	return;
}

// 删除磁盘空间
void delete_memory()
{
}

// 获得第k块内存偏移值
int32_t get_kth_block(int32_t k)
{
	return block_size * k;
}

// 将4096字节写入 第i块内存
void write_ith_block(int32_t block_num, std::ifstream &fin)
{
	using namespace std;
	ofstream fout;

	fout.open(const_address, ios::in | ios::out);
	fout.seekp(get_kth_block(block_num), ios::cur);

	// 写入4096字节的内容
	char ch;
	for (int i = 0; i < block_size; i++)
	{
		if (fin.get(ch))
		{
			fout << ch;
			// debug
		//	printf("%c", ch);

		}
		else
		{
			fout << '$';
			break;
		}
	}
	fout.flush();
	fout.close();
}

// 为文件分配空白块  返回占用的首块地址
int32_t set_block(char file_path[], int file_size)
{
	using namespace std;
	int32_t number_of_block = ceil((double)file_size / block_size);
	
	//debug
	//printf("need block number : %d\n", number_of_block);

	int32_t last_block_num = -1;
	
	ifstream file_in;
	file_in.open(file_path);

	bool use_first_block = false;
	int32_t fisrt_block_number = -1;

	// 遍历FAT表空白块
	for (int32_t i = 6000; i < block_number; i++)
	{
		if (!bitmap[i])
		{
			// 记录首块号
			if (!use_first_block)
			{
				use_first_block = true;
				fisrt_block_number = i;
			}

			//debug
			//printf("used block number : %d\n", i);

			// 将4096字节写入第i块
			write_ith_block(i, file_in);

			bitmap[i] = 1;
			if (last_block_num != -1)
				fat[last_block_num] = i;
			last_block_num = i;
			number_of_block -= 1;
			if (!number_of_block)
				break;
		}
	}

	file_in.close();
	return fisrt_block_number;
}

// 检测获取的当前时间
void debug_time(SYSTEMTIME test_time)
{
	printf("%04d-%02d-%02d ", test_time.wYear, test_time.wMonth, test_time.wDay);
	printf("%02d:%02d:%02d\n", test_time.wHour, test_time.wMinute, test_time.wSecond);
}

// 获取文件大小 单位：字节  需要传入文件路径
int32_t get_file_size(char file_path[])
{
	using namespace std;
	ifstream fin;
	fin.open(file_path);
	fin.seekg(0, ios::end);
	int32_t file_size = fin.tellg();
	fin.close();
	return file_size;
}

void show_file(FileNode x)
{
	printf("file name : ");
	x.show_file_name();
	printf("file create time : ");
	x.show_time();

	if (x.get_folder_number() == -1)
	{
		printf("file size : %dB\n", x.get_file_size());
	}
//	printf("first block number : %d\n", x.get_first_block_number());
	return;
}

/*
FileNode find_fcb(char full_file_name[], bool &is_find)
{
	char file_name[9]; char file_suffix[4];
	memset(file_name, '\0', sizeof file_name);
	memset(file_suffix, '\0', sizeof file_suffix);
	int full_len = strlen(full_file_name);
	int s = 0; int cnt = 0;
	for (int i = 0; i < full_len; i++)
	{
		if (full_file_name[i] == '.')
		{
			s = i;
			break;
		}
		file_name[cnt] = full_file_name[i];
		cnt += 1;
	}
	file_name[cnt] = '\0'; cnt = 0;
	for (int i = s + 1; i < full_len; i++)
	{
		file_suffix[cnt] = full_file_name[i];
		cnt += 1;
	}
	file_suffix[cnt] = '\0';

	//	for (int i = 0; i < strlen(file_name); i++) printf("%c", file_name[i]); printf("\n");

	FileNode find_file = FileNode(file_name, file_suffix, 0, 0, 0, -1);
	//	show_file(find_file);

	for (int i = 0; i < fcb.size(); i++)
	{
		if (fcb[i] == find_file)
		{
			is_find = true;
			find_file = fcb[i];
			break;
		}
	}

	return find_file;
}
*/ 

// 寻找文件夹编号
int32_t find_log(char folder_name[], int now_log)
{
	char folder_suffix[4];
	memset(folder_suffix, '\0', sizeof(folder_suffix));
	folder_suffix[0] = '$';

	FileNode find_folder = FileNode(folder_name, folder_suffix, 0, 0, 0, -1, -1);

	for (int32_t i = 0; i < folder[now_log].size(); i++)
	{
		if (folder[now_log][i] == find_folder)
		{
			return folder[now_log][i].get_folder_number();
		}
	}
	return -1;
}

// 展示文件属性  传入文件名 后缀名
void att(char fs_path[])
{
	char file_name[9];
	char file_suffix[5];
	memset(file_name, '\0', sizeof file_name);
	memset(file_suffix, '\0', sizeof file_suffix);

	// 赋值file_name与file_suffix
	int32_t len_path = strlen(fs_path);
	int32_t cnt = 0, s = 0;
	int32_t now_log = 0;
	bool is_folder = true;
	for (int i = 0; i < len_path; i++)
	{
		if (fs_path[i] == '\\')
		{
			now_log = find_log(file_name, now_log);
			if (now_log == -1)
			{
				printf("Can not find the file!\n");
				return;
			}
			cnt = 0;
			memset(file_name, '\0', sizeof file_name);
		}
		else if (fs_path[i] == '.')
		{
			is_folder = false;
			s = i;
			break;
		}
		else
		{
			file_name[cnt] = fs_path[i];
			cnt += 1;
		}
	}
	if (!is_folder)
	{
		cnt = 0;
		for (int i = s + 1; i < len_path; i++)
		{
			file_suffix[cnt] = fs_path[i];
			cnt += 1;
		}
	}
	else
	{
		file_suffix[0] = '$';
	}

	FileNode find_file = FileNode(file_name, file_suffix, 0, 0, 0, -1, -1);
	// debug 
//	find_file.show_file_name();

	bool is_find = false;
	for (int i = 0; i < folder[now_log].size(); i++)
	{
		if (folder[now_log][i] == find_file)
		{
			is_find = true;
			show_file(folder[now_log][i]);
			break;
		}
	}

	if (!is_find)
		printf("Can not find the file!\n");

	return;
}

// 在磁盘空间创建新文件 从windows目录到minifs目录
void copy_file(char file_path[], char fs_path[])
{
	using namespace std;

	/*
	//debug
	int len_of_name = strlen(file_path);
	int len_of_suffix = strlen(fs_name);
	for (int i = 0; i < len_of_name; i++)
	printf("%c", file_path[i]);
	printf(" ");
	for (int i = 0; i < len_of_suffix; i++)
	printf("%c", fs_name[i]);
	printf("\n");
	//	*/

	int32_t file_size = get_file_size(file_path);

	// 获得创建文件时的时间
	SYSTEMTIME systimeUtc;
	GetSystemTime(&systimeUtc);
	SYSTEMTIME systimeLocal;
	SystemTimeToTzSpecificLocalTime(NULL, &systimeUtc, &systimeLocal);
	FILETIME filetime;
	SystemTimeToFileTime(&systimeLocal, &filetime);
	uint64_t create_file_time = ((uint64_t)filetime.dwHighDateTime << 32) | filetime.dwLowDateTime;

	/*
	// 检测获取的当前时间
	debug_time(systime);
	cout << create_file_time << endl;
	cout << filetime.dwHighDateTime << " " << filetime.dwLowDateTime << endl;
	cout << (create_file_time >> 32) << " " << (create_file_time & 0xffffffff) << endl;
	*/

	// 文件所需存储块数 向上取整
	int32_t number_of_block = ceil((double)file_size / block_size);

	// debug
	//printf("need block number : %d\n", number_of_block);

	// 空间不足 报错
	if (unused_block_num < number_of_block)
	{
		printf("Memory is not enough!!!\n");
		return;
	}

	// 空间足够 分配空白块
	unused_block_num -= number_of_block;
	int32_t first_block_number = set_block(file_path, file_size);

	// debug
//	printf("first block number : %d\n", first_block_number);
//	printf("need block number : %d\n", number_of_block);

	// 创建新的文件节点FileNode
	char file_name[9];
	char file_suffix[5];
	memset(file_name, '\0', sizeof file_name);
	memset(file_suffix, '\0', sizeof file_suffix);

	// 赋值file_name与file_suffix
	int32_t len_path = strlen(fs_path);
	int32_t cnt = 0, s = 0;
	int32_t now_log = 0;
	for (int i = 0; i < len_path; i++)
	{
		if (fs_path[i] == '\\')
		{
			now_log = find_log(file_name, now_log);
			if (now_log == -1)
			{
				printf("Wrong Path!\n");
				return;
			}
			cnt = 0;
			memset(file_name, '\0', sizeof file_name);
		}
		else if (fs_path[i] == '.')
		{
			s = i;
			break;
		}
		else
		{
			file_name[cnt] = fs_path[i];
			cnt += 1;
		}
	}
	cnt = 0;
	for (int i = s + 1; i < len_path; i++)
	{
		file_suffix[cnt] = fs_path[i];
		cnt += 1;
	}

	// 串联文件节点表
	FileNode new_file = FileNode(file_name, file_suffix, create_file_time, file_size, first_block_number, -1, now_log);
	folder[now_log].push_back(new_file);

	// debug
//	new_file.show_file_name();

	//fcb.push_back(new_file);

	printf("Created File Successfully!\n");

	//debug
	//	show_file(new_file);
	//	printf("%d\n", new_file.get_next_file());
	return;
}

///*
// 读取文件内容
void read_file(char full_file_name[])
{
	using namespace std;
	bool is_find = false;

	char file_name[9]; char file_suffix[4];
	memset(file_name, '\0', sizeof(file_name));
	memset(file_suffix, '\0', sizeof(file_suffix));

	int len = strlen(full_file_name);
	int s = 0, cnt = 0;
	for (int i = 0; i < len; i++)
	{
		if (full_file_name[i] == '.')
		{
			s = i;
			cnt = 0;
			break;
		}
		else
		{
			file_name[cnt] = full_file_name[i];
			cnt += 1;
		}
	}
	for (int i = s + 1; i < len; i++)
	{
		file_suffix[i] = full_file_name[i];
		cnt += 1;
	}

	FileNode find_file = FileNode(file_name, file_suffix, 0, 0, 0, 0, 0);
	for (int i = 0; i < folder[now_root_log].size(); i++)
	{
		if (folder[now_root_log][i] == find_file)
		{
			is_find = true;
			find_file = folder[now_root_log][i];
			break;
		}
	}

	if (!is_find)
	{
		printf("Can not find the file!\n");
		return;
	}


	ifstream fin;
	fin.open(const_address);
	char ch;

	// 按fat块读取内容
	int32_t fat_read = find_file.get_first_block_number();
	while (fat_read != -1)
	{
		fin.seekg(get_kth_block(fat_read), ios::cur);
		for (int i = 0; i < block_size; i++)
		{
			if (fin.get(ch))
			{
				if (ch == '$')
					break;
				printf("%c", ch);
			}
			else break;
		}
		fat_read = fat[fat_read];
	}

	fin.close();

	printf("\n");
	return;
}
//*/

//清空fat表与bitmap
void clean_fat(int x)
{
	bitmap[x] = 0;
	while (fat[x] != -1)
	{
		clean_fat(fat[x]);	
		fat[x] = -1;
	}
	// debug
//	printf("fat[%d] : %d\n", x, fat[x]);
	return;
}

///*
//删除文件
void delete_file(char fs_path[])
{
	char file_name[9]; char file_suffix[4];
	memset(file_name, '\0', sizeof file_name);
	memset(file_suffix, '\0', sizeof file_suffix);

	bool is_file = false;
	int32_t len_path = strlen(fs_path);
	int32_t s = 0, cnt = 0, now_log = 0;
	for (int i = 0; i < len_path; i++)
	{
		if (fs_path[i] == '.')
		{
			is_file = true;
			s = i;
			break;
		}
		else
		{
			file_name[cnt] = fs_path[i];
			cnt += 1;
		}
	}

	if (!is_file)
	{
		printf("Cannot find the file!\n");
		return;
	}

	cnt = 0;
	for (int i = s + 1; i < len_path; i++)
	{
		file_suffix[cnt] = fs_path[i];
		cnt += 1;
	}

	FileNode find_file = FileNode(file_name, file_suffix, 0, 0, 0, -1, -1);

	std::vector<FileNode> ::iterator it;

	it = folder[now_root_log].begin();

	bool is_find = false;
	for (; it != folder[now_root_log].end(); it++)
	{
		if (*it == find_file)
		{
			is_find = true;
			clean_fat( (*it).get_first_block_number() );
			folder[now_root_log].erase(it, it + 1);
			break;
		}
	}

	if (!is_find)
	{
		printf("Can not find the file!\n");
		return;
	}

	printf("delete file successfully!\n");
	return;
}
//*/

// 列出文件目录
void list_file()
{
	/*
	if (fs_path[0] == '*' && strlen(fs_path) == 1)
	{
		for (int i = 0; i < folder[0].size(); i++)
			folder[0][i].show_file_name();
		return;
	}

	char file_name[9];
	char file_suffix[5];
	memset(file_name, '\0', sizeof file_name);
	memset(file_suffix, '\0', sizeof file_suffix);

	// 赋值file_name与file_suffix
	int32_t len_path = strlen(fs_path);
	int32_t cnt = 0, s = 0;
	int32_t now_log = 0;
	bool is_folder = true;
	for (int i = 0; i < len_path; i++)
	{
		if (fs_path[i] == '\\')
		{
			now_log = find_log(file_name, now_log);
			if (now_log == -1)
			{
				printf("Wrong Path!\n");
				return;
			}
			cnt = 0;
			memset(file_name, '\0', sizeof file_name);
		}
		else if (fs_path[i] == '.')
		{
			is_folder = false;
			s = i;
			break;
		}
		else
		{
			file_name[cnt] = fs_path[i];
			cnt += 1;
		}
	}
	if (!is_folder)
	{
		printf("Wrong Path!\n");
		return;
	}
	else
	{
		file_suffix[0] = '$';
	}

	FileNode find_folder = FileNode(file_name, file_suffix, 0, 0, 0, 0, -1);
	int find_log = -1;
	for (int i = 0; i < folder[now_log].size(); i++)
	{
		if (folder[now_log][i] == find_folder)
		{
			find_log = folder[now_log][i].get_folder_number();
			break;
		}
	}

	if (find_log == -1)
	{
		printf("Wrong Path!\n");
		return;
	}
	*/
	for (int i = 0; i < folder[now_root_log].size(); i++)
	{
		folder[now_root_log][i].show_file_name();
	}
	return;
}
//*/

void show_fat(int num)
{
	printf("%d ", num);
	if (fat[num] != -1)
		show_fat(fat[num]);
}
///*
// 列出占用fat块编号
void list_fat(char fs_path[])
{
	char file_name[9]; char file_suffix[4];
	memset(file_name, '\0', sizeof file_name);
	memset(file_suffix, '\0', sizeof file_suffix);
	
	bool is_file = false;
	int32_t len_path = strlen(fs_path);
	int32_t s = 0, cnt = 0, now_log = 0;
	for (int i = 0; i < len_path; i++)
	{
		if (fs_path[i] == '\\')
		{
			now_log = find_log(file_name, now_log);
			if (now_log == -1)
			{
				printf("Wrong Path!\n");
				return;
			}
			cnt = 0;
			memset(file_name, '\0', sizeof file_name);
		}
		else if (fs_path[i] == '.')
		{
			is_file = true;
			s = i;
			break;
		}
		else
		{
			file_name[cnt] = fs_path[i];
			cnt += 1;
		}
	}

	if (!is_file)
	{
		printf("Wrong Path!\n");
		return;
	}

	cnt = 0;
	for (int i = s + 1; i < len_path; i++)
	{
		file_suffix[cnt] = fs_path[i];
		cnt += 1;
	}

	FileNode find_file = FileNode(file_name, file_suffix, 0, 0, 0, -1, -1);
	
	bool is_find = false;
	for (int i = 0; i < folder[now_log].size(); i++)
	{
		if (find_file == folder[now_log][i])
		{
			printf("Used fat number : ");
			show_fat(folder[now_log][i].get_first_block_number());
			is_find = true;
			break;
		}
	}

	if (!is_find)
	{
		printf("Can not find the file!\n");
		return;
	}
	printf("\n");
	return;
}
//*/

// 创建文件夹
void create_folder(char fs_path[])
{
	char file_name[9];
	memset(file_name, '\0', sizeof(file_name));

	int32_t len_path = strlen(fs_path);
	int32_t cnt = 0;
	int32_t now_log = 0;
	for (int i = 0; i < len_path; i++)
	{
		if (fs_path[i] == '\\')
		{
			now_log = find_log(file_name, now_log);
			if (now_log == -1)
			{
				printf("Wrong Path!\n");
				return;
			}
			cnt = 0;
			memset(file_name, '\0', sizeof file_name);
		}
		else
		{
			file_name[cnt] = fs_path[i];
			cnt += 1;
		}
	}

	// 文件夹后缀为.$
	char folder_suffix[4];
	memset(folder_suffix, '\0', sizeof(folder_suffix));
	folder_suffix[0] = '$';

	// 获得创建文件时的时间
	SYSTEMTIME systimeUtc;
	GetSystemTime(&systimeUtc);
	SYSTEMTIME systimeLocal;
	SystemTimeToTzSpecificLocalTime(NULL, &systimeUtc, &systimeLocal);
	FILETIME filetime;
	SystemTimeToFileTime(&systimeLocal, &filetime);
	uint64_t create_file_time = ((uint64_t)filetime.dwHighDateTime << 32) | filetime.dwLowDateTime;


	int32_t new_log = -1;
	for (int i = 0; i < max_folder_number; i++)
	{
		if (folder_map[i] == 0)
		{
			new_log = i;
			folder_map[i] = 1;
			break;
		}
	}

	if (new_log == -1)
	{
		printf("Memory is not enough!!!\n");
		return;
	}

	FileNode new_folder = FileNode(file_name, folder_suffix, create_file_time, 0, -1, new_log, now_log);
	folder[now_log].push_back(new_folder);
	fa_folder[new_log] = now_log;

	// debug
//	new_folder.show_file_name();

	printf("Created Successfully!\n");
	return;
}

// 控制输出台字体颜色
void set_console_color(unsigned short color_index)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_index);
}

void show_tree_dep(int now_log, int dep)
{
	for (int i = 0; i < folder[now_log].size(); i++)
	{
		if (folder[now_log][i].get_folder_number() != -1)
		{
			for (int j = 0; j < 4 * dep; j++)
				printf(" ");
			printf("|__");
			folder[now_log][i].show_file_name();
			show_tree_dep(folder[now_log][i].get_folder_number(), dep + 1);
		}
		else
		{
			for (int j = 0; j < 4 * dep; j++)
				printf(" ");
			printf("|__");
			folder[now_log][i].show_file_name();
		}
	}
}

// 树形展示文件列表
void show_tree()
{
	int32_t now_log = 0, dep = 0;
	show_tree_dep(now_log, dep);
}

// 重命名minifs文件
void rename_file(char fs_path[], char fs_path2[])
{
	char fa_name[9]; char fa_suffix[4];
	memset(fa_name, '\0', sizeof(fa_name));
	memset(fa_suffix, '\0', sizeof(fa_suffix));
	int32_t fa_len = strlen(fs_path);
	int32_t s = 0, cnt = 0;
	bool is_file = false;
	for (int i = 0; i < fa_len; i++)
	{
		if (fs_path[i] == '.')
		{
			is_file = true;
			s = i;
			break;
		}
		else
		{
			fa_name[cnt] = fs_path[i];
			cnt += 1;
		}
	}
	if (is_file)
	{
		cnt = 0;
		for (int i = s + 1; i < fa_len; i++)
		{
			fa_suffix[cnt] = fs_path[i];
			cnt += 1;
		}
	}
	else
	{
		fa_suffix[0] = '$';
	}

	char file_name[9]; char file_suffix[4];
	memset(file_name, '\0', sizeof file_name);
	memset(file_suffix, '\0', sizeof file_suffix);
	fa_len = strlen(fs_path2);
	s = 0, cnt = 0;
	is_file = false;
	for (int i = 0; i < fa_len; i++)
	{
		if (fs_path2[i] == '.')
		{
			is_file = true;
			s = i;
			break;
		}
		else
		{
			file_name[cnt] = fs_path2[i];
			cnt += 1;
		}
	}
	if (is_file)
	{
		cnt = 0;
		for (int i = s + 1; i < fa_len; i++)
		{
			file_suffix[cnt] = fs_path2[i];
			cnt += 1;
		}
	}
	else
	{
		file_suffix[0] = '$';
	}

	bool is_find = false;
	FileNode fa_file = FileNode(fa_name, fa_suffix, 0, 0, 0, 0, 0);
	FileNode temp_file;
	for (int i = 0; i < folder[now_root_log].size(); i++)
	{
		if (fa_file == folder[now_root_log][i])
		{
			folder[now_root_log][i].rename_name(file_name);
			folder[now_root_log][i].rename_suffix(file_suffix);
			is_find = true;
			break;
		}
	}

	if (!is_find)
	{
		printf("Cannot find the file!\n");
		return;
	}
	printf("Rename successfully!\n");
	return;
}

void debug_show(FileNode x)
{
	printf("file name : %s\n", x.get_file_name());
	printf("file suffix : %s\n", x.get_file_suffix());
	printf("file size : %d\n", x.get_file_size());
	x.show_time();
	printf("file first block number : %d\n", x.get_first_block_number());
	printf("file folder number : %d\n", x.get_folder_number());
	printf("file belong folder number : %d\n", x.get_belong_folder_number());
}

// folder和fat写回硬盘
void write_to_disk()
{	
	FILE *fp = fopen(const_address, "rb+");

	// 写出fat表
	fwrite(fat, sizeof(int32_t), block_number, fp);

	// 写出bitmap
	fwrite(bitmap, sizeof(bool), block_number, fp);
	
	// 写出folder
	for (int i = 0; i < max_folder_number; i++)
	{
		int32_t len = folder[i].size();

		// debug
//		printf("folder %d : \n", i);
		
		fwrite(&len, sizeof(int32_t), 1, fp);
		for (int j = 0; j < len; j++)
		{
			char * fsn = (char *)calloc(sizeof(char), 9);
			fsn = folder[i][j].get_file_name();
			fwrite(fsn, sizeof(char), 9, fp);

			char * fsnn = (char *)calloc(sizeof(char), 4);
			fsnn = folder[i][j].get_file_suffix();
			fwrite(fsnn, sizeof(char), 4, fp);

			uint64_t date;
			date = folder[i][j].get_create_Date();
			fwrite(&date, sizeof(uint64_t), 1, fp);

			date = folder[i][j].get_change_Date();
			fwrite(&date, sizeof(uint64_t), 1, fp);

			int32_t temp;
			temp = folder[i][j].get_file_size();
			fwrite(&temp, sizeof(int32_t), 1, fp);

			temp = folder[i][j].get_first_block_number();
			fwrite(&temp, sizeof(int32_t), 1, fp);

			temp = folder[i][j].get_folder_number();
			fwrite(&temp, sizeof(int32_t), 1, fp);

			temp = folder[i][j].get_belong_folder_number();
			fwrite(&temp, sizeof(int32_t), 1, fp);
			
			// debug
//			debug_show(folder[i][j]);
		}
	}

	//写出folder_map
	fwrite(folder_map, sizeof(bool), max_folder_number, fp);

	//写出unused_block_number
	fwrite(&unused_block_num, sizeof(int32_t), 1, fp);

	fwrite(fa_folder, sizeof(int32_t), max_folder_number, fp);

	fwrite(minifs_name, sizeof(char), 10, fp);
	fwrite(&is_create, sizeof(bool), 1, fp);
	return;
}

// folder和fat读入内存
void read_to_memory()
{
	fmt();

	FILE *fp = fopen(const_address, "rb+");

	// 写回fat表
	fread(fat, sizeof(int32_t), block_number, fp);

	// 写出bitmap
	fread(bitmap, sizeof(bool), block_number, fp);

	// 写出folder
	for (int i = 0; i < max_folder_number; i++)
	{
		int32_t len = 0;
		fread(&len, sizeof(int32_t), 1, fp);
		for (int j = 0; j < len; j++)
		{
			
			char * file_name = (char *)calloc(sizeof(char), 9);
			fread(file_name, sizeof(char), 9, fp);

			char * file_suffix = (char *)calloc(sizeof(char), 4);
			fread(file_suffix, sizeof(char), 4, fp);
			
			uint64_t create_date;
			fread(&create_date, sizeof(uint64_t), 1, fp);

			uint64_t change_date;
			fread(&change_date, sizeof(uint64_t), 1, fp);

			int32_t file_size;
			fread(&file_size, sizeof(int32_t), 1, fp);

			int32_t first_block_number;
			fread(&first_block_number, sizeof(int32_t), 1, fp);

			int32_t folder_number;
			fread(&folder_number, sizeof(int32_t), 1, fp);

			int32_t belong_folder_number;
			fread(&belong_folder_number, sizeof(int32_t), 1, fp);

			FileNode new_file = FileNode(file_name, file_suffix, create_date, file_size, first_block_number, folder_number, belong_folder_number);
			
			//debug
	//		debug_show(new_file);
			
			folder[belong_folder_number].push_back(new_file);
		}
	}

	//写出folder_map
	fread(folder_map, sizeof(bool), max_folder_number, fp);

	//写出unused_block_number
	fread(&unused_block_num, sizeof(int32_t), 1, fp);

	fread(fa_folder, sizeof(int32_t), max_folder_number, fp);
	fread(minifs_name, sizeof(char), 10, fp);
	fread(&is_create, sizeof(bool), 1, fp);
	return;
}

void cd(char fs_folder_name[])
{
	if (fs_folder_name[0] == '.' && fs_folder_name[1] == '.')
	{
		now_root_log = fa_folder[now_root_log];
		return;
	}

	char fs_folder_suffix[4];
	memset(fs_folder_suffix, '\0', sizeof(fs_folder_suffix));
	fs_folder_suffix[0] = '$';

	FileNode find_file = FileNode(fs_folder_name, fs_folder_suffix, 0, 0, 0, 0, 0);

	bool is_find = false;
	for (int i = 0; i < folder[now_root_log].size(); i++)
	{
		if (folder[now_root_log][i] == find_file)
		{
			is_find = true;
			now_root_log = folder[now_root_log][i].get_folder_number();
			break;
		}
	}

	if (!is_find)
	{
		printf("Cannot find the file!\n");
		return;
	}

	return;
}

void find_fa(int x)
{
	if (fa_folder[x] != x)
	{
		find_fa(fa_folder[x]);
		for (int i = 0; i < folder[ fa_folder[x] ].size(); i ++)
		{
			if (folder[fa_folder[x]][i].get_folder_number() == x)
			{
				printf("%s", folder[fa_folder[x]][i].get_file_name());
				printf(">>");
				break;
			}
		}
	}
	return;
}

void pre()
{
	if (is_create)
	{
		set_console_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		int len = strlen(minifs_name);
		for (int i = 0; i < len; i++)
			printf("%c", minifs_name[i]);
		printf(">>");

		find_fa(now_root_log);
		
		set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
}

int32_t set_file_block(FileNode fs_file, int now_log)
{
	using namespace std;

	int32_t x = fs_file.get_first_block_number();

	ifstream fin;
	fin.open(const_address, ios::in | ios::binary);

	int32_t last_block = -1;
	int32_t first_block = -1;
	bool is_first = true;

	for (int32_t i = 6000; i < block_number; i++)
	{
		if (!bitmap[i])
		{

			if (is_first)
			{
				is_first = false;
				first_block = i;
			}

			bitmap[i] = 1;
			fin.seekg(get_kth_block(x));
			write_ith_block(i, fin);

			if (last_block != -1)
				fat[last_block] = i;
			last_block = i;

			x = fat[x];
			if (x == -1)
				break;
		}
	}
	
	return first_block;
}

// minifs内部拷贝文件 path1 -> path2
void copy_file_to_file(char fs_path1[], char fs_path2[])
{
	char file_name[9]; char file_suffix[4];
	memset(file_name, '\0', sizeof file_name);
	memset(file_suffix, '\0', sizeof file_suffix);

	bool is_file = false;
	int32_t len_path = strlen(fs_path2);
	int32_t s = 0, cnt = 0, now_log = 0;
	for (int i = 0; i < len_path; i++)
	{
		if (fs_path2[i] == '\\')
		{
			now_log = find_log(file_name, now_log);
			if (now_log == -1)
			{
				printf("Wrong Path!\n");
				return;
			}
			cnt = 0;
			memset(file_name, '\0', sizeof file_name);
		}
		else if (fs_path2[i] == '.')
		{
			is_file = true;
			s = i;
			break;
		}
		else
		{
			file_name[cnt] = fs_path2[i];
			cnt += 1;
		}
	}

	if (!is_file)
	{
		printf("Wrong Path!\n");
		return;
	}

	cnt = 0;
	for (int i = s + 1; i < len_path; i++)
	{
		file_suffix[cnt] = fs_path2[i];
		cnt += 1;
	}
	
	char fa_name[9]; char fa_suffix[4];
	memset(fa_name, '\0', sizeof(fa_name));
	memset(fa_suffix, '\0', sizeof(fa_suffix));
	int32_t fa_len = strlen(fs_path1);
	s = 0;  cnt = 0;
	for (int i = 0; i < fa_len; i++)
	{
		if (fs_path1[i] == '.')
		{
			s = i;
			break;
		}
		else
		{
			fa_name[cnt] = fs_path1[i];
			cnt += 1;
		}
	}
	cnt = 0;
	for (int i = s + 1; i < fa_len; i++)
	{
		fa_suffix[cnt] = fs_path1[i];
		cnt += 1;
	}
	
	bool is_find = false;
	FileNode fa_file = FileNode(fa_name, fa_suffix, 0, 0, 0, 0, 0);
	FileNode temp_file;
	for (int i = 0; i < folder[now_root_log].size(); i++)
	{
		if (fa_file == folder[now_root_log][i])
		{
			temp_file = folder[now_root_log][i];
			is_find = true;
			break;
		}
	}

	if (!is_find)
	{
		printf("Cannot find the file!\n");
		return;
	}

	int need_block = ceil((double)temp_file.get_file_size() / block_size);
	if (unused_block_num > need_block)
	{
		unused_block_num -= need_block;
	}
	else
	{
		printf("Memory not enough!\n");
		return;
	}

	SYSTEMTIME systimeUtc;
	GetSystemTime(&systimeUtc);
	SYSTEMTIME systimeLocal;
	SystemTimeToTzSpecificLocalTime(NULL, &systimeUtc, &systimeLocal);
	FILETIME filetime;
	SystemTimeToFileTime(&systimeLocal, &filetime);
	uint64_t create_file_time = ((uint64_t)filetime.dwHighDateTime << 32) | filetime.dwLowDateTime;

	int32_t first_block = set_file_block(temp_file, now_log);

	FileNode copy_file = FileNode(file_name, file_suffix, create_file_time, temp_file.get_file_size(), first_block, -1, now_log);

	folder[now_log].push_back(copy_file);

	printf("Copy successfully!\n");
	return;
}

void create_file(char fs_path1[])
{
	char fa_name[9]; char fa_suffix[4];
	memset(fa_name, '\0', sizeof(fa_name));
	memset(fa_suffix, '\0', sizeof(fa_suffix));
	int32_t fa_len = strlen(fs_path1);
	int32_t s = 0, cnt = 0;
	for (int i = 0; i < fa_len; i++)
	{
		if (fs_path1[i] == '.')
		{
			s = i;
			break;
		}
		else
		{
			fa_name[cnt] = fs_path1[i];
			cnt += 1;
		}
	}
	cnt = 0;
	for (int i = s + 1; i < fa_len; i++)
	{
		fa_suffix[cnt] = fs_path1[i];
		cnt += 1;
	}

	SYSTEMTIME systimeUtc;
	GetSystemTime(&systimeUtc);
	SYSTEMTIME systimeLocal;
	SystemTimeToTzSpecificLocalTime(NULL, &systimeUtc, &systimeLocal);
	FILETIME filetime;
	SystemTimeToFileTime(&systimeLocal, &filetime);
	uint64_t create_file_time = ((uint64_t)filetime.dwHighDateTime << 32) | filetime.dwLowDateTime;

	int32_t used_block;
	for (int i = 6000; i < block_number; i++)
	{
		if (!bitmap[i])
		{
			bitmap[i] = 1;
			used_block = i;
			break;
		}
	}

	FileNode new_file = FileNode(fa_name, fa_suffix, create_file_time, 0, used_block, -1, now_root_log);
	folder[now_root_log].push_back(new_file);

	printf("Created successfully!\n");

	return;
}

void text_file(char fs_path1[])
{
	char fa_name[9]; char fa_suffix[4];
	memset(fa_name, '\0', sizeof(fa_name));
	memset(fa_suffix, '\0', sizeof(fa_suffix));
	int32_t fa_len = strlen(fs_path1);
	int32_t s = 0, cnt = 0;
	for (int i = 0; i < fa_len; i++)
	{
		if (fs_path1[i] == '.')
		{
			s = i;
			break;
		}
		else
		{
			fa_name[cnt] = fs_path1[i];
			cnt += 1;
		}
	}
	cnt = 0;
	for (int i = s + 1; i < fa_len; i++)
	{
		fa_suffix[cnt] = fs_path1[i];
		cnt += 1;
	}

	FileNode find_file = FileNode(fa_name, fa_suffix, 0, 0, 0, 0, 0);

	using namespace std;

	bool is_find = false;
	for (int i = 0; i < folder[now_root_log].size(); i++)
	{
		if (folder[now_root_log][i] == find_file)
		{
			is_find = true;

			ofstream fin;
			fin.open(const_address, ios::in | ios::out);
			fin.seekp(get_kth_block(folder[now_root_log][i].get_first_block_number()), ios::cur);

			// fat bug

			char ch;
			int32_t new_size = 0;
			while (~scanf("%c", &ch))
			{
				fin << ch;
				new_size += 1;
				if (ch == '$')
					break;
			}
			folder[now_root_log][i].change_file_size(new_size);
			break;
		}
	}

	if (!is_find)
	{
		printf("Cannot find the file!\n");
		return;
	}

	printf("Change successfully!\n");

	return;
}