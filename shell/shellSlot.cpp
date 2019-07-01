#include "pch.h"
#include "shellSlot.h"


shellSlot::shellSlot(const String& name, FUNC func):
	_name(name), _func(func)
{
	
}


shellSlot::~shellSlot()
{

}

const String& shellSlot::getName()
{
	return _name;
}

void shellSlot::registerCmd(FUNC fun)
{
	_func = fun;
}

void shellSlot::execute(shellCommandArgs &args)
{
	if (_func != NULL) {
		_func(args);
	}
}

