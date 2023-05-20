#include "server.hpp"

void server::fillRevMimeType()
{
	std::map<std::string, std::string >::iterator it = mimeTypes.begin();
	for (; it != mimeTypes.end(); it++)
		mimeTypes_.insert(std::make_pair(it->second, it->first));
}

void server::fillMimeType()
{
	mimeTypes.insert(std::make_pair(".txt", "text/plain"));
	mimeTypes.insert(std::make_pair(".htm", "text/html"));
	mimeTypes.insert(std::make_pair(".html", "text/html"));
	mimeTypes.insert(std::make_pair(".css", "text/css"));
	mimeTypes.insert(std::make_pair(".js", "application/javascript"));
	mimeTypes.insert(std::make_pair(".json", "application/json"));
	mimeTypes.insert(std::make_pair(".xml", "application/xml"));
	mimeTypes.insert(std::make_pair(".pdf", "application/pdf"));
	mimeTypes.insert(std::make_pair(".zip", "application/zip"));
	mimeTypes.insert(std::make_pair(".jpeg", "image/jpeg"));
	mimeTypes.insert(std::make_pair(".jpg", "image/jpeg"));
	mimeTypes.insert(std::make_pair(".png", "image/png"));
	mimeTypes.insert(std::make_pair(".gif", "image/gif"));
	mimeTypes.insert(std::make_pair(".mp4", "video/mp4"));
	mimeTypes.insert(std::make_pair(".mp3", "audio/mpeg"));
	mimeTypes.insert(std::make_pair(".mpeg", "audio/mpeg"));
	// mimeTypes.insert(std::make_pair(".sh", "application/x-sh"));
	mimeTypes.insert(std::make_pair(".webm", "video/webm"));
	mimeTypes.insert(std::make_pair(".webp", "video/webp"));
	mimeTypes.insert(std::make_pair(".xhtml", "application/xhtml+xml"));
	mimeTypes.insert(std::make_pair(".xls", "application/vnd.ms-excel"));
	mimeTypes.insert(std::make_pair(".md", "text/markdown"));
	mimeTypes.insert(std::make_pair("", "application/octet-stream"));
}

void server::fillStatusCode()
{
	StatusPhrase.insert(std::make_pair(100,"Continue"));
	StatusPhrase.insert(std::make_pair(101,"Switching Protocols"));


	StatusPhrase.insert(std::make_pair(200,"OK"));
	StatusPhrase.insert(std::make_pair(201,"Created"));
	StatusPhrase.insert(std::make_pair(202,"Accepted"));
	StatusPhrase.insert(std::make_pair(203,"Non-Authoritative Information"));
	StatusPhrase.insert(std::make_pair(204,"No Content"));
	StatusPhrase.insert(std::make_pair(205,"Reset Content"));
	StatusPhrase.insert(std::make_pair(206,"Partial Content"));


	StatusPhrase.insert(std::make_pair(300,"Multiple Choices"));
	StatusPhrase.insert(std::make_pair(301,"Moved Permanently"));
	StatusPhrase.insert(std::make_pair(302,"Found"));
	StatusPhrase.insert(std::make_pair(303,"See Other"));
	StatusPhrase.insert(std::make_pair(304,"Not Modified"));
	StatusPhrase.insert(std::make_pair(305,"Use Proxy"));
	StatusPhrase.insert(std::make_pair(306,"(Unused)"));
	StatusPhrase.insert(std::make_pair(307,"Temporary Redirect"));


	StatusPhrase.insert(std::make_pair(400,"Bad Request"));
	StatusPhrase.insert(std::make_pair(401,"Unauthorized"));
	StatusPhrase.insert(std::make_pair(402,"Payment Required"));
	StatusPhrase.insert(std::make_pair(403,"Forbidden"));
	StatusPhrase.insert(std::make_pair(404,"Not Found"));
	StatusPhrase.insert(std::make_pair(405,"Method Not Allowed"));
	StatusPhrase.insert(std::make_pair(406,"Not Acceptable"));
	StatusPhrase.insert(std::make_pair(407,"Proxy Authentication Required"));
	StatusPhrase.insert(std::make_pair(408,"Request Timeout"));
	StatusPhrase.insert(std::make_pair(409,"Conflict"));
	StatusPhrase.insert(std::make_pair(410,"Gone"));
	StatusPhrase.insert(std::make_pair(411,"Length Required"));
	StatusPhrase.insert(std::make_pair(412,"Precondition Failed"));
	StatusPhrase.insert(std::make_pair(413,"Request Entity Too Large"));
	StatusPhrase.insert(std::make_pair(414,"Request-URI Too Long"));
	StatusPhrase.insert(std::make_pair(415,"Unsupported Media Type"));
	StatusPhrase.insert(std::make_pair(416,"Requested Range Not Satisfiable"));
	StatusPhrase.insert(std::make_pair(417,"Expectation Failed"));




	StatusPhrase.insert(std::make_pair(500,"Internal Server Error"));
	StatusPhrase.insert(std::make_pair(501,"Not Implemented"));
	StatusPhrase.insert(std::make_pair(502,"Bad Gateway"));
	StatusPhrase.insert(std::make_pair(503,"Service Unavailable"));
	StatusPhrase.insert(std::make_pair(504,"Gateway Timeout"));
	StatusPhrase.insert(std::make_pair(505,"HTTP Version Not Supported"));
	StatusPhrase.insert(std::make_pair(508,"Loop Detected"));
}

