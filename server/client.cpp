#include "client.hpp"

int client::fillBody(std::map<std::string, std::string> mimetypes)
{
	std::string OpFile = this->path;
	size_t pos = OpFile.find_last_of('.');
	std::map<std::string, std::string>::iterator it;
	if (pos != std::string::npos)
	{
		std::string ext = &OpFile[pos];
		it = mimetypes.find(ext);
		if (it != mimetypes.end())
			this->respond.contenttype = it->second;
		else
		{
			it = mimetypes.find(".txt");
			this->respond.contenttype = it->second;
		}
	}
	else
	{
		it = mimetypes.find(".txt");
		this->respond.contenttype = it->second;
	}
	input.open(OpFile , std::ios::binary);
	if (!input.is_open())
	{
		this->respond.ready = 1;
		this->respond.flagResponse = INTERNALERR;
		return (-1);
	}
	std::stringstream ss;
	ss << input.rdbuf();
	input.close();
	std::string file = ss.str();
	this->respond.body = file;
	return (0);
}

int client::generateListing(std::map<std::string, std::string> mimetypes)
{
	std::string listing = this->listPath;
	DIR* directory = opendir(listing.c_str());
	std::string html;
    if (directory == nullptr) {
        // handle error
        return (-1);
    }

    html += "<html><head><title>Directory Listing</title></head><body><h1>Directory Listing</h1><ul>";

    dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
        // Ignore . and .. directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        html += "<li><a href=\"";
		if (this->URI[this->URI.size() - 1] == '/')
			html += this->URI;
		else
			html += this->URI + "/" ;
		html += entry->d_name;
        html += "\">";
        html += entry->d_name;
        html += "</a></li>";
    }

    html += "</ul></body></html>";

    closedir(directory);
	this->respond.body = html;
	std::map<std::string, std::string>::iterator it;
	it = mimetypes.find(".html");
	if (it != mimetypes.end())
		this->respond.contenttype = it->second;
	// std::cout << "body -- " << html << std::endl;
	return (0);
}

void client::generateErrPage(std::map<std::string, std::string> mimetypes)
{
	std::string html;
	int status = this->respond.status_code;
	html = "<!DOCTYPE html>\n";
	html += "<html>\n";
    html += "<head>\n";
    html += "<title>Error</title>\n";
	html += "<style>\n";
	html += "h1 {\n";
	html += "font-size: 40px;\n";
	html += "text-align: center;\n";
	html += "}\n";
	html += "h3 {\n";
	html += "font-size: 15px;\n";
	html += "text-align: center;\n";
	html += "}\n";
	html += "</style>\n";
    html += "</head>\n";
    html += "<body>\n";
    html += "<h1>" + std::to_string(status) + " Error</h1>\n";
    html += "<hr>\n";
    html += "<h3>Webserv created by hmoubal, med-doba, messalih</h3>\n";
    html += "</body>\n";
    html += "</html>\n";
	std::map<std::string, std::string >::iterator it;
	it = mimetypes.find(".html");
	this->respond.contenttype = it->second;
	this->respond.body = html;
}

void client::findErrorPage(std::map<std::string, std::string> mimetypes,  std::vector<std::pair<int , std::string> > ErrSer)
{
	std::vector<std::pair<int , std::string> >::iterator it = ErrSer.begin();
	int status = this->respond.status_code;
	for (; it != ErrSer.end(); it++)
	{
		if (status == it->first)
		{
			this->path = it->second;
			if (this->fillBody(mimetypes) == -1)
				this->generateErrPage(mimetypes);
			return ;
		}
	}
	// std::cout << "manual generate " << std::endl;
	this->generateErrPage(mimetypes);
}

void client::genereteLoadSucess(std::map<std::string, std::string> mimetypes)
{
	std::string html;
	html = "<!DOCTYPE html>\n";
	html += "<html>\n";
    html += "<head>\n";
    html += "<title>Upload Success</title>\n";
	html += "<style>\n";
	html += "h1 {\n";
	html += "font-size: 40px;\n";
	html += "text-align: center;\n";
	html += "}\n";
	html += "</style>\n";
    html += "</head>\n";
    html += "<body>\n";
    html += "<h1>Successful upload</h1>\n";
    html += "</body>\n";
    html += "</html>\n";
	std::map<std::string, std::string >::iterator it;
	it = mimetypes.find(".html");
	this->respond.contenttype = it->second;
	this->respond.body = html;
}

void client::check(void)
{
	int res;

	res = headerOfRequest.find("/favicon.ico");
	if (res != -1)
	{
		this->respond.flagResponse = NOTFOUND;
		this->respond.ready = 1;
		this->ready = 1;
	}
	else
		this->ready = 1;
}

