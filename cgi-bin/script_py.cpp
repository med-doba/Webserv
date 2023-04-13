/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_py.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 14:17:23 by med-doba          #+#    #+#             */
/*   Updated: 2023/03/28 08:45:32 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

size_t	ft_strlen(char	*str)
{
	int	i = 0;

	while (str && str[i])
		i++;
	return i;
}

char**	ft_enviroment(void)
{
	char	**rtn;

	rtn = (char	**)malloc(sizeof(char *) * 6);

	rtn[0] = strdup("DOCUMENT_ROOT=/Users/med-doba/Desktop/Webserv/cgi-bin");//php/py
	rtn[1] = strdup("REQUEST_METHOD=GET");//php/py
	// rtn[] = strdup("REQUEST_METHOD=POST");
	rtn[2] = strdup("CONTENT_TYPE=text/html");
	rtn[3] = strdup("SCRIPT_NAME=file.py");//php/py
	rtn[4] = strdup("SERVER_PROTOCOL=http");
	// rtn[] = strdup("QUERY_STRING=");//php/py
	// rtn[] = strdup("CONTENT_LENGTH=");
	// rtn[] =  strdup("SERVER_NAME=text/html");
	// rtn[] =  strdup("REQUEST_URI=");//php/py
	// rtn[] =  strdup("HTTP_COOKIE=");//php/py
	// rtn[] =  strdup("REMOTE_ADDR=");//php/py
	// rtn[] =  strdup("HTTP_USER_AGENT=");//php/py
	rtn[5] = NULL;

	return rtn;
}

void	ft_cgi_php(std::string	fileName)
{
	std::ifstream	file(fileName);
	pid_t	pid;
	int		fd, tmp_in;
	std::string	path("/usr/bin/php");

	if (file.is_open())
	{
		fd = open("output_cgi", O_CREAT | O_RDWR, 0777);
		pid = fork();
		if (!pid)
		{
			tmp_in = dup(1);
			dup2(fd, 1);
			char	*argv[] = {strdup("/usr/bin/php"), (char *)fileName.c_str(), NULL};
			char	**envp = ft_enviroment();
			if (execve(path.c_str(), argv, envp) == -1)
				return (std::cerr << "error execve\n", exit(1));
		}
		waitpid(-1, NULL, 0);
	}
	else
		std::cerr << "error: file not open" << std::endl;
	// std::cout << "Content-type: text/html" << std::endl << std::endl;
}

void	ft_cgi_py(std::string	fileName)
{
	std::ifstream	file(fileName);
	std::string	path("/usr/local/bin/python3"), methode;

	if (file.is_open())
	{
		// if (methode == "GET")
		// {

		// }
		// else if (methode == "POST")
		// {

		// }
		int	fd = open("output_cgi", O_CREAT | O_RDWR, 0777);
		pid_t	pid = fork();
		if (!pid)
		{
			int	tmp_in = dup(1);
			dup2(fd, 1);
			char	*argv[] = {strdup("/usr/local/bin/python3"), (char *)fileName.c_str(), NULL};
			char	**envp = ft_enviroment();
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
		av[1] = "file.py";
		ft_cgi_py(av[1]);
		av[1] = "file.php";
		ft_cgi_php(av[1]);
	}
	return 0;
}

/*
    REQUEST_METHOD: GET, POST, PUT, DELETE, etc.
    QUERY_STRING: A string containing the URL query parameters, such as id=123&name=John.
    SCRIPT_NAME: The name of the script, such as index.php.
    REQUEST_URI: The full URL used to access the script, including the query string and any fragment identifier,
	such as /index.php?id=123#top.
    DOCUMENT_ROOT: The root directory of the web server, such as /var/www/html/.
    HTTP_COOKIE: A string containing one or more cookies, such as user=John; session_id=abcd1234.
    HTTP_USER_AGENT: A string containing information about the client's browser and operating system,
	such as Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36
	(KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134.
    REMOTE_ADDR: The IP address of the client making the request, such as 192.168.0.1.

Note that the values of these environment variables can vary depending on the web server and configuration,
and may also be affected by the client making the request.
*/