void server::fillEncoding()
{
	PercentEncoding.insert(std::make_pair("%3A",":"));
	PercentEncoding.insert(std::make_pair("%2F","/"));
	PercentEncoding.insert(std::make_pair("%3F","?"));
	PercentEncoding.insert(std::make_pair("%23","#"));
	PercentEncoding.insert(std::make_pair("%5B","["));
	PercentEncoding.insert(std::make_pair("%5D","]"));
	PercentEncoding.insert(std::make_pair("%40","@"));
	PercentEncoding.insert(std::make_pair("%21","!"));
	PercentEncoding.insert(std::make_pair("%24","$"));
	PercentEncoding.insert(std::make_pair("%26","&"));
	PercentEncoding.insert(std::make_pair("%27","'"));
	PercentEncoding.insert(std::make_pair("%28","("));
	PercentEncoding.insert(std::make_pair("%29",")"));
	PercentEncoding.insert(std::make_pair("%2A","*"));
	PercentEncoding.insert(std::make_pair("%2B","+"));
	PercentEncoding.insert(std::make_pair("%2C",","));
	PercentEncoding.insert(std::make_pair("%3B",";"));
	PercentEncoding.insert(std::make_pair("%3D","="));
	PercentEncoding.insert(std::make_pair("%25","%"));
	PercentEncoding.insert(std::make_pair("%20"," "));
}



server::server()
{
	this->remove = 0;
	this->poll_count = 0;
	this->fillEncoding();
	this->fillStatusCode();
	this->fillMimeType();
	this->fillRevMimeType();
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
		// std::cout << "host == " << bind_info[i].host << std::endl;
		obj.address.sin_addr.s_addr = getAdrress(bind_info[i].host).sin_addr.s_addr;
		for (size_t j = 0; j < bind_info[i].ports.size(); j++)
		{
			// std::cout << "ports == " << bind_info[i].ports[j] << std::endl;
			obj.port = std::stoi(bind_info[i].ports[j]);
			obj.str_port = bind_info[i].ports[j];
			obj.address.sin_family = AF_INET;
			obj.address.sin_port = htons(obj.port);
			obj.addrlen = sizeof(obj.address);
			servers.push_back(obj);
		}
	}
}

void server::check_servers(std::vector<miniserver>::iterator it)
{
	if (it == servers.end())
		return ;
	if (it->fail == -1)
	{
		it = servers.erase(it);
		check_servers(it);
	}
	else
	{
		it++;
		check_servers(it);
	}
}

