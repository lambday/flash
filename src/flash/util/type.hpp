#ifndef TYPE_H_
#define TYPE_H_

#include <string>
#include <typeinfo>

namespace flash
{

namespace util
{

std::string demangle(const char* name);

template <class T>
std::string type_name(const T& t)
{
	return demangle(typeid(t).name());
}

}
}

#endif // TYPE_H_