void client::clear()
{
	if (input.is_open())
	{
		input.clear();
		input.close();
	}
	headerOfRequest.clear();
	bodyParss.clear();
	boundary.clear();
	bodyofRequest.clear();
	buffer.clear();
	ready = 0;
	total_bytes_received = 0;
	respond.clear();
	flag = 0;
	tmp = 0;
	flag_ = 0;
}

client::client()
{
	flagTimeout = -1;
    total_bytes_received  = 0;
    i  = flag = flag_  = j = 0;
    tmp = 0;
    buffer = bodyofRequest = boundary = "";
    tmp = 0;
	ready = 0;
	flag_res = 0;
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
		this->URI = obj.URI;
		this->buffer = obj.buffer;
		this->boundary = obj.boundary;
		this->response_header = obj.response_header;
		this->body = obj.body;
		this->headerOfRequest = obj.headerOfRequest;
		this->bodyofRequest = obj.bodyofRequest;
		this->bytes_read = obj.bytes_read;
		this->flag = obj.flag;
		this->bodyParss = obj.bodyParss;
		this->headerParss = obj.headerParss;
		this->tmp = obj.tmp;
		this->flag_ = obj.flag_;
		this->flag_res = obj.flag_res;
		this->respond = obj.respond;
		this->total_bytes_received = obj.total_bytes_received;
		this->i = obj.i;
		this->j = obj.j;
		this->len = obj.len;
		this->ContentLength = obj.ContentLength;
		this->ready = obj.ready;
		this->path = obj.path;
		this->redirpath = obj.redirpath;
		this->uploadPath = obj.uploadPath;
		this->listPath = obj.listPath;
		this->timeout = obj.timeout;
		this->flagTimeout = obj.flagTimeout;
	}
	return (*this);
}

client::~client()
{
}

