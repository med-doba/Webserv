#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

class location
{
	public:
		location();
		// location(const location &obj);
		// location	&operator=(const location &obj);
		std::vector<std::string>	allow;
		std::vector<std::string>	autoindex;
		std::vector<std::string>	rtn;
		std::vector<std::string>	cgi;
		std::vector<std::string>	root;
		std::vector<std::string>	server_name;
		std::vector<std::string>	index;
		std::vector<std::string>	error_page;
		std::vector<std::string>	client_max_body_size;
		~location();
};

#endif