void server::lunch_servers()
{
	struct pollfd pfd;
	for (size_t i = 0; i < servers.size(); i++)
	{
		if ((servers[i].socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			// perror("socket failed");
			servers[i].fail = -1;
			continue;
		}
		if (setsockopt(servers[i].socket_server, SOL_SOCKET, SO_REUSEADDR, &(servers[i].opt), sizeof(servers[i].opt)))
		{
			// perror("setsockopt");
			servers[i].fail = -1;
			continue;
		}
		if (bind(servers[i].socket_server, (struct sockaddr*)&(servers[i].address), servers[i].addrlen) < 0)
		{
			// perror("bind failed");
			servers[i].fail = -1;
			continue;
		}
		fcntl(servers[i].socket_server, F_SETFL, O_NONBLOCK);
		if (listen(servers[i].socket_server, BACKLOG) < 0)
		{
			// perror("listen");
			servers[i].fail = -1;
			continue;
		}
		pfd.fd = servers[i].socket_server;
		pfd.events = POLLIN;
		pfds.push_back(pfd);
	}
	check_servers(servers.begin());
	if (servers.size() == 0)
		throw(std::runtime_error("Can't Bind To A Single Server"));
}

int server::timeoutCheck()
{
	clock_t hangtime;
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].flagTimeout != -1)
		{
			hangtime = clock() - clients[i].timeout;
			if (((float)hangtime / CLOCKS_PER_SEC) > 5)
				return (i);
		}
	}
	return (-1);
}

void server::ClientHang(int index)
{
	for (size_t i = 0; i < pfds.size(); i++)
	{
		if (clients[index].client_socket == pfds[i].fd)
		{
			clients[index].respond.ready = 1;
			clients[index].ready = 1;
			clients[index].respond.flagResponse = TIMEOUT;
			return ;
		}
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
			for (size_t i = 0; i < this->pfds.size(); i++)
			{
				int cli = timeoutCheck();
				if (cli != -1)
					ClientHang(cli);
				if (this->remove == 0 && this->pfds[i].revents & POLLIN)
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
						if (pfds[i].fd == clients[j].client_socket && clients[j].ready == 0)
						{
							// std::cout << "ready to recv " << clients[j].client_socket << std::endl;
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
							// std::cout << "ready to send " <<  pfds[i].fd << std::endl;
							this->response(this->pfds[i], j);
							break ;
						}
					}
				}
				if (this->remove == 0 && this->pfds[i].revents & (POLLNVAL | POLLHUP | POLLERR))
				{
					for (size_t j = 0; j < clients.size(); j++)
					{
						if (pfds[i].fd == clients[j].client_socket)
						{
							// // std::cout << "from here" << std::endl;
							this->disconnect(j);
							break ;
						}
					}
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
		return ;
	}
	c.fd = obj.client_socket;
	fcntl(obj.client_socket, F_SETFL, O_NONBLOCK);
	c.events = POLLIN | POLLOUT;
	pfds.push_back(c);
	clients.push_back(obj);
	// // std::cout << "new client connected " << obj.client_socket << std::endl;
}

void server::disconnect(int index)
{
	// // std::cout << "client disconnected " << clients[index].client_socket << std::endl;
	close(clients[index].client_socket);
	pfds.erase(pfds.begin() + index + servers.size());
	clients[index].clear();
	clients.erase(clients.begin() + index);
	this->remove = 1;
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
				locationParse ObjLocation = ObjServer.obj_location[i];
				if (ObjLocation.root.size() != 0)
					root = ObjLocation.root[1];
				else if (ObjServer.root.size() != 0)
					root = ObjServer.root[1];
				if (root[root.size() - 1] == '/')
					root.pop_back();
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
			break ;
	}
	if (i == ObjServer.obj_location.size())
	{
		ObjClient.respond.flagResponse = BADREQUEST;
		ObjClient.respond.ready = 1;
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
	return (i);
}

serverParse& server::findServerBlock(int index)
{
	std::string headerreq = clients[index].headerOfRequest;
	std::string line;
	std::string host;
	std::string port;
	if (!headerreq.empty())
	{
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
	}
	if (clients[index].respond.ready != 1)
	{
		clients[index].respond.ready = 1;
		clients[index].respond.flagResponse = BADREQUEST;
	}
	return (block[0]);
}

