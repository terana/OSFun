#include <string>
#include <map>
#include <iostream>

#include "file.h"
#ifndef RIGHTS_H
#define RIGHTS_H
#include "rights.h"
#endif
class Domen
{
public:
	std::string name;

	Domen(std::string name): name(name) {};
	~Domen() {};

	void addAccessRights(File& f, AccessRight ar) {
		_domen.insert(std::pair<std::string, AccessRight>(f.name ,ar));
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