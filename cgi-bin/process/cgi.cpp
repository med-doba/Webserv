/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmoubal <hmoubal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:54:52 by med-doba          #+#    #+#             */
/*   Updated: 2023/05/17 19:18:59 by hmoubal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

cgi::cgi(/* args */)
{
}

cgi::~cgi()
{
}

void cgi::ft_environment()
{
	int		index;
	std::string	tmp;

	envp = (char	**)malloc(sizeof(char *) * 10);

	index = 0;

	envp[index] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	envp[++index] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	envp[++index] = strdup("REDIRECT_STATUS=200");
	//
	tmp = "PATH_INFO=" + this->PATH_INFO;
	envp[++index] = strdup(tmp.data());
	//
	tmp = "REQUEST_URI=" + this->REQUEST_URI;
	envp[++index] = strdup(tmp.data());
	//
	tmp = "CONTENT_TYPE=" + this->CONTENT_TYPE;
	envp[++index] = strdup(tmp.data());

	tmp = "SCRIPT_FILENAME=" + this->SCRIPT_FILENAME;
	envp[++index] = strdup(tmp.data());
	//

	if (!REQUEST_METHOD.compare("POST"))
	{
		envp[++index] = strdup("REQUEST_METHOD=POST");
		envp[++index] = strdup("CONTENT_LENGTH=");
		envp[++index] = strdup("POST_DATA=?");
	}

	else if (!REQUEST_METHOD.compare("GET"))
	{
		envp[++index] = strdup("REQUEST_METHOD=GET");
		//
		tmp = "QUERY_STRING=" + this->QUERY_STRING;
		envp[++index] = strdup(tmp.data());
	}

	// envp[3] = strdup("SCRIPT_NAME=file.py");
	// envp[] =  strdup("HTTP_COOKIE=");
	// envp[] =  strdup("HTTP_USER_AGENT=");

	envp[++index] = NULL;
}

void	cgi::ft_cgi(std::string	fileName)
{
	std::ifstream	file(fileName);
	// char	request_body[0];
	pid_t	pid;

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
			ft_environment();
		}

		// else if (this->REQUEST_METHOD == "POST")
		// {
		// 	std::string	scontent_length = this->.CONTENT_LENGTH;
		// 	int			content_length = std::stoi(this->.CONTENT_LENGTH);

		// 	request_body[content_length];
		// 	read(STDIN_FILENO, request_body, content_length);

		// 	envp = ft_environment();
		// }
		pid = fork();
		if(pid == -1)
			return (std::cerr << "Error: fork failed to creat a new process\n", exit(1));

		if (!pid)
		{
			int	fd = open("output_cgi", O_CREAT | O_RDWR | O_TRUNC, 0777);

			std::cout << "ext == " << this->executable << " filename == " << fileName << std::endl;
			if (fd == -1)
				return (std::cerr << "Error: fd file\n", exit(1));
			dup2(fd, STDOUT_FILENO);
			char	*argv[] = {strdup(this->executable.c_str()), (char *)fileName.c_str(), NULL};
			if (execve(this->executable.c_str(), argv, envp) == -1)
				return (std::cerr << "error execve\n", exit(1));
		}

		waitpid(-1, NULL, 0);
		std::ifstream	output("output_cgi");

		if (output.is_open())
		{
			std::stringstream ss;
			ss << output.rdbuf();
			output.close();
			std::string file = ss.str();
			this->body = file;
			if (std::remove("output_cgi"))
				return (std::cerr << "error remove\n", exit(1));
		}
		else
			return (std::cerr << "error file not open\n", exit(1));

		// test fhe stdout and stdin is workin clean
		// Generate the HTML response using the parameters received
	}
	else
		std::cerr << "error: file not open" << std::endl;
}
