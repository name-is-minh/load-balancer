#include "LoadBalancer.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_RED     "\033[31m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_MAGENTA "\033[35m"

static std::string generateRandomIP() {
    return std::to_string(rand() % 223 + 1) + "." +
           std::to_string(rand() % 256)     + "." +
           std::to_string(rand() % 256)     + "." +
           std::to_string(rand() % 254 + 1);
}

static Request generateRandomRequest() {
    Request r;
    r.ip_in           = generateRandomIP();
    r.ip_out          = generateRandomIP();
    r.processing_time = rand() % 100 + 1;
    r.job_type        = (rand() % 2 == 0) ? 'P' : 'S';
    return r;
}

LoadBalancer::LoadBalancer(int total_servers, int run_time) : curr_sys_time(0), iterator(0), run_time(run_time), blocked_count(0), total_processed_requests(0), added_servers(0), removed_servers(0) {

    srand(static_cast<unsigned>(time(nullptr)));

    log.open("lb_log.txt");
    log << "Servers: " << total_servers << "\n";
    log << "Run time: " << run_time << " cycles\n";
    log << "--------------------------------------------------\n";

    for (int i = 0; i < total_servers; i++) {
        active_web_servers.emplace_back('A' + i);
    }

    int initial_size = total_servers * 100;
    for (int i = 0; i < initial_size; i++) {
        addRequest(generateRandomRequest());
    }
}

LoadBalancer::~LoadBalancer() {
    if (log.is_open()) log.close();
}

void LoadBalancer::addRequest(Request newRequest) {
    if (ip_block.isIPBlocked(newRequest.ip_in)) {
        incrementBlockedIP();
        std::cout << COLOR_RED << "IP " << newRequest.ip_in << " blocked at cycle " << curr_sys_time << COLOR_RESET << "\n";
        log << "IP " << newRequest.ip_in << " blocked at cycle " << curr_sys_time << "\n";
        return;
    }
    request_queue.push(newRequest);
}

void LoadBalancer::incrementBlockedIP() {
    blocked_count++;
}

void LoadBalancer::incrementTime() {
    curr_sys_time++;
}

void LoadBalancer::run() {
    const int waiting_time = 30;

    while (curr_sys_time < run_time) {
        incrementTime();

        for (auto& server : active_web_servers) {
            if (!server.isServerAvailable() && server.hasRequestCompleted(curr_sys_time)) {
                server.availability = true;
                total_processed_requests++;

                std::cout << COLOR_YELLOW << "Server " << server.server_name << " finished request at cycle " << curr_sys_time << COLOR_RESET << "\n";
                log << "Server " << server.server_name << " finished request at cycle " << curr_sys_time << "\n";
            }

            if (server.isServerAvailable() && !request_queue.empty()) {
                Request next = request_queue.front();
                request_queue.pop();
                server.createRequest(next, curr_sys_time);

                std::cout << COLOR_GREEN << "Server " << server.server_name << " got request from " << next.ip_in << " | time=" << next.processing_time << COLOR_RESET << "\n";
                log << "Server " << server.server_name << " got request from " << next.ip_in << " | time=" << next.processing_time << "\n";
            }
        }

        if (rand() % 10 == 0) {
            addRequest(generateRandomRequest());
        }

        if (curr_sys_time % waiting_time == 0) {
            int num_servers = static_cast<int>(active_web_servers.size());
            int q_size      = static_cast<int>(request_queue.size());

            if (q_size > (80 * num_servers)) {
                char new_name = 'A' + static_cast<int>(active_web_servers.size() + added_servers) % 26;
                active_web_servers.emplace_back(new_name);
                added_servers++;

                std::cout << COLOR_CYAN << "Added server " << new_name << " at time=" << curr_sys_time << ". Total active servers: " << active_web_servers.size() << COLOR_RESET << "\n";
                log << "Added server at time=" << curr_sys_time << ". Total: " << active_web_servers.size() << "\n";

            } else if (q_size < (50 * num_servers) && num_servers > 1) {
                if (active_web_servers.back().isServerAvailable()) {
                    active_web_servers.pop_back();
                    removed_servers++;

                    std::cout << COLOR_MAGENTA << "Removed server at time=" << curr_sys_time << ". Total: " << active_web_servers.size() << COLOR_RESET << "\n";
                    log << "Removed server at time=" << curr_sys_time << ". Total: " << active_web_servers.size() << "\n";
                }
            }
        }
    }
}

void LoadBalancer::getStatistics() {
    std::string summary =
        "\n--------------------------------------------------\n"
        "SUMMARY"
        "\n--------------------------------------------------\n"
        "Total clock cycles run       : " + std::to_string(run_time)                      + "\n"
        "Requests blocked             : " + std::to_string(blocked_count)                  + "\n"
        "Requests processed           : " + std::to_string(total_processed_requests)       + "\n"
        "Requests remaining in queue  : " + std::to_string(request_queue.size())           + "\n"
        "Servers added dynamically    : " + std::to_string(added_servers)                  + "\n"
        "Servers removed dynamically  : " + std::to_string(removed_servers)                + "\n"
        "Final active server count    : " + std::to_string(active_web_servers.size())      + "\n";

    std::cout << summary;
    log << summary;
}