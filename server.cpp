#include "server.hpp"
server::server()
{
	this->remove = 0;
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
		this->remove = obj.remove;
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
		this->remove = 0;
		this->poll_count = poll(&pfds[0], this->pfds.size(), 10);
		// std::cout << "poll count == " << poll_count << std::endl;
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
						std::cout << "ready to recv " << clients[j].client_socket << std::endl;
						this->receive(i, j);
						break ;
					}
				}
			}
			if (this->remove == 0 && this->pfds[i].revents & POLLOUT)
			{
				for (size_t j = 0; j < clients.size(); j++)
				{
					if (pfds[i].fd == clients[j].client_socket && clients[j].ready == 1)
					{
						std::cout << "ready to send" << std::endl;
						this->response(this->pfds[i], j);
						break ;
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
	this->remove = 1;
}

void server::response(struct pollfd &pfds, int index)
{
	if (clients[index].flag == ERROR)
	{
		// clients[index].respond.defineContentType();
		std::cout << clients[index].headerOfRequest << std::endl;
		clients[index].respond.generate_response();
		if (clients[index].respond.send_response(clients[index], pfds) == 1)
			this->disconnect(index);
	}
	if (clients[index].tmp == POST)
	{
		std::cout << "post method" << std::endl;
		if (clients[index].postMethod(pfds) == CLOSE)
			this->disconnect(index);
	}
	// else if(clients[index].flag == NONCHUNKED) // if has content length
	// {
	// 	std::cout << "post handle1" << std::endl;
	// 	clients[index].bodyParss.handle_post(clients[index]);
	// 	clients[index].respond.ready = 1;
	// }
	// else if(clients[index].flag == CHUNKED)// // handle chunked data when resend request
	// {
	// 	std::cout << "chunked handle1" << std::endl;
	// 	clients[index].bodyParss.handling_chunked_data(clients[index]);
	// 	clients[index].respond.ready = 1;
	// }
	// else if(clients[index].flag == FORM)
	// {
	// 	std::cout << "form handle1" << std::endl;
	// 	clients[index].bodyParss.handling_form_data(clients[index]);
	// 	clients[index].respond.ready = 1;
	// }
	// if (clients[index].respond.ready == 1)
	// {
	// 	if (clients[index].respond.created == 1)
	// 	{
	// 		clients[index].respond.status_code = 201;
	// 		clients[index].respond.phrase = "created";
	// 		clients[index].respond.type = 1;
	// 		clients[index].respond.content = 1;
	// 		clients[index].respond.body = "successfully uploaded";
	// 		clients[index].respond.generate_response();
	// 		clients[index].respond.send_response(clients[index], pfds);
	// 	}
	// 	clients[index].clear();
	// 	pfds.revents &= ~POLLOUT;
	// }
	else
		clients[index].normal_response(pfds);
	// else if (clients[index].response(pfds_index, pfds) == 1)
	// {
	// 	this->disconnect(index);
	// 	return ;
	// }
}

void server::receive(int pfds_index, int index)
{
    int rtn;
	// int t;

    rtn = clients[index].pushToBuffer();

	// t = rtn;
	if (rtn == -1)
	{
		std::cout << "socket client " << clients[index].client_socket << std::endl;
		std::cout << clients[index].headerOfRequest << std::endl;
		return ;
	}
    if(rtn == 0 || rtn == -1)
	{
		std::cout << "r == " << rtn << " socket client " << clients[index].client_socket << std::endl;
		std::cout << clients[index].headerOfRequest << std::endl;
		this->disconnect(index);
        return ;
	}
    rtn = clients[index].checkHeaderOfreq();
	std::cout << "here tmp -- " << clients[index].tmp << std::endl;
	// std::cout << clients[index].headerOfRequest << std::endl;
	// std::cout << rtn << std::endl;
    if(rtn == -2)
	{
		// cout << "r2 == " << rtn << endl;
        return ;
	}
    if(clients[index].flag == NONCHUNKED) // if has content lenght
	{
		std::cout << "post handle" << std::endl;
		// string test = clients[index].buffer.substr(clients[index].headerOfRequest.size() + 3,clients[index].ContentLength);
		string test = clients[index].buffer.substr(clients[index].headerOfRequest.size() + 3,clients[index].buffer.size() - clients[index].headerOfRequest.size() + 3);
		// std::cout << "header" << std::endl;
		// std::cout << clients[index].headerOfRequest << std::endl;
		// std::cout << "buffer" << std::endl;
		// std::cout << clients[index].buffer << std::endl;
		if((int)test.size() == clients[index].ContentLength)// finish recivng
		{
			clients[index].check();
			pfds[pfds_index].revents &= ~POLLIN;
		}
		else if ((int)test.size() > clients[index].ContentLength)
		{
			clients[index].respond.status_code = 400;
			clients[index].respond.phrase = "Bad Request";
			clients[index].respond.type = 1;
			clients[index].respond.body = "The request is invalid or malformed.";
			clients[index].respond.close = CLOSE;
			clients[index].respond.content = 1;
			clients[index].flag = ERROR;
			clients[index].check();
			pfds[pfds_index].revents &= ~POLLIN;
		}
		// clients[index].bodyParss.handle_post(clients[index]);
	}
   
    else if(clients[index].flag == GET)
    {
		// std::cout << "get method " << clients[index].client_socket << std::endl;
		// std::cout << clients[index].headerOfRequest << std::endl;
		// string test = clients[index].buffer.substr(clients[index].headerOfRequest.size() + 3,clients[index].buffer.size() - clients[index].headerOfRequest.size() + 3);
		// if (!test.empty() && clients[index].tmp == 0)
		// {
		// 	// if (test.find("\r\n\r\n") == std::string::npos)
		// 	// {
		// 		clients[index].respond.status_code = 400;
		// 		clients[index].respond.phrase = "Bad Request";
		// 		clients[index].respond.type = 1;
		// 		clients[index].respond.body = "No Body Should Exist With The Method Get";
		// 		clients[index].respond.close = CLOSE;
		// 		clients[index].respond.content = 1;
		// 		clients[index].flag_res = -1;
		// 	// }
		// }
		clients[index].check();
		pfds[pfds_index].revents &= ~POLLIN;
		return ;
        // without budy => GET method
    }
    else if(clients[index].flag == CHUNKED)// // handle chunked data when resend request
	{
		std::cout << "chunked handle" << std::endl;
		int pos = clients[index].buffer.find("\r\n0\r\n\r\n");
		if (pos != -1)
		{
			clients[index].check();
			pfds[pfds_index].revents &= ~POLLIN;
		}
	}
    else if(clients[index].flag == FORM)
	{
		std::cout << "form handle" << std::endl;
		if(clients[index].total_bytes_received < clients[index].ContentLength)// finish recivng
			clients[index].total_bytes_received += clients[index].bytes_read;
		if (clients[index].total_bytes_received >= clients[index].ContentLength)
		{
			clients[index].check();
			pfds[pfds_index].revents &= ~POLLIN;
		}
	}
	else if (clients[index].flag == ERROR)
	{
		clients[index].check();
		pfds[pfds_index].revents &= ~POLLIN;
	}
        
        
    // return 1;
}

