#pragma once
#ifndef  _SHELLCOMMANDTREE_H
#define  _SHELLCOMMANDTREE_H

#include <list>
#include <string>
#include <unordered_map>
#include "shellSlot.h"
#include "hashTable.hpp"

enum shellCommandType {
	Function,
	Directory
};

class shellCommandTree
{
public:
	shellCommandTree(const String& name);
	shellCommandTree(const String& name, FUNC func);
	~shellCommandTree();

	const String& getName();
	void setName(const String& name);
	const shellCommandType& getType();
	void setParent(shellCommandTree* parent);
	shellCommandTree* parent();
	shellCommandTree* find(const String& name);
	shellCommandTree* enter(const String& name);
	bool contains(const String& _cmd);
	void doCmd(shellCommandArgs& args);
	shellCommandTree* registerCmd(shellCommandTree* child);
	const List<shellCommandTree*> getClidren();
private :
	

private:
	String _name;
	shellCommandType _type;
	shellSlot* _cmd;
	shellCommandTree *_parent;
	hashTable<String, shellCommandTree*> _chlidren;
};


#endif //_SHELLCOMMANDTREE_H

