#include <iostream>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <sys/socket.h>
#include <fstream>

class client
{
	public:
	int client_socket;
	std::string buffer;
	std::string response_header;
	std::string body;
	std::ifstream input;
	std::string header_request;
	std::ofstream file;
	std::vector<char> content_buffer;
	int bytes_read;
	int flag;

	int extractheader();
	void openfile();
	int response();
	client();
	client(const client &obj);
	client& operator=(const client& obj);
	~client();
};
