// #include "parssingOfHeader.hpp"
#include "client.hpp"



parssingOfHeader::parssingOfHeader(/* args */)
{
}

char *ft_substr(char const *s, unsigned int start, size_t len)
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

long long	parssingOfHeader::ft_atoi(const char *str)
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

int parssingOfHeader::check_media(client &obj)
{
	std::string media_handled[8] = {"image/png", "image/jpeg", "image/jpg" ,"application/pdf" , "text/plain" , "text/html" , "text/css", "video/mp4"};
	std::string media;
	int pos = obj.headerOfRequest.find("Content-Type: ");
	if (pos != -1)
	{
        int i = obj.headerOfRequest.find(';', pos);
        std::string check = obj.headerOfRequest.substr(pos + 14 , i - pos - 14);
        // std::cout << "check == %" << (int)check[check.size() - 1] << "%"<< std::endl;
        // std::cout << "check == " << check.size() << std::endl;
        if (check.compare("multipart/form-data") == 0)
            return (0);
		media = obj.headerOfRequest.substr(pos + 14, obj.headerOfRequest.find('\r', pos));
        // std::cout << "media == " << media << std::endl;
        media.pop_back();
        // std::cout << "media == " << media.size() << std::endl;
        //std::cout << "asdhaksdka" << std::endl;
		for (int i = 0; i < 8; i++)
		{
			if (media.compare(media_handled[i]) == 0)
				return (0);
		}
		obj.respond.status_code = 415;
		obj.respond.phrase = "Unsupported Media Type";
		obj.respond.type = 1;
		obj.respond.body = "Unsupported media type. Please use a supported type.";
		obj.respond.close = CLOSE;
		obj.respond.content = 1;
		obj.flag = ERROR;
	}
	return (-1);
}

