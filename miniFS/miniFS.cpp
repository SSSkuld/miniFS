// miniFS.cpp: 定义控制台应用程序的入口点。

#include "stdafx.h"
#include "Memory.h"
#include "FileNode.h"
#include<cstdlib>
#include<cstring>
#include<string>
#include<iostream>


int main()
{
	using namespace std;
	set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	string ins;
	string path, pathh;
	char p[256], pp[256];
	char minifs_name[20];
	bool if_mount = false;
	memset(minifs_name, '\0', sizeof minifs_name);	

	read_to_memory();

	while (1)
	{
		pre();
		
		cin >> ins;
		if (ins == "create")
		{
			create_memory();
		}
		else if (ins == "mount")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
			{
				p[i] = path[i];
				minifs_name[i] = path[i];
				if_mount = true;
			}
			mount_memory(p);
		}
		else if (ins == "copy")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			cin >> pathh;
			memset(pp, '\0', sizeof(pp));
			for (int i = 0; i < pathh.length(); i++)
				pp[i] = pathh[i];// , printf("%c", pp[i]); printf("\n");
			// debug
//			cout << path << " " << pathh << endl;
			copy_file(p, pp);
		}
		else if (ins == "att")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			att(p);
		}
		/*
		if (ins == "text")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			read_file(p);
		}
		*/
		else if (ins == "close")
		{
			write_to_disk();
			printf("exit successfully!\n");
			break;
		}
		else if (ins == "fmt")
		{
			fmt();
		}
		else if (ins == "delete")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			delete_file(p);
		}
		
//		/*
		else if (ins == "list")
		{
			/*
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			*/
			list_file();
		}
		
		else if (ins == "map")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			list_fat(p);
		}
		
		else if (ins == "mkdir")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			create_folder(p);
		}

		else if (ins == "tree")
		{
			show_tree();
		}
		else if (ins == "cd")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			cd(p);
		}
		else if (ins == "copyfs")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			cin >> pathh;
			memset(pp, '\0', sizeof(pp));
			for (int i = 0; i < pathh.length(); i++)
				pp[i] = pathh[i];// , printf("%c", pp[i]); printf("\n");
								 // debug
								 //			cout << path << " " << pathh << endl;
			copy_file_to_file(p, pp);
		}
		else if (ins == "rename")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			cin >> pathh;
			memset(pp, '\0', sizeof(pp));
			for (int i = 0; i < pathh.length(); i++)
				pp[i] = pathh[i];// , printf("%c", pp[i]); printf("\n");
								 // debug
								 //			cout << path << " " << pathh << endl;
			rename_file(p, pp);
		}
		else if (ins == "new")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			create_file(p);
		}
		else if (ins == "text")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			text_file(p);
		}
		else if (ins == "move")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			cin >> pathh;
			memset(pp, '\0', sizeof(pp));
			for (int i = 0; i < pathh.length(); i++)
				pp[i] = pathh[i];// , printf("%c", pp[i]); printf("\n");
								 // debug
								 //			cout << path << " " << pathh << endl;
			copy_file_to_file(p, pp);
			delete_file(p);
		}
		else if (ins == "read")
		{
			cin >> path;
			memset(p, '\0', sizeof(p));
			for (int i = 0; i < path.length(); i++)
				p[i] = path[i];
			read_file(p);
		}
		else
		{
			printf("Wrong instruction!\n");
		}
	}

//	Sleep(1000);
	system("PAUSE");
    return 0;
}

