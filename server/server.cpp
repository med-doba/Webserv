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
		this->block = obj.block;
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
		this->poll_count = poll(&pfds[0], this->pfds.size(), -1);
		if (poll_count > 0)
		{
			// std::cout << "poll count == " << poll_count << std::endl;
			for (size_t i = 0; i < this->pfds.size(); i++)
			{
				if (this->pfds[i].revents & POLLIN)
				{
					for (size_t j = 0; j < servers.size(); j++)
					{
						if (pfds[i].fd == servers[j].socket_server)
						{
							this->new_connection(i, j);
							break ;
						}
					}
					for (size_t j = 0; j < clients.size(); j++)
					{
						// if (pfds[i].fd == clients[j]->client_socket)
						if (pfds[i].fd == clients[j].client_socket && clients[j].ready == 0)
						{
							// std::cout << "ready to recv " << clients[j]->client_socket << std::endl;
							this->receive(i, j);
							break ;
						}
					}
					// if (clients[pfds[i].fd] != nullptr && clients[pfds[i].fd]->ready == 0)
					// {
					// 	// std::cout << "ready to recv " << clients[j]->client_socket << std::endl;
					// 	this->receive(i, pfds[i].fd);
					// 	// break ;
					// }
				}
				else if (this->pfds[i].revents & POLLOUT)
				{
					for (size_t j = 0; j < clients.size(); j++)
					{
						if (pfds[i].fd == clients[j].client_socket && clients[j].ready == 1)
						{
							// std::cout << "ready to send " <<  pfds[i].fd << std::endl;
							this->response(this->pfds[i], j);
							break ;
						}
					}
					// if (clients[pfds[i].fd] != nullptr && clients[pfds[i].fd]->ready == 1)
					// {
					// 	// std::cout << "ready to recv " << clients[j]->client_socket << std::endl;
					// 	this->response(this->pfds[i], pfds[i].fd);
					// 	// break ;
					// }
				}
				if (this->remove == 0 && this->pfds[i].revents & (POLLNVAL | POLLHUP | POLLERR))
				{
					for (size_t j = 0; j < clients.size(); j++)
					{
						if (pfds[i].fd == clients[j].client_socket)
						{
							std::cout << "from here" << std::endl;
							this->disconnect(j);
							break ;
						}
					}
					// if (clients[pfds[i].fd] != nullptr)
					// {
					// 	// std::cout << "ready to recv " << clients[j]->client_socket << std::endl;
					// 	this->disconnect(pfds[i].fd);
					// 	// break ;
					// }
				}
				if (this->remove == 1)
				{
					std::cout << "size pfds == " << pfds.size() << std::endl;
					std::cout << "size clients == " << clients.size() << std::endl;
					std::cout << "i == " << i << std::endl;
					// for (size_t i = 0; i < clients.size(); i++)
					// {
					// 	std::cout << "clients[" << i << "] => " << clients[i].client_socket << std::endl;
					// }
					
					// break ;
				}
			}
		}
	}
}

void server::new_connection(int indexPfds, int index)
{
	(void)indexPfds;
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
	// clients[obj.client_socket] = new client(obj);
	// clients.push_back(new client(obj));
	clients.push_back(obj);
	// size_t size = obj.input.tellg();
	// std::cout << "size == " << size << std::endl;
	// this->pfds[indexPfds].revents &= ~POLLIN;
	std::cout << "new client connected " << obj.client_socket << std::endl;
}

