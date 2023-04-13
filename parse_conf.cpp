/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 13:50:43 by med-doba          #+#    #+#             */
/*   Updated: 2023/04/13 17:33:43 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "server.hpp"
#include <stdlib.h>


std::vector<server>	ft_parse_conf(std::string fileConf)
{
	server	classconfig;
	// server	classconfig_tmp;
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
				classconfig_tmp = classconfig.ft_split(lines, " \t");
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
					classconfig.ft_error("error: invalid directives2");
			}

			else if (InTheLocationBlock && lines != "}")
			{
				lines.pop_back();
				if(lines.substr(0, 4) == "root")
					ft_check_root(classconfig, lines, location_, 2);
				else if (lines.substr(0, 5) == "index")
					ft_check_index(classconfig, lines, location_, 2);
				else if (lines.find("error_page") != std::string::npos)
					location_.error_page = classconfig.ft_parse_errorpage(lines);
				else if (lines.find("client_max_body_size") != std::string::npos)
					ft_check_cmbsize(classconfig, lines, location_, 2);
				else if (lines.substr(0, 6) == "return")
					location_.rtn = classconfig.ft_split(lines, " \t");
				else if (lines.substr(0, 13) == "allow_methods")
					ft_check_allow_methods(classconfig, lines, location_);
				else if (lines.substr(0, 9) == "autoindex")
					ft_check_autoindex(classconfig, lines, location_);
				else if (lines.substr(0, 8) == "cgi_pass")
					ft_check_cgi(classconfig, lines, location_);
				else if (lines.substr(0, 21) == "client_body_temp_path")
					location_.client_body_temp_path = classconfig.ft_split(lines, " \t");
				else
					classconfig.ft_error("error: invalid directives1");
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
					classconfig.ft_error("error: missing or mismatched brackets");
			}

			else
				classconfig.ft_error("error: missing or mismatched brackets");
		}
		file_conf.close();
		if (!(classconfig.root_find && classconfig.error_page_find && classconfig.location_find && classconfig.listen_find))
			classconfig.ft_error("error: Missing required directives");
		classconfig.ft_show(block);
	}
	return block;
}
