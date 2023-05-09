// #include "parssingOfBody.hpp"
#include "client.hpp"
#include <cstdlib>

#include <string.h>

parssingOfBody::parssingOfBody(/* args */)
{
    file = "";
 
}

char *ft_substr_(char const *s, unsigned int start, size_t len)
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

void parssingOfBody::clear()
{
	file.clear();
	exetention.clear();
	close(fd);
}

void parssingOfBody::create_file_and_put_content(string & bodyofRequest,string & headerOfRequest, int &flagResponse, std::string path)
{
    int rtn;

	file = path;
    exetention = std::to_string(rand() % 100000);
    if( (rtn = headerOfRequest.find("mp4") ) != -1) 
        fd = open((char*)(file.append(exetention).append(".mp4").data()),O_CREAT | O_RDWR , 0777);
    else if((rtn = headerOfRequest.find("mp3")) != -1 )
        fd = open((char*)(file.append(exetention).append(".mp3").data()),O_CREAT | O_RDWR , 0777);
    else if((rtn = headerOfRequest.find("jpeg")) != -1 )
        fd = open((char*)(file.append(exetention).append(".jpeg").data()),O_CREAT | O_RDWR , 0777);
    else if((rtn = headerOfRequest.find("jpg")) != -1 )
        fd = open((char*)(file.append(exetention).append(".jpg").data()),O_CREAT | O_RDWR , 0777);
    else if((rtn = headerOfRequest.find("png")) != -1 )
        fd = open((char*)(file.append(exetention).append(".png").data()),O_CREAT | O_RDWR , 0777);
    else if((rtn = headerOfRequest.find("pdf")) != -1)
        fd = open((char*)(file.append(exetention).append(".pdf").data()),O_CREAT | O_RDWR , 0777);
    else if((rtn = headerOfRequest.find("html")) != -1)
        fd = open((char*)(file.append(exetention).append(".html").data()),O_CREAT | O_RDWR , 0777);
    else if((rtn = headerOfRequest.find("css")) != -1)
        fd = open((char*)(file.append(exetention).append(".css").data()),O_CREAT | O_RDWR , 0777);
    else // is a text file such as html ..
        fd = open((char*)(file.append(exetention).data()),O_CREAT | O_RDWR , 0777);
    if (fd < 0)
    {
        std::cout << "couldn't open file" << std::endl;
        flagResponse = FORBIDEN;
        return ;
    }
    int i = write(fd,(void*)(bodyofRequest.data()),bodyofRequest.size());
	if (i == (int)bodyofRequest.size())
		flagResponse = CREATED;
	//std::cout << "write i == " << i << std::endl;
	//std::cout << "buffer size == " << bodyofRequest.size() << std::endl;
    close(fd);
}

void parssingOfBody::putDataTofile(string  data, client & obj)
{

    int pos = data.find("filename=\"");
    
    if(pos != -1)
    {    
        int t = pos + 10;
        while (data[t] != '"')
            t++;
		file = obj.path + "/";
        file +=  data.substr(pos + 10,t - (pos + 10));
		std::cout << "file == " << file << std::endl;
        fd = open((char*)(file.data()),O_CREAT | O_RDWR | O_EXCL , 0777);
        if (fd < 0)
        {
            obj.respond.flagResponse = EXIST;
            file.clear();
            obj.bodyofRequest.clear();
            return ;
        }
        pos = data.find("Content-Type:");
        while (data[pos] != '\r' && data[pos + 1] != '\n')
            pos++;
        pos += 4;
        while (pos < (int)data.size() - 2)// for /r/n
        {
            obj.bodyofRequest.push_back(data[pos]);
            pos++;
        }
        int i = write(fd,(void*)(obj.bodyofRequest.data()),obj.bodyofRequest.size());
        if (obj.bodyofRequest.empty())
            obj.respond.flagResponse = EMPTY;
        else if (i == (int)obj.bodyofRequest.size())
            obj.respond.flagResponse = CREATED;
        file.clear();
        obj.bodyofRequest.clear();
        close(fd);
    }
}

