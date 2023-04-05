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
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <poll.h>

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
        void handling_chunked_data(client &obj);
        void handling_form_data(client &obj);
        void putDataTofile(std::string  data, std::string & bodyofRequest);
        void  create_file_and_put_content(std::string & bodyofRequest,std::string & headerOfRequest);

        ~parssingOfBody();
};

class response
{
    public:
		int status_code;
		std::string phrase;
		int type;
		std::string response_req;
		std::string body;
		std::vector<std::string>headers;
		std::string version;
		std::string del;
		std::string closeheader;
		int close;
		std::string contentlength;
		std::string contenttype;
		int content;
		
        response(/* args */);
		void generate_response();
		int send_response(client &obj, struct pollfd &pfds);
		void defineContentType();
		response(const response &obj);
		response& operator=(const response &obj);
		void clear();
        ~response();

};

class parssingOfHeader
{
    private:

    public:

        parssingOfHeader();

        long long	ft_atoi(const char *str);

        int checkHeaderOfreq_(std::string&, int &, response& respond);
        int checkHeaderLine(std::string, int &,response & respond);
        int checkHeaders(std::string, int &, response& respond);
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
	int ready;


	parssingOfHeader headerParss;
	parssingOfBody   bodyParss;
	response respond;
	int tmp;
	int flag_;
	int total_bytes_received;
	int i;
	int j;
	int len;
	int flag_res;
	int ContentLength;

	int extractheader();
	void openfile();
	// int response(int pfds_index, vector<struct pollfd> &pfds);
	int pushToBuffer();
	void clear();
	int checkHeaderOfreq();
	long long	ft_atoi(const char *str);
	char *ft_substr(char const *s, unsigned int start, size_t len);
	void check(void);
	int check_method();
	void error_method(struct pollfd &pfds);
	int check_version();
	void error_version(struct pollfd &pfds);
	int check_location();
	void error_location(struct pollfd &pfds);
	void error_headers(struct pollfd &pfds);
	void normal_response(struct pollfd &pfds);
	client();
	client(const client &obj);
	client& operator=(const client& obj);
	~client();
};

#endif
