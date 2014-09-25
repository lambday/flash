#include <iostream>
#include <type.hpp>

struct Base { virtual ~Base() {} };

struct Derived : public Base { };

int main() {
	Base* ptr_base = new Derived();
	std::cout << flash::util::type_name(ptr_base) << std::endl;
	std::cout << flash::util::type_name(*ptr_base) << std::endl;
	delete ptr_base;
}
