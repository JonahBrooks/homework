/*
 * switch_parser.hpp
 *
 */

#include <map>
#include <string>

namespace switch_parser {

class switcher {

	std::map<std::string, std::string> parse_map;

	public:
		switcher(int argc, char *argv[]);
		std::string find(std::string key, std::string if_not_found);
};


switcher::switcher(int argc, char *argv[]) {

	//std::map<std::string,std::string> parse_map;

	for (int i = argc-1; i > 0; --i) {
		parse_map[(argv[i-1]+1)] = argv[i];
		//parse_map[argv[i-1]] = argv[i];

	}

}

std::string switcher::find(std::string key, std::string if_not_found) {
	std::string value;
	if (parse_map[key] != "") {
		value = parse_map[key];
	} else {
		value = if_not_found;
	}

	return value;
}

}
