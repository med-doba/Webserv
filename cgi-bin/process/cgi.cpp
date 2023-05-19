/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:54:52 by med-doba          #+#    #+#             */
/*   Updated: 2023/05/19 02:00:19 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

cgi::cgi()
{
}

cgi::cgi(const cgi &obj)
{
	*this = obj;
}

cgi	&cgi::operator=(const cgi &obj)
{
	if (this != &obj)
	{
		this->CONTENT_TYPE = obj.CONTENT_TYPE;
		this->CONTENT_LENGTH = obj.CONTENT_LENGTH;
		this->PATH_INFO = obj.PATH_INFO;
		this->QUERY_STRING = obj.QUERY_STRING;
		this->REQUEST_METHOD = obj.REQUEST_METHOD;
		this->SCRIPT_NAME = obj.SCRIPT_NAME;
		this->SCRIPT_FILENAME = obj.SCRIPT_FILENAME;
		this->REQUEST_URI = obj.REQUEST_URI;
		this->POST_DATA = obj.POST_DATA;
		this->HTTP_USER_AGENT = obj.HTTP_USER_AGENT;
		this->HTTP_COOKIE = obj.HTTP_COOKIE;
		this->body = obj.body;
		this->executable = obj.executable;
		this->envp = obj.envp;
	}
	return *this;
}

cgi::~cgi()
{
}

void	cgi::free_2d(int from)
{
	int	index = --from;

	while (index >= 0)
	{
		free(envp[index]);
		index--;
	}
	free(envp);
}

int	cgi::ft_environment()
{
	int		index;
	std::string	tmp;

	envp = (char	**)malloc(sizeof(char *) * 12);
	if (!envp)
		return (-1);

	index = 0;

	envp[index] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
	envp[++index] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
	envp[++index] = strdup("REDIRECT_STATUS=200");
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
	tmp = "PATH_INFO=" + this->PATH_INFO;
	envp[++index] = strdup(tmp.data());
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
	tmp = "REQUEST_URI=" + this->REQUEST_URI;
	envp[++index] = strdup(tmp.data());
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
	this->CONTENT_TYPE = "text/html";
	tmp = "CONTENT_TYPE=" + this->CONTENT_TYPE;
	envp[++index] = strdup(tmp.data());
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
	tmp = "SCRIPT_FILENAME=" + this->SCRIPT_FILENAME;
	envp[++index] = strdup(tmp.data());
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
	tmp = "SCRIPT_NAME=" + this->SCRIPT_NAME;
	envp[++index] = strdup(tmp.data());
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
	tmp = "HTTP_USER_AGENT=" + this->HTTP_USER_AGENT;
	envp[++index] =  strdup(tmp.data());
	if (!envp[index])
	{
		free_2d(index);
		return (-1);
	}
	//
		// envp[] =  strdup("HTTP_COOKIE=");
		// if (!envp[index])
		// {
		// 	free_2d(index);
		// 	return (-1);
		// }
	//

	if (!REQUEST_METHOD.compare("POST"))
	{
		envp[++index] = strdup("REQUEST_METHOD=POST");
		if (!envp[index])
		{
			free_2d(index);
			return (-1);
		}
		//
		tmp = "CONTENT_LENGTH=" + this->CONTENT_LENGTH;
		envp[++index] = strdup(tmp.data());
		if (!envp[index])
		{
			free_2d(index);
			return (-1);
		}
	}

	else if (!REQUEST_METHOD.compare("GET"))
	{
		envp[++index] = strdup("REQUEST_METHOD=GET");
		if (!envp[index])
		{
			free_2d(index);
			return (-1);
		}
		//
		tmp = "QUERY_STRING=" + this->QUERY_STRING;
		envp[++index] = strdup(tmp.data());
		if (!envp[index])
		{
			free_2d(index);
			return (-1);
		}
	}
	envp[++index] = NULL;
	return 0;
}

int	cgi::ft_cgi(std::string	fileName)
{
	std::ifstream	file(fileName);
	pid_t			pid;
	int				fds[2];
	int 			std_in = 0;
	int				status;

	if (file.is_open())
	{
		if (this->REQUEST_METHOD == "GET")
		{
			const char* queryParameters = strchr((const char *)this->REQUEST_URI.c_str(), '?');
 			if (queryParameters != NULL)
			{
				queryParameters++;
				this->QUERY_STRING = queryParameters;
			}
			std::string tmp_path_info = REQUEST_URI.substr(this->SCRIPT_NAME.size());
			size_t pos = tmp_path_info.find('?');
			if (pos != std::string::npos)
				tmp_path_info.erase(pos);
			this->PATH_INFO = tmp_path_info;
			if (ft_environment() == -1)
				return -1;
		}

		if (this->REQUEST_METHOD == "POST")
		{
			std::string	scontent_length = this->CONTENT_LENGTH;
			int			content_length = std::stoi(this->CONTENT_LENGTH);
			if (content_length > 0)
			{
				if (pipe(fds) == -1)
					return (-1);
				std_in = dup(STDIN_FILENO);
				write(fds[1], this->POST_DATA.data(), content_length);
				close(fds[1]);
				dup2(fds[0], STDIN_FILENO);
				close(fds[0]);
			}
			if (ft_environment() == -1)
				return -1;
		}

		pid = fork();
		if(pid == -1)
			return (-1);

		if (!pid)
		{
			int	fd = open("output_cgi", O_CREAT | O_RDWR | O_TRUNC, 0777);
			if (fd == -1)
				exit(1);
			dup2(fd, STDOUT_FILENO);
			char str[] = "/usr/bin/python3";
			char	*argv[3] = {str, (char *)fileName.c_str(), NULL};
			// char	*argv[] = {strdup("/usr/bin/python3"), (char *)fileName.c_str(), NULL};
			if (execve("/usr/bin/python3", argv, envp) == -1)
				exit(1);
		}
		else
		{
			waitpid(-1, &status, 0);
			free_2d(12);
			if (!this->REQUEST_METHOD.compare("POST"))
			{
				dup2(STDIN_FILENO, std_in);
				close(std_in);
			}
			if (status == -1)
				return (-1);
		}

		std::ifstream	output("output_cgi");

		if (output.is_open())
		{
			std::stringstream ss;
			ss << output.rdbuf();
			output.close();
			std::string file = ss.str();
			this->body = file;
			if (std::remove("output_cgi"))
				return (-1);
		}
		else
			return (-1);
		// Generate the HTML response using the parameters received
	}
	else
		return (-1);
	return (0);
}
