#ifndef CGI
#define CGI

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <algorithm>

class cgi
{
	public:
		std::string	CONTENT_TYPE;
		std::string	CONTENT_LENGTH;
		std::string	PATH_INFO;
		std::string	QUERY_STRING;
		std::string	REQUEST_METHOD;
		std::string	SCRIPT_NAME;
		std::string	SCRIPT_FILENAME;
		std::string	REQUEST_URI;
		std::string	POST_DATA;
		std::string	HTTP_USER_AGENT;
		std::string	HTTP_COOKIE;
		std::string body;

		std::string	executable;
		char	**envp;

		cgi();
		cgi(const cgi &obj);
		cgi	&operator=(const cgi &obj);
		~cgi();

		//methodes
		int		ft_cgi(std::string	fileName);
		int		ft_environment();
		void	free_2d(int from);
		void	clear();

};

#endif
