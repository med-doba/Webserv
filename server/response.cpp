#include "client.hpp"

response::response()
{
	type = 0;
	redirectUrl = "";
	status_code = 0;
	contenttype = "";
	version = "HTTP/1.1";
	del = "\r\n";
	closeheader = "Connection: close";
	aliveheader = "Connection: keep-alive";
	flagResponse = -1;
	ready = 0;
	body = "";
	contentlength = "Content-Length: ";
}

void response::generate_response(std::map<int, std::string> statusPhrase)
{
	std::map<int, std::string>::iterator it;
	if (type == 1)
	{
		response_req = version + " " + std::to_string(status_code);
		it = statusPhrase.find(status_code);
		if (it != statusPhrase.end())
			response_req += " " + it->second + del;
		if (close == CLOSE)
			response_req += closeheader + del;
		else if (close == ALIVE)
			response_req += aliveheader + del;
		for (size_t i = 0; i < this->headers.size(); i++)
			response_req += headers[i] + del;
		if (!contenttype.empty())
			response_req += "Content-Type: " + contenttype + del;
		else if (!body.empty())
			response_req += "Content-Type: text/plain" + del;
		response_req += contentlength + std::to_string(body.size()) + del;
		response_req += del;
		// std::cout << response_req << std::endl;
		if (!body.empty())
			response_req += body;
		type = 0;
	}
}

void response::clear()
{
	response_req.clear();
	status_code = 0;
	redirectUrl.clear();
	type = 0;
	contenttype.clear();
	headers.clear();
	body.clear();
	close = 0;
	flagResponse = -1;
	ready = 0;
}

int response::send_response(client &obj, struct pollfd &pfds)
{
	int i = -1;
	int close = 0;
	(void)pfds;

	i = send(obj.client_socket, response_req.c_str(), response_req.size(), 0);
	if (i < 0)
	{
		this->flagResponse = INTERNALERR;
		this->ready = 1;
		this->contenttype.clear();
		return (-1);
	}
	else if (i == (int)response_req.size())
	{
		// std::cout << "sent complete == " << pfds.fd << std::endl;
		close = obj.respond.close;
		obj.clear();
		if (close == CLOSE)
			return (1);
		else
			return (0);
	}
	else if (i < (int)response_req.size())
	{
		response_req.erase(0, i);
		// std::cout << "chunks == " << pfds.fd << std::endl;
	}
	return (0);
}

response::response(const response &obj)
{
	*this = obj;
}

response& response::operator=(const response &obj)
{
	if (this != &obj)
	{
		// this->phrase = obj.phrase;
		this->status_code = obj.status_code;
		this->type = obj.type;
		this->close = obj.close;
		this->body = obj.body;
		this->version = obj.version;
		this->headers = obj.headers;
		this->redirectUrl = obj.redirectUrl;
		this->response_req = obj.response_req;
		this->del = obj.del;
		this->closeheader = obj.closeheader;
		this->aliveheader = obj.aliveheader;
		this->contentlength = obj.contentlength;
		this->contenttype = obj.contenttype;
		this->flagResponse = obj.flagResponse;
		this->ready = obj.ready;
	}
	return  (*this);
}

response::~response()
{

}
