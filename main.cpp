#include <iostream>
#include "LoadBalancer.h"

int main() {
    int total_servers;
    int run_time;

    std::cout << "Enter number of servers: ";
    std::cin >> total_servers;

    std::cout << "Enter the time you want to run the load balancer: ";
    std::cin >> run_time;

    LoadBalancer loadBalancer(total_servers, run_time);

    loadBalancer.run();
    loadBalancer.getStatistics();

    return 0;
}