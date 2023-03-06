#ifndef SERVER_HPP
#define SERVER_HPP

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "location.hpp"

class location;
class server
{
	public:
		server();
		// server(const server &obj);
		// server	&operator=(const server &obj);
		~server();
		location					obj_location;
		std::vector<std::string>	listen;
		std::vector<std::string>	host;
		std::vector<std::string>	root;
		std::vector<std::string>	server_name;
		std::vector<std::string>	index;
		std::vector<std::string>	error_page;
		std::vector<std::string>	client_max_body_size;
};

#endif