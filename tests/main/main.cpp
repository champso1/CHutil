#include <string>
#include <print>

#include "chutil/chutil.hpp"

int main()
{
	std::string output = chutil::run_command_with_stdout("ls -l");
	std::println("{}", output);
}