void server::disconnect(int index)
{
	// std::cout << "header == " << clients[index].headerOfRequest << std::endl;
	// if (clients[1]->input.is_open())
	// 	std::cout << "is open" << std::endl;
	// clients[index].input.close();
	std::cout << "index == " << index << std::endl;
	std::cout << "client disconnected " << clients[index].client_socket << std::endl;
	// int sock = clients[index].client_socket;
	close(clients[index].client_socket);
	// for (size_t i = 0; i < pfds.size(); i++)
	// {
	// 	if (pfds[i].fd == sock)
	// 	{
	// 		pfds.erase(pfds.begin() + i + servers.size());
	// 		break ;
	// 	}
	// }
	
	// std::cout << "done disconnecting " << std::endl;
	// delete clients[index];
	// clients[index] = nullptr;
	pfds.erase(pfds.begin() + index + servers.size());
	clients[index].ready = -1;
	clients[index].clear();
	clients.erase(clients.begin() + index);
	// std::cout << "client size " << clients.size() << std::endl;
	this->remove = 1;
	// std::cout << "sock == " << clients[0]->client_socket << std::endl;
	// if (clients[0]->input.is_open())
	// 	std::cout << "is open" << std::endl;
	// size_t size = clients[0]->input.tellg();
	// std::cout << "size == " << size << std::endl;
	// std::cout << "response == " << clients[0]->respond.response_req.substr(0, 500) << std::endl;
}

int server::checkLocation(client &ObjClient, serverParse ObjServer)
{
	std::string root;
	std::string locToFind = ObjClient.URI;
	while (!locToFind.empty())
	{
		for (size_t i = 0; i < ObjServer.obj_location.size(); i++)
		{
			if (locToFind.compare(ObjServer.obj_location[i].path) == 0)
			{
				// std::cout << "path => " << ObjServer.obj_location[i].path << std::endl;
				locationParse ObjLocation = ObjServer.obj_location[i];
				if (ObjLocation.root.size() != 0)
					root = ObjLocation.root[1];
				else if (ObjServer.root.size() != 0)
					root = ObjServer.root[1];
				if (root[root.size() - 1] == '/')
					root.pop_back();
				// root = root + ObjClient.URI;
				ObjClient.path = root;
				return (i);
			}
		}
		locToFind = locToFind.substr(0, locToFind.find_last_of('/'));
	}

	size_t i = 0;
	for (;i < ObjServer.obj_location.size(); i++)
	{
		if (ObjServer.obj_location[i].path.compare("/") == 0)
		{
			// std::cout << "path => " << ObjServer.obj_location[i].path << std::endl;
			break ;
		}
	}
	if (i == ObjServer.obj_location.size())
	{
		if (ObjClient.flag != ERROR)
		{
			ObjClient.flag = ERROR;
			ObjClient.respond.type = 1;
			ObjClient.respond.status_code = 400;
			ObjClient.respond.phrase = "Bad Request";
			ObjClient.respond.content = 1;
			ObjClient.respond.body = "Location Not Found";
			ObjClient.respond.close = CLOSE;
		}
		return (-1);
	}
	locationParse ObjLocation = ObjServer.obj_location[i];
	if (ObjLocation.root.size() != 0)
		root = ObjLocation.root[1];
	else if (ObjServer.root.size() != 0)
		root = ObjLocation.root[1];
	if (root[root.size() - 1] == '/')
		root.pop_back();
	ObjClient.path = root;
	// root = root + ObjClient.URI;
	// if (access(root.data(), F_OK) != 0)
	// {
	// 	if (ObjClient.flag != ERROR)
	// 	{
	// 		ObjClient.flag = ERROR;
	// 		ObjClient.respond.type = 1;
	// 		ObjClient.respond.status_code = 400;
	// 		ObjClient.respond.phrase = "Bad Request";
	// 		ObjClient.respond.content = 1;
	// 		ObjClient.respond.body = "Location Not Found";
	// 		ObjClient.respond.close = CLOSE;
	// 	}
	// }
	// else
	return (i);
	// return (-1);
}

serverParse& server::findServerBlock(int index)
{
	std::string headerreq = clients[index].headerOfRequest;
	std::string line;
	std::string host;
	std::string port;
	int pos = headerreq.find("Host: ");
	line = headerreq.substr(pos + 6, headerreq.find('\r', pos + 6) - (pos + 6));
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
	if (clients[index].flag != ERROR)
	{
		clients[index].flag = ERROR;
		clients[index].respond.type = 1;
		clients[index].respond.status_code = 400;
		clients[index].respond.phrase = "Bad Request";
		clients[index].respond.content = 1;
		clients[index].respond.body = "No Server Block Matches With The Host Header";
		clients[index].respond.close = CLOSE;
		return (block[0]);
	}
	return (block[i]);
}

