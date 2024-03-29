#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <sys/ioctl.h>
#include <signal.h>

#include "miniserver.hpp"
#include "../parsingConf/serverParse.hpp"
#include "client.hpp"
#include <dirent.h>
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
	std::map<std::string, std::string> PercentEncoding;
	std::map<int, std::string> StatusPhrase;
	std::map<std::string, std::string> mimeTypes;
	std::multimap<std::string, std::string> mimeTypes_;
	int remove;
	int poll_count;

	server();
	server(const server &obj);
	server& operator=(const server& obj);
	~server();
	void check_servers(std::vector<miniserver>::iterator it);
	void lunch_servers();
	void monitor();
	void disconnect(int index);
	void fill(MapType	bind_info);
	void response(struct pollfd &pfds, int index);
	void receive(int pfds_index, int index);
	void new_connection(int indexPfds, int index);
	int checkLocation(client &objClient, serverParse obj);
	void checkMaxBodySize(client &objClient, serverParse obj, int loc);
	void checkMethodAllowed(client &objClient, serverParse obj, int loc);
	void checkRedirection(client &objClient, serverParse obj, int loc);
	void GetBehaviour(client &ObjClient, serverParse ObjServer, int loc);
	void PostBehaviour(client &ObjClient, serverParse ObjServer, int loc);
	void DeleteBehaviour(client &ObjClient, serverParse ObjServer, int loc);
	int DeleteAllContent(std::string path);
	int DeleteFile(std::string path);
	void fillEncoding();
	void fillStatusCode();
	void fillMimeType();
	void fillRevMimeType();
	int timeoutCheck();
	void fillCGI(client &ObjClient, serverParse ObjServer, int loc);
	void ClientHang(int index);
	int checkExtension(std::string pathCgi, locationParse ObjLocation);
	std::string trim_path(client &ObjClient, locationParse ObjLocation);
	serverParse& findServerBlock(int index);
};

#endif
