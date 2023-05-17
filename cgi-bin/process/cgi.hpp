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
	// private:
	public:
		std::string	CONTENT_TYPE;//h
		std::string	CONTENT_LENGTH;//h
		std::string	HTTP_COOKIE;
		std::string	HTTP_USER_AGENT;//h
		std::string	PATH_INFO;
		std::string	QUERY_STRING;
		std::string	REQUEST_METHOD;
		std::string	SCRIPT_FILENAME;
		std::string	DOCUMENT_ROOT;
		std::string	REQUEST_URI;
		std::string	POST_DATA;
		std::string	SCRIPT_NAME;
		std::string body;

		std::string	executable;
		char	**envp;
		// std::string	REMOTE_ADDR;

		cgi();
		cgi(const cgi &obj);
		cgi	&operator=(const cgi &obj);
		~cgi();

		//methodes
		void	ft_cgi(std::string	fileName);
		void ft_environment();

};

#endif
