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

void parssingOfBody::create_file_and_put_content(string & bodyofRequest,string & headerOfRequest, int &flagResponse, std::string path, std::multimap<std::string, std::string> mimetypes_)
{
    if (path[path.size() - 1] == '/')
        file = path;
    else
        file = path + "/";
    if (access(file.c_str(), F_OK) != 0)
    {
        flagResponse = NOTFOUND;
        file.clear();
        bodyofRequest.clear();
        return ;
    }
    exetention = std::to_string(rand() % 100000);
    file.append(exetention);
    std::string ext;
    std::multimap<std::string, std::string>::iterator it;
    int pos = headerOfRequest.find("Content-Type: ");
    if (pos != -1)
    {
        ext = headerOfRequest.substr(pos + 14, headerOfRequest.find('\r', pos));
        ext.pop_back();
        it = mimetypes_.find(ext);
        if (it != mimetypes_.end())
            file.append(it->second);
        else
        {
            it = mimetypes_.find("text/plain");
            file.append(it->second);
        }
    }
    fd = open((char*)(file.data()),O_CREAT | O_RDWR | O_EXCL, 0777);
    if (fd < 0)
    {
        flagResponse = CONFLICT;
        return ;
    }
    int i = write(fd,(void*)(bodyofRequest.data()),bodyofRequest.size());
	if (i == (int)bodyofRequest.size())
		flagResponse = CREATED;
    close(fd);
}

int parssingOfBody::checkMedia(std::string file, std::map<std::string, std::string> mimetypes)
{
    std::map<std::string, std::string>::iterator it;
    int pos = file.find('.');
    if (pos != -1)
    {
        std::cout << &file[pos] << std::endl;
        std::string ext = &file[pos];
        it = mimetypes.find(ext);
        if (it != mimetypes.end())
            return (0);
        else
            return (-1);
    }
    return (0);
}

void parssingOfBody::putDataTofile(string  data, client & obj, std::map<std::string, std::string> mimetypes)
{

    int pos = data.find("filename=\"");
    if(pos != -1)
    {    
        int t = pos + 10;
        while (data[t] != '"')
            t++;
        if (obj.uploadPath[obj.uploadPath.size() - 1] == '/')
            obj.uploadPath.pop_back();
		file = obj.uploadPath + "/";
        if (access(file.c_str(), F_OK) != 0)
        {
            obj.respond.flagResponse = NOTFOUND;
            file.clear();
            obj.bodyofRequest.clear();
            return ;
        }
        file +=  data.substr(pos + 10,t - (pos + 10));
        // std::cout << "file == " << file << std::endl;
        // std::cout << "data == " << data << std::endl;
        if (checkMedia(file, mimetypes) == -1)
        {
            obj.respond.flagResponse = MEDIANOTSUPPORTED;
            return ;
        }
        fd = open((char*)(file.data()),O_CREAT | O_RDWR | O_EXCL , 0777);
        if (fd < 0)
        {
            obj.respond.flagResponse = CONFLICT;
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
        if (!obj.bodyofRequest.empty())
        {
            int i = write(fd,(void*)(obj.bodyofRequest.data()),obj.bodyofRequest.size());
            if (i == (int)obj.bodyofRequest.size())
                obj.respond.flagResponse = CREATED;
        }
        else
            obj.respond.flagResponse = CREATED;
        file.clear();
        obj.bodyofRequest.clear();
        close(fd);
    }
}

void parssingOfBody::handling_form_data(client &obj, std::map<std::string, std::string> mimetypes)
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
        if (size > obj.ContentLength)
        {
            obj.respond.flagResponse = BADREQUEST;
            return ;
        }
        substrings.erase(substrings.end() - 1);// remove "--" after last boundr
        vector<string>::iterator it = substrings.begin();
        while (it != substrings.end())
        {
            if(!it->empty())
                putDataTofile(*it,obj, mimetypes);
            it++;
        }
    }
}




void  parssingOfBody::handling_chunked_data(client &obj, std::map<std::string, std::string> mimetypes, std::multimap<std::string, std::string> mimetypes_)
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
                handling_form_data(obj, mimetypes);
                return ;
            }
            if (obj.ContentLength < (int)obj.bodyofRequest.size())
            {
                obj.respond.flagResponse = BADREQUEST;
                return;
            }
            create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest, obj.respond.flagResponse, obj.path, mimetypes_);
            
            obj.flag_ = 10;
        } 
    }
}


void parssingOfBody::handle_post(client &obj, std::multimap<std::string, std::string> mimetypes_)
{
	string test = obj.buffer.substr(obj.headerOfRequest.size() + 3,obj.ContentLength);
	if((int)test.size() >= obj.ContentLength)// finish recivng
    {
        obj.bodyofRequest = obj.buffer.substr(obj.headerOfRequest.size() + 3,obj.ContentLength);
        create_file_and_put_content(obj.bodyofRequest,obj.headerOfRequest, obj.respond.flagResponse, obj.uploadPath, mimetypes_);
    }
}

parssingOfBody::~parssingOfBody()
{
}
