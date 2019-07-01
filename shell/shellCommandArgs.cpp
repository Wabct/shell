#include "pch.h"
#include "shellCommandArgs.h"


shellCommandArgs::shellCommandArgs(const String& str, const String& sender):
	_inputstr(str),_sender(sender)
{
	List<String> strs = split(str, ' ');
	if (strs.size() > 0) {
		_cmd = strs.front();
		strs.pop_front();
		_args = strs;
	}
}


shellCommandArgs::~shellCommandArgs()
{

}

const String& shellCommandArgs::getCmd()
{
	return _cmd;
}

const String& shellCommandArgs::operator[](int index)
{
	List<String>::iterator iter = _args.begin();
	for (int i = 0; i < index; ++ i) {
		++iter;
	}
	return (*iter);
}

const List<String>& shellCommandArgs::getArgs()
{
	return _args;
}

const String & shellCommandArgs::getSender()
{
	return _sender;
}

void shellCommandArgs::append(const String& str)
{
	_args.push_back(str);
}

int shellCommandArgs::argCount()
{
	return _args.size();
}

const String & shellCommandArgs::toString()
{
	return _inputstr;
}

List<String> shellCommandArgs::split(const String & str, const char & splitter)
{
	List<String> res;
	if (str == "") return res;
	String strs = str + splitter;
	int pos = strs.find(splitter);

	while (pos != strs.npos) {
		String substr = strs.substr(0, pos);
		if (substr != "") res.push_back(substr);
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(splitter);
	}
	return res;
}
