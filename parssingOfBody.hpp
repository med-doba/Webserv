#ifndef PARSSINGOFBODY_HPP
#define PARSSINGOFBODY_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER 1024

class parssingOfBody
{
    private:

    public:
        std::string             file;
        std::string             exetention;
        int                     fd;

        parssingOfBody(/* args */);
        void putDataTofile(std::string  data, std::string & bodyofRequest);
        void handling_form_data(std::string& buffer, std::string &boundary,std::string &bodyofRequest,  int &total_bytes_received,unsigned long &ContentLength, int  &, int &);
        void handle_post(std::string &headerOfRequest, std::string &buffer, unsigned long &ContentLength, int &i, int &flag);
        void handle_post(std::string &headerOfRequest,std::string &bodyofRequest, unsigned long & ContentLength, int & i,int & flag_,int &);
        void handling_chunked_data(std::string &buffer,std::string &headerOfRequest, std::string &bodyofRequest, int & flag_);
        ~parssingOfBody();
};



#endif
