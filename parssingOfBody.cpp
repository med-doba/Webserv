// #include "parssingOfBody.hpp"
#include "client.hpp"
#include <cstdlib>

#include <string.h>

using std::endl;
using std::cout;

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

void parssingOfBody::create_file_and_put_content(string & bodyofRequest,string & headerOfRequest, int &created)
{
    int rtn;

    exetention = std::to_string(rand() % 100000);
    std::cout << headerOfRequest << std::endl;
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
        created = 1;
	std::cout << "write i == " << i << std::endl;
	std::cout << "buffer size == " << bodyofRequest.size() << std::endl;
    close(fd);
}

void parssingOfBody::putDataTofile(string  data, string & bodyofRequest, int &created)
{

    int pos = data.find("filename=\"");
    
    if(pos != -1)
    {    
         std::cout << "here \n";
        int t = pos + 10;
        while (data[t] != '"')
            t++;
        file =  data.substr(pos + 10,t - (pos + 10));
        fd = open((char*)(file.data()),O_CREAT | O_RDWR , 0777);
        pos = data.find("Content-Type:");
        while (data[pos] != '\r' && data[pos + 1] != '\n')
            pos++;
        pos += 4;
        while (pos < (int)data.size() - 2)// for /r/n
        {
            bodyofRequest.push_back(data[pos]);
            pos++;
        }
        int i = write(fd,(void*)(bodyofRequest.data()),bodyofRequest.size());
        if (i == (int)bodyofRequest.size())
            created = 1;
        file.clear();
        bodyofRequest.clear();
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
        vector<string> substrings; // clear ?
        // std::cout << obj.buffer << std::endl;
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
        // std::cout << substrings[0] << std::endl;

        substrings.erase(substrings.end() - 1);// remove "--" after last boundr
        // std::cout << substrings[0] << std::endl;
        vector<string>::iterator it = substrings.begin();
        
        // std::cout << "here " << obj.boundary << std::endl;
        while (it != substrings.end())
        { 
            // std::cout << "lolloop" << std::endl;
            if(!it->empty())
                putDataTofile(*it,obj.bodyofRequest, obj.respond.created);
                // cout << *it << endl;
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
                obj.buffer = obj.bodyofRequest;
                obj.flag_ = 5;
				std::cout << "lol" << std::endl;
                // handling_form_data(buffer,headerOfRequest,boundary,bodyofRequest ,total_bytes_received,ContentLength,  i, bytes_received,flag_);
                handling_form_data(obj);
                return ;
            }
            if (!obj.bodyofRequest.empty())
                create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest, obj.respond.created);
            
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
            create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest, obj.respond.created);
    }
}

parssingOfBody::~parssingOfBody()
{
}
