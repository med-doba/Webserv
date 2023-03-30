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
	obj.port = 8081;
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
					{
						std::cout << "ready to recv" << std::endl;
						this->receive(j);
					}
				}
			}
			if (this->pfds[i].revents & POLLOUT)
			{
				for (size_t j = 0; j < clients.size(); j++)
				{
					if (pfds[i].fd == clients[j].client_socket && clients[j].ready == 1)
					{
						std::cout << "ready to send" << std::endl;
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
	unsigned int addrlen = sizeof(address);

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
	if (clients[index].response() == 1)
	{
		this->disconnect(index);
		return ;
	}
}

void server::receive(int index)
{
    int rtn;
	int t;

    rtn = clients[index].pushToBuffer();

	t = rtn;
    if(rtn == 0 || rtn == -1)
	{
		std::cout << "r == " << rtn << std::endl;
		this->disconnect(index);
        return ;
	}
    rtn = clients[index].checkHeaderOfreq(t);
	// std::cout << clients[index].headerOfRequest << std::endl;
	// std::cout << rtn << std::endl;
    if(rtn == -2)
	{
		// cout << "r2 == " << rtn << endl;
        return ;
	}
    if(clients[index].flag == 1) // if has content lenght
		clients[index].bodyParss.handle_post(clients[index]);
    
    else if(clients[index].flag == 2)
    {
		std::cout << "get method" << std::endl;
		clients[index].check();
		// clients[index].ready = 1;
		return ;
        // without budy => GET method
    }
    else if(clients[index].flag == 3)// // handle chunked data when resend request
	{
		// std::cout << "chunked " << std::endl;
		// std::cout << "flag outside  == " << clients[index].flag_ << std::endl;
		// clients[index].bodyParss.handling_chunked_data(clients[index].buffer,clients[index].headerOfRequest,clients[index].boundary,clients[index].bodyofRequest,clients[index].total_bytes_received,clients[index].ContentLength,clients[index].i,t,clients[index].flag_);
		clients[index].bodyParss.handling_chunked_data(clients[index]);
	}
    else if(clients[index].flag == 4)
	{
		clients[index].bodyParss.handling_form_data(clients[index]);
		// clients[index].bodyParss.handling_form_data(clients[index].buffer,clients[index].headerOfRequest,clients[index].boundary,clients[index].bodyofRequest,clients[index].total_bytes_received,clients[index].ContentLength,clients[index].i,clients[index].bytes_read,clients[index].flag_);
	}
        
        
    // return 1;
}

