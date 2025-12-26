#include <string>
#include <iostream>

#include "chutil/chutil.hpp"

int main()
{
	std::string output = chutil::run_command_with_stdout("Z:\\home\\champson\\Documents\\coding\\CHutil\\bin\\mingw32-debug\\tests\\main\\test.exe");
	std::cout << output << std::endl;
}
