#include <iostream>
#include <memory>
#include <string>
#include <utility>


class std_string
{
public:
	explicit std_string(std::string my_string);
	void print() const;

private:
	std::string my_string_;
};

std_string::std_string(std::string my_string): my_string_(std::move(my_string))
{
}

void std_string::print() const
{
	std::cout << my_string_;
}


class smart_ptr
{
public:
	explicit smart_ptr(const std::string& my_string);

private:
	std::unique_ptr<char[]> my_string_;
};

smart_ptr::smart_ptr(const std::string& my_string)
{
	my_string_ = std::make_unique<char[]>(20);
	my_string.copy(my_string_.get(), 20, 0);
	
	
}

void check_mem()
{
	const std_string pex("hello");
	const smart_ptr rex("neck");
	pex.print();
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	check_mem();
	return 0;
}