int parssingOfHeader::checkHeaders(client &obj, std::string copy)
{
     
    // when add char after \r\n will add in first of the second line
    int i;
    string str = "";
    i = 0; 
      
    while (i < (int)copy.size())// sometimes in content has '\0' , dont loop
    {
        str.push_back(copy[i]);
        i++;
    }

    i = 0;
    while (str[i])
    {
        while (str[i] && str[i] != ':')
            i++;
        i+=2;
        int k = i;
        while (str[i] && str[i] != '\r' && str[i] != '\n'  && str[i + 1] != '\n')  
            i++;
        if(str.substr(k,i - k).size() == 0)// if has empty value
        {
            obj.respond.type = 1;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.content = 1;
            obj.respond.body = "The request is invalid or malformed.";
            obj.respond.close = CLOSE;
            return -2;
        }
        if(str[i] != '\r' && str[i] != '\n' && str[i + 1] == '\n')// if line dont end by '\r'
        {
            obj.respond.type = 1;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.content = 1;
            obj.respond.body = "The request is invalid or malformed.";
            obj.respond.close = CLOSE;
            return -2;
        }
        i++;
    }

	i = str.find("Host: ");
	if(i == -1)
	{
		obj.respond.status_code = 400;
		obj.respond.phrase = "Bad Request";
		obj.respond.type = 1;
		obj.respond.body = "No Host Header Found";
		obj.respond.close = CLOSE;
        obj.respond.content = 1;
		return -4;
	}
    if(obj.tmp == POST)// when upload to to server sould present this headers
    {
        i = str.find("Content-Length: ");
        if(i == -1)
        {
           obj.respond.type = 1;
           obj.respond.status_code = 411;
           obj.respond.phrase = "Length Required";
           obj.respond.content = 1;
           obj.respond.body = "No Content-Length Header Found";
           obj.respond.close = CLOSE;
            return -5;
        }
        i = str.find("Content-Type: ");
        if(i == -1)
        {
			std::cout << str << std::endl;
            obj.respond.type = 1;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.content = 1;
            obj.respond.body = "1No Content-Type Header Found";
            obj.respond.close = CLOSE;
            return -6;
        }
        else
        {
            if (check_media(obj) == -1)
                return (-1);
        }
        i = str.find("If-Modified-Since: ");
        if(i != -1)
        {
           obj.respond.type = 1;
           obj.respond.status_code = 405;
           obj.respond.phrase = "Requested Range Not Satisfiable";
           obj.respond.content = 1;
           obj.respond.body = "The request has a malformed header";
           obj.respond.close = CLOSE;
            return -7;
        }
        i = str.find("Range: ");
        if(i != -1)
        {
           obj.respond.type = 1;
           obj.respond.status_code = 416;
           obj.respond.phrase = "Bad Request";
           obj.respond.content = 1;
           obj.respond.body = "The request has a malformed header";
           obj.respond.close = CLOSE;
            return -8;
        }
        i = str.find("Transfer-Encoding: ");
        if (i != -1)
        {
            std::string type = str.substr(i + 19, str.find('\r', i) - (i + 19));
            if (type.compare("chunked") != 0)
            {
                obj.respond.status_code = 501;
                obj.respond.phrase = "Not Implemented";
                obj.respond.type = 1;
                obj.respond.body = "Server Only Handles Chunked Encoding";
                obj.respond.close = CLOSE;
                obj.respond.content = 1;
                return (-1);
            }
        }
    }
    else if(obj.tmp == GET)//get method
    {
        i = str.find("Content-Length: ");
        if(i != -1)
		{
			int ContentLength = ft_atoi(copy.substr(i + 16,copy.size()).c_str());
			if (ContentLength > 0)
            {
                obj.respond.type = 1;
                obj.respond.status_code = 400;
                obj.respond.phrase = "Bad Request";
                obj.respond.content = 1;
                obj.respond.body = "The request has a malformed header";
                obj.respond.close = CLOSE;
				return -9;
            }
		}
        i = str.find("Transfer-Encoding: ");
		if (i != -1)
        {
            obj.respond.type = 1;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.content = 1;
            obj.respond.body = "Invalid request: GET requests must not have a body with Transfer-Encoding.";
            obj.respond.close = CLOSE;
			return (-10);
        }
        i = str.find("Content-Type: ");
        if(i != -1)
        {
            obj.respond.type = 1;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.content = 1;
            obj.respond.body = "The request contained a Content-Type header, but it should not be included in a GET request.";
            obj.respond.close = CLOSE;
            return -11;
        }
    }
    else if(obj.tmp == DELETE)//delete method
    {
        i = str.find("If-Modified-Since: ");
        if(i != -1)
        {
			obj.respond.type = 1;
			obj.respond.status_code = 405;
			obj.respond.phrase = "Method Not Allowed";
			obj.respond.content = 1;
			obj.respond.body = "The request has a malformed header";
			obj.respond.close = CLOSE;
            return -12;
        }
        i = str.find("Content-Type: ");
        if(i != -1)
        {
            obj.respond.type = 1;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.content = 1;
            obj.respond.body = "Content-Type Header Found";
            obj.respond.close = CLOSE;
            return -6;
        }
        i = str.find("Content-Length: ");
        if(i != -1)
		{
			int ContentLength = ft_atoi(copy.substr(i + 16,copy.size()).c_str());
			//std::cout << "content == "<< ContentLength<< std::endl;
			if (ContentLength > 0)
            {
                obj.respond.type = 1;
                obj.respond.status_code = 400;
                obj.respond.phrase = "Bad Request";
                obj.respond.content = 1;
                obj.respond.body = "The request has a malformed header";
                obj.respond.close = CLOSE;
				return -9;
            }
		}
        i = str.find("Transfer-Encoding: ");
		if (i != -1)
        {
            obj.respond.type = 1;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.content = 1;
            obj.respond.body = "Invalid request: Delete requests must not have a body with Transfer-Encoding.";
            obj.respond.close = CLOSE;
			return (-10);
        }
    }
    
    return 1;
}

