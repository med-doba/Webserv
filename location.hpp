#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "server.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

// class server;

class location : public server
{
	public:
		location();
		// location(const location &obj);
		// location	&operator=(const location &obj);
		std::vector<std::string>	allow;
		std::vector<std::string>	autoindex;
		std::vector<std::string>	rtn;
		std::vector<std::string>	cgi;
		~location();
};

#endif
