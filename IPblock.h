/**
 * @file IPblock.h
 * @brief Defines the IPblock class to act as a firewall.
 */

#ifndef IPBLOCK_H
#define IPBLOCK_H

#include <string>
#include <vector>
#include <array>

/**
 * @class IPblock
 * @brief Manages blocked IP ranges and checks incoming IPs against them.
 */
class IPblock {
    public:
        std::vector<std::array<int, 4>> ip_block_start; /**< Starting IPs for blocked ranges. */
        std::vector<std::array<int, 4>> ip_block_end;   /**< Ending IPs for blocked ranges. */

        /**
         * @brief Constructor that initializes default blocked IP ranges.
         */
        IPblock();

        /**
         * @brief Splits a string IP address into an array of 4 integers.
         * @param ip The IP address string (e.g., "192.168.1.1").
         * @return An array of 4 integers representing the octets.
         */
        std::array<int, 4> splitIP(const std::string& ip);
        
        /**
         * @brief Checks if a given IP address falls within any blocked range.
         * @param ip The IP address string to check.
         * @return True if the IP is blocked, false if it is allowed.
         */
        bool isIPBlocked(const std::string& ip);
};

#endif