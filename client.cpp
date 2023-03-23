#include "client.hpp"

int client::extractheader()
{
	size_t res = 0;
	size_t pos = 0;

	res = buffer.find("/favicon.ico");
	if (res != std::string::npos)
	{
		std::cout << "lol\n";
		this->ignore = 1;
		// buffer.clear();
		return (0);
	}
	// std::cout << "extract sock " << this->client_socket << std::endl;
		// std::cout << "lol" << std::endl;
	if (this->headerOfRequest.empty() == true)
	{
		while (pos < buffer.size())
		{
			res = buffer.find("\r\n", pos);
			if (res != std::string::npos)
			{
				if (buffer[res] == '\r' && buffer[res + 1] == '\n')
				{
					res += 2;
					if (buffer[res] == '\r' && buffer[res + 1] == '\n')
					{
						headerOfRequest = buffer.substr(0, res + 1);
						// std::cout << "extracted header "<< this->client_socket << std::endl;
						// std::cout << header_request << std::endl;
						// std::cout << "end header" << std::endl;
						buffer.erase(0, res + 2);
						break ;
					}
					else
						pos = res;
				}
			}
			else
				pos += 1;
		}
	}
	return (0);
}

void client::openfile()
{
	size_t res = 0;
	res = headerOfRequest.find("Content-Type");
	if (res != std::string::npos)
	{
		if (!file.is_open())
		{
			res = headerOfRequest.find("pdf");
			if (res != std::string::npos)
				file.open("file.pdf");
			res = headerOfRequest.find("mp4");
			if (res != std::string::npos)
				file.open("file.mp4");
			res = headerOfRequest.find("png");
			if (res != std::string::npos)
				file.open("file.png");
			res = headerOfRequest.find("text");
			if (res != std::string::npos)
				file.open("file");
		}
		if (!file.is_open())
			file.open("data");
	}
}

int client::response()
{
	char c;

	if (file.is_open())
	{
		std::cout << "lol1 " << client_socket << std::endl;
		file << buffer;
		file.flush();
	}
	else if (!file.is_open() && headerOfRequest.empty() == false)
	{
		std::cout << "lol2 " << headerOfRequest << std::endl;
		if (!input.is_open())
		{
			input.open("../test/test.pdf");
			if (!input.is_open())
			{
				std::cout << "couldn't open file" << std::endl;
				return (1);
			}
			while (input.get(c))
				content_buffer.push_back(c);
			response_header = "HTTP/1.1 200 OK\r\n"
							"Content-Type: application/pdf\r\n"
							"Content-length: " + std::to_string(content_buffer.size()) + "\r\n"
							"\r\n";
			// std::cout << "buffer begin " << this->client_socket << std::endl;
			// std::cout << buffer << std::endl;
			// std::cout << "buffer end" << std::endl;
			// std::cout << "header begin " << this->client_socket << std::endl;
			// std::cout << headerOfRequest << std::endl;
			// std::cout << "header end" << std::endl;
			// std::cout << "ignore == " << ignore << std::endl;
			response_header += std::string(content_buffer.begin(), content_buffer.end());
		}
		// size_t size = 100000;
		if (!response_header.empty())
		{
			// if (response_header.size() < size)
			// 	size = response_header.size();
			// std::string send_req = response_header.substr(0, size);
			// int i = send(this->client_socket, send_req.c_str(), send_req.size(), 0);
			// response_header.erase(0, i);
			int i = send(this->client_socket, response_header.c_str(), response_header.size(), 0);
			if (i < 0)
			{
				response_header.clear();
				return (0);
			}
			response_header.erase(0, i);
			std::cout << "i == " << i  << " socket == "  << this->client_socket << std::endl;
		}
			else
			{ 
				std::cout << "sent complete " << this->client_socket << std::endl;
				input.close();
				buffer.clear();
				headerOfRequest.clear();
				content_buffer.clear();
				return (0);
			}
	}
	return (0);
}

client::client()
{
	flag = 0;
	tmp = 0;
	i = 0;
	ignore = 0;
	flag_ = 0;
}

client::client(const client &obj)
{
	*this = obj;
}

client& client::operator=(const client& obj)
{
	if (this != &obj)
	{
		this->client_socket = obj.client_socket;
		this->buffer = obj.buffer;
		this->headerOfRequest = obj.headerOfRequest;
		this->bytes_read = obj.bytes_read;
		this->flag = obj.flag;
		this->headerOfRequest = obj.headerOfRequest;
		this->bodyParss = obj.bodyParss;
		this->tmp = obj.tmp;
		this->i = obj.i;
		this->ContentLength = obj.ContentLength;
	}
	return (*this);
}

client::~client()
{
}

int client::checkHeaderOfreq()
{
    int pos = 0;
    
    while (buffer[pos] && flag == 0)// for entring one time
    { 
        if(buffer[pos] == '\r' && buffer[pos + 1] == '\n')
        {
            pos += 2;
            if(buffer[pos] == '\r' && buffer[pos + 1] == '\n' )
            {
                headerOfRequest = buffer.substr(0,pos - 1);// not include \r\n
                if(headerParss.checkHeaderOfreq_(headerOfRequest,tmp) == -2)
                    return -2;
                
                pos = headerOfRequest.find("Content-Length");  
                if(pos != -1)
                {
                    ContentLength = ft_atoi(headerOfRequest.substr(pos + 16,headerOfRequest.size()).c_str());
                    if(ContentLength == 0)
                        return -2;
                    flag = 1;
                    i = headerOfRequest.size();
                    return  1;
                }
                else
                {
                    // without body
                    flag = 2;
                    return 1;
                }
            }
            --pos;
        }
        pos++;
    }
    // in entring second times
    if(flag == 1 || flag == 2 || flag == 3 || flag == 4)
        return 1;
    else
        return -2;
}

long long	client::ft_atoi(const char *str)
{
	long long	res;
	long long	negative;

	negative = 1;
	res = 0;
	while (*str && (*str == ' ' || *str == '\n' || *str == '\t' ||
			*str == '\v' || *str == '\f' || *str == '\r'))
		++str;
	if (*str == '-')
		negative = -1;
	if (*str == '-' || *str == '+')
		++str;
	while (*str && *str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - 48);
		++str;
	}
	return (res * negative);
}
  
char * client::ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	str = (char*)malloc(sizeof(*s) * (len - start + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && j < len - start)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = 0;
	return (str);
}  

int client::pushToBuffer()
{
    char data[BUFFER];
    bzero(data, BUFFER);
    this->bytes_read = recv(client_socket, &data, BUFFER, 0);

    if(bytes_read == -1)
        return -1;
    if(bytes_read == 0)
        return 0;
    int j = 0; 
    
    while (j < bytes_read)
    {
        buffer.push_back(data[j]);
        j++;
    }
    return 1;
}