void server::checkMaxBodySize(client &ObjClient, serverParse obj, int loc)
{
	size_t allowedSize = 1;
	int i = ObjClient.headerOfRequest.find("Content-Length: ");
	if (i != -1)
	{
		size_t length = std::stoi(ObjClient.headerOfRequest.substr(i + 16,ObjClient.headerOfRequest.size()).c_str());
		if (obj.obj_location[loc].client_max_body_size_)
			allowedSize = obj.obj_location[loc].client_max_body_size;
		else if (obj.client_max_body_size_)
			allowedSize = obj.client_max_body_size;
		// std::cout << "allowed == " << allowedSize << std::endl;
		if (length > allowedSize)
		{
			ObjClient.respond.ready = 1;
			ObjClient.respond.flagResponse = TOOLARGE;
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
		if (method.compare(locobj.allow_methods[i]) == 0)
			return;
	}
	ObjClient.respond.ready = 1;
	ObjClient.respond.flagResponse = METHODNOTALLOWED;
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
				ObjClient.respond.ready = 1;
				ObjClient.respond.flagResponse = LOOP;
				return ;
			}
			else
			{
				for (size_t i = 0; i < obj.obj_location.size(); i++)
				{
					if (ObjLocation.rtn[2].compare(obj.obj_location[i].path) == 0)
					{
						ObjClient.respond.ready = 1;
						ObjClient.respond.flagResponse = REDIRECT;
						ObjClient.redirpath = ObjLocation.rtn[2];
						ObjClient.respond.status_code = stoi(ObjLocation.rtn[1]);
						return ;
					}
				}
				ObjClient.respond.ready = 1;
				ObjClient.respond.flagResponse = NOTFOUND;
				return ;
			}
		}
		else
		{
			ObjClient.respond.ready = 1;
			ObjClient.respond.flagResponse = REDIRECT;
			ObjClient.redirpath = ObjLocation.rtn[2];
			ObjClient.respond.status_code = stoi(ObjLocation.rtn[1]);
		}
		return ;
	}
}

int server::checkExtension(std::string pathCgi, locationParse ObjLocation)
{
	int pos = pathCgi.find('.');
	std::string ext;
	if (pos != -1)
	{
		int pos2 = pathCgi.find('?', pos);
		if (pos2 != -1)
			ext = pathCgi.substr(pos,pos2 - pos);
		else
			ext = pathCgi.substr(pos);
		// std::cout << "extentsion gci == " << ext << std::endl;
		if (ext.compare(".py") == 0 || ext.compare(".lua") == 0)
		{
			if (ObjLocation.cgi.size() != 0)
			{
				if (ext.compare(ObjLocation.cgi[1]) == 0)
					return (1);
				else
					return (-1);
			}
			else
				return (-1);
		}
	}
	return (0);
}

std::string server::trim_path(client &ObjClient, locationParse ObjLocation)
{
	std::string root;
	std::string resourseRequested = ObjClient.URI.substr(ObjLocation.path.size());
	if (resourseRequested[0] == '/')
		resourseRequested = resourseRequested.substr(1);
	int pos = resourseRequested.find('?');
	if (pos != -1)
		resourseRequested = resourseRequested.substr(0,pos);
	pos = resourseRequested.find('.');
	if (pos != -1)
	{
		std::string ext;
		int pos2 = resourseRequested.find('/',pos);
		if (pos2 != -1)
			ext = resourseRequested.substr(pos, pos2 - pos);
		else
			ext = resourseRequested.substr(pos);
		if (ext.compare(".php") == 0 || ext.compare(".py") == 0)
			resourseRequested = resourseRequested.substr(0,pos2);
	}
	// std::cout << "resourseRequested == " << resourseRequested << std::endl;
	root = ObjClient.path + "/" + resourseRequested;
	return (root);
}

