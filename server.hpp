#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <cstring>
#include <sys/ioctl.h>
#include "miniserver.hpp"
#include "client.hpp"

class server
{
	public:
	std::vector<miniserver> servers;
	std::vector<client> clients;
	std::vector<struct pollfd> pfds;
	int remove;
	int poll_count;

	server();
	server(const server &obj);
	server& operator=(const server& obj);
	~server();
	void lunch_servers();
	void monitor();
	void disconnect(int index);
	void fill();
	void response(struct pollfd &pfds, int index);
	void receive(int pfds_index, int index);
	// void response(int index);
	void new_connection(int index);
};

#endif
