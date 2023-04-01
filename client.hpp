#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <strings.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <fstream>
#include <fcntl.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
// #include "parssingOfBody.hpp"
// #include "parssingOfHeader.hpp"

using std::string;
using std::vector;
#define BUFFER 500000

class client;

class parssingOfBody
{
    private:

    public:
        std::string             file;
        std::string             exetention;
        int                     fd;

        parssingOfBody(/* args */);
        void handle_post(client &obj);
        // void handling_chunked_data(std::string &buffer,std::string &headerOfRequest, std::string &boundary,std::string &bodyofRequest, int & total_bytes_received, int & ContentLength, int & i, int & bytes_received,int & flag_);
        void handling_chunked_data(client &obj);
        // void handling_form_data(std::string& buffer, std::string &headerOfRequest, std::string &boundary,std::string & bodyofRequest ,int &total_bytes_received,int &ContentLength,  int & i, int & bytes_received, int & flag_);
        void handling_form_data(client &obj);
        void putDataTofile(std::string  data, std::string & bodyofRequest);
        void  create_file_and_put_content(std::string & bodyofRequest,std::string & headerOfRequest);

        ~parssingOfBody();
};

class parssingOfHeader
{
    private:

    public:

        parssingOfHeader();

        long long	ft_atoi(const char *str);

        int checkHeaderOfreq_(std::string, int &);
        int checkHeaderLine(std::string, int &);
        int checkHeaders(int index, std::string, int &);
        ~parssingOfHeader();
};

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
	// int ignore;
	int ready;


	parssingOfHeader headerParss;
	parssingOfBody   bodyParss;
	int tmp;
	int flag_;
	int total_bytes_received;
	int i;
	int j;
	int len;
	int ContentLength;

	int extractheader();
	void openfile();
	int response(int pfds_index, vector<struct pollfd> &pfds);
	int pushToBuffer();
	int checkHeaderOfreq(int &len);
	long long	ft_atoi(const char *str);
	char *ft_substr(char const *s, unsigned int start, size_t len);
	void check(void);
	client();
	client(const client &obj);
	client& operator=(const client& obj);
	~client();
};

#endif
