#ifndef PARSSINGOFBODY_HPP
#define PARSSINGOFBODY_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER 10000

class parssingOfBody
{
    private:

    public:
        std::string             file;
        std::string             exetention;
        int                     index;
        int                     fd;

        parssingOfBody(/* args */);

        void handle_post(std::string &headerOfRequest,std::string &bodyofRequest, unsigned long & ContentLength, unsigned long & i, int& flag);
        
        ~parssingOfBody();
};


#endif