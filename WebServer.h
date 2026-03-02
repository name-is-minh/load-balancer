#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

class WebServer {
    public:
        Request request;
        int start_time;
        char server_name;
        bool availability;

        WebServer(char name);
        void createRequest(Request new_request, int current_time);
        bool hasRequestCompleted(int current_time);
        bool isServerAvailable();
};

#endif