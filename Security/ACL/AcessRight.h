#include <string>

class AccessRight
{
public:
	AccessRight(std::string right);
	AccessRight(int right) : _right(right) {};
	std::string str();
	int oct() { return _right; };
	void change(std::string right);
	~AccessRight(){};
private:
	int _right;
};