void server::checkMaxBodySize(client &ObjClient, serverParse obj, int loc)
{
	size_t allowedSize = 1048576;
	int i = ObjClient.headerOfRequest.find("Content-Length: ");
	if (i != -1)
	{
		size_t length = std::stoi(ObjClient.headerOfRequest.substr(i + 16,ObjClient.headerOfRequest.size()).c_str());
		if (obj.obj_location[loc].client_max_body_size_)
			allowedSize = obj.obj_location[loc].client_max_body_size;
		else if (obj.client_max_body_size_)
			allowedSize = obj.client_max_body_size;
		std::cout << "allowed == " << allowedSize << std::endl;
		if (length > allowedSize && ObjClient.flag != ERROR)
		{
			ObjClient.flag = ERROR;
			ObjClient.respond.type = 1;
			ObjClient.respond.status_code = 413;
			ObjClient.respond.phrase = "Request Entity Too Large";
			ObjClient.respond.content = 1;
			ObjClient.respond.body = "File Too Big";
			ObjClient.respond.close = CLOSE;
			return ;
		}
	}
}

void server::checkMethodAllowed(client &ObjClient, serverParse obj, int loc)
{
	std::string method;
	locationParse locobj = obj.obj_location[loc];
	if (ObjClient.tmp == POST)
	{
		method = "POST";
		return ;
	}
	if (ObjClient.tmp == GET)
		method = "GET";
	else if (ObjClient.tmp == DELETE)
		method = "DELETE";
	for (size_t i = 1; i < locobj.allow_methods.size(); i++)
	{
		// std::cout << "methods == " << locobj.allow_methods[i] << std::endl;
		if (method.compare(locobj.allow_methods[i]) == 0)
			return;
	}
	if (ObjClient.flag != ERROR)
	{
		ObjClient.flag = ERROR;
		ObjClient.respond.type = 1;
		ObjClient.respond.status_code = 405;
		ObjClient.respond.phrase = "Method Not Allowed";
		ObjClient.respond.content = 1;
		ObjClient.respond.body = "Method " + method + " Not Allowed In This Location";
		ObjClient.respond.close = CLOSE;
		return ;
	}

}

void server::checkRedirection(client &ObjClient, serverParse obj, int loc)
{
	locationParse ObjLocation = obj.obj_location[loc];
	if (ObjLocation.rtn.size() != 0)
	{
		if (ObjLocation.rtn[2][0] == '/')
		{
			if (ObjLocation.rtn[2].compare(ObjLocation.path) == 0)
			{
				ObjClient.flag = ERROR;
				ObjClient.respond.type = 1;
				ObjClient.respond.status_code = 508;
				ObjClient.respond.phrase = "Loop Detected";
				ObjClient.respond.body = "Infinite Redirect";
				ObjClient.respond.content = 1;
				return ;
			}
			else
			{
				for (size_t i = 0; i < obj.obj_location.size(); i++)
				{
					// std::cout << "retuunr -- " << ObjLocation.rtn[2].size() << std::endl;
					// std::cout << "location == " << obj.obj_location[i].path .size() << std::endl;
					if (ObjLocation.rtn[2].compare(obj.obj_location[i].path) == 0)
					{
						ObjClient.redirpath = ObjLocation.rtn[2];
						ObjClient.generateUrl();
						ObjClient.flag = ERROR;
						ObjClient.respond.type = 1;
						ObjClient.respond.status_code = stoi(ObjLocation.rtn[1]);
						if (ObjClient.respond.status_code == 301)
							ObjClient.respond.phrase = "Moved Permanently";
						else
							ObjClient.respond.phrase = "Found";
						ObjClient.respond.headers.push_back("Location: " + ObjClient.redirpath);
						ObjClient.respond.headers.push_back("Cache-Control: no-cache, no-store, must-revalidate");
						ObjClient.respond.headers.push_back("Pragma: no-cache");
						ObjClient.respond.headers.push_back("Expires: 0");
						return ;
					}
				}
				ObjClient.flag = ERROR;
				ObjClient.respond.type = 1;
				ObjClient.respond.status_code = 404;
				ObjClient.respond.phrase = "Not Found";
				ObjClient.respond.body = "Redirect Not Found";
				ObjClient.respond.content = 1;
				return ;
			}
		}
		else
		{
			std::string redirectUrl = ObjLocation.rtn[2] + "/";
			ObjClient.flag = ERROR;
			ObjClient.respond.type = 1;
			ObjClient.respond.status_code = stoi(ObjLocation.rtn[1]);
			if (ObjClient.respond.status_code == 301)
				ObjClient.respond.phrase = "Moved Permanently";
			else
				ObjClient.respond.phrase = "Found";
			ObjClient.respond.headers.push_back("Location: " + redirectUrl);
			ObjClient.respond.headers.push_back("Cache-Control: no-cache, no-store, must-revalidate");
			ObjClient.respond.headers.push_back("Pragma: no-cache");
			ObjClient.respond.headers.push_back("Expires: 0");
		}
		// ObjClient.respond.content = 5;
		// ObjClient.respond.body = responseBody;
		// ObjClient.respond.close = CLOSE;
		return ;
	}
}

