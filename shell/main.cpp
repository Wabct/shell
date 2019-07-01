// shell.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <direct.h>
#include <time.h>
#include "shell.h"

using namespace std;

void test(shellCommandArgs args) 
{
	std::cout << '\n' <<args.toString();
}

int main()
{
	//system("ping 8.8.8.8");
	//shell s("user");
	//s.registerCmd("shell/usr", "test", test);
	//s.shellCore(); 
	hashTable<String, int> htb;
	unordered_map<String, int> umap;

	clock_t t0 = clock();

	for (int i = 0; i < 10000; ++i) {
		htb.insert(std::to_string(i), i);
	}
	clock_t t1 = clock();
	for (int i = 0; i < 10000; ++i) {
		umap.insert(pair<String,int>(std::to_string(i), i));
	}
	clock_t t2 = clock();
	cout << (t1 - t0) << endl;
	cout << (t2 - t1) << endl;
    std::cout << "Hello World!\n"; 
}

