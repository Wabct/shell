#include "pch.h"
#include <conio.h>
#include <stack>

#include "shell.h"
#include "shellCommandArgs.h"

hashTable<String, shell*> shell::_shells;

shell::shell(const String& user):
	_user(user),_host("yShell"), _isrunning(false), _isexit(false), _sysshell(false)
{
	_shells.insert(user, this);
	_cmdtree = new shellCommandTree("~");
	_cmdtree->registerCmd(new shellCommandTree("shell"));
	_cmdtree->registerCmd(new shellCommandTree("sys"));
	_currentdir = _cmdtree->find("shell");
	_currentdir->registerCmd(new shellCommandTree("bin"));
	_currentdir = _cmdtree->find("bin");
	_currentdir->registerCmd(new shellCommandTree("cd", &shellcmd_cd));
	_currentdir->registerCmd(new shellCommandTree("ls", &shellcmd_ls));
	_currentdir->registerCmd(new shellCommandTree("quit", &shellcmd_quit));
	_currentdir->registerCmd(new shellCommandTree("exit", &shellcmd_exit));
	_currentdir->registerCmd(new shellCommandTree("switch", &shellcmd_switch));
}

shell::~shell()
{

}

void shell::type_prompt()
{
	print('\n' + _user + "@" + _host + ":");
	if (!_sysshell) {
		print(getdir() + "$");
	}
	else{
		print(String("sys") + "$");
	}
}

String shell::getdir()
{
	shellCommandTree* temp = _currentdir;
	Stack<String> dir;
	String path;
	while (temp != NULL)
	{
		if (temp->getType() == Directory) {
			dir.push(temp->getName());
		}
		temp = temp->parent();
	}
	while (dir.empty() == false)
	{
		path.append(dir.top());
		path.append("/");
		dir.pop();
	}
	return path;
}

shellCommandTree *shell::enter(const String& path)
{
	List<String> ph = shellCommandArgs::split(path, '/');
	
	return enter(ph);
}

shellCommandTree *shell::enter(const List<String>& path) 
{
	List<String> ph = path;
	shellCommandTree *res = NULL;

	if (ph.front() == ".." || _currentdir->contains(ph.front())) {
		res = _currentdir;
	}
	else if (_cmdtree->contains(ph.front())) {
		res = _cmdtree;
	}
	while (ph.size() > 0 && res != NULL){
		res = res->enter(ph.front());
		ph.pop_front();
	} 
	if (res != NULL) {
		_currentdir = res;
	}

	return res;
}

bool shell::execSysCmd(shellCommandArgs args)
{
	//print("\nthis is sys cmd!");
	String syscmd = args.toString();
	system(syscmd.c_str());
	return false;
}

void shell::print(const String& str)
{
	std::cout << str;
}

void shell::print(const char& c)
{
	std::cout << c;
}

void shell::shellcmd_cd(shellCommandArgs args)
{
	shell* shell = _shells[args.getSender()];
	if (shell != NULL) {
		shell->enter(shellCommandArgs::split(args.getArgs().front(),'/'));
	}
}

void shell::shellcmd_ls(shellCommandArgs args)
{
	shell* shell = _shells[args.getSender()];
	if (shell != NULL) {
		shellCommandTree *cur = shell->getCurrentDir();
		List<shellCommandTree *> cmds = cur->getClidren();
		int len = cmds.size();
		print('\n');
		for (size_t i = 0; i < len; ++ i){
			print(cmds.front()->getName() + " ");
			cmds.pop_front();
		}
	}
}

void shell::shellcmd_exit(shellCommandArgs args)
{
	shell* shell = _shells[args.getSender()];
	if (shell != NULL) {
		shell->exit();
	}
}

void shell::shellcmd_quit(shellCommandArgs args)
{
	print("\nhere is quit");
}

void shell::shellcmd_switch(shellCommandArgs args)
{
	shell* shell = _shells[args.getSender()];
	if (shell != NULL && args.argCount() == 1) {
		if (!shell->switchMode(args[0])) {
			print("\n please enter switch sys|yshell");
		}
	}
}

void shell::shellCore()
{
	String buffer;
	uint pos = 0;
	print("/********************************************************\n \
*********************Shell Start!**********************\n \
********************************************************");
	type_prompt();
	while (!_isexit) {
		char c = getChar();
		
		if (c == '\r') {
			if (buffer != "") {
				//print(endl);
				execut(buffer);
			}
			buffer.clear();
			pos = 0;
			type_prompt();
		}
		else if(c == '\b' || c == '0x7f'){
			if (pos > 0) {
				if (pos <= buffer.size()) {
					buffer.erase(pos - 1, 1);
					print('\b');
					--pos;
					print(buffer.substr(pos, buffer.size())+" ");
					for (uint i = 0; i < (buffer.size() - pos+1); ++i) {
						print('\b');
					}
				}
				else {
					--pos;
					print('\b');
					print(' ');
					print('\b');
				}
			}
		}
		else if (c == -32) {
			char cc = getChar();
			
			if (cc == 0x4b) {
				if (pos > 0) {
					--pos;
					print('\b');
				}	
			}
			else if (cc == 77) {
				if (pos < buffer.size()) {
					print(buffer.substr(pos, buffer.size()));
					++pos;
					for (uint i = 0; i < buffer.size() - pos; ++i) {
						print('\b');
					}
				}
			}
		}
		else {
			
			if (buffer.size() == pos) {
				buffer += c;
				++pos;
				print(c);
			}	
			else {
				buffer.insert(pos, 1, c);
				String substr = buffer.substr(pos, buffer.size());
				print(buffer.substr(pos, buffer.size()));
				++pos;
				for (uint i = 0; i < buffer.size() - pos; ++i) {
					print('\b');
				}
			}
		}
	}
}

char shell::getChar()
{
	return _getch();
}

void shell::execut(const String& str)
{	
	if (str != "") {
		shellCommandTree* exec = NULL;
		shellCommandArgs args(str, this->_user);
		String cmd = args.getCmd();
		if (!_sysshell || cmd == "switch") {
			
			if (_currentdir->contains(cmd)) {
				exec = _currentdir->find(cmd);
			}
			else {
				shellCommandTree* syscmd = _cmdtree->enter("shell")->enter("bin");
				exec = syscmd->find(cmd);
			}
			if (exec != NULL) {
				exec->doCmd(args);
			}
		}
		else 
		{
			execSysCmd(args);
		}
	}
}

void shell::registerCmd(const String& path, const String& name, FUNC func)
{
	List<String> pth = shellCommandArgs::split(path, '/');
	int len = pth.size();
	shellCommandTree* findpath = _cmdtree;
	for (int i = 0; i < len; ++ i) {
		if (findpath->contains(pth.front())) {
			findpath = findpath->enter(pth.front());
		}
		else {
			findpath = findpath->registerCmd(new shellCommandTree(pth.front()));
		}
		pth.pop_front();
	}
	if (findpath != NULL && findpath->getType() == Directory) {
		shellSlot *cmd = new shellSlot(name, func);
		findpath->registerCmd(new shellCommandTree(cmd->getName(), func));
	}
	else {
		print("register" + name + "falid" + '\n');
	}
}

shellCommandTree * shell::getCurrentDir()
{
	return _currentdir;
}

void shell::exit()
{
	_isexit = true;
}

bool shell::switchMode(const String & mode)
{
	if (mode == "sys") {
		_sysshell = true;
		return true;
	}
	else if (mode == "yshell") {
		_sysshell = false;
		return true;
	}
	return false;
}

