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

void parssingOfBody::create_file_and_put_content(string & bodyofRequest,string & headerOfRequest, int &flagResponse)
{
    int rtn;

	file = "upload/";
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
    else // is a text file such as html ..
        fd = open((char*)(file.append(exetention).data()),O_CREAT | O_RDWR , 0777);
    
    int i = write(fd,(void*)(bodyofRequest.data()),bodyofRequest.size());
	if (i == (int)bodyofRequest.size())
		flagResponse = CREATED;
	std::cout << "write i == " << i << std::endl;
	std::cout << "buffer size == " << bodyofRequest.size() << std::endl;
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
		file = "upload/";
        file +=  data.substr(pos + 10,t - (pos + 10));
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
        obj.boundary.append("--").append(ft_substr_(temp,0,tmp));// free boundry and temp?    
        obj.total_bytes_received = obj.ContentLength;
        obj.i = 0;
        obj.bodyofRequest.clear();
         
    }
    // else
    //     obj.total_bytes_received += obj.bytes_read;
    if(obj.total_bytes_received >= obj.ContentLength)
    { 
        size_t start_idx = obj.i;
        string separator = obj.boundary;
		std::cout << obj.boundary << std::endl;
        vector<string> substrings; // clear ?
        while (true) 
        {
            size_t end_idx = obj.buffer.find(separator, start_idx);
            if (end_idx == string::npos) 
            {
                // std::cout << obj.buffer.substr(start_idx) << std::endl;
                substrings.push_back(obj.buffer.substr(start_idx));
                break;
            }
            // std::cout << obj.buffer.substr(start_idx, end_idx - start_idx) << std::endl;
            substrings.push_back(obj.buffer.substr(start_idx, end_idx - start_idx));
            start_idx = end_idx + separator.length();
        }
        // std::cout << obj.buffer << std::endl;
        int size = obj.buffer.size() - (obj.headerOfRequest.size() + 3);
        // std::cout << substrings[0] << std::endl;
        if (size > obj.ContentLength)
        {
			std::cout << "contentLength " << obj.ContentLength << std::endl;
            obj.respond.status_code = 400;
            obj.respond.phrase = "Bad Request";
            obj.respond.type = 1;
            obj.respond.body = "The size of the request body does not match the Content-Length header.";
            obj.respond.close = CLOSE;
            obj.respond.content = 1;
            return ;
        }
        substrings.erase(substrings.end() - 1);// remove "--" after last boundr
        // std::cout << substrings[0] << std::endl;
        vector<string>::iterator it = substrings.begin();
        // std::cout << "here " << obj.boundary << std::endl;
        while (it != substrings.end())
        { 
            // std::cout << "here" << std::endl;
            // std::cout << "lolloop" << std::endl;
            if(!it->empty())
            {
				// std::cout << *it << std::endl;
                // cout << *it << endl;
                // cout << "content size == "<< it->size() << endl;
                // std::cout << "contentLength == " << obj.ContentLength << std::endl;
                putDataTofile(*it,obj);
            }
            it++;
        }
        // std::cout << "here2 " << obj.boundary << std::endl;
    }
}




// void  parssingOfBody::handling_chunked_data(string &buffer,string &headerOfRequest, string &boundary,string &bodyofRequest, int & total_bytes_received, int & ContentLength, int & i, int & bytes_received,int & flag_)
void  parssingOfBody::handling_chunked_data(client &obj)
{
    int pos = obj.buffer.find("\r\n0\r\n\r\n");
    if(pos != -1 )
    {
		std::cout << "in here" << std::endl;
		// std::cout << "obj.i == "<< obj.i << std::endl;
		// std::cout << "obj.buffer == "<< obj.buffer.size() << std::endl;
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
		std::cout << "out of loop" << std::endl;
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
				std::cout << "contentLength == " << obj.ContentLength << std::endl;
				std::cout << "size == " << obj.bodyofRequest.size() << std::endl;
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
				std::cout << "create file " << std::endl;
				create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest, obj.respond.flagResponse);
			}
            else
			{
				obj.respond.flagResponse = EMPTY;
				std::cout << "body empty " << std::endl;
			}
            
            obj.flag_ = 10;
        } 
    }
}


void parssingOfBody::handle_post(client &obj)
{
	string test = obj.buffer.substr(obj.headerOfRequest.size() + 3,obj.ContentLength);
    // obj.i += obj.len;
	std::cout << "test size == "<< test.size() << std::endl;
    // obj.i += BUFFER;
	// if(obj.i >= (int)obj.ContentLength)// finish recivng
	// if(obj.buffer.size() >= obj.ContentLength)// finish recivng
	if((int)test.size() >= obj.ContentLength)// finish recivng
    { 
		std::cout << "i == " << obj.i << std::endl;
		std::cout << "content == "<< obj.ContentLength << std::endl;
		std::cout << "buffer23 size == " << obj.buffer.size()<< std::endl;
        obj.bodyofRequest = obj.buffer.substr(obj.headerOfRequest.size() + 3,obj.ContentLength);
        if (!obj.bodyofRequest.empty())
            create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest, obj.respond.flagResponse);
        else
            obj.respond.flagResponse = EMPTY;
    }
}

parssingOfBody::~parssingOfBody()
{
}
