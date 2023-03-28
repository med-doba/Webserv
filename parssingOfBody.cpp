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

void parssingOfBody::create_file_and_put_content(string & bodyofRequest,string & headerOfRequest)
{
    int rtn;

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
	std::cout << "write i == " << i << std::endl;
	std::cout << "buffer size == " << bodyofRequest.size() << std::endl;
    close(fd);
}

void parssingOfBody::putDataTofile(string  data, string & bodyofRequest)
{
    int pos = data.find("filename=\"");
 
    if(pos != -1)
    {
        int t = pos + 10;
        while (data[t] != '"')
            t++;
        file =  data.substr(pos + 10,t - (pos + 10));
        fd = open((char*)(file.data()),O_CREAT | O_RDWR , 0777);
        pos = data.find("Content-Type:");
        while (data[pos] != '\r' && data[pos + 1] != '\n')
            pos++;
        pos += 4;
        while (pos < (int)data.size())
        {
            bodyofRequest.push_back(data[pos]);
            pos++;
        }
        write(fd,(void*)(bodyofRequest.data()),bodyofRequest.size());
        file.clear();
        bodyofRequest.clear();
        close(fd);
    }
}

void parssingOfBody::handling_form_data(string& buffer, string &headerOfRequest, string &boundary,string & bodyofRequest ,int &total_bytes_received,unsigned long &ContentLength,  int & i, int & bytes_received, int & flag_)
{
    if(flag_ == 5)
    {
        int j = headerOfRequest.find("boundary");
       
        int tmp = j + 9;
       
        char *temp = (char*)headerOfRequest.data() + tmp;// because string() dont handle '\r'
        tmp = 0;
        while (temp[tmp] != '\r' && temp[tmp + 1] != '\n')
            tmp++;
        boundary.append("--").append(ft_substr_(temp,0,tmp));// free boundry and temp?    
        total_bytes_received = ContentLength;
        i = 0;
        bodyofRequest.clear();
    }
    else
        total_bytes_received += bytes_received;
    if(total_bytes_received >= (int)ContentLength)
    { 
        
        size_t start_idx = i;
        string separator = boundary;
        vector<string> substrings; // clear ?

        while (true) 
        {
            size_t end_idx = buffer.find(separator, start_idx);
            if (end_idx == string::npos) 
            {
                substrings.push_back(buffer.substr(start_idx));
                break;
            }

            substrings.push_back(buffer.substr(start_idx, end_idx - start_idx));
            start_idx = end_idx + separator.length();
        }
        substrings.erase(substrings.end() - 1);// remove "--" after last boundry
        string s;
        vector<string>::iterator it = substrings.begin();
        while (it != substrings.end())
        { 
            if(!it->empty())
                putDataTofile(*it,bodyofRequest);
            it++;
        }
    }
}




// void  parssingOfBody::handling_chunked_data(client &obj)
void  parssingOfBody::handling_chunked_data(string &buffer,string &headerOfRequest, string &boundary,string &bodyofRequest, int & total_bytes_received, unsigned long & ContentLength, int & i, int & bytes_received,int & flag_)
{
    
	(void)boundary;
	(void)total_bytes_received;
    int dec;
     int j = 0;
    while (j < bytes_received)
    {
        if(isalnum(buffer[i]) || isalpha(buffer[i]))
        {
            int k = i;
            while ((isalnum(buffer[i]) || isalpha(buffer[i])))
            {
                i++;
                j++;
            }
             
            dec = std::stoul(buffer.substr(k,i), NULL, 16);
             
            i+=2;
            j+=2;
            if(dec == 0 && flag_ == 0)
            {
                if(dec == 0 && flag_ == 0)
                {
                    dec = headerOfRequest.find("boundary");
                    if(dec != -1)
                    {
                        i = 0;
                        buffer = bodyofRequest;
                        flag_ = 5;
                        // handling_form_data(buffer,headerOfRequest,boundary,bodyofRequest ,total_bytes_received,ContentLength,  i, bytes_received,flag_);
                        return ;
                    }
					std::cout << "content == " << ContentLength<< std::endl;
                    create_file_and_put_content(bodyofRequest,headerOfRequest);
                    flag_ = 10;
                }
                return;
            }
            while (dec--)
            {
                bodyofRequest.push_back(buffer[i]);
                i++;
                j++;
            }
            i+=2; 
            j+=2; 
        }
        j++;
    }
}


void parssingOfBody::handle_post(client &obj)
{
	string test = obj.buffer.substr(obj.headerOfRequest.size() + 3,obj.ContentLength);
    obj.i += obj.len;
	std::cout << "test size == "<< test.size() << std::endl;
    // obj.i += BUFFER;
	// if(obj.i >= (int)obj.ContentLength)// finish recivng
	// if(obj.buffer.size() >= obj.ContentLength)// finish recivng
	if(test.size() >= obj.ContentLength)// finish recivng
    { 
		std::cout << "i == " << obj.i << std::endl;
		std::cout << "content == "<< obj.ContentLength << std::endl;
		std::cout << "buffer23 size == " << obj.buffer.size()<< std::endl;
        obj.bodyofRequest = obj.buffer.substr(obj.headerOfRequest.size() + 3,obj.ContentLength);
        create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest);
    }
}

parssingOfBody::~parssingOfBody()
{
}
