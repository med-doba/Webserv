/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmoubal <hmoubal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:54:52 by med-doba          #+#    #+#             */
/*   Updated: 2023/05/23 01:01:09 by hmoubal          ###   ########.fr       */
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

	envp = (char	**)malloc(sizeof(char *) * 11);
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
	// envp[++index] = strdup("REDIRECT_STATUS=200");
	// if (!envp[index])
	// {
	// 	free_2d(index);
	// 	return (-1);
	// }
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
	pid_t			pid;
	int				fds[2];
	int				output[2];
	int 			std_in = 0;
	int				status;

	if (pipe(output) == -1)
		return (-1);
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
		{
			close(output[0]);
			close(output[1]);
			return -1;
		}
	}

	if (this->REQUEST_METHOD == "POST")
	{
		std::string	scontent_length = this->CONTENT_LENGTH;
		int			content_length = std::stoi(this->CONTENT_LENGTH);
		if (content_length > 0)
		{
			if (pipe(fds) == -1)
			{
				close(output[0]);
				close(output[1]);
				return (-1);
			}
			std_in = dup(STDIN_FILENO);
			write(fds[1], this->POST_DATA.data(), content_length);
			close(fds[1]);
			dup2(fds[0], STDIN_FILENO);
			close(fds[0]);
		}
		if (ft_environment() == -1)
		{
			close(output[0]);
			close(output[1]);
			return -1;
		}
	}

	pid = fork();
	if(pid == -1)
	{
		free_2d(11);
		close(output[0]);
		close(output[1]);
		return (-1);
	}

	if (!pid)
	{
		dup2(output[1], STDOUT_FILENO);
		close(output[1]);
		close(output[0]);
		char	*argv[3] = {(char *)this->executable.c_str(), (char *)fileName.c_str(), NULL};
		if (execve((char *)this->executable.c_str(), argv, envp) == -1)
		{
			free_2d(11);
			// std::cout << "helo from exec" << std::endl;
			exit(1);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		close(output[1]);
		free_2d(11);
		if (!this->REQUEST_METHOD.compare("POST"))
		{
			dup2(STDIN_FILENO, std_in);
			// close(std_in);
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			// std::cout << "jadas " << std::endl;
			// close(output[1]);
			close(output[0]);
			return (-1);
		}
	}
	const int BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead;

	while ((bytesRead = read(output[0], buffer, BUFFER_SIZE)) > 0) 
	{
		// Process the data read from the pipe
		buffer[bytesRead] = '\0';
		this->body += buffer;
	}

	// Close the read end of the pipe
	// close(output[1]);
	close(output[0]);
	return (0);
}

void	cgi::clear()
{
	CONTENT_TYPE.clear();
	CONTENT_LENGTH.clear();
	PATH_INFO.clear();
	QUERY_STRING.clear();
	REQUEST_METHOD.clear();
	SCRIPT_NAME.clear();
	SCRIPT_FILENAME.clear();
	REQUEST_URI.clear();
	POST_DATA.clear();
	HTTP_USER_AGENT.clear();
	HTTP_COOKIE.clear();
	body.clear();
	executable.clear();
}

