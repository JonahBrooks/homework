/*
 * Original Author: Jonah Brooks (brookjon)
 * File: switch_parser.cpp
 * Created: 2012 April 14 by brookjon
 * Last Modified: 2012 April 17 by brookjon
 *
 * This file contains a basic class for parsing command line inputs and storing
 * the flags found in a map. This makes it much simpler to access and maintain 
 * the values submitted by the user. 
 */	

#include <map>
#include <string>
#include <cstdlib>
#include <fstream>

namespace switch_parser {

class switcher {

	std::map<std::string, std::string> parse_map;

	public:
		switcher(int argc, char *argv[]);
		std::string find(std::string key, std::string if_not_found);
		long find_long(std::string key, long if_not_found);
		void find_new_ofstream(std::string key, 
		                       std::string if_not_found, 
		                       std::ofstream &out);
		void find_ofstream(std::string key, 
		                   std::string if_not_found, 
		                   std::ofstream &out);
		void find_ifstream(std::string key, 
		                   std::string if_not_found, 
		                   std::ifstream &out);
};


switcher::switcher(int argc, char *argv[]) {
	for (int i = argc-1; i > 0; --i) {
		parse_map[(argv[i-1]+1)] = argv[i];
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


long switcher::find_long(std::string key, long if_not_found) {
	long value;
	if (parse_map[key] != "") {
		value = strtol(parse_map[key].c_str(), NULL, 0);
	} else {
		value = if_not_found;
	}

	return value;
}

// Opens file found in map[key], or default, ONLY if file does not exist
void switcher::find_new_ofstream(std::string key, 
                                 std::string if_not_found, 
                                 std::ofstream &out) {
	std::ifstream test(find(key, if_not_found).c_str());
	if (test.is_open()) {
		test.close();
		out.open(if_not_found.c_str());
	} else {
		test.close();
		out.open(find(key, if_not_found).c_str());
	}
}


// Opens file found in map[key], or default, truncating if file exists
void switcher::find_ofstream(std::string key, 
                             std::string if_not_found, 
                             std::ofstream &out) {
	out.open(find(key, if_not_found).c_str());
}


// Opens file found in map[key], or default
void switcher::find_ifstream(std::string key, 
                             std::string if_not_found, 
                             std::ifstream &out) {
	out.open(find(key, if_not_found).c_str());
}



}
