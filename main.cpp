/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:29:45 by med-doba          #+#    #+#             */
/*   Updated: 2023/03/24 17:19:14 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "server.hpp"
#include <stdlib.h>

void	ft_delete_comment(std::string	&str)
{
	size_t pos = str.find("#");
	if (pos != std::string::npos)
		str.erase(pos, (str.length() - pos));
}

void	ft_check_listen(server	&classconfig, std::string	&lines)
{
	if (!classconfig.listen_)
	{
		classconfig.listen_find = true;
		classconfig.listen = classconfig.ft_split(lines, " \t");
		classconfig.listen.back().pop_back();
		classconfig.listen_ = true;
	}
	else
	{
		std::vector<std::string>	tmp_listen;
		tmp_listen = classconfig.ft_split(lines, " \t");
		tmp_listen.back().pop_back();
		classconfig.listen.insert(classconfig.listen.end(), (tmp_listen.begin() + 1), tmp_listen.end());
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
	classconfig.host_find = true;
	classconfig.host = classconfig.ft_split(lines, " \t");
	classconfig.host.back().pop_back();
	std::vector<std::string>::iterator it;
	for (it = classconfig.host.begin() + 1; it != classconfig.host.end(); it++)
	{
		if (!classconfig.ft_checkRang_nbr(*(it)))
			classconfig.ft_error("host: error rang");
	}
}

void	ft_check_server_name(server &classconfig, std::string &lines)
{
	if (!classconfig.server_name_)
	{
		classconfig.server_name = classconfig.ft_split(lines, " \t");
		classconfig.server_name.back().pop_back();
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
		classconfig.error_page.back().pop_back();
		classconfig.error_page_ = true;
	}
	else
	{
		std::vector<std::string>	tmp_error_page;
		tmp_error_page = classconfig.ft_parse_errorpage(lines);
		tmp_error_page.back().pop_back();
		classconfig.error_page.insert(classconfig.error_page.end(), (tmp_error_page.begin() + 1), tmp_error_page.end());
	}
}

void	ft_check_root(server &classconfig, std::string &lines)
{
	classconfig.root_find = true;
	if (!classconfig.root_)
	{
		classconfig.root = classconfig.ft_parse_root(lines);
		classconfig.root.back().pop_back();
		classconfig.root_ = true;
	}
	else
		classconfig.ft_error("22error: Duplicate directives");
}

void	ft_check_index(server &classconfig, std::string &lines)
{
	if (!classconfig.index_)
	{
		lines.pop_back();
		classconfig.index = classconfig.ft_parse_index(lines);
		classconfig.index_ = true;
	}
	else
		classconfig.ft_error("33error: Duplicate directives");
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

//********************///////////***********************//---------*/

