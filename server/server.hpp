#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <fcntl.h>
#include <cstring>
#include <sys/ioctl.h>

#include "miniserver.hpp"
#include "../parsingConf/serverParse.hpp"
#include "client.hpp"
#include <sys/stat.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
#include <netdb.h>





class server
{
	public:
	std::vector<miniserver> servers;
	std::vector<client> clients;
	std::vector<struct pollfd> pfds;
	std::vector<serverParse>	block;
	int remove;
	int poll_count;

	server();
	server(const server &obj);
	server& operator=(const server& obj);
	~server();
	void lunch_servers();
	void monitor();
	void disconnect(int index);
	void fill(MapType	bind_info);
	void response(struct pollfd &pfds, int index);
	void receive(int pfds_index, int index);
	void new_connection(int indexPfds, int index);
	int checkLocation(client &objClient, serverParse obj);
	void checkMaxBodySize(client& objClient, serverParse obj, int loc);
	void checkMethodAllowed(client& objClient, serverParse obj, int loc);
	void checkRedirection(client& objClient, serverParse obj, int loc);
	void GetBehaviour(client &ObjClient, serverParse ObjServer, int loc);
	void PostBehaviour(client &ObjClient, serverParse ObjServer, int loc);
	serverParse& findServerBlock(int index);
};

#endif
