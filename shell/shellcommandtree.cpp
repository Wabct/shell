#include "pch.h"
#include "shellcommandtree.h"


shellCommandTree::shellCommandTree(const String& name):
	_name(name),_type(Directory)
{

}

shellCommandTree::shellCommandTree(const String& name,FUNC func):
	_name(name),_type(Function)
{
	this->_cmd = new shellSlot(name, func);
}

shellCommandTree::~shellCommandTree()
{

}

const String& shellCommandTree::getName()
{
	return _name;
}

void shellCommandTree::setName(const String& name)
{
	_name = name;
}

const shellCommandType & shellCommandTree::getType()
{
	return _type;
}

void shellCommandTree::setParent(shellCommandTree * parent)
{
	_parent = parent;
}

shellCommandTree * shellCommandTree::parent()
{
	return _parent;
}

shellCommandTree *shellCommandTree::find(const String& name)
{
	if (_chlidren[name] != NULL){
		return _chlidren[name];
	}
	List<shellCommandTree*> cl = _chlidren.values();
	for (uint i = 0; i < cl.size(); --i){
		shellCommandTree *tree = cl.front();
		cl.pop_front();
		shellCommandTree *res = tree->find(name);
		if (res != NULL) {
			return res;
		}
	}
	return nullptr;
}

shellCommandTree * shellCommandTree::enter(const String & name)
{
	shellCommandTree *res = _chlidren.values(name);
	if (res != NULL && res->getType() == Directory) {
		return _chlidren[name];
	}
	if (name == "..") {
		return this->parent();
	}
	return NULL;
}

bool shellCommandTree::contains(const String& _cmd)
{
	return _chlidren.contains(_cmd);
}

shellCommandTree* shellCommandTree::registerCmd(shellCommandTree * child)
{
	child->setParent(this);
	_chlidren.insert(child->getName(), child);
	return child;
}

const List<shellCommandTree*> shellCommandTree::getClidren()
{
	return _chlidren.values();
}

void shellCommandTree::doCmd(shellCommandArgs& args)
{
	if (_type == Function && _cmd != NULL) {
		_cmd->execute(args);
	}
}