void server::fillCGI(client &ObjClient, serverParse ObjServer, int loc)
{
	locationParse ObjLocation = ObjServer.obj_location[loc];
	// if (ObjLocation.cgi[1].compare(".lua") == 0)
		ObjClient.obj.executable = ObjLocation.cgi[2];
	// else
	// 	ObjClient.obj.executable = "." + ObjLocation.cgi[2];
	ObjClient.obj.CONTENT_TYPE = "text/html";
	std::string tmp = ObjClient.URI.substr(ObjLocation.path.size());
	int pos = tmp.find("/cgi-bin");
	if (pos != -1)
		ObjClient.obj.REQUEST_URI = tmp.substr(1);
	else
		ObjClient.obj.REQUEST_URI = "cgi-bin" + tmp;
	pos = ObjClient.obj.REQUEST_URI.find(".py");
	if (pos != -1)
	{
		ObjClient.obj.SCRIPT_NAME = ObjClient.obj.REQUEST_URI.substr(0, pos + 3);
	}
	else
	{
		pos = ObjClient.obj.REQUEST_URI.find(".lua");
		if (pos != -1)
			ObjClient.obj.SCRIPT_NAME = ObjClient.obj.REQUEST_URI.substr(0, pos + 4);
	}
	ObjClient.obj.SCRIPT_FILENAME = ObjClient.obj.SCRIPT_NAME;
	if (ObjClient.tmp == GET)
	{
		ObjClient.obj.REQUEST_METHOD = "GET";
		ObjClient.obj.CONTENT_LENGTH = "0";
	}
	else if (ObjClient.tmp == POST)
	{
		ObjClient.obj.REQUEST_METHOD = "POST";
		int ps = ObjClient.headerOfRequest.find("Content-Length: ");
		if (ps != -1)
		{
			int ps2 = ObjClient.headerOfRequest.find('\r', ps);
			std::string length = ObjClient.headerOfRequest.substr(ps + 16, ps2 - (ps + 16));
			ObjClient.obj.CONTENT_LENGTH = length;
		}
		ObjClient.obj.POST_DATA = ObjClient.buffer.substr(ObjClient.headerOfRequest.size() + 2);
	}
	pos = ObjClient.headerOfRequest.find("User-Agent: ");
	if (pos != -1)
	{
		int ps = ObjClient.headerOfRequest.find('\r', pos);
		if (ps != -1)
			ObjClient.obj.HTTP_USER_AGENT = ObjClient.headerOfRequest.substr(pos + 12, ps - (pos + 12));
	}
}

