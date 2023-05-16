/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_cgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 14:17:23 by med-doba          #+#    #+#             */
/*   Updated: 2023/05/16 16:39:36 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include "cgi.hpp"
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

size_t	ft_strlen(char	*str)
{
	int	i = 0;

	while (str && str[i])
		i++;
	return i;
}

char**	ft_enviroment(cgi &cgi_obj, char *methode)
{
	char	**rtn;

	rtn = (char	**)malloc(sizeof(char *) * 4);

	// if (!strcmp(methode, "POST"))
	// {
	// 	rtn[1] = strdup("REQUEST_METHOD=POST");//php/py
	// 	rtn[] = strdup("CONTENT_LENGTH=");

	// }
	(void)cgi_obj;
	if (!strcmp(methode, "GET"))
	{
		rtn[0] = strdup("REQUEST_METHOD=GET");//php/py
		rtn[1] = strdup("QUERY_STRING=moha=eddo&last_name=badr");//php/py
		rtn[2] = strdup("DOCUMENT_ROOT=/Users/med-doba/Desktop/Webserv/cgi-bin");//php/py
		// rtn[] =  strdup("REQUEST_URI=");//php/py
		rtn[3] = NULL;
	}
	// rtn[] = strdup("REQUEST_METHOD=POST");
	// rtn[2] = strdup("CONTENT_TYPE=text/html");
	// rtn[3] = strdup("SCRIPT_NAME=file.py");//php/py
	// rtn[4] = strdup("serverParse_PROTOCOL=http");
	// rtn[] =  strdup("serverParse_NAME=text/html");
	// rtn[] =  strdup("HTTP_COOKIE=");//php/py
	// rtn[] =  strdup("REMOTE_ADDR=");//php/py
	// rtn[] =  strdup("HTTP_USER_AGENT=");//php/py

	return rtn;
}

void	ft_cgi(std::string	fileName)
{
	std::ifstream	file(fileName);
	std::string		path;
	std::string		methode;
	cgi	cgi_obj;
	char	request_body[0];
	char	**envp;
	pid_t	pid;

	if (file.is_open())
	{
		//check extention of file
		if (path.substr(path.length() - 4) == ".php")
			path = "php-cgi";
		else if (path.substr(path.length() - 3) == ".py")
			path = "/usr/local/bin/python3";
		else
			return (std::cerr << "Error: extention file not supported\n", exit(1));

		if (methode == "GET")
		{
			// Parse the query string
			const char* queryParameters = strchr((const char *)cgi_obj.REQUEST_URI.c_str(), '?');
			if (queryParameters != NULL)
			{
				queryParameters++;
				cgi_obj.QUERY_STRING = queryParameters;
			}

			envp = ft_enviroment(cgi_obj, (char *)"GET");

			// Generate the HTML response using the parameters received
		}

		else if (methode == "POST")
		{
			std::string	scontent_length = cgi_obj.CONTENT_LENGTH;
			int			content_length = std::stoi(cgi_obj.CONTENT_LENGTH);

			request_body[content_length];
			read(STDIN_FILENO, request_body, content_length);

			envp = ft_enviroment(cgi_obj, "POST");
		}

		pid = fork();
		if(pid == -1)
			return (std::cerr << "Error: fork failed to creat a new process\n", exit(1));

		if (!pid)
		{
			int	fd = open("output_cgi", O_CREAT | O_RDWR, 0777);

			if (fd == -1)
				return (std::cerr << "Error: fd file\n", exit(1));
			dup2(fd, STDOUT_FILENO);

			char	*argv[] = {strdup(path.c_str()), (char *)fileName.c_str(), NULL};
			if (execve(path.c_str(), argv, envp) == -1)
				return (std::cerr << "error execve\n", exit(1));
		}

		waitpid(-1, NULL, 0);
		// test fhe stdout and stdin is workin clean
		std::cout << "meddo" << std::endl;
	}
	else
		std::cerr << "error: file not open" << std::endl;
	std::cout << "Content-type: text/html" << std::endl << std::endl;
}

int	main(int ac, char **av)
{
	if(ac == 2)
	{
		ft_cgi(av[1]);
	}
	return 0;
}

/*
    REQUEST_METHOD: GET, POST, PUT, DELETE, etc.
    QUERY_STRING: A string containing the URL query parameters, such as id=123&name=John.
    SCRIPT_NAME: The name of the script, such as index.php.
    REQUEST_URI: The full URL used to access the script, including the query string and any fragment identifier,
	such as /index.php?id=123#top.
    DOCUMENT_ROOT: The root directory of the web serverParse, such as /var/www/html/.
    HTTP_COOKIE: A string containing one or more cookies, such as user=John; session_id=abcd1234.
    HTTP_USER_AGENT: A string containing information about the client's browser and operating system,
	such as Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36
	(KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134.
    REMOTE_ADDR: The IP address of the client making the request, such as 192.168.0.1.

Note that the values of these environment variables can vary depending on the web serverParse and configuration,
and may also be affected by the client making the request.
*/
