#pragma once

#ifndef  _SHELL_H
#define  _SHELL_H

#include <iostream>
#include "common.h"
#include "shellcommandtree.h"


class shell
{
public:
	shell(const String& user="default");
	~shell();

	void shellCore();
	char getChar();
	void execut(const String& str);
	void registerCmd(const String& path, const String& name, FUNC slot);

	shellCommandTree *getCurrentDir();
	void exit();
	bool switchMode(const String& mode);
private:
	void type_prompt();
	String getdir();
	shellCommandTree *enter(const String& path);
	shellCommandTree *enter(const List<String>& path);

	bool execSysCmd(shellCommandArgs args);

	static void shellcmd_cd(shellCommandArgs args);
	static void shellcmd_ls(shellCommandArgs args);
	static void shellcmd_exit(shellCommandArgs args);
	static void shellcmd_quit(shellCommandArgs args);
	static void shellcmd_switch(shellCommandArgs args);

	static void print(const String& str);
	static void print(const char& c);


private:
	String _user;
	String _host;
	bool _isrunning;
	bool _isexit;
	bool _sysshell;
	shellCommandTree *_cmdtree;
	shellCommandTree *_currentdir;
	static hashTable<String, shell*> _shells;
};

#endif //_SHELL_H

