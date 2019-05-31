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

// ����ռ�
void create_memory();

// ��װ�ռ�
void mount_memory(char []);

// ɾ���ռ�
void delete_memory();

// ��windows·�������ļ�
void copy_file(char file_path[], char fs_name[]);

// ���ļ�
void read_file(char fs_name[]);

// ɾ���ļ�
void delete_file(char fs_name[]);

// ��ȡwindows�ļ���С  ��λ���ֽ�  ��Ҫ�����ļ�·��
int32_t get_file_size(char file_path[]);

// ��ʾ�ļ�����
void att(char fs_name[]);

// �г��ļ�Ŀ¼
void list_file();

// �г�ռ�õ�fat����
void list_fat(char fs_name[]);

// �����ļ���
void create_folder(char fs_path[]);

// �������̨��ɫ
void set_console_color(unsigned short color_index);

// ����չʾĿ¼
void show_tree();

// �ڴ�����д�ش���
void write_to_disk();

// folder��fat�����ڴ�
void read_to_memory();

// Ԥ����
void pre();

// ��ʼ��
void fmt();

// ����Ŀ¼
void cd(char fs_path[]);

// miniFS�ڲ��ļ�����
void copy_file_to_file(char[], char[]);

// ������
void rename_file(char[], char[]);

// �½��ļ�
void create_file(char[]);

void text_file(char[]);

void move_file(char[], char[]);