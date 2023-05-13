#ifndef MINISERVER_HPP
#define MINISERVER_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include "client.hpp"

#define BACKLOG 10

class miniserver
{
	public:
	int socket_server;
	short port;
	int opt;
	std::string str_port;
	struct sockaddr_in address;
	int addrlen;
	int fail;

	miniserver();
	miniserver(const miniserver &obj);
	miniserver& operator=(const miniserver& obj);
	~miniserver();
};

#endif
