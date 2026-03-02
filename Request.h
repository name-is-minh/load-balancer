#ifndef REQUEST_H
#define REQUEST_H

#include <string>

class Request {
    public:
        std::string ip_in;
        std::string ip_out;
        int processing_time;
        char job_type;
};

#endif