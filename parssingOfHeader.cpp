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


int parssingOfHeader::checkHeaders(string headerOfRequest, int & tmp, response& respond)
{
     
    // when add char after \r\n will add in first of the second line
    int i;
    string str = "";
    i = 0; 
      
    while (i < (int)headerOfRequest.size())// sometimes in content has '\0' , dont loop
    {
        str.push_back(headerOfRequest[i]);
        i++;
    }

	i = str.find("Host: ");
	if(i == -1)
	{
		respond.status_code = 400;
		respond.phrase = "Bad Request";
		respond.type = 1;
		respond.body = "No Host Header Found";
		respond.close = CLOSE;
        respond.content = 1;
		return -4;
	}
    if(tmp == POST)// when upload to to server sould present this headers
    {
        i = str.find("Content-Length: ");
        if(i == -1)
        {
            respond.type = 1;
            respond.status_code = 411;
            respond.phrase = "Length Required";
            respond.content = 1;
            respond.body = "No Content-Length Header Found";
            respond.close = CLOSE;
            return -5;
        }
        i = str.find("Content-Type: ");
        if(i == -1)
        {
            respond.type = 1;
            respond.status_code = 400;
            respond.phrase = "Bad Request";
            respond.content = 1;
            respond.body = "No Content-Type Header Found";
            respond.close = CLOSE;
            return -6;
        }
        i = str.find("If-Modified-Since: ");
        if(i != -1)
        {
            respond.type = 1;
            respond.status_code = 405;
            respond.phrase = "Requested Range Not Satisfiable";
            respond.content = 1;
            respond.body = "The request has a malformed header";
            respond.close = CLOSE;
            return -7;
        }
        i = str.find("Range: ");
        if(i != -1)
        {
            respond.type = 1;
            respond.status_code = 416;
            respond.phrase = "Bad Request";
            respond.content = 1;
            respond.body = "The request has a malformed header";
            respond.close = CLOSE;
            return -8;
        }
    }
    else if(tmp == GET)//get method
    {
        i = str.find("Content-Length: ");
        if(i != -1)
		{
			int ContentLength = ft_atoi(headerOfRequest.substr(i + 16,headerOfRequest.size()).c_str());
			if (ContentLength > 0)
            {
                respond.type = 1;
                respond.status_code = 400;
                respond.phrase = "Bad Request";
                respond.content = 1;
                respond.body = "The request has a malformed header";
                respond.close = CLOSE;
				return -9;
            }
		}
        i = str.find("Transfer-Encoding: ");
		if (i != -1)
        {
            respond.type = 1;
            respond.status_code = 400;
            respond.phrase = "Bad Request";
            respond.content = 1;
            respond.body = "Invalid request: GET requests must not have a body with Transfer-Encoding.";
            respond.close = CLOSE;
			return (-10);
        }
        i = str.find("Content-Type: ");
        if(i != -1)
        {
            respond.type = 1;
            respond.status_code = 400;
            respond.phrase = "Bad Request";
            respond.content = 1;
            respond.body = "The request contained a Content-Type header, but it should not be included in a GET request.";
            respond.close = CLOSE;
            return -11;
        }
    }
    else if(tmp == DELETE)//delete method
    {
        i = str.find("If-Modified-Since: ");
        if(i != -1)
        {
            respond.type = 1;
            respond.status_code = 405;
            respond.phrase = "Method Not Allowed";
            respond.content = 1;
            respond.body = "The request has a malformed header";
            respond.close = CLOSE;
            return -12;
        }
        i = str.find("Range: ");
        if(i != -1)
            return -13;
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
            return -2;
        if(str[i] != '\r' && str[i] != '\n' && str[i + 1] == '\n')// if line dont end by '\r'
        {
            respond.type = 1;
            respond.status_code = 400;
            respond.phrase = "Bad Request";
            respond.content = 1;
            respond.body = "The request is invalid or malformed.";
            respond.close = 1;
            return -2;
        }
        i++;
    }
    
    return 1;
}


int parssingOfHeader::checkHeaderLine(string headerOfRequest, int &tmp, response& respond, std::string &URI)
{
    int i = 0;
    int j = 0;
    char *temp;
	(void)respond;


    j = i;
    while (headerOfRequest[i] && headerOfRequest[i] != ' ')
        i++;
    temp = ft_substr(headerOfRequest.data(),j,i);
    if(strcmp(temp,"POST") == 0)
        tmp = POST;
    if(strcmp(temp,"GET") == 0)
        tmp = GET;
    if(strcmp(temp,"DELETE") == 0)
        tmp = DELETE;
    // std::cout << "method == " << tmp << std::endl;
    if( strcmp(temp,"GET") != 0 && strcmp(temp,"POST") != 0 && strcmp(temp,"DELETE") != 0)
    {
        free(temp);
        respond.type = 1;
        respond.status_code = 405;
        respond.phrase = "Method Not Allowed";
        respond.headers.push_back("Allow: GET, POST, DELETE");
        // respond.body = "No Host Header Found";
        // respond.close = CLOSE;
        return -1;
    }
    free(temp);

    i++;
    j = i;
    while (headerOfRequest[i] && headerOfRequest[i] != ' ')
        i++;
    temp = ft_substr(headerOfRequest.data(),j,i);
    if(temp[0] != '/')
    {
        free(temp);
        return -2;
    }
    URI.assign(temp);
    free(temp);
    std::cout << "URI == " << URI << std::endl;

    i++;
    j = i;
    while (headerOfRequest[i] &&  headerOfRequest[i] != '\r' && headerOfRequest[i] != '\n'  && headerOfRequest[i + 1] != '\n' )
        i++;
    temp = ft_substr(headerOfRequest.data(),j,i + 2); 
     
    if( strcmp(temp,"HTTP/1.1\r\n") != 0 && strcmp(temp,"HTTP/1.1\n\n") != 0)
    {
        free(temp);
        respond.type = 1;
        respond.status_code = 505;
        respond.phrase = "HTTP Version Not Supported";
        // respond.body = "No Host Header Found";
        // respond.close = CLOSE;
        return -3;
    }
    free(temp);
    i+=2;
    return i;
}
 

int parssingOfHeader::checkHeaderOfreq_(string &headerOfRequest, int & tmp, response& respond, std::string& URI)
{
    int rtn = checkHeaderLine(headerOfRequest, tmp, respond, URI);

	if(rtn < 0)
		return rtn;

	headerOfRequest = &headerOfRequest[rtn];
    rtn = checkHeaders(headerOfRequest,tmp, respond);
    
    return rtn;
}



parssingOfHeader::~parssingOfHeader()
{
}

