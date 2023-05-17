/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:54:52 by med-doba          #+#    #+#             */
/*   Updated: 2023/05/17 04:03:09 by hmoubal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

cgi::cgi(/* args */)
{
	this->executable = "/usr/bin/python3";
	this->QUERY_STRING="name=moha&amigo=zenon&team=hmoubal";
	this->CONTENT_TYPE= "text/plain";
	this->REQUEST_METHOD  = "GET";
	this->REQUEST_URI = "/cgi-bin/scripts_cgi/file.py/sdfsd/asfa?name=moha&amigo=zenon&team=hmoubal";
	this->PATH_INFO = "/path/info";
	this->SCRIPT_NAME = "../scripts_cgi/file.py";
}

cgi::~cgi()
{
}

void cgi::ft_environment()
{
	// char	**rtn;
	int		index;
	std::string	tmp;

	envp = (char	**)malloc(sizeof(char *) * 8);

	index = 0;

	envp[index] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	envp[++index] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	//
	tmp = "PATH_INFO=" + this->PATH_INFO;
	envp[++index] = strdup(tmp.data());
	//
	tmp = "REQUEST_URI=" + this->REQUEST_URI;
	envp[++index] = strdup(tmp.data());
	//
	tmp = "CONTENT_TYPE=" + this->CONTENT_TYPE;
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

	// return rtn;
}

void	cgi::ft_cgi(std::string	fileName)
{
	std::ifstream	file(fileName);
	// std::string		path;
	// std::string		methode;
	// char	request_body[0];
	// char	**envp;
	pid_t	pid;

	if (file.is_open())
	{
		// path = this->executable;
		//check extention of file
		// if (path.substr(path.length() - 4) == ".php")
		// 	path = "php-cgi";
		// else if (path.substr(path.length() - 3) == ".py")
		// 	path = "/usr/local/bin/python3";
		// else
		// 	return (std::cerr << "Error: extention file not supported\n", exit(1));

		if (this->REQUEST_METHOD == "GET")
		{
			// Parse the query string
			// const char* queryParameters = strchr((const char *)this->REQUEST_URI.c_str(), '?');
			// if (queryParameters != NULL)
			// {
			// 	queryParameters++;
			// 	this->QUERY_STRING = queryParameters;
			// 	//add path info
			// }

			ft_environment();

			// Generate the HTML response using the parameters received
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
		// std::cout << "meddo" << std::endl;
	}
	else
		std::cerr << "error: file not open" << std::endl;
	// std::cout << "Content-type: text/html" << std::endl << std::endl;
}
