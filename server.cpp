#include "server.hpp"
server::server()
{
	this->flag_con = 0;
	this->poll_count = 0;

}
server::server(const server &obj)
{
	*this = obj;
}

server& server::operator=(const server& obj)
{
	if (this != &obj)
	{
		this->servers = obj.servers;
		this->clients = obj.clients;
		this->pfds = obj.pfds;
		this->flag_con = obj.flag_con;
		this->poll_count = obj.poll_count;
	}
	return (*this);
}

server::~server()
{

}

void server::fill()
{
	miniserver obj;
	obj.port = 8080;
	obj.str_port = std::to_string(obj.port);
	obj.address.sin_family = AF_INET;
	obj.address.sin_addr.s_addr = INADDR_ANY;
	obj.address.sin_port = htons(obj.port);
	obj.addrlen = sizeof(obj.address);
	servers.push_back(obj);
	// obj.port = 8081;
	// obj.str_port = std::to_string(obj.port);
	// obj.address.sin_family = AF_INET;
	// obj.address.sin_addr.s_addr = INADDR_ANY;
	// obj.address.sin_port = htons(obj.port);
	// obj.addrlen = sizeof(obj.address);
	// servers.push_back(obj);
}

void server::lunch_servers()
{
	struct pollfd pfd;
	for (size_t i = 0; i < servers.size(); i++)
	{
		if ((servers[i].socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("socket failed");
			exit(EXIT_FAILURE);
		}
		if (setsockopt(servers[i].socket_server, SOL_SOCKET, SO_REUSEADDR, &(servers[i].opt), sizeof(servers[i].opt)))
		{
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		if (bind(servers[i].socket_server, (struct sockaddr*)&(servers[i].address), servers[i].addrlen) < 0)
		{
			perror("bind failed");
			exit(EXIT_FAILURE);
		}
		fcntl(servers[i].socket_server, F_SETFL, O_NONBLOCK);
		if (listen(servers[i].socket_server, BACKLOG) < 0)
		{
			perror("listen");
			exit(EXIT_FAILURE);
		}
		pfd.fd = servers[i].socket_server;
		pfd.events = POLLIN;
		pfds.push_back(pfd);
	}
}

void server::monitor()
{
	while (1)
	{
		this->poll_count = poll(&pfds[0], this->pfds.size(), 10);
		for (size_t i = 0; i < this->pfds.size(); i++)
		{
			if (this->pfds[i].revents & POLLIN)
			{
				for (size_t j = 0; j < servers.size(); j++)
				{
					if (pfds[i].fd == servers[j].socket_server)
					{
						this->new_connection(j);
						break ;
					}
				}
				for (size_t j = 0; j < clients.size(); j++)
				{
					if (pfds[i].fd == clients[j].client_socket)
						this->receive(j);
				}
			}
			else if (this->pfds[i].revents & POLLOUT)
			{
				for (size_t j = 0; j < clients.size(); j++)
				{
					if (pfds[i].fd == clients[j].client_socket)
					{
						// if (clients[j].flag != 1)
						// 	break ;
						// else
							this->response(j);
					}
				}
			}
		}
	}
}

void server::new_connection(int index)
{
	client obj;
	struct pollfd c;
	struct sockaddr_in address;
	unsigned int addrlen;

	if ((obj.client_socket = accept(servers[index].socket_server, (struct sockaddr*)&(address), (socklen_t*)&(addrlen))) < 0) 
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	c.fd = obj.client_socket;
	fcntl(obj.client_socket, F_SETFL, O_NONBLOCK);
	c.events = POLLIN | POLLOUT;
	pfds.push_back(c);
	clients.push_back(obj);
	std::cout << "new client connected " << obj.client_socket << std::endl;
}

void server::disconnect(int index)
{
	std::cout << "client disconnected " << clients[index].client_socket << std::endl;
	close(clients[index].client_socket);
	pfds.erase(pfds.begin() + index + servers.size());
	clients.erase(clients.begin() + index);
}

void server::response(int index)
{
	if (clients[index].extractheader() == 1)
	{
		std::cout << "fav sock client before disconnect == " << clients[index].client_socket << std::endl;
		std::cout << clients[index].header_request << std::endl;
		this->disconnect(index);
		return ;
	}
	// std::cout << clients[index].ignore << std::endl;
	if (!clients[index].header_request.empty())
	{
		clients[index].openfile();
		if (clients[index].response() == 1)
		{
			// std::cout << "disconnect response " << clients[index].client_socket << std::endl;
			// std::cout << clients[index].header_request << std::endl;
			this->disconnect(index);
			return ;
		}
	}
}

void server::receive(int index)
{
	char buffer[BUFFER];
	memset(buffer, 0, BUFFER);
	clients[index].bytes_read = recv(clients[index].client_socket, buffer, BUFFER, 0);
	if (clients[index].bytes_read < 0)
	{
		std::cout << "lol" << std::endl;
		clients[index].flag = 1;
		return ;
	}
	else if (clients[index].bytes_read <= 0)
	{
		std::cout << "disconnect from recv " << std::endl;
		// std::cout << "index == " << index << std::endl;
		// std::cout << "sock == " << clients[index].client_socket << std::endl;
		// std::cout << "size == " << clients.size() << std::endl;
		std::cout << "bytes == " << clients[index].bytes_read << std::endl;
		std::cout << clients[index].header_request << std::endl;
		// std::cout << "disconnect recv " << clients[index].client_socket << std::endl;
		// std::cout << clients[index].header_request << std::endl;
		this->disconnect(index);
		return ;
	}
	// std::cout << "bytes == " << clients[index].bytes_read << std::endl;
	// std::cout << clients[index].bytes_read << "\n";
	if (clients[index].bytes_read > BUFFER)
		clients[index].bytes_read = BUFFER;

	// std::cout << "read from recv "  << clients[index].client_socket << std::endl;
	// std::cout << buffer << std::endl;
	// std::cout << "end recv" << std::endl;
	// std::cout << "create str"  << clients[index].bytes_read << std::endl;
	std::string str(buffer,clients[index].bytes_read);	
	// std::cout << str << std::endl;
	// std::cout << "end str" << std::endl;
	clients[index].buffer += str;
}