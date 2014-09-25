#include <type.hpp>

#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

using namespace flash;

namespace util
{
/*
std::string demangle(const char* name)
{
	int status = -4;
	const char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
	return (status == 0) ? demangled : name;
}

#else
*/
std::string demangle(const char* name)
{
	return name;
}

}

#endif
