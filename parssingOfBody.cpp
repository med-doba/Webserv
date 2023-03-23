#include "parssingOfBody.hpp"
#include <fstream>


parssingOfBody::parssingOfBody(/* args */)
{
    file = "";
    index = 0;// use rundom
}

void parssingOfBody::handle_post(std::string &headerOfRequest,std::string &buffer, unsigned long & ContentLength, unsigned long & i, int& flag)
{
    int rtn;
     
    i += BUFFER;
    if(i >= ContentLength && flag == 1)// finish recivng
    { 
        index++;
        exetention = std::to_string(index);
        std::cout << ContentLength << " " << i << std::endl;
        rtn = headerOfRequest.find("mp4");
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).append(".mp4").data()),O_CREAT | O_RDWR , 0777);
        rtn = headerOfRequest.find("jpg");
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).append(".jpg").data()),O_CREAT | O_RDWR , 0777);

        rtn = headerOfRequest.find("pdf");
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).append(".pdf").data()),O_CREAT | O_RDWR , 0777);
        rtn = headerOfRequest.find("text/plain"); // ?
            
        if(rtn != -1)
            fd = open((char*)(file.append(exetention).data()),O_CREAT | O_RDWR , 0777);// should handle any text file
        std::fstream MyFile("film.mp4", std::ios::out);
        // MyFile.open("film.mp4");
        MyFile << buffer.substr(headerOfRequest.size() + 3,ContentLength);
        MyFile.flush();
        // write(fd,(void*)(buffer.substr(headerOfRequest.size() + 3,ContentLength).data()),buffer.substr(headerOfRequest.size() + 3,ContentLength).size());
        flag = 2;
        close(fd);
    }
}

// void parssingOfBody::handle_post(int len, string &headerOfRequest,string &bodyofRequest , int & ContentLength, int & i,int & flag_)
// {
// }


parssingOfBody::~parssingOfBody()
{
}