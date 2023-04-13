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
		location(const location &obj);
		location	&operator=(const location &obj);
		~location();
		std::string	path;
		std::vector<std::string>	allow_methods;
		std::vector<std::string>	autoindex;
		std::vector<std::string>	rtn;
		std::vector<std::string>	cgi;
		std::vector<std::string>	root;
		std::vector<std::string>	index;
		std::vector<std::string>	error_page;
		std::vector<std::string>	client_max_body_size;
		std::vector<std::string>	cgi_path;
		std::vector<std::string>	cgi_ext;
		std::vector<std::string>	client_body_temp_path;
		//
		bool	ft_check_autoindex(std::string &str);
		void	ft_clearclasslocation(location &location_);
		bool	ft_check_allow_methodsed(std::vector<std::string> &allow_methodsed);
		//
		bool	client_max_body_size_;
		bool	index_;
		bool	root_;
		bool	cgi_;
		bool	autoindex_;
		bool	allow_methods_;
		//
};
// class server;
// std::vector<server>	ft_parse_conf(std::string fileConf);

#endif
