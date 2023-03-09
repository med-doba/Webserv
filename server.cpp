/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:48:14 by med-doba          #+#    #+#             */
/*   Updated: 2023/03/09 21:54:33 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "location.hpp"

server::server()
{
	listen_ = false;
	root_ = false;
	server_name_ = false;
	index_ = false;
	client_max_body_size_ = false;
	listen_find = false;
	host_find = false;
	root_find = false;
	// index_find = false;
	error_page_find = false;
	// client_max_body_size_find = false;
}

server::~server()
{
}

server::server(const server &obj)
{
	*this = obj;
}

server	&server::operator=(const server &obj)
{
	if (this != &obj)
	{
		this->obj_location = obj.obj_location;
		this->listen = obj.listen;
		this->host = obj.host;
		this->root = obj.root;
		this->server_name = obj.server_name;
		this->index = obj.index;
		this->error_page = obj.error_page;
		this->client_max_body_size = obj.client_max_body_size;
	}
	return *this;
}

bool	server::ft_check_extention(std::string str)
{
	std::string	sub;
	int	i = -1;

	std::string	tmp[3] = {".py", ".php", ".html"};
	sub = str.substr(str.find_last_of("."));

	while(++i < 3)
	{
		if (sub == tmp[i])
			return true;
	}
	return false;
}

bool	server::ft_check_cmbsize(std::string &str)
{
	std::string	tmp = str;

	if (!str.empty())
	{
		if (str.back() == 'M')
		{
			str.pop_back();
			if (ft_isDigit(str))
				return true;
			else
				str = tmp;
		}
	}
	return false;
}

bool	server::ft_checkRang_nbr(std::string str)
{
	std::vector<std::string>	array;
	std::vector<std::string>::iterator	it;
	
	if (ft_occurrences_of_char_v2(str, '.') != 3)
		return false;
	array = ft_split(str, ".");
	for (it = array.begin(); it != array.end(); it++)
	{
		if (!ft_isDigit(*it) || !(std::stoll(*it) >= 0 && std::stoll(*it) <= 255))
			return false;
	}
	return true;
}

std::vector<std::string>	server::ft_parse_root(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t");
	if (tmp.size() != 2)
		ft_error("root: error root");
	return tmp;
}

std::vector<std::string>	server::ft_parse_index(std::string &lines)
{
	std::vector<std::string>	tmp;
	std::vector<std::string>::iterator it;

	tmp = ft_split(lines, " \t");
	for (it = tmp.begin() + 1 ; it != tmp.end(); it++)
	{
		if (ft_check_extention(*it) == false)
			ft_error("index: error extention");
	}
	return tmp;
}

std::vector<std::string>	server::ft_parse_errorpage(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t");
	if (tmp.size() != 3)
		ft_error("error_page: error in error page");
	return tmp;
}

std::vector<std::string>	server::ft_parse_cmbsize(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t");
	if (tmp.size() > 2)
		ft_error("ClMaxBodySize: error is didgit");
	if (!ft_check_cmbsize(tmp[1]))
		ft_error("ClMaxBodySize: error cmbs");
	return tmp;
}

void	server::ft_clearvectorserv(server &classconfig)
{
	classconfig.client_max_body_size.clear();
	classconfig.error_page.clear();
	classconfig.host.clear();
	classconfig.index.clear();
	classconfig.listen.clear();
	classconfig.root.clear();
	classconfig.server_name.clear();
}

void	server::ft_clearvectorlocation_test(std::vector<location> &location_)
{
	location_.clear();
}

bool	server::ft_check_rangeofports(std::string &str)
{
	int	i, nbr;

	i = 0;
	nbr = std::stoi(str);
	if (nbr >= 1 && nbr <= 65535)
		return true;
	return false;
}

//--------**-------**----------**-----------**---------//

void	server::ft_ft(std::string str)
{
	std::cout << str << std::endl;
}

void	server::ft_error(std::string msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}

void	server::ft_trim(std::string &str)
{
	int	i,j;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	str.erase(0, i);
	i = str.size();
	j = 0;
	while (str[i] == ' ' || str[i] == '\t')
	{
		j++;
		i--;
	}
	str.erase(i, j);
}

void	server::ft_check_double(std::vector<std::string> &container)
{
	std::vector<std::string>::iterator	it1, it2;
	int	count;

	for(it1 = container.begin(); it1 != container.end(); it1++)
	{
		count = 0;
		for (it2 = container.begin(); it2 != container.end(); it2++)
		{
			if(*it1 == *it2)
				count++;
		}
		if(count > 1)
			ft_error("error double");
	}
}

int	server::ft_occurrences_of_char(std::string &line, char c)
{
	int	count = 0;

	for(int i = 0; line.find(c, i) != std::string::npos; i++)
	{
		count++;
		if (count > 1)
			return -1;
		i = line.find(c, i);
	}
	return count;
}

int	server::ft_occurrences_of_char_v2(std::string &line, char c)
{
	int	count = 0;

	for(int i = 0; line.find(c, i) != std::string::npos; i++)
	{
		count++;
		i = line.find(c, i);
	}
	return count;
}

std::vector<std::string>	server::ft_split(const std::string str, std::string split)
{
	std::vector<std::string> rtn;
	size_t	i = 0,j = 0;

	for (i = 0; str.find_first_of(split, i) != std::string::npos; i++)
	{
		j = str.find_first_of(split, i);
		if (j == std::string::npos)
			break;
		rtn.push_back(str.substr(i, (j - i)));
		i = j;
	}
	if (i < str.length())
		rtn.push_back(str.substr(i));
	return rtn;
}

bool	server::ft_isDigit(std::string &str)
{
	int	i = -1;

	while ((size_t)++i < str.size())
		if (!std::isdigit(str[i]))
			return false;
	return true;
}

void	server::ft_count(void)
{
	static int autoindex = 0;
	autoindex++;
	if (autoindex == 2)
		ft_error("times count");
}

void	server::ft_show(std::vector<server> &block)
{
	for (size_t i = 0; i <  block.size(); i++)
	{
		std::cout << "-----------server block-----------------\n";
		std::vector<std::string>::iterator it6;
		for (it6 = block[i].listen.begin(); it6 != block[i].listen.end(); it6++)
			std::cout << *it6 << std::endl;
		std::vector<std::string>::iterator it8;
		for (it8 = block[i].host.begin(); it8 != block[i].host.end(); it8++)
			std::cout << *it8 << std::endl;
		std::vector<std::string>::iterator it4;
		for (it4 = block[i].server_name.begin(); it4 != block[i].server_name.end(); it4++)
			std::cout << *it4 << std::endl;
		std::vector<std::string>::iterator it3;
		for (it3 = block[i].root.begin(); it3 != block[i].root.end(); it3++)
			std::cout << *it3 << std::endl;
		std::vector<std::string>::iterator it1;
		for (it1 = block[i].error_page.begin(); it1 != block[i].error_page.end(); it1++)
			std::cout << *it1 << std::endl;
		std::vector<std::string>::iterator it2;
		for (it2 = block[i].index.begin(); it2 != block[i].index.end(); it2++)
			std::cout << *it2 << std::endl;
		std::vector<std::string>::iterator it5;
		for (it5 = block[i].client_max_body_size.begin(); it5 != block[i].client_max_body_size.end(); it5++)
			std::cout << *it5 << std::endl;
		for (size_t j = 0; j < block[i].obj_location.size(); j++)
		{
			std::cout << "-----------location block-----------------\n";
			std::vector<std::string>::iterator it7;
			for (it7 = block[i].obj_location[j].error_page.begin(); it7 != block[i].obj_location[j].error_page.end(); it7++)
				std::cout << *it7 << std::endl;
			std::vector<std::string>::iterator it00;
			for (it00 = block[i].obj_location[j].root.begin(); it00 != block[i].obj_location[j].root.end(); it00++)
				std::cout << *it00 << std::endl;
			std::vector<std::string>::iterator it11;
			for (it11 = block[i].obj_location[j].index.begin(); it11 != block[i].obj_location[j].index.end(); it11++)
				std::cout << *it11 << std::endl;
			std::vector<std::string>::iterator it17;
			for (it17 = block[i].obj_location[j].client_max_body_size.begin(); it17 != block[i].obj_location[j].client_max_body_size.end(); it17++)
				std::cout << *it17 << std::endl;
			std::vector<std::string>::iterator it9;
			for (it9 = block[i].obj_location[j].allow_methods.begin(); it9 != block[i].obj_location[j].allow_methods.end(); it9++)
				std::cout << *it9 << std::endl;
			std::vector<std::string>::iterator it10;
			for (it10 = block[i].obj_location[j].autoindex.begin(); it10 != block[i].obj_location[j].autoindex.end(); it10++)
				std::cout << *it10 << std::endl;
			std::vector<std::string>::iterator it111;
			for (it111 = block[i].obj_location[j].cgi.begin(); it111 != block[i].obj_location[j].cgi.end(); it111++)
				std::cout << *it111 << std::endl;
			std::vector<std::string>::iterator it12;
			for (it12 = block[i].obj_location[j].rtn.begin(); it12 != block[i].obj_location[j].rtn.end(); it12++)
				std::cout << *it12 << std::endl;
		}
		std::cout << "\n-----------next server-----------------\n\n";
	}
}