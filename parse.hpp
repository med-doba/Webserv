#include <fstream>
#include <string>
#include <iostream>
#include <vector>


class parse
{
	private:
		// std::vector<std::string>	container;
		std::vector<std::string>	listen;
		std::vector<std::string>	root;
		std::vector<std::string>	index;
		std::vector<std::string>	host;
		std::vector<std::string>	server_name;
		std::vector<std::string>	error_page;
		std::vector<std::string>	client_max_body_size;
		std::vector<std::string>	location;

		//
	public:
		parse();
		parse(const parse &obj);
		parse	&operator=(const parse &obj);
		~parse();
};
