#ifndef PARSSINGOFBODY_HPP
#define PARSSINGOFBODY_HPP

#include <iostream>

class parssingOfBody
{
    private:

    public:
        std::string             file;
        std::string             exetention;
        int                     fd;

        parssingOfBody(/* args */);
        void handle_post(int len,std::string &bodyofRequest ,std::string &headerOfRequest,std::string & , unsigned long & ContentLength, int &i,int & flag_,int &);
        void handling_chunked_data(std::string &buffer,std::string &headerOfRequest, std::string &boundary,std::string &bodyofRequest, int & total_bytes_received, unsigned long & ContentLength, int & i, int & bytes_received,int & flag_);
        void handling_form_data(std::string& buffer, std::string &headerOfRequest, std::string &boundary,std::string & bodyofRequest ,int &total_bytes_received,unsigned long &ContentLength,  int & i, int & bytes_received, int & flag_);
        void putDataTofile(std::string  data, std::string & bodyofRequest);
        void  create_file_and_put_content(std::string & bodyofRequest,std::string & headerOfRequest);

        ~parssingOfBody();
};


#endif
