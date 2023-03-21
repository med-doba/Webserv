/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_py.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 14:17:23 by med-doba          #+#    #+#             */
/*   Updated: 2023/03/21 13:14:01 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>

size_t	ft_strlen(char	*str)
{
	int	i = 0;

	while (str && str[i])
		i++;
	return i;
}

char**	ft_enviroment(void)
{
	// std::string	env[4];
	char	**rtn;

	rtn = (char	**)malloc(sizeof(char *) * 6);

	// env[0] = "DOCUMENT_ROOT=/Users/med-doba/Desktop/Webserv/cgi-bin";
	// rtn[0] = (char *)env[4].c_str();
	rtn[0] = strdup("DOCUMENT_ROOT=/Users/med-doba/Desktop/Webserv/cgi-bin");
	// rtn[0] = (char*)malloc(sizeof(char) * ft_strlen("DOCUMENT_ROOT=/Users/med-doba/Desktop/Webserv/cgi-bin"));
	// env[1] = "REQUEST_METHOD=GET";
	// rtn[1] = (char *)env[1].c_str();
	rtn[1] = strdup("REQUEST_METHOD=GET");
	// rtn[1] = strdup("REQUEST_METHOD=POST");
	// env[2] = "CONTENT_TYPE=text/html";
	// rtn[2] = (char *)env[2].c_str();
	rtn[2] = strdup("CONTENT_TYPE=text/html");
	// env[3] = "SCRIPT_NAME=cgi-bin/script_py.cpp";
	// rtn[3] = (char *)env[3].c_str();
	rtn[3] = strdup("SCRIPT_NAME=cgi-bin/script_py.cpp");
	// env[4] = "SERVER_PROTOCOL=http";
	// rtn[4] = (char *)env[4].c_str();
	rtn[4] = strdup("SERVER_PROTOCOL=http");
	// env[5] = "QUERY_STRING=";
	// env[2] = "CONTENT_LENGTH=";
	// rtn[2] = (char *)env[2].c_str();
	// env[4] = "SERVER_NAME=text/html";
	// rtn[4] = (char *)env[4].c_str();
	rtn[5] = NULL;

	return rtn;
}


void	ft_cgi(std::string	fileName)
{
	std::ifstream	file(fileName);
	pid_t	pid;
	std::string	path("/usr/local/bin/python3");

	if (file.is_open())
	{
		pid = fork();
		if (!pid)
		{
			char	*argv[] = {strdup("/usr/local/bin/python3"), (char *)fileName.c_str(), NULL};
			char	**envp = ft_enviroment();
			if (execve(path.c_str(), argv, envp) == -1)
				return (std::cerr << "error execve\n", exit(1));
		}
		waitpid(-1, NULL, 0);
	}
	else
		std::cerr << "error: file not open" << std::endl;
	std::cout << "Content-type: text/html" << std::endl << std::endl;

}
int	main(void)
{
	ft_cgi("file.py");
	return 0;
}