void server::GetBehaviour(client &ObjClient, serverParse ObjServer, int loc)
{
	struct stat info;
	std::string root;
	locationParse ObjLocation = ObjServer.obj_location[loc];
	root = trim_path(ObjClient, ObjLocation);
	// std::cout << "root get == " << root << std::endl;
	if (access(root.data(), F_OK) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = NOTFOUND;
		return ;
	}

    if (stat(root.data(), &info) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = INTERNALERR;
        return ;
    }
	if (S_ISDIR(info.st_mode))
	{
		if (root[root.size() - 1] == '/')
		{
			if (ObjLocation.index.size() != 0)
			{
				for (size_t i = 1; i < ObjLocation.index.size() ; i++) {
					std::string testPath = root + ObjLocation.index[i];
					if (access(testPath.data(), F_OK) == 0)
					{
						ObjClient.respond.ready = 1;
						std::string path = ObjLocation.path;
						if (path[path.size() - 1] == '/')
							path.pop_back();
						ObjClient.redirpath = path + "/" + ObjLocation.index[i];
						int ret = checkExtension(ObjClient.redirpath, ObjLocation);
						if (ret == 1)
						{
							this->fillCGI(ObjClient, ObjServer, loc);
							ObjClient.respond.flagResponse = CGIPRO;
							return;
						}
						else if (ret == -1)
						{
							ObjClient.respond.flagResponse = FORBIDEN;
							return;
						}
						ObjClient.respond.flagResponse = REDIRECT;
						return;
					}
				}
			}
			if (ObjLocation.autoindex.size() != 0)
			{
				if (ObjLocation.autoindex[1].compare("off") == 0)
				{
					for (size_t i = 1; i < ObjServer.index.size(); i++)
					{
						std::string testPath = root + ObjServer.index[i];
						if (access(testPath.data(), F_OK) == 0)
						{
							ObjClient.respond.ready = 1;
							std::string path = ObjLocation.path;
							if (path[path.size() - 1] == '/')
								path.pop_back();
							ObjClient.redirpath = path + "/" + ObjServer.index[i];
							int ret = checkExtension(ObjClient.redirpath, ObjLocation);
							if (ret == 1)
							{
								this->fillCGI(ObjClient, ObjServer, loc);
								ObjClient.respond.flagResponse = CGIPRO;
								return;
							}
							else if (ret == -1)
							{
								ObjClient.respond.flagResponse = FORBIDEN;
								return;
							}
							ObjClient.respond.flagResponse = REDIRECT;
							return;
						}
					}
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
			// // std::cout << "here == " << root << std::endl;
			return;
		}
    }
	else if (S_ISREG(info.st_mode))
	{
		if (!ObjClient.input.is_open())
		{
			ObjClient.respond.ready = 1;
			ObjClient.path = root;
			int ret = checkExtension(ObjClient.path, ObjLocation);
			if (ret == 1)
			{
				this->fillCGI(ObjClient, ObjServer, loc);
				ObjClient.respond.flagResponse = CGIPRO;
				return;
			}
			else if (ret == -1)
			{
				ObjClient.respond.flagResponse = FORBIDEN;
				return;
			}
			ObjClient.respond.flagResponse = OPFILE;
			return ;
		}
	}
}

void server::PostBehaviour(client &ObjClient, serverParse ObjServer, int loc)
{
	struct stat info;
	locationParse ObjLocation = ObjServer.obj_location[loc];
	std::string root;
	ObjClient.uploadPath = "/Users/hmoubal/Desktop/upload";
	if (ObjLocation.client_body_temp_path.size() != 0)
		ObjClient.uploadPath = ObjLocation.client_body_temp_path[1];
	for (size_t i = 1; i < ObjLocation.allow_methods.size(); i++)
	{
		if (ObjLocation.allow_methods[i].compare("POST") == 0)
		{
			ObjClient.postMethod(mimeTypes_, mimeTypes);
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
	if (access(root.data(), F_OK) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = NOTFOUND;
		return ;
	}
    if (stat(root.data(), &info) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = INTERNALERR;
        return ;
    }
	if (S_ISDIR(info.st_mode))
	{
		if (root[root.size() - 1] == '/')
		{
			if (ObjLocation.index.size() != 0)
			{
				for (size_t i = 1; i < ObjLocation.index.size() ; i++) {
					std::string testPath = root + ObjLocation.index[i];
					if (access(testPath.data(), F_OK) == 0)
					{
						ObjClient.respond.ready = 1;
						ObjClient.path = testPath;
						int ret = checkExtension(ObjClient.path, ObjLocation);
						if (ret == 1)
						{
							this->fillCGI(ObjClient, ObjServer, loc);
							ObjClient.respond.flagResponse = CGIPRO;
							return;
						}
						ObjClient.respond.flagResponse = FORBIDEN;
						return;
					}
				}
			}
			ObjClient.respond.flagResponse = NOTFOUND;
			ObjClient.respond.ready = 1;
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
		ObjClient.path = root;
		int ret = checkExtension(ObjClient.path, ObjLocation);
		if (ret == 1)
		{
			this->fillCGI(ObjClient, ObjServer, loc);
			ObjClient.respond.flagResponse = CGIPRO;
			return;
		}
		ObjClient.respond.flagResponse = FORBIDEN;
		return;
	}
}

int server::DeleteAllContent(std::string path)
{
    DIR* folder = opendir(path.data());
    if (!folder) {
        return(1);
    }
    struct dirent* entry;
    while ((entry = readdir(folder)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            // ignore . and .. entries
            continue;
        }
		std::string newPath = path + entry->d_name;
        if (entry->d_type == DT_DIR) {
            DeleteAllContent(newPath);
        } else {
            if (std::remove(newPath.data()) != 0)
				return (1);
        }
    }
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
	if (access(root.data(), F_OK) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = NOTFOUND;
		return ;
	}
    if (stat(root.data(), &info) != 0)
	{
		ObjClient.respond.ready = 1;
		ObjClient.respond.flagResponse = INTERNALERR;
        return ;
    }
	if (S_ISDIR(info.st_mode))
	{
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
		return;
	}
}

void server::response(struct pollfd &pfds, int index)
{
	int loc = -1;
	clients[index].flagTimeout = -1;
	serverParse objServer;
	objServer = findServerBlock(index);
	if (clients[index].respond.ready != 1)
		loc = checkLocation(clients[index], objServer);
	if (loc != -1 && clients[index].respond.ready != 1)
		checkMaxBodySize(clients[index], objServer, loc);
	if (loc != -1 && clients[index].respond.ready != 1)
		checkRedirection(clients[index], objServer, loc);
	if (loc != -1 && clients[index].respond.ready != 1)
		checkMethodAllowed(clients[index], objServer, loc);
	if (clients[index].tmp == GET && clients[index].respond.ready != 1)
		this->GetBehaviour(clients[index], objServer, loc);
	else if (clients[index].tmp == POST && clients[index].respond.ready != 1)
		this->PostBehaviour(clients[index], objServer, loc);
	else if (clients[index].tmp == DELETE && clients[index].respond.ready != 1)
		this->DeleteBehaviour(clients[index], objServer, loc);
	if (clients[index].respond.ready == 1)
	{
		clients[index].initResponse(mimeTypes, objServer.ErrorPages);
		clients[index].respond.generate_response(StatusPhrase);
		int status = clients[index].respond.send_response(clients[index], pfds);
		if (status == 1)
			this->disconnect(index);
	}
}

void server::receive(int pfds_index, int index)
{
    int rtn;
	(void)pfds_index;
	clients[index].flagTimeout = 1;
	clients[index].timeout = clock();
    rtn = clients[index].pushToBuffer();
	if (rtn == -1)
	{
		clients[index].ready = 1;
		clients[index].respond.ready = 1;
		clients[index].respond.flagResponse = INTERNALERR;
		return ;
	}
    if(rtn == 0)
	{
		// std::cout << "from recv " << std::endl;
		this->disconnect(index);
        return ;
	}
    rtn = clients[index].checkHeaderOfreq(PercentEncoding, mimeTypes_);
	if(rtn == -2)
	{
		// std::cout << "gets out from here" << std::endl;
		return ;
	}
	if(clients[index].flag == NONCHUNKED) // if has content lenght
	{
		string test = clients[index].buffer.substr(clients[index].headerOfRequest.size() + 2,clients[index].buffer.size() - clients[index].headerOfRequest.size() + 2);
		if((int)test.size() == clients[index].ContentLength)// finish recivng
			clients[index].check();
		else if ((int)test.size() > clients[index].ContentLength)
		{
			clients[index].respond.flagResponse = BADREQUEST;
			clients[index].respond.ready = 1;
			clients[index].check();
		}
	}
	else if(clients[index].flag == GET)
	{
		clients[index].check();
		return ;
	}
	else if(clients[index].flag == DELETE)
	{
		clients[index].check();
		return ;
	}
	else if(clients[index].flag == CHUNKED)// // handle chunked data when resend request
	{
		int pos = clients[index].buffer.find("\r\n0\r\n\r\n");
		if (pos != -1)
			clients[index].check();
	}
	else if(clients[index].flag == FORM)
	{
		if(clients[index].total_bytes_received < clients[index].ContentLength)// finish recivng
			clients[index].total_bytes_received += clients[index].bytes_read;
		if (clients[index].total_bytes_received >= clients[index].ContentLength)
			clients[index].check();
	}
	else if (clients[index].flag == ERROR)
		clients[index].check();
}

