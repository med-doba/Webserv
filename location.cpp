/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:49:40 by med-doba          #+#    #+#             */
/*   Updated: 2023/03/24 15:44:16 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

location::location()
{
	client_max_body_size_ = false;
	index_ = false;
	root_ = false;
	cgi_ = false;
	autoindex_ = false;
	allow_methods_ = false;
}

location::~location()
{
}

location::location(const location &obj)
{
	*this = obj;
}

location	&location::operator=(const location &obj)
{
	if (this != &obj)
	{
		this->allow_methods = obj.allow_methods;
		this->autoindex = obj.autoindex;
		this->rtn = obj.rtn;
		this->cgi = obj.cgi;
		this->root = obj.root;
		this->index = obj.index;
		this->error_page = obj.error_page;
		this->client_max_body_size = obj.client_max_body_size;
		this->path = obj.path;
		this->cgi_path = obj.cgi_path;
		this->cgi_ext = obj.cgi_ext;
		this->client_body_temp_path = obj.client_body_temp_path;
	}
	return *this;
}

//

bool	location::ft_check_allow_methodsed(std::vector<std::string> &allow_methodsed)
{
	std::vector<std::string>::iterator	it;
	for (it = allow_methodsed.begin() + 1 ; it != allow_methodsed.end(); it++)
	{
		if (*it != "POST" && *it != "GET" && *it != "DELETE" && *it != "ALL")
			return false;
	}
	return true;
}

bool	location::ft_check_autoindex(std::string &str)
{
	if (str == "on" || str == "off")
		return true;
	return false;
}

void	location::ft_clearclasslocation(location &location_)
{
	location_.path.clear();
	location_.client_max_body_size.clear();
	location_.error_page.clear();
	location_.root.clear();
	location_.index.clear();
	location_.cgi.clear();
	location_.autoindex.clear();
	location_.rtn.clear();
	location_.allow_methods.clear();
}
