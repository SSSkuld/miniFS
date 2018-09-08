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
#define const_address "1.txt"

// ����ռ�
void create_memory();

// ɾ���ռ�
void delete_memory();

// ��ȡ��k���׵�ַ
char *get_kth_address(int k);

// �����׵�ַ��ÿ���
int get_kth_number(char *p);

// �����ļ�
void create_file(char [], int);

// ���ļ�
void read_file(char []);

// ɾ���ļ�
void delete_file(char []);