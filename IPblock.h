#ifndef IPBLOCK_H
#define IPBLOCK_H

#include <string>
#include <vector>
#include <array>

class IPblock {
    public:
        std::vector<std::array<int, 4>> ip_block_start;
        std::vector<std::array<int, 4>> ip_block_end;

        IPblock();

        std::array<int, 4> splitIP(const std::string& ip);
        
        bool isIPBlocked(const std::string& ip);
};

#endif