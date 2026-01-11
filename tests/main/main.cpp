#include <algorithm>
#include <filesystem>
#include <iterator>
#include <string>
#include <iostream>

#include "chutil/chutil.hpp"
using namespace chutil;


int main()
{
	namespace fs = std::filesystem;
	std::string path = (fs::current_path()/"test.exe").string();
    run_command(path);

	fs::path txt_file_path = fs::current_path()/"test_file.txt";
	std::vector<char> data = read_file(txt_file_path);
	std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(std::cout));
}
