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

struct sockaddr_in getAdrress(std::string host)
{
	struct addrinfo hints, *res, *p;
	struct sockaddr_in ipv4;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(host.data(), NULL, &hints, &res) != 0)
		throw (std::runtime_error("error host"));
	for(p = res;p != NULL; p = p->ai_next)
	{
		if (p->ai_family == AF_INET)
		{
			ipv4 = *((struct sockaddr_in *)p->ai_addr);
			break ;
		}
	}
	freeaddrinfo(res);
	return (ipv4);
}

void server::fill(MapType	bind_info)
{
	miniserver obj;
	for (size_t i = 0; i < bind_info.size(); i++)
	{
		std::cout << "host == " << bind_info[i].host << std::endl;
		obj.address.sin_addr.s_addr = getAdrress(bind_info[i].host).sin_addr.s_addr;
		for (size_t j = 0; j < bind_info[i].ports.size(); j++)
		{
			std::cout << "ports == " << bind_info[i].ports[j] << std::endl;
			obj.port = std::stoi(bind_info[i].ports[j]);
			obj.str_port = bind_info[i].ports[j];
			obj.address.sin_family = AF_INET;
			obj.address.sin_port = htons(obj.port);
			obj.addrlen = sizeof(obj.address);
			servers.push_back(obj);
		}
	}
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
		// //std::cout << "poll count == " << poll_count << std::endl;
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

int server::checkLocation(client &objClient, serverParse obj)
{
	for (size_t i = 0; i < obj.obj_location.size(); i++)
	{
		if (objClient.URI.compare(obj.obj_location[i].path) == 0)
		{
			std::cout << "path => " << obj.obj_location[i].path << std::endl;
			return (i);
		}
	}
	if (objClient.flag != ERROR)
	{
		objClient.flag = ERROR;
		objClient.respond.type = 1;
		objClient.respond.status_code = 400;
		objClient.respond.phrase = "Bad Request";
		objClient.respond.content = 1;
		objClient.respond.body = "Location Not Found";
		objClient.respond.close = CLOSE;
	}
	return (-1);
}

serverParse server::findServerBlock(int index)
{
	std::string headerreq = clients[index].headerOfRequest;
	std::string line;
	std::string host;
	std::string port;
	int pos = headerreq.find("Host: ");
	line = headerreq.substr(pos + 6, headerreq.find('\r', pos + 6) - pos - 6);
	host = line.substr(0, line.find(':'));
	port = line.substr(line.find(':') + 1);
	size_t i = 0;
	while (i < this->block.size())
	{
		if (host.compare(block[i].host) != 0)
		{
			for (size_t j = 0; j < block[i].server_name.size(); j++)
			{
				if (host.compare(block[i].server_name[j]) == 0)
				{
					for (size_t l = 1; l < block[i].listen.size(); l++)
					{
						if (port.compare(block[i].listen[l]) == 0)
							return (block[i]);
					}
				}
			}
		}
		else
		{
			for (size_t l = 1; l < block[i].listen.size(); l++)
			{
				if (port.compare(block[i].listen[l]) == 0)
					return (block[i]);
			}
		}
		i++;
	}

	std::cout << "host == %"<< host  << "%"<< std::endl;
	std::cout << "end" << std::endl;
	std::cout << "port == %"<< port  << "%"<< std::endl;
	std::cout << "end" << std::endl;
	return (block[i]);
}

void server::checkMaxBodySize(client objClient, serverParse obj, int loc)
{
	(void)obj;
	size_t allowedSize = 1048576;
	(void)loc;
	(void)objClient;
	int i = objClient.headerOfRequest.find("Content-Length: ");
	if (i != -1)
	{
		long long length = std::stoi(objClient.headerOfRequest.substr(i + 16,objClient.headerOfRequest.size()).c_str());
		std::cout << "bool1 == " << obj.obj_location[loc].client_max_body_size_ << std::endl;
		std::cout << "bool2 == " << obj.client_max_body_size_ << std::endl;
		if (obj.obj_location[loc].client_max_body_size_)
		{
			std::cout << "one\n";
			allowedSize = obj.obj_location[loc].client_max_body_size;
		}
		else if (obj.client_max_body_size_)
		{
			std::cout << "two\n";
			allowedSize = obj.client_max_body_size;
		}
		std::cout << "allowed == " << allowedSize << std::endl;
		if (length > 0)
		{
			std::cout << "length == " << length << std::endl;
			objClient.respond.type = 1;
			objClient.respond.status_code = 400;
			objClient.respond.phrase = "Bad Request";
			objClient.respond.content = 1;
			objClient.respond.body = "The request has a malformed header1";
			objClient.respond.close = CLOSE;
			return ;
		}
	}
}

void server::response(struct pollfd &pfds, int index)
{
	serverParse objServer = findServerBlock(index);
	int loc = checkLocation(clients[index], objServer);
	if (loc != -1)
		checkMaxBodySize(clients[index], objServer, loc);
	if (clients[index].flag == ERROR)
	{
		std::cout << "ERROR" << std::endl;
		clients[index].respond.generate_response();
		if (clients[index].respond.send_response(clients[index], pfds) == 1)
			this->disconnect(index);
		return;
	}
	if (clients[index].tmp == POST)
	{
		std::cout << "POST method" << std::endl;
		if (clients[index].postMethod(pfds) == CLOSE)
			this->disconnect(index);
	}
	else if (clients[index].tmp == DELETE)
	{
		std::cout << "DELETE method" << std::endl;
		if (clients[index].deleteMethod(pfds) == CLOSE)
			this->disconnect(index);
	}
	else if (clients[index].tmp == GET)
	{
		std::cout << "GEt method" << std::endl;
		clients[index].normal_response(pfds);
	}
}

void server::receive(int pfds_index, int index)
{
    int rtn;
	// int t;

    rtn = clients[index].pushToBuffer();

	// t = rtn;
	if (rtn == -1)
	{
		//std::cout << "socket client " << clients[index].client_socket << std::endl;
		//std::cout << clients[index].headerOfRequest << std::endl;
		return ;
	}
    if(rtn == 0 || rtn == -1)
	{
		//std::cout << "r == " << rtn << " socket client " << clients[index].client_socket << std::endl;
		//std::cout << clients[index].headerOfRequest << std::endl;
		this->disconnect(index);
        return ;
	}
    rtn = clients[index].checkHeaderOfreq();
	// //std::cout << "here tmp -- " << clients[index].tmp << std::endl;
	// //std::cout << clients[index].headerOfRequest << std::endl;
	// //std::cout << rtn << std::endl;
	if(rtn == -2)
	{
		// cout << "r2 == " << rtn << endl;
		return ;
	}
	if(clients[index].flag == NONCHUNKED) // if has content lenght
	{
		// //std::cout << "post handle" << std::endl;
		// string test = clients[index].buffer.substr(clients[index].headerOfRequest.size() + 3,clients[index].ContentLength);
		string test = clients[index].buffer.substr(clients[index].headerOfRequest.size() + 3,clients[index].buffer.size() - clients[index].headerOfRequest.size() + 3);
		// //std::cout << "header" << std::endl;
		// //std::cout << clients[index].headerOfRequest << std::endl;
		// //std::cout << "buffer" << std::endl;
		// //std::cout << clients[index].buffer << std::endl;
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
		// //std::cout << "get method " << clients[index].client_socket << std::endl;
		// //std::cout << clients[index].headerOfRequest << std::endl;
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
	else if(clients[index].flag == DELETE)
	{
		clients[index].check();
		pfds[pfds_index].revents &= ~POLLIN;
		return ;
		// without budy => DELETE method
	}
	else if(clients[index].flag == CHUNKED)// // handle chunked data when resend request
	{
		// //std::cout << "chunked handle" << std::endl;
		int pos = clients[index].buffer.find("\r\n0\r\n\r\n");
		if (pos != -1)
		{
			clients[index].check();
			pfds[pfds_index].revents &= ~POLLIN;
		}
	}
	else if(clients[index].flag == FORM)
	{
		// //std::cout << "form handle" << std::endl;
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

    // //std::cout << "out of recv" << std::endl;
    // return 1;
}

