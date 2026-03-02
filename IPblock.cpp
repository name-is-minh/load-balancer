#include "IPblock.h"
#include <sstream>

IPblock::IPblock() {
    ip_block_start.push_back({192, 168, 0, 0});
    ip_block_end.push_back({192, 168, 255, 255});
    
    ip_block_start.push_back({10, 0, 0, 0});
    ip_block_end.push_back({10, 255, 255, 255});
}

std::array<int, 4> IPblock::splitIP(const std::string& ip) {
    std::array<int, 4> ip_address_array = {0, 0, 0, 0};
    std::stringstream ip_address_ss(ip);
    std::string ip_address_octet;
    int index = 0;
    
    while (std::getline(ip_address_ss, ip_address_octet, '.') && index < 4) {
        ip_address_array[index++] = std::stoi(ip_address_octet);
    }
    return ip_address_array;
}

bool IPblock::isIPBlocked(const std::string& ip) {
    std::array<int, 4> ip_parts = splitIP(ip);
    
    for (size_t i = 0; i < ip_block_start.size(); i++) {
        bool is_in_range = true;

        for (int j = 0; j < 4; j++) {
            if (ip_parts[j] < ip_block_start[i][j]) {
                is_in_range = false;
                break;
            }
            if (ip_parts[j] > ip_block_end[i][j]) {
                is_in_range = false;
                break;
            }
        }
        
        if (is_in_range) {
            return true;
        }
    }
    return false;
}