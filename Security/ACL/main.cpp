#include "domen.h"

int main() {
	File f1("Terana's File", "terana");
	File f2("Josef's File", "josef");
	f1.printACL();
	f2.printACL();
	Domen d("josef");
	d.addAccessRights(f1, AccessRight("rw-"));
	d.print();
	f1.printACL();
}