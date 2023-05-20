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

int parssingOfHeader::check_media(client &obj, std::multimap<std::string,std::string> mimetypes_)
{
	std::string media;
	int pos = obj.headerOfRequest.find("Content-Type: ");
	if (pos != -1)
	{
        int i = obj.headerOfRequest.find('\r', pos);
        std::string check = obj.headerOfRequest.substr(pos + 14 , i - pos - 14);
		int ps = check.find(';');
		if (ps != -1)
			check = check.substr(0, ps);
        if (check.compare("multipart/form-data") == 0 || check.compare("application/x-www-form-urlencoded") == 0)
            return (0);
		media = obj.headerOfRequest.substr(pos + 14, obj.headerOfRequest.find('\r', pos) - (pos + 14));
		// std::cout << "media == " << media << std::endl;
        std::multimap<std::string, std::string>::iterator it = mimetypes_.find(media);
        if (it != mimetypes_.end())
            return (0);
        else
        {
            obj.respond.flagResponse = MEDIANOTSUPPORTED;
            obj.respond.ready = 1;
            return (-1);
        }
	}
	return (-1);
}

int parssingOfHeader::checkHeaders(client &obj, std::string copy, std::multimap<std::string, std::string> mimetypes_)
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
        if (i > (int)str.size())
            break;
        int k = i;
        while (str[i] && str[i] != '\r' && str[i] != '\n')  
            i++;
        if(k < (int)str.size() && str.substr(k,i - k).size() == 0)// if has empty value
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
            return -2;
        }
        if(str[i] == '\r' && str[i + 1] != '\n')// if line dont end by '\r'
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
            return -2;
        }
        else if (str[i] == '\n' && str[i - 1] != '\r')
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
            return -2;
        }
        i++;
    }

	i = str.find("Host: ");
	if(i == -1)
	{
        obj.respond.flagResponse = BADREQUEST;
        obj.respond.ready = 1;
		return -4;
	}
    if(obj.tmp == POST)// when upload to to server sould present this headers
    {
        i = str.find("Content-Length: ");
        if(i == -1)
        {
            obj.respond.flagResponse = LENGTHREQUIRED;
            obj.respond.ready = 1;
            return -5;
        }
        i = str.find("Content-Type: ");
        if(i == -1)
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
            return -6;
        }
        else
        {
            if (check_media(obj, mimetypes_) == -1)
                return (-1);
        }
        i = str.find("If-Modified-Since: ");
        if(i != -1)
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
            return -7;
        }
        i = str.find("Range: ");
        if(i != -1)
        {
            obj.respond.flagResponse = NOTVALIDRANGE;
            obj.respond.ready = 1;
            return -8;
        }
        i = str.find("Transfer-Encoding: ");
        if (i != -1)
        {
            std::string type = str.substr(i + 19, str.find('\r', i) - (i + 19));
            if (type.compare("chunked") != 0)
            {
                obj.respond.flagResponse = NOTIMPLEMENTED;
                obj.respond.ready = 1;
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
                obj.respond.flagResponse = BADREQUEST;
                obj.respond.ready = 1;
				return -9;
            }
		}
        i = str.find("Transfer-Encoding: ");
		if (i != -1)
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
			return (-10);
        }
        i = str.find("Content-Type: ");
        if(i != -1)
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
            return -11;
        }
    }
    else if(obj.tmp == DELETE)//delete method
    {
        i = str.find("Content-Type: ");
        if(i != -1)
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
            return -6;
        }
        i = str.find("Content-Length: ");
        if(i != -1)
		{
			int ContentLength = ft_atoi(copy.substr(i + 16,copy.size()).c_str());
			if (ContentLength > 0)
            {
                obj.respond.flagResponse = BADREQUEST;
                obj.respond.ready = 1;
				return -9;
            }
		}
        i = str.find("Transfer-Encoding: ");
		if (i != -1)
        {
            obj.respond.flagResponse = BADREQUEST;
            obj.respond.ready = 1;
			return (-10);
        }
    }
    
    return 1;
}

