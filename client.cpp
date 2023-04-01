#include "client.hpp"

int client::response(int pfds_index, std::vector<struct pollfd> &pfds)
{
	// char c;

	if (!input.is_open())
	{
		// std::cout << "lol2 " << std::endl;
		// input.open("../tests/pdf.pdf");
		input.open("../test/bigpdf.pdf");
		if (!input.is_open())
		{
			std::cout << "couldn't open file" << std::endl;
			return (1);
		}
		// while (input.get(c))
		// {
		// 	std::cout << "loop" << std::endl;
		// 	content_buffer.push_back(c);
		// }
		input.seekg(0, std::ios::end);
		size_t size = input.tellg();
		input.seekg(0, std::ios::beg);

		// Reserve space in the buffer
		std::vector<char> content(size);

		// Read the file in chunks
		input.read(&content[0], size);
		response_header = "HTTP/1.1 200 OK\r\n"
						"Content-Type: application/pdf\r\n"
						"Content-length: " + std::to_string(content.size()) + "\r\n"
						"\r\n";
		response_header += std::string(content.begin(), content.end());
	}
	if (!response_header.empty())
	{
		// std::cout << "send chunks" << std::endl;
		int i = send(this->client_socket, response_header.c_str(), response_header.size(), 0);
		if (i < 0)
		{
			std::cout << "error "  << this->client_socket << std::endl;
			std::cout << "ready == " << this->ready << " socket client == " << this->client_socket << std::endl;
			std::cout << this->headerOfRequest << std::endl;
			printf("errno = %d: %s\n", errno, strerror(errno));
			// response_header.clear();
			return (0);
		}
		response_header.erase(0, i);
		// std::cout << "i == " << i  << " socket == "  << this->client_socket << std::endl;
	}
	else
	{ 
		std::cout << "sent complete " << this->client_socket << std::endl;
		std::cout << "ready -- " << this->ready << std::endl;
		std::cout << this->headerOfRequest << std::endl;
		headerOfRequest.clear();
		// content_buffer.clear();
		buffer.clear();
		ready = 0;
		flag = 0;
		pfds[pfds_index].revents &= ~POLLOUT;
		input.close();
		return (0);
	}
	return (0);
}

void client::check(void)
{
	int res;

	res = headerOfRequest.find("/favicon.ico");
	if (res != -1)
		this->ready = 0;
	else
		this->ready = 1;
}

client::client()
{
    total_bytes_received  = 0;
    i  = flag = flag_  = j = 0;
    tmp = 0;
    buffer = bodyofRequest = boundary = "";
    tmp = 0;
	ready = 0;
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
		this->boundary = obj.boundary;
		this->response_header = obj.response_header;
		this->body = obj.body;
		this->headerOfRequest = obj.headerOfRequest;
		this->bodyofRequest = obj.bodyofRequest;
		this->content_buffer = obj.content_buffer;
		this->bytes_read = obj.bytes_read;
		this->flag = obj.flag;
		this->bodyParss = obj.bodyParss;
		this->headerParss = obj.headerParss;
		this->tmp = obj.tmp;
		this->flag_ = obj.flag_;
		this->total_bytes_received = obj.total_bytes_received;
		this->i = obj.i;
		this->j = obj.j;
		this->len = obj.len;
		this->ContentLength = obj.ContentLength;
	}
	return (*this);
}

client::~client()
{
}

int client::checkHeaderOfreq(int &len)
{
    int pos = 0;
    
    while (buffer[pos] && flag == 0)// for entring one time
    { 
        if((buffer[pos] == '\r' || buffer[pos] == '\n') && buffer[pos + 1] == '\n')
        {
            pos += 2;
            if((buffer[pos] == '\r' || buffer[pos] == '\n') && buffer[pos + 1] == '\n' )
            {
                headerOfRequest = buffer.substr(0,pos - 1);// not include \r\n
                if(headerParss.checkHeaderOfreq_(headerOfRequest,tmp) == -2)
                    return -2;
                
                i = headerOfRequest.find("Transfer-Encoding: chunked");   // find way to check if boundry
                if(i != -1)
                { 
					std::cout << "lol" << std::endl;
                    // buffer.erase(buffer.begin(),buffer.begin() + pos + 2);
                    i = pos  + 2;
					
                    len -= i;
					
                    flag = 3;
                    return 1;
                }
                pos = headerOfRequest.find("Content-Length");  
                if(pos != -1)
                {
                    j = headerOfRequest.find("boundary");
                    if(j != -1)
                    {
                        flag = 4;
                        ContentLength = ft_atoi(headerOfRequest.substr(pos + 16,headerOfRequest.size()).c_str());
                        if(ContentLength == 0)
                            return -2;
                        i = headerOfRequest.size() + 3;// after herder
                        bytes_read -= i;
                        tmp = j + 9;
                        char *temp = (char*)buffer.data() + tmp;// because string() dont handle '\r'
                        tmp = 0;
                        while (temp[tmp] != '\r' && temp[tmp] != '\n' && temp[tmp + 1] != '\n')
                            tmp++;
                        boundary.append("--").append(ft_substr(temp,0,tmp));// free boundry and temp?
                        
                        return 1;
                    }
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
    return this->bytes_read;
}
