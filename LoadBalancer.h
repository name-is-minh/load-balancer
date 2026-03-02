#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>
#include <fstream>
#include "Request.h"
#include "WebServer.h"
#include "IPblock.h"

class LoadBalancer {
    private:
        int curr_sys_time;
        int iterator;
        std::queue<Request> request_queue;
        std::vector<WebServer> active_web_servers;
        IPblock ip_block;
        
        std::ofstream log;
        int run_time;
        int blocked_count;
        int total_processed_requests;
        int added_servers;
        int removed_servers;

    public:
        LoadBalancer(int total_servers, int run_time);
        ~LoadBalancer();

        void addRequest(Request newRequest);
        void incrementTime();
        void incrementBlockedIP();
        void run();
        void getStatistics();
};

#endif