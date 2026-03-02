#include "WebServer.h"

/**
 * @brief Constructs a WebServer with the given name.
 *
 * Initializes the server in an available state with no active request time.
 *
 * @param name Character identifier for the server (e.g., 'A', 'B', 'C').
 */
WebServer::WebServer(char name) {
    server_name = name;
    availability = true;
    start_time = 0;
}

/**
 * @brief Assigns a request to this server and marks it busy.
 *
 * Stores the incoming request and records the current simulation time as
 * the request start time.
 *
 * @param new_request The request to process.
 * @param current_time The current simulation cycle when processing starts.
 */
void WebServer::createRequest(Request new_request, int current_time) {
    request = new_request;
    start_time = current_time;
    availability = false;
}

/**
 * @brief Checks whether the current request has finished processing.
 *
 * A request is complete when the current simulation time is at least
 * start_time + processing_time. If complete, the server is marked available.
 *
 * @param current_time The current simulation cycle.
 * @return true if the request has completed during this check; otherwise false.
 */
bool WebServer::hasRequestCompleted(int current_time) {
    if (!availability && current_time >= (start_time + request.processing_time)) {
        availability = true;
        return true;
    }
    return false;
}

/**
 * @brief Indicates whether the server is currently available.
 *
 * @return true if the server is idle and can accept a new request; otherwise false.
 */
bool WebServer::isServerAvailable() {
    return availability;
}