int parssingOfHeader::VerifyURI(client &obj)
{
	std::string compare = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    if (obj.URI.size() > 2048)
    {
        obj.respond.type = 1;
        obj.respond.status_code = 414;
        obj.respond.phrase = "Request-URI Too Long";
        obj.respond.content = 1;
        obj.respond.body = "Request-URI is bigger than 2048";
        obj.respond.close = CLOSE;
        return -1;
    }
	for (size_t i = 0; i < obj.URI.size(); i++)
	{
		for (size_t j = 0; j < compare.size(); j++)
		{
			if (obj.URI[i] == compare[j])
				break ;
			if (obj.URI[i] != compare[j] && j == compare.size() - 1)
			{
				obj.respond.type = 1;
				obj.respond.status_code = 400;
				obj.respond.phrase = "Bad Request";
				obj.respond.content = 1;
				obj.respond.body = "Character Not Allowed In The URI";
				obj.respond.close = CLOSE;
				return -1;
			}
		}
	}
	for (size_t i = 0; i < obj.URI.size(); i++)
	{
		if (obj.URI[i] == '%')
		{
			std::string space = obj.URI.substr(i,3);
			if (space.compare("%20") == 0)
			{
				std::string tmp = obj.URI;
				obj.URI = tmp.substr(0, i);
				obj.URI += " ";
				obj.URI += tmp.substr(i + 3);
			}
		}
	}
	return (0);

}

int parssingOfHeader::checkHeaderLine(client &obj)
{
    int i = 0;
    int j = 0;
    char *temp;


    j = i;
    while (obj.headerOfRequest[i] && obj.headerOfRequest[i] != ' ')
        i++;
    temp = ft_substr(obj.headerOfRequest.data(),j,i);
    if(strcmp(temp,"POST") == 0)
        obj.tmp = POST;
    if(strcmp(temp,"GET") == 0)
        obj.tmp = GET;
    if(strcmp(temp,"DELETE") == 0)
        obj.tmp = DELETE;
    // //std::cout << "method == " << tmp << std::endl;
    if( strcmp(temp,"GET") != 0 && strcmp(temp,"POST") != 0 && strcmp(temp,"DELETE") != 0)
    {
        free(temp);
        obj.respond.type = 1;
        // obj.respond.status_code = 501;
        // obj.respond.phrase = "Not Implemented";
		obj.respond.status_code = 405;
        obj.respond.phrase = "Method Not Allowed";
        obj.respond.headers.push_back("Allow: GET, POST, DELETE");
        // respond.body = "No Host Header Found";
        // respond.close = CLOSE;
        return -1;
    }
    free(temp);

    i++;
    j = i;
    while (obj.headerOfRequest[i] && obj.headerOfRequest[i] != ' ')
        i++;
    temp = ft_substr(obj.headerOfRequest.data(),j,i);
    if(temp[0] != '/')
    {
        free(temp);
        obj.respond.type = 1;
        obj.respond.status_code = 400;
        obj.respond.phrase = "Bad Request";
        obj.respond.content = 1;
        obj.respond.body = "The request has a malformed header";
        obj.respond.close = CLOSE;
        return -2;
    }
    obj.URI.assign(&temp[0]);
    free(temp);
	if (VerifyURI(obj) == -1)
		return (-2);
    
    i++;
    j = i;
    while (obj.headerOfRequest[i] && obj.headerOfRequest[i] != '\r' && obj.headerOfRequest[i] != '\n'  && obj.headerOfRequest[i + 1] != '\n' )
        i++;
    temp = ft_substr(obj.headerOfRequest.data(),j,i + 2); 
     
    if( strcmp(temp,"HTTP/1.1\r\n") != 0 && strcmp(temp,"HTTP/1.1\n\n") != 0)
    {
        free(temp);
        obj.respond.type = 1;
        obj.respond.status_code = 505;
        obj.respond.phrase = "HTTP Version Not Supported";
        // respond.body = "No Host Header Found";
        // respond.close = CLOSE;
        return -3;
    }
    free(temp);
    i+=2;
    return i;
}
 

int parssingOfHeader::checkHeaderOfreq_(client &obj, std::string copy)
{
    int rtn = checkHeaderLine(obj);

	if(rtn < 0)
		return rtn;

	copy = &copy[rtn];
    rtn = checkHeaders(obj, copy);
    
    return rtn;
}



parssingOfHeader::~parssingOfHeader()
{
}

