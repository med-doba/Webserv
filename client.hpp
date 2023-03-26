#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <strings.h>
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
#define BUFFER 1024

// class parssingOfBody;
class client;

// class parssingOfBody
// {
//     private:

//     public:
//         std::string             file;
//         std::string             exetention;
//         int                     fd;

//         parssingOfBody(/* args */);
//         void putDataTofile(std::string  data, std::string & bodyofRequest);
//         void handling_form_data(std::string& buffer, std::string &boundary,std::string &bodyofRequest,  int &total_bytes_received,unsigned long &ContentLength, int  &, int &);
//         // void handle_post(std::string &headerOfRequest, std::string &buffer, unsigned long &ContentLength, int &i, int &flag);
//         void handle_post(client &obj);
//         void handle_post(std::string &headerOfRequest,std::string &bodyofRequest, unsigned long & ContentLength, int & i,int & flag_,int &);
//         void handling_chunked_data(std::string &buffer,std::string &headerOfRequest, std::string &bodyofRequest, int & flag_);
//         ~parssingOfBody();
// };


class client
{
	public:
	int client_socket;
	std::string buffer;
	std::string boundary;
	std::string response_header;
	std::string body;
	std::ifstream input;
	std::string headerOfRequest;
	std::string bodyofRequest;
	std::ofstream file;
	std::vector<char> content_buffer;
	int bytes_read;
	int flag;
	int ignore;


	parssingOfHeader headerParss;
	parssingOfBody   bodyParss;
	int tmp;
	int flag_;
	int total_bytes_received;
	int i;
	int j;
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

typedef client rapper;
#endif
