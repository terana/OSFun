#include <string>
#include <map>
#include <iostream>

#include "file.h"
#ifndef RIGHTS_H
#define RIGHTS_H

#include "rights.h"

#endif
class domen
{
public:
	std::string name;

	domen(std::string name): name(name) {};
	~domen() {};

	void addAccessRights(file& f, AccessRight ar) {
		_domen.insert(std::pair<std::string, AccessRight::AccessRight>(f.name ,ar));
		f.addToACL(name, ar);
	}

	void print() {
		std::cout<<name<<": \n";
		for(auto it = _domen.begin(); it != _domen.end(); it++)
			std::cout<<it->first<<" "<<(it->second).str()<<'\n';
	}
private:
	std::map<std::string, AccessRight> _domen;
};