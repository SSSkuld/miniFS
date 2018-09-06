#pragma once

#include<Windows.h>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<malloc.h>
#include<fstream>
#include<iostream>

#define disk_size 1024 * 1024 * 1024
#define block_number 262144
#define block_size 4 * 1024
#define filenode_size 64
#define fat_size 4

// 申请空间
void create_memory();

// 删除空间
void delete_memory();

// 获取第k块首地址
char *get_kth_address(int k);

// 根据首地址获得块编号
int get_kth_number(char *p);

// 创建文件
void create_file(char [], int);

// 读文件
void read_file(char []);

// 删除文件
void delete_file(char []);