void server::GetBehaviour(client &ObjClient, serverParse ObjServer, int loc)
{
	struct stat info;
	std::string root;
	locationParse ObjLocation = ObjServer.obj_location[loc];
	// if (ObjLocation.root.size() != 0)
	// 	root = ObjLocation.root[1];
	// else if (ObjServer.root.size() != 0)
	// 	root = ObjLocation.root[1];
	// if (root[root.size() - 1] == '/')
	// 	root.pop_back();
	// root = root + ObjClient.URI;
	// ObjClient.path = root;
	std::string resourseRequested = ObjClient.URI.substr(ObjLocation.path.size());
	if (resourseRequested[0] == '/')
		resourseRequested = resourseRequested.substr(1);
	std::cout << "requested  %" << resourseRequested << "%" << std::endl;
	root = ObjClient.path + "/" + resourseRequested;
	std::cout << "[athhhhh ]= = " << root << std::endl;
	if (access(root.data(), F_OK) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = NOTFOUND;
		return ;
	}

    if (stat(root.data(), &info) != 0)
	{
        std::cerr << "Error: Unable to stat file/directory.\n";
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = INTERNALERR;
        return ;
    }
	if (S_ISDIR(info.st_mode))
	{
        std::cout << root << " is a directory.\n";
		if (root[root.size() - 1] == '/')
		{
			if (ObjLocation.index.size() != 0)
			{
				for (size_t i = 1; i < ObjLocation.index.size() ; i++) {
					std::string testPath = root + ObjLocation.index[i];
					if (access(testPath.data(), F_OK) == 0)
					{
						ObjClient.respond.ready = 1;
						// ObjClient.respond.flagResponse = OPFILE;
						ObjClient.respond.flagResponse = REDIRECT;
						// ObjClient.path = testPath;
						std::string path = ObjLocation.path;
						if (path[path.size() - 1] == '/')
							path.pop_back();
						ObjClient.redirpath = path + "/" + ObjLocation.index[i];
						return;
					}
				}
			}
			if (ObjLocation.autoindex.size() != 0)
			{
				if (ObjLocation.autoindex[1].compare("off") == 0)
				{
					ObjClient.respond.ready = 1;
					ObjClient.respond.flagResponse = FORBIDEN;
					return ;
				}
				else
				{
					ObjClient.respond.ready = 1;
					ObjClient.listPath = ObjClient.path  + "/";
					std::string opfile = ObjClient.URI.substr(ObjLocation.path.size());
					if (opfile[0] == '/')
						opfile = opfile.substr(1);
					ObjClient.listPath += opfile;
					ObjClient.respond.flagResponse = AUTOINDEX;
					return ;
				}
			}
			if (ObjLocation.autoindex.size() == 0)
			{
				ObjClient.respond.ready = 1;
				ObjClient.respond.flagResponse = FORBIDEN;
				return;
			}

		}
		else
		{
			ObjClient.respond.ready = 1;
			ObjClient.respond.flagResponse = REDIRECT;
			ObjClient.redirpath = ObjClient.URI + "/";
			return;
		}
    }
	else if (S_ISREG(info.st_mode))
	{
        // std::cout << root << " is a file.\n";
		// std::cout << "sock === " << ObjClient.client_socket << std::endl;
		if (!ObjClient.input.is_open())
		{
			std::cout << "inside check\n";
			ObjClient.respond.ready = 1;
			ObjClient.path = root;
			std::cout << "file path == " << ObjClient.path << std::endl;
			ObjClient.respond.flagResponse = OPFILE;
			return ;
		}
		else
		{
			std::cout << "flag == " << ObjClient.respond.flagResponse << std::endl;
			std::cout << "ready === " << ObjClient.respond.ready << std::endl;
			std::cout << "file path == " << ObjClient.path << std::endl;
			std::cout << "size == " << ObjClient.input.tellg() << std::endl;
			if (ObjClient.input.is_open())
				std::cout << "input is opennnn " << std::endl;
			std::cout << "client sock == " << ObjClient.client_socket << std::endl;
			std::cout << ObjClient.headerOfRequest << std::endl;
			std::cout << "response == " << ObjClient.respond.response_req.substr(0, 500) << std::endl;
			exit(1);
			ObjClient.respond.ready = 1;
		}
	}
	// else
	// {
	// 	ObjClient.respond.ready = 1;
	// 	ObjClient.respond.flagResponse = INTERNALERR;
    //     return ;
	// }
	// std::cout << "before input check \n";
	std::cout << "after input check \n";
	// std::cout << "root == "<< root << std::endl;
}

