#include "domen.h"

int main() {
	file f1("Terana's file", "terana");
	file f2("Josef's file", "josef");
	f1.printACL();
	f2.printACL();
	domen d("josef");
	d.addAccessRights(f1, AccessRight("rw-"));
	d.print();
	f1.printACL();
}