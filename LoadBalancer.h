/**
 * @file LoadBalancer.h
 * @brief Defines the LoadBalancer class for managing web servers and a request queue.
 */

#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>
#include <fstream>
#include "Request.h"
#include "WebServer.h"
#include "IPblock.h"

/**
 * @class LoadBalancer
 * @brief Manages a queue of requests and distributes them among active web servers.
 * * The LoadBalancer dynamically scales the number of active servers based on queue size,
 * blocks malicious IPs, and tracks statistics over a specified run time.
 */
class LoadBalancer {
    private:
        int curr_sys_time;                           /**< Current system clock cycle. */
        int iterator;                                /**< Utility iterator for tracking. */
        std::queue<Request> request_queue;           /**< Queue of pending requests. */
        std::vector<WebServer> active_web_servers;   /**< List of currently active servers. */
        IPblock ip_block;                            /**< Firewall instance for checking IPs. */
        
        std::ofstream log;                           /**< File stream for logging events. */
        int run_time;                                /**< Total clock cycles to run the simulation. */
        int blocked_count;                           /**< Total number of blocked requests. */
        int total_processed_requests;                /**< Total number of successfully completed requests. */
        int added_servers;                           /**< Count of dynamically added servers. */
        int removed_servers;                         /**< Count of dynamically removed servers. */

    public:
        /**
         * @brief Constructor for the LoadBalancer.
         * @param total_servers Initial number of web servers.
         * @param run_time Total system clock cycles the simulation will run.
         */
        LoadBalancer(int total_servers, int run_time);

        /**
         * @brief Destructor to ensure the log file is safely closed.
         */
        ~LoadBalancer();

        /**
         * @brief Validates and adds a new request to the queue.
         * @param new_request The request to be added.
         */
        void addRequest(Request new_request);

        /**
         * @brief Increments the system clock cycle by 1.
         */
        void incrementTime();

        /**
         * @brief Increments the counter for blocked IPs.
         */
        void incrementBlockedIP();

        /**
         * @brief The main simulation loop that manages servers, routing, and dynamic scaling.
         */
        void run();

        /**
         * @brief Outputs final simulation metrics to the console and log file.
         */
        void getStatistics();
};

#endif