void server::PostBehaviour(client &ObjClient, serverParse ObjServer, int loc)
{
	struct stat info;
	locationParse ObjLocation = ObjServer.obj_location[loc];
	std::string root;
	ObjClient.uploadPath = "/Users/hmoubal/Desktop/webserv/upload";
	if (ObjLocation.client_body_temp_path.size() != 0)
		ObjClient.uploadPath = ObjLocation.client_body_temp_path[1];
	for (size_t i = 1; i < ObjLocation.allow_methods.size(); i++)
	{
		if (ObjLocation.allow_methods[i].compare("POST") == 0)
		{
			std::cout << "upload sec " << std::endl;
			ObjClient.postMethod();
			return;
		}
	}
	if (ObjLocation.root.size() != 0)
		root = ObjLocation.root[1];
	else if (ObjServer.root.size() != 0)
		root = ObjServer.root[1];
	if (root[root.size() - 1] == '/')
		root.pop_back();
	std::string resourseRequested = ObjClient.URI.substr(ObjLocation.path.size());
	if (resourseRequested[0] == '/')
		resourseRequested = resourseRequested.substr(1);
	root = root + "/" + resourseRequested;
	std::cout << "root === " << root << std::endl;
	if (access(root.data(), F_OK) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = NOTFOUND;
		return ;
	}
    if (stat(root.data(), &info) != 0)
	{
        std::cerr << "Error: Unable to stat file/directory.\n";
        return ;
    }
	if (S_ISDIR(info.st_mode))
	{
        std::cout << root << " is a directory.\n";
		if (root[root.size() - 1] == '/')
		{
			if (ObjLocation.index.size() != 0)
			{
				for (size_t i = 1; i < ObjLocation.index.size() ; i++) {
					std::string testPath = root + ObjLocation.index[i];
					if (access(testPath.data(), F_OK) == 0)
					{
						ObjClient.respond.ready = 1;
						// ObjClient.respond.flagResponse = OPFILE;
						ObjClient.respond.flagResponse = CGI;
						// ObjClient.path = testPath;
						// ObjClient.redirpath = "/" + ObjLocation.index[i];
						return;
					}
				}
			}
			if (ObjLocation.index.size() == 0)
			{
				ObjClient.respond.ready = 1;
				ObjClient.respond.flagResponse = FORBIDEN;
			}
		}
		else
		{
			ObjClient.respond.ready = 1;
			ObjClient.respond.flagResponse = REDIRECT;
			ObjClient.redirpath = ObjClient.URI + "/";
			return;
		}
    }
	else if (S_ISREG(info.st_mode))
	{
		ObjClient.respond.ready = 1;
		// ObjClient.respond.flagResponse = OPFILE;
		ObjClient.respond.flagResponse = CGI;
		// ObjClient.path = testPath;
		// ObjClient.red = "/" + ;
		return;
	}
}

