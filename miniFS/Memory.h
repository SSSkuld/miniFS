#pragma once

#include<Windows.h>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<malloc.h>
#include<fstream>
#include<iostream>

#define disk_size 1073741824 // 1024 * 1024 * 1024
#define block_number 262144
#define block_size 4096
#define filenode_size 64
#define fat_size 4
#define max_folder_number 512
#define const_address "D:\\1.txt"

// 申请空间
void create_memory();

// 安装空间
void mount_memory(char []);

// 删除空间
void delete_memory();

// 从windows路径复制文件
void copy_file(char file_path[], char fs_name[]);

// 读文件
void read_file(char fs_name[]);

// 删除文件
void delete_file(char fs_name[]);

// 获取windows文件大小  单位：字节  需要传入文件路径
int32_t get_file_size(char file_path[]);

// 显示文件属性
void att(char fs_name[]);

// 列出文件目录
void list_file();

// 列出占用的fat块编号
void list_fat(char fs_name[]);

// 创建文件夹
void create_folder(char fs_path[]);

// 更改输出台颜色
void set_console_color(unsigned short color_index);

// 树形展示目录
void show_tree();

// 内存内容写回磁盘
void write_to_disk();

// folder和fat读入内存
void read_to_memory();

// 预处理
void pre();

// 初始化
void fmt();

// 进入目录
void cd(char fs_path[]);

// miniFS内部文件复制
void copy_file_to_file(char[], char[]);

// 重命名
void rename_file(char[], char[]);

// 新建文件
void create_file(char[]);

void text_file(char[]);

void move_file(char[], char[]);