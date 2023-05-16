#ifndef CGI
#define CGI

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

class cgi
{
	// private:
	public:
		std::string	REQUEST_METHOD;
		std::string	QUERY_STRING;
		std::string	CONTENT_LENGTH;
		std::string	CONTENT_TYPE;
		std::string	PATH_INFO;
		std::string	DOCUMENT_ROOT;
		std::string	HTTP_COOKIE;
		std::string	REQUEST_URI;
		std::string	POST_DATA;
		char	*command;
		// std::string	REMOTE_ADDR;

		cgi();
		cgi(const cgi &obj);
		cgi	&operator=(const cgi &obj);
		~cgi();
};

#endif