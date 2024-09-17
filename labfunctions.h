#include <iostream>
#include <string>
#include <regex>
std::string getString();
void manageState(char in);

std::string getString(){
	std::string input;
	std::cout<<"Enter input String: ";
	std::getline(std::cin, input);
	return input;
}

std::string matches_regex(std::regex& check, std::string input){
	return regex_match(input, check)? "true":"false";
}
