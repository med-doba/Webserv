#include "miniserver.hpp"

miniserver::miniserver()
{
	this->opt = 1;
	this->fail = 0;
}

miniserver::miniserver(const miniserver &obj)
{
	*this = obj;
}

miniserver& miniserver::operator=(const miniserver& obj)
{
	if (this != &obj)
	{
		this->socket_server = obj.socket_server;
		this->port = obj.port;
		this->str_port = obj.str_port;
		this->address = obj.address;
		this->opt = obj.opt;
		this->addrlen = obj.addrlen;
		this->fail = obj.fail;
	}
	return (*this);
}

miniserver::~miniserver()
{

}
