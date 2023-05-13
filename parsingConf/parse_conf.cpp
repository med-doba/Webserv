/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmoubal <hmoubal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 13:50:43 by med-doba          #+#    #+#             */
/*   Updated: 2023/05/13 20:19:08 by hmoubal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverParse.hpp"


#include <utility>
void	print_vector(const std::vector<std::pair<int, std::string> >& v)
{
	for (std::vector<std::pair<int, std::string> >::const_iterator it = v.begin(); it != v.end(); ++it)
	{
		std::cout << "(" << it->first << ", " << it->second << ")" << std::endl;
	}
}

std::vector<serverParse>	ft_parse_conf(std::string fileConf, MapType& bind_info)
{
	serverParse						classconfig;
	locationParse					location_;
	std::string					lines;
	std::ifstream				file_conf(fileConf);
	std::vector<serverParse>			block;
	std::vector<std::string>	classconfig_tmp;

	bool	InTheserverBlock = false;
	bool	InTheLocationBlock = false;
	if (file_conf.is_open())
	{
		try
		{
			while(std::getline(file_conf, lines))
			{
				classconfig.ft_trim(lines);
				if (lines.empty() || lines[0] == '#')
					continue;
				ft_delete_comment(lines);
				classconfig.ft_trim(lines);

				if (!lines.empty() && lines.back() == '{')
				{
					if (lines.find("server") != std::string::npos)
						InTheserverBlock = true;
					else if (InTheserverBlock && (lines.find("location") != std::string::npos))
					{
						InTheLocationBlock = true;
						classconfig.location_find = true;
						if (classconfig.ft_split(lines, " \t").begin()->compare("location"))
							classconfig.ft_error("Error: invalid directives");
						location_.path = *(classconfig.ft_split(lines, " \t{").begin() + 1);
					}
					continue;
				}

				else if(InTheserverBlock && !InTheLocationBlock && lines != "}")
				{
					if (lines.back() != ';' || classconfig.ft_occurrences_of_char(lines, ';') >= 2)
						classconfig.ft_error("missing or misplaced commas");
					classconfig_tmp = classconfig.ft_split(lines, " \t;");
					if (classconfig_tmp.size() < 2)
						classconfig.ft_error("Error: empty value for a directives");
					else if (!classconfig_tmp.begin()->compare("listen"))
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

				else if (InTheLocationBlock && InTheserverBlock && lines != "}")
				{
					classconfig_tmp = classconfig.ft_split(lines, " \t;");
					if(!classconfig_tmp.begin()->compare("root"))
						ft_check_root(classconfig, lines, location_, 2);
					else if (!classconfig_tmp.begin()->compare("index"))
						ft_check_index(classconfig, lines, location_, 2);
					else if (!classconfig_tmp.begin()->compare("error_page"))
					{
						checkErrorPageLocation(classconfig, location_, lines);
					}
					else if (!classconfig_tmp.begin()->compare("client_max_body_size"))
						ft_check_cmbsize(classconfig, lines, location_, 2);
					else if (!classconfig_tmp.begin()->compare("return"))
					{
						if (!location_.rtn_)
						{
							location_.rtn_ = true;
							location_.rtn = classconfig.ft_split(lines, " \t;");
							if (location_.rtn.size() != 3)
								classconfig.ft_error("Error: not a valide line for return");
							if (!location_.rtn[2].compare(0, 4, "www."))
								location_.rtn[2].insert(0, "https://");
							else if (location_.rtn[2].compare(0, 1, "/"))
								classconfig.ft_error("Error: not a valide value for return");
						}
						else
							classconfig.ft_error("Error: duplicate directives > return");
					}
					else if (!classconfig_tmp.begin()->compare("allow_methods"))
						ft_check_allow_methods(classconfig, lines, location_);
					else if (!classconfig_tmp.begin()->compare("autoindex"))
						ft_check_autoindex(classconfig, lines, location_);
					else if (!classconfig_tmp.begin()->compare("cgi_pass"))
						ft_check_cgi(classconfig, lines, location_);
					else if (!classconfig_tmp.begin()->compare("client_body_temp_path"))
					{
						location_.client_body_temp_path = classconfig.ft_split(lines, " \t;");
						if (location_.client_body_temp_path.size() != 2)
							classconfig.ft_error("Error: not a valide value for client_body_temp_path");
					}
					else
						classconfig.ft_error("Error: invalid directives");
				}

				else if (lines == "}")
				{
					if (InTheserverBlock && !InTheLocationBlock)
					{
						if (!(classconfig.root_find && classconfig.error_page_find && classconfig.location_find && classconfig.listen_find && classconfig.host_find))
							classconfig.ft_error("Error: Missing required directives");
						InTheserverBlock = false;
						ft_2bind(classconfig, bind_info);
						block.push_back(classconfig);
						classconfig.ft_clearvectorlocation_test(classconfig.obj_location);
						classconfig.ft_clearvectorserv(classconfig);
						ft_setDirective2False(classconfig, location_, 1);
					}
					else if (InTheserverBlock && InTheLocationBlock)
					{
						classconfig.obj_location.push_back(location_);
						location_.ft_clearclasslocation(location_);
						InTheLocationBlock = false;
						ft_setDirective2False(classconfig, location_, 2);
					}
					else
						classconfig.ft_error("Error: missing or mismatched brackets");
				}

				else
					classconfig.ft_error("Error: missing or mismatched brackets");
			}
			file_conf.close();
			// if (!(classconfig.root_find && classconfig.error_page_find && classconfig.location_find && classconfig.listen_find))
			// 	classconfig.ft_error("Error: Missing required directives");
		}
		catch(...)
		{
			std::cerr << "Error" << std::endl;
		}
		// print_vector_of_structs(bind_info);
		// classconfig.ft_show(block);
		// for (size_t i = 0; i <  block.size(); i++)
		// {
		// 	std::cout << "server block " << i << std::endl;
		// 	print_vector(block[i].ErrorPages);
		// 	std::cout << "location block" << std::endl;
		// 	for (size_t j = 0; j < block[i].obj_location.size(); j++)
		// 	{
		// 		print_vector(block[i].obj_location[j].ErrorPages);
		// 	}
		// 	// std::cout << "location block" << std::endl;
		// 	std::cout << "//\\//\\//\\//\\//\\//\\" << std::endl;
		// }
	}
	else
		throw(std::runtime_error("Config File Not Found"));
	return block;
}
