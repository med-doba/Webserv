#ifndef SERVERPARSE_HPP
#define SERVERPARSE_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "locationParse.hpp"


class locationParse;
class serverParse
{
	public:
		//
		serverParse();
		serverParse(const serverParse &obj);
		serverParse	&operator=(const serverParse &obj);
		~serverParse();
		//
		std::vector<locationParse>		obj_location;
		std::vector<std::string>	listen;
		std::string					host;
		std::vector<std::string>	root;
		std::vector<std::string>	server_name;
		std::vector<std::string>	index;
		std::vector<std::string>	error_page;
		size_t						client_max_body_size;
		//methodes
		bool	ft_check_extention(std::string str);
		bool	ft_check_cmbsize(std::string &str);
		bool	ft_check_rangeofports(std::string &str);
		bool	ft_checkRang_nbr(std::string str);
		std::vector<std::string>	ft_parse_root(std::string &lines);
		std::vector<std::string>	ft_parse_index(std::string &lines);
		std::vector<std::string>	ft_parse_errorpage(std::string &lines);
		std::vector<std::string>	ft_parse_cmbsize(std::string &lines);
		void	ft_clearvectorserv(serverParse &classconfig);
		void	ft_clearvectorlocation_test(std::vector<locationParse> &location_);
		//get
		std::string					get_host();
		std::vector<std::string>	get_listen();
		//utils
		int		ft_occurrences_of_char(std::string &line, char c);
		void	ft_ft(std::string str);
		void	ft_trim(std::string &str);
		void	ft_error(std::string msg);
		void	ft_check_double(std::vector<std::string> &container);
		bool	ft_isDigit(std::string &str);
		std::vector<std::string>	ft_split(const std::string str, std::string split);
		//show
		void	ft_show(std::vector<serverParse>  &block);
		//
		bool	listen_;
		bool	server_name_;
		bool	client_max_body_size_;
		bool	index_;
		bool	root_;
		bool	error_page_;
		//
		bool	listen_find;
		bool	root_find;
		bool	error_page_find;
		bool	host_find;
		bool	location_find;
};

void	ft_delete_comment(std::string	&str);
void	ft_setDirective2False(serverParse &classconfig, locationParse &location_, int n);
void	ft_check_cmbsize(serverParse &classconfig, std::string &lines, locationParse &location_, int n);
void	ft_check_index(serverParse &classconfig, std::string &lines, locationParse &location_, int n);
void	ft_check_root(serverParse &classconfig, std::string &lines, locationParse &location_, int n);
void	ft_check_errorpage(serverParse &classconfig, std::string &lines);
void	ft_check_server_name(serverParse &classconfig, std::string &lines);
void	ft_check_host(serverParse &classconfig, std::string &lines);
void	ft_check_listen(serverParse	&classconfig, std::string	&lines);
void	ft_check_allow_methods(serverParse &classconfig, std::string &lines, locationParse &location_);
void	ft_check_autoindex(serverParse &classconfig, std::string &lines, locationParse &location_);
void	ft_check_cgi(serverParse &classconfig, std::string &lines, locationParse &location_);
//
typedef	struct bind_info
{
	std::vector<std::string>	ports;
	std::string					host;
}	bind_info;

typedef	std::vector<bind_info>	MapType;
std::vector<serverParse>		ft_parse_conf(std::string fileConf, MapType & bind_info);

void	print_vector_of_structs(MapType& v);
void	ft_rm_double_ports(std::vector<std::string>	&ports);
void	ft_2bind(serverParse &my_serverParse, MapType	&my_map);

#endif
