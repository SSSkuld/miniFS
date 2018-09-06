#pragma once
#ifndef FATList.h
#define FATList.h

#include <cstdio>
#include <cstring>

#define MAX_Block 262144

class FATlist
{
public:
	FATlist();
	~FATlist();
	
private:
	int fat[MAX_Block];
};

FATlist::FATlist()
{
	std::memset(fat, -1, sizeof fat);
}

FATlist::~FATlist()
{

}


#endif // !FATList.h
