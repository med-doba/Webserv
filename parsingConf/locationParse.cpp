/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationParse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:49:40 by med-doba          #+#    #+#             */
/*   Updated: 2023/05/03 20:01:35 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locationParse.hpp"

locationParse::locationParse()
{
	client_max_body_size_ = false;
	index_ = false;
	root_ = false;
	cgi_ = false;
	autoindex_ = false;
	allow_methods_ = false;
}

locationParse::~locationParse()
{
}

locationParse::locationParse(const locationParse &obj)
{
	*this = obj;
}

locationParse	&locationParse::operator=(const locationParse &obj)
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
		this->client_max_body_size_ = obj.client_max_body_size_;
		this->path = obj.path;
		this->cgi_path = obj.cgi_path;
		this->cgi_ext = obj.cgi_ext;
		//
		this->index_ = obj.index_;
		this->root_ = obj.root_;
		this->cgi_ = obj.cgi_;
		this->autoindex_ = obj.autoindex_;
		this->allow_methods_ = obj.allow_methods_;
		this->client_max_body_size_ = obj.client_max_body_size_;
	}
	return *this;
}

//

bool	locationParse::ft_check_allow_methodsed(std::vector<std::string> &allow_methodsed)
{
	std::vector<std::string>::iterator	it;

	for (it = allow_methodsed.begin() + 1 ; it != allow_methodsed.end(); it++)
	{
		if (*it != "POST" && *it != "GET" && *it != "DELETE" && *it != "ALL")
			return false;
	}
	return true;
}

bool	locationParse::ft_check_autoindex(std::string &str)
{
	if (str == "on" || str == "off")
		return true;
	return false;
}

void	locationParse::ft_clearclasslocation(locationParse &location_)
{
	location_.path.clear();
	location_.error_page.clear();
	location_.root.clear();
	location_.index.clear();
	location_.cgi.clear();
	location_.autoindex.clear();
	location_.rtn.clear();
	location_.allow_methods.clear();
}
