/**
 * @file WebServer.h
 * @brief Defines the WebServer class for processing requests.
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @class WebServer
 * @brief Simulates a single web server that processes requests.
 */
class WebServer {
    public:
        Request request;       /**< The current request being processed. */
        int start_time;        /**< The system cycle when the current request started. */
        char server_name;      /**< A character identifier for the server (e.g., 'A', 'B'). */
        bool availability;     /**< True if the server is free, false if currently processing. */

        /**
         * @brief Constructor for the WebServer.
         * @param name A character representing the server's name.
         */
        WebServer(char name);

        /**
         * @brief Assigns a new request to the server.
         * @param new_request The request object to be processed.
         * @param current_time The current system clock cycle.
         */
        void createRequest(Request new_request, int current_time);

        /**
         * @brief Checks if the server has finished its current request.
         * @param current_time The current system clock cycle.
         * @return True if the request is done, false otherwise.
         */
        bool hasRequestCompleted(int current_time);

        /**
         * @brief Checks the server's availability.
         * @return True if available, false if busy.
         */
        bool isServerAvailable();
};

#endif