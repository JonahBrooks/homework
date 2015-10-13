#include <iostream>
#include <string>
#include "switch_parser.hpp"

int main(int argc, char *argv[]) {


	switch_parser::switcher flag_switcher(argc, argv);

	std::cout << flag_switcher.find("i", "default") << std::endl;


	return 0;
}