int	main(void)
{
	server	classconfig;
	location	location_;
	std::vector<server>	block;
	std::vector<location>	in_block;
	std::string		lines;
	std::ifstream	file_conf("configuration.conf");


	bool	InTheServerBlock = false;
	bool	InTheLocationBlock = false;

	if (file_conf.is_open())
	{
		while(std::getline(file_conf, lines))
		{
			if (lines.empty())
				continue;
			classconfig.ft_trim(lines);
			if (lines[0] == '#')
				continue;
			ft_delete_comment(lines);
			if (!lines.empty() && lines.back() == '{')
			{
				if (lines.find("server") != std::string::npos)
					InTheServerBlock = true;
				else if (InTheServerBlock && (lines.find("location") != std::string::npos))
				{
					InTheLocationBlock = true;
					classconfig.location_find = true;
					location_.path = *(classconfig.ft_split(lines, " \t").begin() + 1);
				}
				continue;
			}
			if(InTheServerBlock && InTheLocationBlock == false && lines != "}")
			{
				if (lines.back() != ';')
					return (classconfig.ft_error("error: missing or misplaced commas"), 1);
				if (classconfig.ft_occurrences_of_char(lines, ';') == -1)
					return (classconfig.ft_error("error: occurrences_of_char"), 1);
				if (lines.find("listen") != std::string::npos)
					ft_check_listen(classconfig, lines);
				else if (lines.substr(0, 4) == "host")
					ft_check_host(classconfig, lines);
				else if (lines.substr(0, 11) == "server_name")
					ft_check_server_name(classconfig, lines);
				else if(lines.substr(0, 4) == "root")
					ft_check_root(classconfig, lines);
				else if (lines.substr(0, 5) == "index")
					ft_check_index(classconfig, lines);
				else if (lines.find("client_max_body_size") != std::string::npos)
				{
					if (!classconfig.client_max_body_size_)
					{
						lines.pop_back();
						classconfig.client_max_body_size = classconfig.ft_parse_cmbsize(lines);
						classconfig.client_max_body_size_ = true;
					}
					else
						return (classconfig.ft_error("44error: Duplicate directives"), 1);
				}
				else if (lines.find("error_page") != std::string::npos)
					ft_check_errorpage(classconfig, lines);
				else
					return (classconfig.ft_error("error: invalid directives2"), 1);
			}
			else if (InTheLocationBlock && lines != "}")
			{
				lines.pop_back();
				if(lines.substr(0, 4) == "root")
				{
					if (!location_.root_)
					{
						location_.root = classconfig.ft_parse_root(lines);
						location_.root_ = true;
					}
					else
						return (classconfig.ft_error("6error: Duplicate directives"), 1);
				}
				else if (lines.substr(0, 5) == "index")
				{
					if (!location_.index_)
					{
						location_.index = classconfig.ft_parse_index(lines);
						location_.index_ = true;
					}
					else
						return (classconfig.ft_error("5error: Duplicate directives"), 1);
				}
				else if (lines.find("error_page") != std::string::npos)
					location_.error_page = classconfig.ft_parse_errorpage(lines);
				else if (lines.find("client_max_body_size") != std::string::npos)
				{
					if (!location_.client_max_body_size_)
					{
						location_.client_max_body_size = classconfig.ft_parse_cmbsize(lines);
						location_.client_max_body_size_ = true;
					}
					else
						return (classconfig.ft_error("4error: Duplicate directives"), 1);
				}
				else if (lines.substr(0, 6) == "return")
					location_.rtn = classconfig.ft_split(lines, " \t");
				else if (lines.substr(0, 8) == "cgi_pass")
				{
					if (!location_.cgi_)
					{
						location_.cgi = classconfig.ft_split(lines, " \t");
						location_.cgi_ = true;
					}
					else
						return (classconfig.ft_error("3error: Duplicate directives"), 1);
				}
				else if (lines.substr(0, 13) == "allow_methods")
				{
					if (!location_.allow_methods_)
					{
						location_.allow_methods = classconfig.ft_split(lines, " \t");
						if (!location_.ft_check_allow_methodsed(location_.allow_methods))
							return (classconfig.ft_error("location_ allow_methods: error allow_methods"), 1);
						location_.allow_methods_ = true;
					}
					else
						return (classconfig.ft_error("2error: Duplicate directives"), 1);
				}
				else if (lines.substr(0, 9) == "autoindex")
				{
					if (!location_.autoindex_)
					{
						location_.autoindex = classconfig.ft_split(lines, " \t");
						if (location_.autoindex.size() != 2)
							return (classconfig.ft_error("location_ autoindex: error autoindex1"), 1);
						std::vector<std::string>::iterator	it;
						it = (location_.autoindex.begin() + 1);
						if (!location_.ft_check_autoindex(*it))
							return (classconfig.ft_error("location_ autoindex: error autoindex2"), 1);
						location_.autoindex_ = true;
					}
					else
						return (classconfig.ft_error("1error: Duplicate directives"), 1);
				}
				else if (lines.substr(0, 8) == "cgi_path")
					location_.cgi_path = classconfig.ft_split(lines, " \t");
				else if (lines.substr(0, 7) == "cgi_ext")
					location_.cgi_ext = classconfig.ft_split(lines, " \t");
				else if (lines.substr(0, 21) == "client_body_temp_path")
					location_.client_body_temp_path = classconfig.ft_split(lines, " \t");
				else
					return (classconfig.ft_error("error: invalid directives1"), 1);
			}

			else if (lines == "}")
			{
				if (InTheServerBlock && !InTheLocationBlock)
				{
					InTheServerBlock = false;
					block.push_back(classconfig);
					classconfig.ft_clearvectorlocation_test(classconfig.obj_location);
					classconfig.ft_clearvectorserv(classconfig);
					ft_setDirective2False(classconfig, location_, 1);
				}
				else if (InTheServerBlock && InTheLocationBlock)
				{
					classconfig.obj_location.push_back(location_);
					location_.ft_clearclasslocation(location_);
					InTheLocationBlock = false;
					ft_setDirective2False(classconfig, location_, 2);
				}
				else
					return (classconfig.ft_error("error: missing or mismatched brackets"), 1);
			}
			else
				return (classconfig.ft_error("error: missing or mismatched brackets"), 1);
		}
		file_conf.close();
		if (!(classconfig.root_find && classconfig.error_page_find && classconfig.location_find && classconfig.listen_find))
			return (classconfig.ft_error("error: Missing required directives"), 1);
		classconfig.ft_show(block);
	}
	return 0;
}
