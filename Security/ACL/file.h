#include <map>  
#include <iostream>

#ifndef RIGHTS_H
#define RIGHTS_H

#include "rights.h"

#endif
class file
{
public:
	std::string name;
	std::string owner;

	file(std::string name, std::string owner): name(name), owner(owner) {
		_acl.insert(std::pair<std::string, AccessRight::AccessRight>(owner,AccessRight::AccessRight("rwx")) );
	}

	void printACL() {
		for(auto it = _acl.begin(); it != _acl.end(); it++)
			std::cout<<it->first<<" "<<(it->second).str()<<'\n';
	}

	void addToACL(std::string domen, AccessRight right) {
		_acl.insert(std::pair<std::string, AccessRight::AccessRight>(domen, right));
	}

	~file(){};
private:
	std::map<std::string, AccessRight::AccessRight> _acl;
};