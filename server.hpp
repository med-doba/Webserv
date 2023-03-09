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
		//
		server();
		server(const server &obj);
		server	&operator=(const server &obj);
		~server();
		//
		std::vector<location>		obj_location;
		std::vector<std::string>	listen;
		std::vector<std::string>	host;
		std::vector<std::string>	root;
		std::vector<std::string>	server_name;
		std::vector<std::string>	index;
		std::vector<std::string>	error_page;
		std::vector<std::string>	client_max_body_size;
		//methodes
		bool	ft_check_extention(std::string str);
		bool	ft_check_cmbsize(std::string &str);
		bool	ft_check_rangeofports(std::string &str);
		bool	ft_checkRang_nbr(std::string str);
		std::vector<std::string>	ft_parse_root(std::string &lines);
		std::vector<std::string>	ft_parse_index(std::string &lines);
		std::vector<std::string>	ft_parse_errorpage(std::string &lines);
		std::vector<std::string>	ft_parse_cmbsize(std::string &lines);
		void	ft_clearvectorserv(server &classconfig);
		void	ft_clearvectorlocation_test(std::vector<location> &location_);
		//utils
		void	ft_ft(std::string str);
		void	ft_trim(std::string &str);
		void	ft_error(std::string msg);
		void	ft_check_double(std::vector<std::string> &container);
		int		ft_occurrences_of_char(std::string &line, char c);
		int		ft_occurrences_of_char_v2(std::string &line, char c);
		void	ft_count(void);
		std::vector<std::string>	ft_split(const std::string str, std::string split);
		bool	ft_isDigit(std::string &str);
		//show
		void	ft_show(std::vector<server> &block);
		//
		bool	listen_;
		bool	server_name_;
		bool	client_max_body_size_;
		bool	index_;
		bool	root_;
		//
		bool	listen_find;
		bool	host_find;
		bool	root_find;
		// bool	index_find;
		bool	error_page_find;
		bool	location_find;
		// bool	client_max_body_size_find;
};

#endif