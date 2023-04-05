#include "client.hpp"

response::response()
{
	type = 0;
	status_code = 0;
	phrase = "";
}

void response::generate_response()
{
	std::string version = "HTTP/1.1";
	std::string del = "\r\n";
	std::string extra = "Content-Length: " + std::to_string(body.size());
	std::string closeheader = "Connection: close";
	std::string content = "Content-Type: text/plain";
	if (response_req.empty())
	{
		std::cout << "enter here" << std::endl;
		if (type == 1)
		{
			response_req = version + " " + std::to_string(status_code);
			response_req += " " + phrase + del;
			response_req += extra + del;
			response_req += content + del;
			if (close == 1)
				response_req += closeheader + del;
			response_req += del;
			if (!body.empty())
				response_req += body;
			std::cout << response_req << std::endl;
		}
	}
}

void response::clear()
{
	response_req.clear();
	status_code = 0;
	type = 0;
	phrase.clear();
	body.clear();
	close = 0;
}

void response::send_response(client &obj, struct pollfd &pfds)
{
	int i;

	i = send(obj.client_socket, response_req.c_str(), response_req.size(), 0);
	if (i < 0)
	{
		std::cout << "error in sending" << std::endl;
		return;
	}
	else if (i == (int)response_req.size())
	{
		std::cout << "sent complete" << std::endl;
		this->clear();
		obj.headerOfRequest.clear();
		obj.buffer.clear();
		obj.ready = 0;
		obj.flag = 0;
		pfds.revents&= ~POLLOUT;
	}
	else if (i < (int)response_req.size())
		response_req.erase(0, i);
}

response::response(const response &obj)
{
	*this = obj;
}

response& response::operator=(const response &obj)
{
	if (this != &obj)
	{
		this->phrase = obj.phrase;
		this->status_code = obj.status_code;
		this->type = obj.type;
		this->close = obj.close;
	}
	return  (*this);
}

response::~response()
{

}
