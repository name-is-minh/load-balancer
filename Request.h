/**
 * @file Request.h
 * @brief Defines the Request class which holds data for individual network requests.
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @class Request
 * @brief Represents a single network request processed by the load balancer.
 * * Contains information about the source IP, destination IP, the time required 
 * to process the job, and the type of job (Processing or Streaming).
 */
class Request {
    public:
        std::string ip_in;         /**< The incoming IP address of the requester. */
        std::string ip_out;        /**< The outgoing IP address for the results. */
        int processing_time;       /**< Time in clock cycles required to complete the request. */
        char job_type;             /**< 'P' for Processing, 'S' for Streaming. */
};

#endif