int parssingOfHeader::VerifyURI(client &obj, std::map<std::string, std::string> PercentEncoding)
{
    std::map<std::string, std::string>::iterator it;
	std::string compare = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    if (obj.URI.size() > 2048)
    {
        obj.respond.flagResponse = TOOLONG;
        obj.respond.ready = 1;
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
                obj.respond.flagResponse = BADREQUEST;
                obj.respond.ready = 1;
				return -1;
			}
		}
	}
	for (size_t i = 0; i < obj.URI.size(); i++)
	{
		if (obj.URI[i] == '%')
		{
			std::string Percent = obj.URI.substr(i,3);
            it = PercentEncoding.find(Percent);
            if (it != PercentEncoding.end())
            {
				std::string tmp = obj.URI;
				obj.URI = tmp.substr(0, i);
				obj.URI += it->second;
				obj.URI += tmp.substr(i + 3);
            }
		}
	}
	return (0);

}

int parssingOfHeader::checkHeaderLine(client &obj, std::map<std::string, std::string> Percent)
{
    int i = 0;
    int j = 0;
    char *temp;


    j = i;
    while (obj.headerOfRequest[i] && obj.headerOfRequest[i] != ' ')
        i++;
    temp = ft_substr(obj.headerOfRequest.data(),j,i);
    if (temp == NULL)
    {
        obj.ready = 1;
        obj.respond.ready = 1;
        obj.respond.flagResponse = INTERNALERR;
        obj.flag = ERROR;
        return (-1);
    }
    if(strcmp(temp,"POST") == 0)
        obj.tmp = POST;
    if(strcmp(temp,"GET") == 0)
        obj.tmp = GET;
    if(strcmp(temp,"DELETE") == 0)
        obj.tmp = DELETE;
    if( strcmp(temp,"GET") != 0 && strcmp(temp,"POST") != 0 && strcmp(temp,"DELETE") != 0)
    {
        free(temp);
        obj.respond.flagResponse = METHODNOTALLOWED;
        obj.respond.ready = 1;
        return -1;
    }
    free(temp);

    i++;
    j = i;
    while (obj.headerOfRequest[i] && obj.headerOfRequest[i] != ' ')
        i++;
    temp = ft_substr(obj.headerOfRequest.data(),j,i);
    if (temp == NULL)
    {
        obj.ready = 1;
        obj.respond.ready = 1;
        obj.respond.flagResponse = INTERNALERR;
        obj.flag = ERROR;
        return (-1);
    }
    if(temp[0] != '/')
    {
        free(temp);
        obj.respond.flagResponse = BADREQUEST;
        obj.respond.ready = 1;
        return -2;
    }
    obj.URI.assign(&temp[0]);
    free(temp);
	if (VerifyURI(obj, Percent) == -1)
		return (-2);
    
    i++;
    j = i;
    while (obj.headerOfRequest[i] && obj.headerOfRequest[i] != '\r' && obj.headerOfRequest[i] != '\n'  && obj.headerOfRequest[i + 1] != '\n' )
        i++;
    temp = ft_substr(obj.headerOfRequest.data(),j,i + 2); 
    if (temp == NULL)
    {
        obj.ready = 1;
        obj.respond.ready = 1;
        obj.respond.flagResponse = INTERNALERR;
        obj.flag = ERROR;
        return (-1);
    }
    if( strcmp(temp,"HTTP/1.1\r\n") != 0 && strcmp(temp,"HTTP/1.1\n\n") != 0)
    {
        free(temp);
        obj.respond.flagResponse = NOTSUPPORTED;
        obj.respond.ready = 1;
        return -3;
    }
    free(temp);
    i+=2;
    return i;
}
 

int parssingOfHeader::checkHeaderOfreq_(client &obj, std::string copy, std::map<std::string, std::string> Percent, std::multimap<std::string, std::string> mimetypes_)
{
    int rtn = checkHeaderLine(obj, Percent);

	if(rtn < 0)
		return rtn;
    
	copy = &copy[rtn];
    rtn = checkHeaders(obj, copy, mimetypes_);
    
    return rtn;
}



parssingOfHeader::~parssingOfHeader()
{
}

