#include <filesystem>
#include <string>
#include <print>

#include "chutil/chutil.hpp"

static void usage()
{
	std::println("==================================================");
	std::println("USAGE: ./main <path-to-test-executable>");
	std::println("\t<path-to-test-executable>: An absolute path to an executable of some kind.");
	std::println("\t\tThe default is the test executable build by this project.");
	std::println("==================================================");
}

int main(int argc, char* argv[])
{
	std::string path;
	if (argc > 2)
		usage();
	if (argc == 1)
		path = std::filesystem::current_path()/"test";
	else
		path = argv[1];
	
	std::string output = chutil::run_command_with_stdout(path);
	std::println("{}", output);
}
