/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:48:14 by med-doba          #+#    #+#             */
/*   Updated: 2023/04/18 11:01:10 by med-doba         ###   ########.fr       */
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
	error_page_ = false;
	listen_find = false;
	host_find = false;
	root_find = false;
	error_page_find = false;
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

std::vector<std::string>	server::get_listen()
{
	return this->listen;
}

std::string	server::get_host()
{
	return this->host;
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
	if (array.size() != 4)
		return false;
	for (it = array.begin(); it != array.end(); it++)
	{
		if (it->size() <= 0)
			return false;
		if (!ft_isDigit(*it) || !(std::stoll(*it) >= 0 && std::stoll(*it) <= 255))
			return false;
	}
	return true;
}

std::vector<std::string>	server::ft_parse_root(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t;");

	if (tmp.size() != 2)
		ft_error("root: error root");
	return tmp;
}

std::vector<std::string>	server::ft_parse_index(std::string &lines)
{
	std::vector<std::string>	tmp;
	std::vector<std::string>::iterator it;

	tmp = ft_split(lines, " \t;");
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

	tmp = ft_split(lines, " \t;");
	if (tmp.size() != 3)
		ft_error("Error: in error page");
	return tmp;
}

std::vector<std::string>	server::ft_parse_cmbsize(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t;");
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
	int	nbr;

	if (!str.empty())
	{
		nbr = std::stoi(str);
		if (nbr >= 1 && nbr <= 65535)
			return true;
	}
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

std::vector<std::string>	server::ft_split(std::string str, std::string split)
{
	std::istringstream iss(str);
	std::vector<std::string> tokens1;
	std::vector<std::string> tokens2;
	std::string token;

	for (size_t i = 0; i < split.size(); i++)
	{
		if (i == 0)
		{
			while (std::getline(iss, token, split[i]))
			{
				if (!token.empty())
					tokens2.push_back(token);
			}
		}
		else
		{
			for (size_t index = 0; index < tokens2.size(); index++)
			{
				std::istringstream iss(tokens2[index]);
				while (std::getline(iss, token, split[i]))
				{
					if (!token.empty())
						tokens1.push_back(token);
				}
			}
			tokens2 = tokens1;
			tokens1.clear();
		}
	}
	return tokens2;
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
		std::cout << block[i].get_host() << std::endl;
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
			std::cout << "-----------path-----------------\n";
			std::cout << "path = " << block[i].obj_location[j].path << "\n";
			std::cout << "-----------path-----------------\n";
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void	ft_delete_comment(std::string	&str)
{
	size_t pos = str.find("#");
	if (pos != std::string::npos)
		str.erase(pos, (str.length() - pos));
}

void	ft_check_autoindex(server &classconfig, std::string &lines, location &location_)
{
	if (!location_.autoindex_)
	{
		location_.autoindex = classconfig.ft_split(lines, " \t;");
		if (location_.autoindex.size() != 2)
			classconfig.ft_error("Error: error autoindex1");
		std::vector<std::string>::iterator	it;
		it = (location_.autoindex.begin() + 1);
		if (!location_.ft_check_autoindex(*it))
			classconfig.ft_error("Error: error autoindex2");
		location_.autoindex_ = true;
	}
	else
		classconfig.ft_error("Error: Duplicate directives");
}

void	ft_check_allow_methods(server &classconfig, std::string &lines, location &location_)
{
	if (!location_.allow_methods_)
	{
		location_.allow_methods = classconfig.ft_split(lines, " \t;");
		if (!location_.ft_check_allow_methodsed(location_.allow_methods))
			classconfig.ft_error("Error: in location allow_methods");
		location_.allow_methods_ = true;
	}
	else
		classconfig.ft_error("Error: in location duplicate directives");
}

void	ft_check_listen(server	&classconfig, std::string	&lines)
{
	if (!classconfig.listen_)
	{
		classconfig.listen_find = true;
		classconfig.listen = classconfig.ft_split(lines, " \t;");
		classconfig.listen_ = true;
	}
	else
	{
		std::vector<std::string>	tmp_listen;
		tmp_listen = classconfig.ft_split(lines, " \t;");
	}
	std::vector<std::string>::iterator	it;
	for (it = (classconfig.listen.begin() + 1); it != classconfig.listen.end(); it++)
	{
		if (!classconfig.ft_isDigit(*it) || !classconfig.ft_check_rangeofports(*it))
			classconfig.ft_error("listen: error is didgit or range");
	}
	classconfig.ft_check_double(classconfig.listen);
}

void	ft_check_host(server &classconfig, std::string &lines)
{
	if (!classconfig.host_find)
	{
		classconfig.host_find = true;
		std::vector<std::string>	tmp_host;
		tmp_host = classconfig.ft_split(lines, " \t;");
		if (tmp_host.size() != 2)
			classconfig.ft_error("Error: Only one host accepted");
		std::vector<std::string>::iterator it;
		for (it = tmp_host.begin() + 1; it != tmp_host.end(); it++)
		{
			if (!classconfig.ft_checkRang_nbr(*(it)))
				classconfig.ft_error("host: error rang");
		}
		classconfig.host = *(tmp_host.begin() + 1);
	}
	else
		classconfig.ft_error("9Error: Duplicate directives");
}

void	ft_check_server_name(server &classconfig, std::string &lines)
{
	if (!classconfig.server_name_)
	{
		classconfig.server_name = classconfig.ft_split(lines, " \t;");
		classconfig.server_name_ = true;
	}
	else
		classconfig.ft_error("11error: Duplicate directives");
}

void	ft_check_errorpage(server &classconfig, std::string &lines)
{
	if (!classconfig.error_page_)
	{
		classconfig.error_page_find = true;
		classconfig.error_page =  classconfig.ft_parse_errorpage(lines);
		classconfig.error_page_ = true;
	}
	else
	{
		std::vector<std::string>	tmp_error_page;
		tmp_error_page = classconfig.ft_parse_errorpage(lines);
		classconfig.error_page.insert(classconfig.error_page.end(), (tmp_error_page.begin() + 1), tmp_error_page.end());
	}
}

void	ft_check_root(server &classconfig, std::string &lines, location &location_, int n)
{
	if (n == 1)
	{
		classconfig.root_find = true;
		if (!classconfig.root_)
		{
			classconfig.root = classconfig.ft_parse_root(lines);
			classconfig.root_ = true;
		}
	}
	else if (n == 2)
	{
		if (!location_.root_)
		{
			location_.root = classconfig.ft_parse_root(lines);
			location_.root_ = true;
		}
	}
	else
		classconfig.ft_error("root error: Duplicate directives");
}

void	ft_check_index(server &classconfig, std::string &lines, location &location_, int n)
{
	if (n == 1)
	{
		if (!classconfig.index_)
		{
			classconfig.index = classconfig.ft_parse_index(lines);
			classconfig.index_ = true;
		}
	}
	else if (n == 2)
	{
		if (!location_.index_)
		{
			location_.index = classconfig.ft_parse_index(lines);
			location_.index_ = true;
		}
	}
	else
		classconfig.ft_error("index error: Duplicate directives");
}

void	ft_check_cmbsize(server &classconfig, std::string &lines, location &location_, int n)
{
	if (n == 1)
	{
		if (!classconfig.client_max_body_size_)
		{
			classconfig.client_max_body_size = classconfig.ft_parse_cmbsize(lines);
			classconfig.client_max_body_size_ = true;
		}
	}
	else if (n == 2)
	{
		if (!location_.client_max_body_size_)
		{
			location_.client_max_body_size = classconfig.ft_parse_cmbsize(lines);
			location_.client_max_body_size_ = true;
		}
	}
	else
		classconfig.ft_error("7Error: Duplicate directives");
}

void	ft_setDirective2False(server &classconfig, location &location_, int n)
{
	if (n == 1)
	{
		classconfig.error_page_ = false;
		classconfig.listen_ = false;
		classconfig.client_max_body_size_ = false;
		classconfig.root_ = false;
		classconfig.index_ = false;
		classconfig.server_name_ = false;
		classconfig.host_find = false;
	}
	else if (n == 2)
	{
		location_.client_max_body_size_ = false;
		location_.index_ = false;
		location_.root_ = false;
		location_.cgi_ = false;
		location_.autoindex_ = false;
		location_.allow_methods_ = false;
	}
}

void	ft_check_cgi(server &classconfig, std::string &lines, location &location_)
{
	if (!location_.cgi_)
	{
		location_.cgi = classconfig.ft_split(lines, " \t;");
		location_.cgi_ = true;
	}
	else
		classconfig.ft_error("Error: Duplicate directives");
}
