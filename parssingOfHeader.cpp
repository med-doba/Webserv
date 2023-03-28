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


int parssingOfHeader::checkHeaders(int index, string headerOfRequest, int & tmp)
{
     
    // when add char after \r\n will add in first of the second line
    int i;
    string str = "";
    i = index; 
      
    while (i < (int)headerOfRequest.size())// sometimes in content has '\0' , dont loop
    {
        str.push_back(headerOfRequest[i]);
        i++;
    }

    if(tmp)// when upload to to server sould present this headers
    {
        i = str.find("Host: ");
        if(i == -1)
            return -2;
        i = str.find("Content-Length: ");
        if(i == -1)
            return -2;
        i = str.find("Content-Type: ");
        if(i == -1)
            return -2;
    }
    else
    {
        i = str.find("Host: ");
        if(i == -1)
            return -2;
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
            return -2;
        i++;
    }
    
    return 1;
}


int parssingOfHeader::checkHeaderLine(string headerOfRequest, int &tmp)
{
    int i = 0;
    int j = 0;
    char *temp;
     

    j = i;
    while (headerOfRequest[i] && headerOfRequest[i] != ' ')
        i++;
    
    temp = ft_substr(headerOfRequest.data(),j,i);
    if(strcmp(temp,"POST") == 0)
        tmp = 1;
    if( strcmp(temp,"GET") != 0 && strcmp(temp,"POST") != 0 && strcmp(temp,"DELETE") != 0)
    {
        free(temp);
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
        return -1;
    }
    free(temp);

    i++;
    j = i;
    while (headerOfRequest[i] &&  headerOfRequest[i] != '\r' && headerOfRequest[i] != '\n'  && headerOfRequest[i + 1] != '\n' )
        i++;
    temp = ft_substr(headerOfRequest.data(),j,i + 2); 
     
    if( strcmp(temp,"HTTP/1.1\r\n") != 0 && strcmp(temp,"HTTP/1.1\n\n") != 0)
    {
        free(temp);
        return -1;
    }
    free(temp);
    i+=2;
    return i;
}
 

int parssingOfHeader::checkHeaderOfreq_(string headerOfRequest, int & tmp)
{
    int rtn = checkHeaderLine(headerOfRequest, tmp);
 
    if(rtn == -1)
        return -2;
    rtn = checkHeaders(rtn,headerOfRequest,tmp);
    
    return rtn;
}



parssingOfHeader::~parssingOfHeader()
{
}

