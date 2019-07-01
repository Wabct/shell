#pragma once

#ifndef  _SHELLSLOT_H
#define  _SHELLSLOT_H

#include<string>
#include "common.h"
#include "shellCommandArgs.h"
#include <functional>
//typedef void (*FUNC)(shellCommandArgs);

using FUNC = std::function<void(shellCommandArgs)>;

class shellSlot
{
public:
	shellSlot(const String& name, FUNC func);
	~shellSlot();

	const String& getName();
	void registerCmd(FUNC fun);
	void execute(shellCommandArgs &args);
private:
	String _name;
	FUNC _func;
};

#endif //_SHELLSLOT_H
