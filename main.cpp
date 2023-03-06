/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:29:45 by med-doba          #+#    #+#             */
/*   Updated: 2023/03/06 13:27:15 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "server.hpp"

void	ft_ft(std::string str)
{
	std::cout << str << std::endl;
}

bool	ft_check_extention(std::string str)
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

bool	ft_check_allowed(std::vector<std::string> &allowed)
{
	std::vector<std::string>::iterator	it;
	for (it = allowed.begin() + 1 ; it != allowed.end(); it++)
	{
		if (*it != "POST" && *it != "GET" && *it != "DELETE" && *it != "ALL")
			return false;
	}
	return true;
}

void	ft_trim(std::string &str)
{
	int	i,j;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	str.erase(0, i);
	i = str.size();
	j = 0;
	while (str[i] == ' ' || str[i--] == '\t')
		j++;
	str.erase(i, j);
}

void	ft_error(std::string msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}

void	ft_check_double(std::vector<std::string> &container)
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

int	ft_occurrences_of_char(std::string &line, char c)
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

std::vector<std::string>	ft_split(const std::string str, std::string split)
{
	std::vector<std::string> rtn;
	size_t	i = 0,j = 0;

	for (i = 0; str.find_first_of(split, i) != std::string::npos; i++)
	{
		j = str.find_first_of(split, i);
		rtn.push_back(str.substr(i, (j - i)));
		i = j;
	}
	if (i < str.length())
		rtn.push_back(str.substr(i, (j - i)));
	return rtn;
}

bool	ft_isDigit(std::string &str)
{
	int	i = -1;

	while ((size_t)++i < str.size())
		if (!std::isdigit(str[i]))
			return false;
	return true;
}

bool	ft_check_autoindex(std::string &str)
{
	// ft_ft(str);
	if (str == "on" || str == "off")
		return true;
	return false;
};


void	ft_count(void)
{
	static int autoindex = 0;
	autoindex++;
	if (autoindex == 2)
		ft_error("times count");
}

bool	ft_check_cmbsize(std::string &str)
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

bool	ft_checkRang_nbr(std::string str)
{
	std::vector<std::string>	array;
	std::vector<std::string>::iterator	it;

	array = ft_split(str, ".");
	for (it = array.begin(); it != array.end(); it++)
	{
		if (!ft_isDigit(*it) || !(std::stoll(*it) >= 0 && std::stoll(*it) <= 255) || array.size() != 4)
			return false;
	}
	return true;
}
std::vector<std::string>	ft_parse_root(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t");
	if (tmp.size() != 2)
		ft_error("root: error root");
	return tmp;
}

std::vector<std::string>	ft_parse_index(std::string &lines)
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

std::vector<std::string>	ft_parse_errorpage(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t");
	if (tmp.size() != 3)
		ft_error("error_page: error in error page");
	return tmp;
}

std::vector<std::string>	ft_parse_cmbsize(std::string &lines)
{
	std::vector<std::string>	tmp;

	tmp = ft_split(lines, " \t");
	if (tmp.size() > 2)
		ft_error("ClMaxBodySize: error is didgit");
	if (!ft_check_cmbsize(tmp[1]))
		ft_error("ClMaxBodySize: error cmbs");
	return tmp;
}

void	ft_clearvectorserv(server &classconfig)
{
	classconfig.client_max_body_size.clear();
	classconfig.error_page.clear();
	classconfig.host.clear();
	classconfig.index.clear();
	classconfig.listen.clear();
	classconfig.root.clear();
	classconfig.server_name.clear();
}

void	ft_clearvectorlocation(location &location_)
{
	location_.client_max_body_size.clear();
	location_.error_page.clear();
	location_.root.clear();
	location_.index.clear();
	location_.cgi.clear();
	location_.autoindex.clear();
	location_.rtn.clear();
	location_.allow.clear();
}

void	ft_show(std::vector<server> &block)
{
	puts("\n\nin\n\n");
	for (size_t i = 0; i <  block.size(); i++)
	{
		std::cout << "-----------server block-----------------\n";
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it6;
		for (it6 = block[i].listen.begin(); it6 != block[i].listen.end(); it6++)
			std::cout << *it6 << std::endl;
		std::cout << "----------------------------\n";
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it8;
		for (it8 = block[i].host.begin(); it8 != block[i].host.end(); it8++)
			std::cout << *it8 << std::endl;
		std::cout << "----------------------------\n";
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it4;
		for (it4 = block[i].server_name.begin(); it4 != block[i].server_name.end(); it4++)
			std::cout << *it4 << std::endl;
		std::cout << "----------------------------\n";
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it3;
		for (it3 = block[i].root.begin(); it3 != block[i].root.end(); it3++)
			std::cout << *it3 << std::endl;
		std::cout << "----------------------------\n";
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it1;
		for (it1 = block[i].error_page.begin(); it1 != block[i].error_page.end(); it1++)
			std::cout << *it1 << std::endl;
		std::cout << "----------------------------\n";
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it2;
		for (it2 = block[i].index.begin(); it2 != block[i].index.end(); it2++)
			std::cout << *it2 << std::endl;
		std::cout << "----------------------------\n";
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it5;
		for (it5 = block[i].client_max_body_size.begin(); it5 != block[i].client_max_body_size.end(); it5++)
			std::cout << *it5 << std::endl;
		std::cout << "----------------------------\n";
		std::cout << "----------------------------\n";
		std::cout << "-----------server block-----------------\n";
		std::cout << "----------------------------\n";
		std::cout << "-----------location block-----------------\n";
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it7;
		for (it7 = block[i].obj_location.error_page.begin(); it7 != block[i].obj_location.error_page.end(); it7++)
			std::cout << *it7 << std::endl;
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it9;
		for (it9 = block[i].obj_location.allow.begin(); it9 != block[i].obj_location.allow.end(); it9++)
			std::cout << *it9 << std::endl;
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it10;
		for (it10 = block[i].obj_location.autoindex.begin(); it10 != block[i].obj_location.autoindex.end(); it10++)
			std::cout << *it10 << std::endl;
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it11;
		for (it11 = block[i].obj_location.cgi.begin(); it11 != block[i].obj_location.cgi.end(); it11++)
			std::cout << *it11 << std::endl;
		std::cout << "----------------------------\n";
		std::vector<std::string>::iterator it12;
		for (it12 = block[i].obj_location.rtn.begin(); it12 != block[i].obj_location.rtn.end(); it12++)
			std::cout << *it12 << std::endl;
		std::cout << "----------------------------\n";
		std::cout << "-----------location block-----------------\n";
	}

}

void printVector(const std::vector<std::string>& vec)
{
	for (std::vector<std::string>::const_iterator iter = vec.begin(); iter != vec.end(); ++iter) {
		std::cout << *iter << " ";
	}
	std::cout << std::endl;
}

int	main(void)
{
	server	classconfig;
	location	location_;
	std::vector<server>	block;
	std::vector<location>	in_block;
	std::string		lines;
	std::ifstream	file_conf("filetest.conf");


	bool	InTheServerBlock = false;
	bool	InTheLocationBlock = false;

	if (file_conf.is_open())
	{
		while(std::getline(file_conf, lines))
		{
			if (lines.empty())
				continue;
			ft_trim(lines);
			if (lines[0] == '#')
				continue;
			if (!lines.empty() && lines.back() == '{')
			{
				if (lines.find("server") != std::string::npos)
					InTheServerBlock = true;
				else if (InTheServerBlock && (lines.find("location") != std::string::npos))
					InTheLocationBlock = true;
				continue;
			}

			if(InTheServerBlock && InTheLocationBlock == false)
			{
				if (ft_occurrences_of_char(lines, ';') == -1)
					return (ft_error("error: occurrences_of_char"), 1);
				ft_ft(lines);
				if (lines.back() != '}')
					lines.pop_back();
				if (lines.find("listen") != std::string::npos)
				{
					classconfig.listen = ft_split(lines, " \t");
					classconfig.listen.back().pop_back();
					std::vector<std::string>::iterator it;
					for (it = (classconfig.listen.begin() + 1); it != classconfig.listen.end(); it++)
					{
						if (!ft_isDigit(*it))
							return (ft_error("listen: error is didgit"), 1);
					}
					ft_check_double(classconfig.listen);
				}
				else if (lines.substr(0, 4) == "host")
				{
					classconfig.host = ft_split(lines, " \t");
					std::vector<std::string>::iterator it;
					for (it = classconfig.host.begin(); it != classconfig.host.end(); it++)
					{
						if (!ft_checkRang_nbr(*(it +1)))
							return (ft_error("host: error rang"), 1);
					}
				}
				else if (lines.substr(0, 11) == "server_name")
					classconfig.server_name = ft_split(lines, " \t");
				else if(lines.substr(0, 4) == "root")
					classconfig.root = ft_parse_root(lines);
				else if (lines.substr(0, 5) == "index")
					classconfig.index = ft_parse_index(lines);
				else if (lines.find("client_max_body_size") != std::string::npos)
					classconfig.client_max_body_size = ft_parse_cmbsize(lines);
				else if (lines.find("error_page") != std::string::npos)
					classconfig.error_page =  ft_parse_errorpage(lines);
			}
			if (InTheLocationBlock)
			{
				ft_ft(lines);
				if (lines.back() != '}')
					lines.pop_back();
				if(lines.substr(0, 4) == "root")
				{
					classconfig.obj_location.root = ft_parse_root(lines);

				}
				else if (lines.substr(0, 5) == "index")
					classconfig.obj_location.index = ft_parse_index(lines);
				else if (lines.find("error_page") != std::string::npos)
					classconfig.obj_location.error_page = ft_parse_errorpage(lines);
				else if (lines.find("client_max_body_size") != std::string::npos)
					classconfig.obj_location.client_max_body_size = ft_parse_cmbsize(lines);
				else if (lines.substr(0, 6) == "return")
					classconfig.obj_location.rtn = ft_split(lines, " \t");
				else if (lines.substr(0, 8) == "cgi_pass")
					classconfig.obj_location.cgi = ft_split(lines, " \t");
				else if (lines.substr(0, 5) == "allow")
				{
					classconfig.obj_location.allow = ft_split(lines, " \t");
					if (!ft_check_allowed(classconfig.obj_location.allow))
						return (ft_error("classconfig.obj_location allow: error allow"), 1);
				}
				else if (lines.substr(0, 9) == "autoindex")
				{
					classconfig.obj_location.autoindex = ft_split(lines, " \t");
					if (classconfig.obj_location.autoindex.size() != 2)
						return (ft_error("location_ autoindex: error autoindex1"), 1);
					std::vector<std::string>::iterator	it;
					it = (classconfig.obj_location.autoindex.begin() + 1);
					if (!ft_check_autoindex(*it))
						return (ft_error("location_ autoindex: error autoindex2"), 1);
				}
			}

			if (lines == "}")
			{
				// puts("\n\ntimes\n\n");
				if (InTheServerBlock && InTheLocationBlock == false)
				{
					InTheServerBlock = false;
					block.push_back(classconfig);
					ft_clearvectorserv(classconfig);
				}
				if (InTheServerBlock && InTheLocationBlock)
				{
					puts("\n\n#@@#2#2#2#232\n\n");
					classconfig.obj_location = location_;
					std::vector<std::string>::iterator it10;
					for (it10 = location_.autoindex.begin(); it10 != location_.autoindex.end(); it10++)
						std::cout << "it10 = " << *it10 << std::endl;
					in_block.push_back(classconfig.obj_location);
					InTheLocationBlock = false;
					ft_clearvectorlocation(location_);
				}
			}
		}
		file_conf.close();
		puts("\n^^^^^^^^^^^^^^^^^^^^^^^\n");
		ft_show(block);
		puts("\n^^^^^^^^^^^^^^^^^^^^^^^\n");
	}
	return 0;
}