#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <sys/socket.h>
#include <fstream>
#include <fcntl.h>
#include "parssingOfBody.hpp"
#include "parssingOfHeader.hpp"

using std::string;
using std::cout;
using std::endl;

#define BUFFER 10000

class client
{
	public:
	int client_socket;
	std::string buffer;
	std::string response_header;
	std::string body;
	std::ifstream input;
	std::string headerOfRequest;
	std::ofstream file;
	std::vector<char> content_buffer;
	int bytes_read;
	int flag;


	parssingOfHeader headerParss;
	parssingOfBody   bodyParss;
	int tmp;
	int flag_;
	unsigned long i;
	unsigned long ContentLength;

	int extractheader();
	void openfile();
	int response();
	int pushToBuffer();
	int checkHeaderOfreq();
	long long	ft_atoi(const char *str);
	char *ft_substr(char const *s, unsigned int start, size_t len);
	client();
	client(const client &obj);
	client& operator=(const client& obj);
	~client();
};
#endif