/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_cgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 14:17:23 by med-doba          #+#    #+#             */
/*   Updated: 2023/05/15 22:25:35 by med-doba         ###   ########.fr       */
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
	std::string	path("/usr/local/bin/python3"), methode("GET");
	cgi	cgi_obj;
	char	request_body[0];
	char	**envp;

	if (file.is_open())
	{
		if (methode == "GET")
		{
			// Get the query string from the environment variables
			std::string queryString = cgi_obj.QUERY_STRING;

			// Parse the query string

			envp = ft_enviroment(cgi_obj, (char *)"GET");

			// Generate the HTML response using the parameters received
		}

		// else if (methode == "POST")
		// {
		// 	std::string	scontent_length = cgi_obj.CONTENT_LENGTH;
		// 	int			content_length = std::stoi(cgi_obj.CONTENT_LENGTH);

		// 	request_body[content_length];
		// 	read(STDIN_FILENO, request_body, content_length);

		// 	envp = ft_enviroment(cgi_obj, "POST");
		// }

		pid_t	pid = fork();
		// std::cout << "fd = " << fd << std::endl;

		if (!pid)
		{

			int	fd = open("output_cgi", O_CREAT | O_RDWR, 0777);
			dup2(fd, STDOUT_FILENO);
			// write(1, "has", 3);
			// std::cout << "fd = " << fd << std::endl;
			// int	tmp_in = dup(1);
			char	*argv[] = {strdup("/usr/local/bin/python3"), (char *)fileName.c_str(), NULL};
			// char	*argv[] = {(char *)fileName.c_str(), NULL};
			// for (int i = 0; i < 3; i++)
			// {
			// 	std::cout << "argv == " << argv[i] << std::endl;
			// }

			// puts("lol");

			if (execve(path.c_str(), argv, envp) == -1)
				return (std::cerr << "error execve\n", exit(1));
		}

		waitpid(-1, NULL, 0);
	}
	else
		std::cerr << "error: file not open" << std::endl;
	// std::cout << "Content-type: text/html" << std::endl << std::endl;
}

int	main(int ac, char	**av)
{
	if(ac == 2)
	{
		// av[1] = "file.py";
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