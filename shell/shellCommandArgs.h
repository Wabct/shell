#pragma once

#ifndef  _SHELLCOMMANDARGS_H
#define  _SHELLCOMMANDARGS_H

#include "common.h"

class shellCommandArgs
{
public:
	shellCommandArgs(const String& str, const String& sender);
	~shellCommandArgs();

	const String& getCmd();
	const String& operator[] (int index);
	const List<String>& getArgs();
	const String& getSender();
	void append(const String& str);
	int argCount();

	const String& toString();
	static List<String> split(const String&str, const char &splitter);
private:
	
private:
	String _sender;
	String _inputstr;
	String _cmd;
	List<String> _args;
};

#endif // _SHELLCOMMANDARGS_H