int server::DeleteAllContent(std::string path)
{
	std::cout << "path to delete1 == " << path << std::endl;
    DIR* folder = opendir(path.data());
    if (!folder) {
        // handle error
        return(1);
    }
    struct dirent* entry;
    while ((entry = readdir(folder)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            // ignore . and .. entries
            continue;
        }
        // char entry_path[1024];
        // snprintf(entry_path, sizeof(entry_path), "%s%s", path.data(), entry->d_name);
		std::string newPath = path + entry->d_name;
		std::cout << "newpath == " << newPath << std::endl;
        if (entry->d_type == DT_DIR) {
            // recursively delete subfolder
            DeleteAllContent(newPath);
        } else {
            if (std::remove(newPath.data()) != 0)
				return (1);
        }
    }
	std::cout << "path to delete2 == " << path << std::endl;
	if (rmdir(path.data()) == -1)
		return (1);
    closedir(folder);
	return (0);
}

int server::DeleteFile(std::string path)
{
	int i = std::remove((char *)path.data());
	if (i != 0)
		return (1);
	return (0);
}

void server::DeleteBehaviour(client &ObjClient, serverParse ObjServer, int loc)
{
	struct stat info;
	locationParse ObjLocation = ObjServer.obj_location[loc];
	std::string root;
	if (ObjLocation.root.size() != 0)
		root = ObjLocation.root[1];
	else if (ObjServer.root.size() != 0)
		root = ObjServer.root[1];
	if (root[root.size() - 1] == '/')
		root.pop_back();
	std::string resourseRequested = ObjClient.URI.substr(ObjLocation.path.size());
	if (resourseRequested[0] == '/')
		resourseRequested = resourseRequested.substr(1);
	root = root + "/" + resourseRequested;
	std::cout << "root === " << root << std::endl;
	if (access(root.data(), F_OK) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = NOTFOUND;
		return ;
	}
    if (stat(root.data(), &info) != 0)
	{
        std::cerr << "Error: Unable to stat file/directory.\n";
        return ;
    }
	if (S_ISDIR(info.st_mode))
	{
        std::cout << root << " is a directory.\n";
		if (root[root.size() - 1] == '/')
		{
			ObjClient.respond.ready = 1;
			if (DeleteAllContent(root) == 0)
				ObjClient.respond.flagResponse = DELETED;
			else
			{
				if (access(root.data(), W_OK) == 0)
					ObjClient.respond.flagResponse = INTERNALERR;
				else
					ObjClient.respond.flagResponse = FORBIDEN;
			}
			return ;
		}
		else
		{
			ObjClient.respond.ready = 1;
			ObjClient.respond.flagResponse = CONFLICT;
			// ObjClient.redirpath = ObjClient.URI + "/";
			return;
		}
    }
	else if (S_ISREG(info.st_mode))
	{
		ObjClient.respond.ready = 1;
		if (DeleteFile(root) == 0)
			ObjClient.respond.flagResponse = DELETED;
		else
			ObjClient.respond.flagResponse = INTERNALERR;
		// ObjClient.redirpath = ObjClient.URI + "/";
		return;
	}
}

