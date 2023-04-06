#include "client.hpp"

response::response()
{
	type = 0;
	status_code = 0;
	phrase = "";
	version = "HTTP/1.1";
	del = "\r\n";
	closeheader = "Connection: close";
	content = 0;
	contentlength = "Content-Length: ";
	contenttype = "Content-Type: ";
}

void response::defineContentType()
{
	if (content == 1)
		contenttype += "text/plain";
	else if (content == 2)
		contenttype += "application/pdf";
	else if (content == 3)
		contenttype += "image/png";
	else if (content == 4)
		contenttype += "image/jpg";
	else if (content == 5)
		contenttype += "text/html";
	else if (content == 6)
		contenttype += "video/mp4";
}

void response::generate_response()
{
	std::cout << "enter here" << std::endl;
	std::cout << "type == " << type << std::endl;
	if (type == 1)
	{
		response_req = version + " " + std::to_string(status_code);
		response_req += " " + phrase + del;
		if (content != 0)
		{
			response_req += contenttype + del;
			response_req += contentlength + std::to_string(body.size()) + del;
			content = 0;
		}
		if (close == 1)
		{
			response_req += closeheader + del;
			close = 0;
		}
		for (size_t i = 0; i < this->headers.size(); i++)
			response_req += headers[i] + del;
		response_req += del;
		if (!body.empty())
			response_req += body;
		type = 0;
		std::cout << response_req << std::endl;
	}
}

void response::clear()
{
	response_req.clear();
	status_code = 0;
	type = 0;
	phrase.clear();
	headers.clear();
	body.clear();
	close = 0;
	content = 0;
}

int response::send_response(client &obj, struct pollfd &pfds)
{
	int i;

	i = send(obj.client_socket, response_req.c_str(), response_req.size(), 0);
	if (i < 0)
	{
		std::cout << "error in sending" << std::endl;
		return (-1);
	}
	else if (i == (int)response_req.size())
	{
		std::cout << "sent complete" << std::endl;
		this->clear();
		obj.clear();
		pfds.revents&= ~POLLOUT;
		return (1);
	}
	else if (i < (int)response_req.size())
		response_req.erase(0, i);
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