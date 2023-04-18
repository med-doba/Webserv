/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 13:50:43 by med-doba          #+#    #+#             */
/*   Updated: 2023/04/18 00:05:44 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include <algorithm>
#include "server.hpp"
#include <stdlib.h>
#include <map>


typedef	struct bind_info
{
	std::vector<std::string>	ports;
	std::string					host;
}	bind_info;

typedef	std::vector<bind_info>	MapType;

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

std::vector<server>	ft_parse_conf(std::string fileConf)
{
	MapType	bind_info;
	server	classconfig;
	std::vector<std::string>	classconfig_tmp;
	location	location_;
	std::vector<server>	block;
	std::string		lines;
	std::ifstream	file_conf(fileConf);


	bool	InTheServerBlock = false;
	bool	InTheLocationBlock = false;

	if (file_conf.is_open())
	{
		while(std::getline(file_conf, lines))
		{
			// if (lines.empty())
			// 	continue;
			classconfig.ft_trim(lines);
			if (lines.empty())
				continue;
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
					if (classconfig.ft_split(lines, " \t").begin()->compare("location"))
						classconfig.ft_error("error: invalid directives");
					location_.path = *(classconfig.ft_split(lines, " \t").begin() + 1);
				}
				continue;
			}

			if(InTheServerBlock && InTheLocationBlock == false && lines != "}")
			{
				if (lines.back() != ';')
					classconfig.ft_error("error: missing or misplaced commas");
				if (classconfig.ft_occurrences_of_char(lines, ';') == -1)
					classconfig.ft_error("error: occurrences_of_char");
				classconfig_tmp = classconfig.ft_split(lines, " \t;");
				//
				std::cout << "size = " << classconfig_tmp.size() << std::endl;
				//
				// if (classconfig_tmp.size() < 2)
				// {
				// 	classconfig.ft_error("error: invalid directives");
				// }
				if (!classconfig_tmp.begin()->compare("listen"))
					ft_check_listen(classconfig, lines);
				else if (!classconfig_tmp.begin()->compare("host"))
					ft_check_host(classconfig, lines);
				else if (!classconfig_tmp.begin()->compare("server_name"))
					ft_check_server_name(classconfig, lines);
				else if(!classconfig_tmp.begin()->compare("root"))
					ft_check_root(classconfig, lines, location_, 1);
				else if (!classconfig_tmp.begin()->compare("index"))
					ft_check_index(classconfig, lines, location_, 1);
				else if (!classconfig_tmp.begin()->compare("client_max_body_size"))
					ft_check_cmbsize(classconfig, lines, location_, 1);
				else if (!classconfig_tmp.begin()->compare("error_page"))
					ft_check_errorpage(classconfig, lines);
				else
					classconfig.ft_error("error: invalid directives");
			}

			else if (InTheLocationBlock && lines != "}")
			{
				lines.pop_back();
				classconfig_tmp = classconfig.ft_split(lines, " \t");
				if(!classconfig_tmp.begin()->compare("root"))
					ft_check_root(classconfig, lines, location_, 2);
				else if (!classconfig_tmp.begin()->compare("index"))
					ft_check_index(classconfig, lines, location_, 2);
				else if (!classconfig_tmp.begin()->compare("error_page"))
					location_.error_page = classconfig.ft_parse_errorpage(lines);
				else if (!classconfig_tmp.begin()->compare("client_max_body_size"))
					ft_check_cmbsize(classconfig, lines, location_, 2);
				else if (!classconfig_tmp.begin()->compare("return"))
					location_.rtn = classconfig.ft_split(lines, " \t");
				else if (!classconfig_tmp.begin()->compare("allow_methods"))
					ft_check_allow_methods(classconfig, lines, location_);
				else if (!classconfig_tmp.begin()->compare("autoindex"))
					ft_check_autoindex(classconfig, lines, location_);
				else if (!classconfig_tmp.begin()->compare("cgi_pass"))
					ft_check_cgi(classconfig, lines, location_);
				else if (!classconfig_tmp.begin()->compare("client_body_temp_path"))
					location_.client_body_temp_path = classconfig.ft_split(lines, " \t");
				else
					classconfig.ft_error("Error: invalid directives");
			}

			else if (lines == "}")
			{
				if (InTheServerBlock && !InTheLocationBlock)
				{
					InTheServerBlock = false;
					// ft_2bind(classconfig, bind_info);
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
					classconfig.ft_error("error: missing or mismatched brackets");
			}

			else
				classconfig.ft_error("error: missing or mismatched brackets");
		}
		file_conf.close();
		if (!(classconfig.root_find && classconfig.error_page_find && classconfig.location_find && classconfig.listen_find))
			classconfig.ft_error("error: Missing required directives");
		print_vector_of_structs(bind_info);
		// classconfig.ft_show(block);
	}
	return block;
}