void server::response(struct pollfd &pfds, int index)
{
	int loc = -1;
	serverParse objServer = findServerBlock(index);
	if (clients[index].respond.ready != 1)
	{
		if (clients[index].flag != ERROR)
			loc = checkLocation(clients[index], objServer);
		if (loc != -1 && clients[index].flag != ERROR)
			checkMaxBodySize(clients[index], objServer, loc);
		if (loc != -1 && clients[index].flag != ERROR)
			checkRedirection(clients[index], objServer, loc);
		if (loc != -1 && clients[index].flag != ERROR)
			checkMethodAllowed(clients[index], objServer, loc);
	}
	if (clients[index].flag == ERROR)
	{
		std::cout << "ERROR" << std::endl;
		clients[index].respond.generate_response();
		if (clients[index].respond.send_response(clients[index], pfds) == 1)
		{
			std::cout << "from error response "<< std::endl;
			this->disconnect(index);
		}
		return;
	}
	// std::cout << "respond ready == " << clients[index].respond.ready << " sock == " << clients[index].client_socket << std::endl;
	if (clients[index].tmp == GET && clients[index].respond.ready != 1)
	{
		// std::cout << "GEt method == " << clients[index].client_socket << std::endl;
		// std::cout << "headers == " << clients[index].headerOfRequest << std::endl;
		this->GetBehaviour(clients[index], objServer, loc);
	}
	else if (clients[index].tmp == POST && clients[index].respond.ready != 1)
	{
		std::cout << "POST method" << std::endl;
		this->PostBehaviour(clients[index], objServer, loc);
		// if (clients[index].postMethod(pfds) == CLOSE)
		// 	this->disconnect(index);
	}
	else if (clients[index].tmp == DELETE && clients[index].respond.ready != 1)
	{
		std::cout << "DELETE method" << std::endl;
		this->DeleteBehaviour(clients[index], objServer, loc);
		// if (clients[index].deleteMethod(pfds) == CLOSE)
		// 	this->disconnect(index);
	}
	// std::cout << "respond2 ready == " << clients[index].respond.ready << " sock == " << clients[index].client_socket << std::endl;
	if (clients[index].respond.ready == 1)
	{
		clients[index].initResponse();
		clients[index].respond.generate_response();
		if (clients[index].respond.send_response(clients[index], pfds) == 1)
		{
			std::cout << "from response " << std::endl;
			this->disconnect(index);
		}
	}
}

void server::receive(int pfds_index, int index)
{
    int rtn;
	(void)pfds_index;
	// int t;
	// std::cout << "buffer before " << clients[index].buffer << std::endl;
    rtn = clients[index].pushToBuffer();
	// std::cout << "buffer after " << clients[index].buffer << std::endl;

	// t = rtn;
	// if (rtn == -1)
	// {
	// 	//std::cout << "socket client " << clients[index].client_socket << std::endl;
	// 	//std::cout << clients[index].headerOfRequest << std::endl;
	// 	return ;
	// }
    if(rtn == 0 || rtn == -1)
	{
		std::cout << "r == " << rtn << " socket client " << clients[index].client_socket << std::endl;
		//std::cout << clients[index].headerOfRequest << std::endl;
		std::cout << "from recv " << std::endl;
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
			// pfds[pfds_index].revents &= ~POLLIN;
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
			// pfds[pfds_index].revents &= ~POLLIN;
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
		std::cout << "here get method " << clients[index].client_socket << std::endl;
		// std::cout << "headers == " << clients[index].headerOfRequest << std::endl;
		// std::cout << "flag == " << clients[index].flag << std::endl;
		std::cout << "size == " << clients[index].buffer.size() << std::endl;
		std::cout << "buffer == " << clients[index].buffer << std::endl;
		clients[index].check();
		// pfds[pfds_index].revents &= ~POLLIN;
		// std::cout << "ready -- " << clients[index].ready << std::endl;
		return ;
		// without budy => GET method
	}
	else if(clients[index].flag == DELETE)
	{
		clients[index].check();
		// pfds[pfds_index].revents &= ~POLLIN;
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
			// pfds[pfds_index].revents &= ~POLLIN;
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
			// pfds[pfds_index].revents &= ~POLLIN;
		}
	}
	else if (clients[index].flag == ERROR)
	{
		clients[index].check();
		// pfds[pfds_index].revents &= ~POLLIN;
	}

    // //std::cout << "out of recv" << std::endl;
    // return 1;
}

