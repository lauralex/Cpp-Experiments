#include <iostream>
#include <memory>
#include <string>
#include <utility>


/**
 * \brief This is a class using a std::string to allocate memory
 */
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


/**
 * \brief This is a class using a unique pointer to allocate memory
 */
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
	const std_string pex{"hello"}; // Use a std::string to allocate memory
	const smart_ptr rex{"neck"}; // Use a unique pointer to allocate memory (both will manage memory deallocation!)
	pex.print();
}

int main()
{
	// Check if there are any memory leaks! (if we use smart pointers, there won't be any!)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	check_mem();
	return 0;
}