void parssingOfBody::handling_form_data(client &obj)
{
    if(obj.flag_ == 5)
    {
         
        int j = obj.headerOfRequest.find("boundary");
       
        int tmp = j + 9;
       
        char *temp = (char*)obj.headerOfRequest.data() + tmp;// because string() dont handle '\r'
        tmp = 0;
        while (temp[tmp] != '\r' && temp[tmp + 1] != '\n')
            tmp++;
        char *strtmp = ft_substr_(temp,0,tmp);
        obj.boundary.append("--").append(strtmp);// free boundry and temp?    
        free(strtmp);
        obj.total_bytes_received = obj.ContentLength;
        obj.i = 0;
        obj.bodyofRequest.clear();
         
    }
    if(obj.total_bytes_received >= obj.ContentLength)
    { 
        size_t start_idx = obj.i;
        string separator = obj.boundary;
        vector<string> substrings; // clear ?
        while (true) 
        {
            size_t end_idx = obj.buffer.find(separator, start_idx);
            if (end_idx == string::npos) 
            {
                substrings.push_back(obj.buffer.substr(start_idx));
                break;
            }
            substrings.push_back(obj.buffer.substr(start_idx, end_idx - start_idx));
            start_idx = end_idx + separator.length();
        }
        int size = obj.buffer.size() - (obj.headerOfRequest.size() + 3);
        //std::cout << obj.buffer.size() << std::endl;
        //std::cout << obj.headerOfRequest << std::endl;
        if (size > obj.ContentLength)
        {
            //std::cout << "size == " << size << std::endl;
            //std::cout << "content == " << obj.ContentLength << std::endl;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.type = 1;
            obj.respond.body = "The size of the request body does not match the Content-Length header.";
            obj.respond.close = CLOSE;
            obj.respond.content = 1;
            return ;
        }
        substrings.erase(substrings.end() - 1);// remove "--" after last boundr
        vector<string>::iterator it = substrings.begin();
        while (it != substrings.end())
        {
            if(!it->empty())
                putDataTofile(*it,obj);
            it++;
        }
    }
}




void  parssingOfBody::handling_chunked_data(client &obj)
{
    int pos = obj.buffer.find("\r\n0\r\n\r\n");
    if(pos != -1 )
    {
        while (obj.i < (int)obj.buffer.size())
        {
            if(isalnum(obj.buffer[obj.i]) || isalpha(obj.buffer[obj.i]))
            {
                int k = obj.i;
                while ((isalnum(obj.buffer[obj.i]) || isalpha(obj.buffer[obj.i])))
                    obj.i++;
                
                int dec = std::stoul(obj.buffer.substr(k,obj.i), NULL, 16);
                
                obj.i+=2;
                if(dec == 0 )
                    break;
                
                while (dec--)
                {
                   
                    obj.bodyofRequest.push_back(obj.buffer[obj.i]);
                    obj.i++;
                }
                obj.i+=2;
            }
        }
        if(obj.flag_ == 0)
        {
            int dec = obj.headerOfRequest.find("boundary");
            if(dec != -1)
            {
                obj.buffer = obj.headerOfRequest;
                obj.buffer += obj.bodyofRequest;
                obj.flag_ = 5;
                handling_form_data(obj);
                return ;
            }
            if (!obj.bodyofRequest.empty())
			{
				if (obj.ContentLength < (int)obj.bodyofRequest.size())
				{
					obj.respond.status_code = 400;
					obj.respond.phrase = "Bad Request";
					obj.respond.type = 1;
					obj.respond.body = "The request is invalid or malformed.";
					obj.respond.close = CLOSE;
					obj.respond.content = 1;
					return;
				}
				create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest, obj.respond.flagResponse, obj.path);
			}
            else
				obj.respond.flagResponse = EMPTY;
            
            obj.flag_ = 10;
        } 
    }
}


void parssingOfBody::handle_post(client &obj)
{
	string test = obj.buffer.substr(obj.headerOfRequest.size() + 3,obj.ContentLength);
	if((int)test.size() >= obj.ContentLength)// finish recivng
    { 
		//std::cout << "i == " << obj.i << std::endl;
		//std::cout << "content == "<< obj.ContentLength << std::endl;
		//std::cout << "buffer23 size == " << obj.buffer.size()<< std::endl;
        obj.bodyofRequest = obj.buffer.substr(obj.headerOfRequest.size() + 3,obj.ContentLength);
        if (!obj.bodyofRequest.empty())
            create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest, obj.respond.flagResponse, obj.path);
        else
            obj.respond.flagResponse = EMPTY;
    }
}

parssingOfBody::~parssingOfBody()
{
}