int client::checkHeaderOfreq(std::map<std::string, std::string> Percent,std::multimap<std::string, std::string> mimetypes_)
{
    int pos = 0;
	int _tmp = 0;

    while (buffer[pos] && flag == 0)// for entring one time
    {
        if((buffer[pos] == '\r' || buffer[pos] == '\n') && buffer[pos + 1] == '\n')
        {
            pos += 2;
            if((buffer[pos] == '\r' || buffer[pos] == '\n') && buffer[pos + 1] == '\n' )
            {
                headerOfRequest = buffer.substr(0,pos);// not include \r\n
				std::string copyheader = headerOfRequest;
				this->flag_res = headerParss.checkHeaderOfreq_(*this, copyheader, Percent, mimetypes_);
				if (this->flag_res < 0)
				{
					flag = ERROR;
					return (1);
				}
				if (this->tmp == POST)
				{
					i = copyheader.find("Transfer-Encoding: chunked");   // find way to check if boundry
					if(i != -1)
					{
						i = pos  + 2;
						pos = copyheader.find("Content-Length");
						if(pos != -1)
							ContentLength = ft_atoi(copyheader.substr(pos + 16,copyheader.size()).c_str());
						flag = CHUNKED;
						return 1;
					}
					pos = copyheader.find("Content-Length");
					if(pos != -1)
					{
						j = copyheader.find("boundary");
						if(j != -1)
						{
							flag = FORM;
							ContentLength = ft_atoi(copyheader.substr(pos + 16,copyheader.size()).c_str());
							i = headerOfRequest.size() + 3;// after herder
							bytes_read -= i;
							_tmp = j + 9;
							char *temp = (char*)copyheader.data() + _tmp;// because string() dont handle '\r'
							_tmp = 0;
							while (temp[_tmp] != '\r' && temp[_tmp] != '\n' && temp[_tmp + 1] != '\n')
								_tmp++;
							char *strtmp = ft_substr(temp,0,_tmp);
							if (strtmp == NULL)
							{
								this->ready = 1;
								this->respond.ready = 1;
								this->respond.flagResponse = INTERNALERR;
								this->flag = ERROR;
								return 1;
							}
							boundary.append("--").append(strtmp);// free boundry and temp?
							free(strtmp);
							return 1;
						}
						ContentLength = ft_atoi(copyheader.substr(pos + 16,copyheader.size()).c_str());
						flag = NONCHUNKED;
						i = headerOfRequest.size();
						return  1;
					}
				}
				else if (this->tmp == GET)
				{
					flag = GET;
					return (1);
				}
				else if (this->tmp == DELETE)
				{
					flag = DELETE;
					return (1);
				}
            }
            --pos;
        }
        pos++;
    }
    // in entring second times
    if(flag == GET || flag == CHUNKED || flag == NONCHUNKED || flag == FORM || flag == ERROR || flag == DELETE)
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
	// // std::cout << "bytes read == " << this->bytes_read << std::endl;
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

int client::postMethod(std::multimap<std::string, std::string> mimetypes_, std::map<std::string, std::string> mimetypes)
{
	if(this->flag == NONCHUNKED) // if has content length
	{
		this->bodyParss.handle_post(*this, mimetypes_);
		this->respond.ready = 1;
	}
	else if(this->flag == CHUNKED)// // handle chunked data when resend request
	{
		this->bodyParss.handling_chunked_data(*this, mimetypes, mimetypes_);
		this->respond.ready = 1;
	}
	else if(this->flag == FORM)
	{
		this->bodyParss.handling_form_data(*this, mimetypes);
		this->respond.ready = 1;
	}
	return (0);
}

void client::generateUrl()
{
	std::string redirectUrl;
	int pos = this->headerOfRequest.find("Host: ");
	std::string line = this->headerOfRequest.substr(pos + 6, this->headerOfRequest.find('\r', pos + 6) - (pos + 6));
	std::string host = line.substr(0, line.find(':'));
	std::string port = line.substr(line.find(':') + 1);
	if (this->redirpath[0] == '/')
		redirectUrl = "http://" + host + ":" + port + this->redirpath;
	else
		redirectUrl = this->redirpath + "/";
	this->respond.redirectUrl = redirectUrl;
}

void client::initResponse(std::map<std::string, std::string> mimetypes, std::vector<std::pair<int , std::string> >& ErrSer)
{
	if (this->respond.flagResponse == NOTFOUND)
	{
		this->respond.status_code = 404;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == LOOP)
	{
		this->respond.status_code = 508;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == TOOLARGE)
	{
		this->respond.status_code = 413;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == TOOLONG)
	{
		this->respond.status_code = 414;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == MEDIANOTSUPPORTED)
	{
		this->respond.status_code = 415;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == NOTVALIDRANGE)
	{
		this->respond.status_code = 416;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == NOTIMPLEMENTED)
	{
		this->respond.status_code = 501;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == LENGTHREQUIRED)
	{
		this->respond.status_code = 411;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == METHODNOTALLOWED)
	{
		this->respond.status_code = 405;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.headers.push_back("Allow: GET, POST, DELETE");
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == NOTSUPPORTED)
	{
		this->respond.status_code = 505;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == BADREQUEST)
	{
		this->respond.status_code = 400;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == CONFLICT)
	{
		this->respond.status_code = 409;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == OPFILE)
	{
		if (this->fillBody(mimetypes) == -1)
		{
			this->respond.status_code = 500;
			this->findErrorPage(mimetypes, ErrSer);
			this->respond.type = 1;
			this->respond.close = CLOSE;
			this->respond.flagResponse = -1;
			return;
		}
		this->respond.status_code = 200;
		this->respond.close = ALIVE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == REDIRECT)
	{
		this->generateUrl();
		if (this->respond.status_code == 0)
			this->respond.status_code = 301;
		this->respond.close = ALIVE;
		this->respond.headers.push_back("Cache-Control: no-cache, no-store");
		this->respond.headers.push_back("Pragma: no-cache");
		this->respond.headers.push_back("Expires: 0");
		this->respond.headers.push_back("Location: " + this->respond.redirectUrl);
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == FORBIDEN)
	{
		this->respond.status_code = 403;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.type = 1;
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
	}
	else if (this->respond.flagResponse == AUTOINDEX)
	{
		this->generateListing(mimetypes);
		this->respond.status_code = 200;
		this->respond.close = ALIVE;
		this->respond.flagResponse = -1;
		this->respond.type = 1;
	}
	else if (this->respond.flagResponse == CREATED)
	{
		this->genereteLoadSucess(mimetypes);
		this->respond.status_code = 201;
		this->respond.type = 1;
		this->respond.close = ALIVE;
		this->respond.flagResponse = -1;
	}
	else if (this->respond.flagResponse == INTERNALERR)
	{
		this->respond.status_code = 500;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.type = 1;
		this->respond.close = CLOSE;
		this->respond.flagResponse = -1;
	}
	else if (this->respond.flagResponse == DELETED)
	{
		this->respond.close = ALIVE;
		this->respond.status_code = 204;
		this->respond.type = 1;
		this->respond.flagResponse = -1;
	}
	else if (this->respond.flagResponse == CGIPRO)
	{
		this->respond.close = ALIVE;
		if (this->obj.ft_cgi(this->obj.SCRIPT_NAME) == -1)
		{
			this->respond.status_code = 500;
			this->findErrorPage(mimetypes, ErrSer);
			this->respond.type = 1;
			this->respond.close = CLOSE;
			this->respond.flagResponse = -1;
			return ;
		}
		this->respond.contenttype = "text/html";
		this->respond.status_code = 200;
		this->respond.body = this->obj.body;
		this->respond.type = 1;
		this->respond.flagResponse = -1;
	}
	else if (this->respond.flagResponse == TIMEOUT)
	{
		this->respond.status_code = 504;
		this->findErrorPage(mimetypes, ErrSer);
		this->respond.close = CLOSE;
		this->respond.type = 1;
		this->respond.flagResponse = -1;
	}
}
