/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:48:14 by med-doba          #+#    #+#             */
/*   Updated: 2023/04/23 19:49:53 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverParse.hpp"

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

	if (ft_occurrences_of_char(str, '.') != 3)
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
		ft_error("Error: not a valide value for error_page");
	return tmp;
}

std::vector<std::string>	server::ft_parse_cmbsize(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t;");
	if (tmp.size() > 2)
		ft_error("Erro: only one value accepted in ClMaxBodySize");
	if (!ft_check_cmbsize(tmp[1]))
		ft_error("Error: not a valid value for ClMaxBodySize");
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
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
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
	size_t	i = -1;

	while (++i < str.size())
		if (!std::isdigit(str[i]))
			return false;
	return true;
}

void	server::ft_show(std::vector<server> &block)
{
	for (size_t i = 0; i <  block.size(); i++)
	{
		std::cout << "-----------server block-----------------\n";
		std::vector<std::string>::iterator it6;
		for (it6 = block[i].listen.begin(); it6 != block[i].listen.end(); it6++)
			std::cout << *it6 << " ";
		std::cout << "\n";
		std::cout << block[i].get_host() << std::endl;
		std::vector<std::string>::iterator it4;
		for (it4 = block[i].server_name.begin(); it4 != block[i].server_name.end(); it4++)
			std::cout << *it4 << " ";
		std::cout << "\n";
		std::vector<std::string>::iterator it3;
		for (it3 = block[i].root.begin(); it3 != block[i].root.end(); it3++)
			std::cout << *it3 << " ";
		std::cout << "\n";
		std::vector<std::string>::iterator it1;
		for (it1 = block[i].error_page.begin(); it1 != block[i].error_page.end(); it1++)
			std::cout << *it1 << " ";
		std::cout << "\n";
		std::vector<std::string>::iterator it2;
		for (it2 = block[i].index.begin(); it2 != block[i].index.end(); it2++)
			std::cout << *it2 << " ";
		std::cout << "\n";
		std::vector<std::string>::iterator it5;
		for (it5 = block[i].client_max_body_size.begin(); it5 != block[i].client_max_body_size.end(); it5++)
			std::cout << *it5 << " ";
		std::cout << "\n";
		for (size_t j = 0; j < block[i].obj_location.size(); j++)
		{
			std::cout << "-----------location block-----------------\n";
			std::cout << "-----------path-----------------\n";
			std::cout << "path = " << block[i].obj_location[j].path << " >>> ";
			std::vector<std::string>::iterator it7;
			for (it7 = block[i].obj_location[j].error_page.begin(); it7 != block[i].obj_location[j].error_page.end(); it7++)
				std::cout << *it7 << " ";
			std::vector<std::string>::iterator it00;
			for (it00 = block[i].obj_location[j].root.begin(); it00 != block[i].obj_location[j].root.end(); it00++)
				std::cout << *it00 << " ";
			std::vector<std::string>::iterator it11;
			for (it11 = block[i].obj_location[j].index.begin(); it11 != block[i].obj_location[j].index.end(); it11++)
				std::cout << *it11 << " ";
			std::vector<std::string>::iterator it17;
			for (it17 = block[i].obj_location[j].client_max_body_size.begin(); it17 != block[i].obj_location[j].client_max_body_size.end(); it17++)
				std::cout << *it17 << " ";
			std::vector<std::string>::iterator it9;
			for (it9 = block[i].obj_location[j].allow_methods.begin(); it9 != block[i].obj_location[j].allow_methods.end(); it9++)
				std::cout << *it9 << " ";
			std::vector<std::string>::iterator it10;
			for (it10 = block[i].obj_location[j].autoindex.begin(); it10 != block[i].obj_location[j].autoindex.end(); it10++)
				std::cout << *it10 << " ";
			std::vector<std::string>::iterator it111;
			for (it111 = block[i].obj_location[j].cgi.begin(); it111 != block[i].obj_location[j].cgi.end(); it111++)
				std::cout << *it111 << " ";
			std::vector<std::string>::iterator it12;
			for (it12 = block[i].obj_location[j].rtn.begin(); it12 != block[i].obj_location[j].rtn.end(); it12++)
				std::cout << *it12 << " ";
			std::cout << "\n";
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
	std::vector<std::string>::iterator	it;

	if (!location_.autoindex_)
	{
		location_.autoindex = classconfig.ft_split(lines, " \t;");
		if (location_.autoindex.size() != 2)
			classconfig.ft_error("Error: only one value accepte for autoindex");
		it = (location_.autoindex.begin() + 1);
		if (!location_.ft_check_autoindex(*it))
			classconfig.ft_error("Error: not a valide value for autoindex");
		location_.autoindex_ = true;
	}
	else
		classconfig.ft_error("Error: Duplicate directives > autoindex");
}

void	ft_check_allow_methods(server &classconfig, std::string &lines, location &location_)
{
	if (!location_.allow_methods_)
	{
		location_.allow_methods = classconfig.ft_split(lines, " \t;");
		if (!location_.ft_check_allow_methodsed(location_.allow_methods))
			classconfig.ft_error("Error: not a valide value for allow_methods");
		location_.allow_methods_ = true;
	}
	else
		classconfig.ft_error("Error: duplicate directives > allow_methods");
}

void	ft_check_listen(server	&classconfig, std::string	&lines)
{
	std::vector<std::string>			tmp_listen;
	std::vector<std::string>::iterator	it;

	if (!classconfig.listen_)
	{
		classconfig.listen = classconfig.ft_split(lines, " \t;");
		classconfig.listen_find = true;
		classconfig.listen_ = true;
	}
	else
	{
		tmp_listen = classconfig.ft_split(lines, " \t;");
		classconfig.listen.insert(classconfig.listen.end(), (tmp_listen.begin() + 1), tmp_listen.end());
	}
	for (it = (classconfig.listen.begin() + 1); it != classconfig.listen.end(); it++)
	{
		if (!classconfig.ft_isDigit(*it) || !classconfig.ft_check_rangeofports(*it))
			classconfig.ft_error("Error: in listen value");
	}
	classconfig.ft_check_double(classconfig.listen);
}

void	ft_check_host(server &classconfig, std::string &lines)
{
	std::vector<std::string>			tmp_host;
	std::vector<std::string>::iterator	it;

	if (!classconfig.host_find)
	{
		tmp_host = classconfig.ft_split(lines, " \t;");
		if (tmp_host.size() != 2)
			classconfig.ft_error("Error: Only one host accepted");
		for (it = tmp_host.begin() + 1; it != tmp_host.end(); it++)
		{
			if (!classconfig.ft_checkRang_nbr(*(it)))
				classconfig.ft_error("Error: not a valide value for host");
		}
		classconfig.host = *(tmp_host.begin() + 1);
		classconfig.host_find = true;
	}
	else
		classconfig.ft_error("Error: Duplicate directives > host");
}

void	ft_check_server_name(server &classconfig, std::string &lines)
{
	if (!classconfig.server_name_)
	{
		classconfig.server_name = classconfig.ft_split(lines, " \t;");
		classconfig.server_name_ = true;
	}
	else
		classconfig.ft_error("Error: Duplicate directives > server_name");
}

void	ft_check_errorpage(server &classconfig, std::string &lines)
{
	std::vector<std::string>	tmp_error_page;

	if (!classconfig.error_page_)
	{
		classconfig.error_page =  classconfig.ft_parse_errorpage(lines);
		classconfig.error_page_find = true;
		classconfig.error_page_ = true;
	}
	else
	{
		tmp_error_page = classconfig.ft_parse_errorpage(lines);
		classconfig.error_page.insert(classconfig.error_page.end(), (tmp_error_page.begin() + 1), tmp_error_page.end());
	}
}

void	ft_check_root(server &classconfig, std::string &lines, location &location_, int n)
{
	if (n == 1 && !classconfig.root_)
	{
		classconfig.root = classconfig.ft_parse_root(lines);
		classconfig.root_find = true;
		classconfig.root_ = true;
	}
	else if (n == 2 && !location_.root_)
	{
		location_.root = classconfig.ft_parse_root(lines);
		location_.root_ = true;
	}
	else
		classconfig.ft_error("Error: Duplicate directives > root");
}

void	ft_check_index(server &classconfig, std::string &lines, location &location_, int n)
{
	if (n == 1 && !classconfig.index_)
	{
		classconfig.index = classconfig.ft_parse_index(lines);
		classconfig.index_ = true;
	}
	else if (n == 2 && !location_.index_)
	{
		location_.index = classconfig.ft_parse_index(lines);
		location_.index_ = true;
	}
	else
		classconfig.ft_error("Error: Duplicate directives > index");
}

void	ft_check_cmbsize(server &classconfig, std::string &lines, location &location_, int n)
{
	if (n == 1 && !classconfig.client_max_body_size_)
	{
		classconfig.client_max_body_size = classconfig.ft_parse_cmbsize(lines);
		classconfig.client_max_body_size_ = true;
	}
	else if (n == 2 && !location_.client_max_body_size_)
	{
		location_.client_max_body_size = classconfig.ft_parse_cmbsize(lines);
		location_.client_max_body_size_ = true;
	}
	else
		classconfig.ft_error("Error: Duplicate directives > cmbsize");
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
		classconfig.ft_error("Error: Duplicate directives > cgi_pass");
}

//parse config

void print_vector_of_structs(MapType& v)
{
	for (MapType::iterator it = v.begin(); it != v.end(); ++it)
	{
		bind_info& s = *it;
		std::cout << "host: " << s.host << std::endl;
		std::cout << "ports: ";
		for (std::vector<std::string>::iterator str_it = s.ports.begin(); str_it != s.ports.end(); ++str_it)
		{
			const std::string& str = *str_it;
			std::cout << str << " | ";
		}
		std::cout << std::endl;
	}
}

void	ft_rm_double_ports(std::vector<std::string>	&ports)
{
	int	count;
	std::vector<std::string>			tmp;
	std::vector<std::string>::iterator	it1;
	std::vector<std::string>::iterator	it2;

	for (it1 = ports.begin(); it1 != ports.end(); it1++)
	{
		count = 0;
		for (it2 = ports.begin(); it2 != ports.end(); it2++)
		{
			if (it1->compare(*it2) == 0)
			{
				count++;
				if (count > 1)
					it2->erase();
			}
		}
	}
	it1 = ports.begin();
	for (size_t i = 0; i < ports.size(); i++)
	{
		if (!it1->empty())
			tmp.push_back(*it1);
		it1++;
	}
	ports.clear();
	ports = tmp;
}

bool	ft_handle_same_host(MapType &info, std::string	to_find, std::vector<std::string> &tr)
{
	MapType::iterator	it;

	for (it = info.begin(); it != info.end(); it++)
	{
		if (it->host.compare(to_find) == 0)
		{
			it->ports.insert(it->ports.end(), tr.begin() , tr.end());
			ft_rm_double_ports(it->ports);
			return true;
		}
	}
	return false;
}

void	ft_2bind(server &my_server, MapType	&my_map)
{
	std::vector<std::string>	tmp;
	bind_info	collect;

	tmp = my_server.get_listen();
	tmp.erase(tmp.begin());
	collect.ports = tmp;

	if (ft_handle_same_host(my_map, my_server.get_host(), collect.ports))
		return ;
	else
	{
		collect.host = my_server.get_host();
		my_map.push_back(collect);
	}
}
