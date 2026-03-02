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

/**
 * @brief Generates a random IP address string.
 *
 * Creates a random IPv4 address with valid octets:
 * - First octet: 1-223 (public IP range)
 * - Second and third octets: 0-255
 * - Fourth octet: 1-254 (avoiding network/broadcast addresses)
 *
 * @return A randomly generated IP address as a string (e.g., "192.168.45.127").
 */
static std::string generateRandomIP() {
    return std::to_string(rand() % 223 + 1) + "." +
           std::to_string(rand() % 256)     + "." +
           std::to_string(rand() % 256)     + "." +
           std::to_string(rand() % 254 + 1);
}

/**
 * @brief Generates a random request for the simulation.
 *
 * Creates a Request object with randomly generated source and destination
 * IP addresses, a random processing time (1-100 cycles), and a random job
 * type (Processing or Streaming).
 *
 * @return A Request object with randomized fields.
 */
static Request generateRandomRequest() {
    Request r;
    r.ip_in           = generateRandomIP();
    r.ip_out          = generateRandomIP();
    r.processing_time = rand() % 100 + 1;
    r.job_type        = (rand() % 2 == 0) ? 'P' : 'S';
    return r;
}

/**
 * @brief Constructs a LoadBalancer instance and initializes the simulation.
 *
 * Sets up the load balancer by:
 * - Seeding the random number generator
 * - Opening the log file
 * - Creating the specified number of web servers
 * - Populating the initial request queue with servers*100 requests
 *
 * @param total_servers The number of web servers to create initially.
 * @param run_time The total duration of the simulation in clock cycles.
 */
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

/**
 * @brief Destroys the LoadBalancer and closes the log file.
 */
LoadBalancer::~LoadBalancer() {
    if (log.is_open()) log.close();
}

/**
 * @brief Adds a request to the queue, checking firewall rules first.
 *
 * Validates the request's source IP against the IP block list (firewall).
 * If the IP is blocked, increments the blocked counter and logs the event.
 * Otherwise, enqueues the request for processing.
 *
 * @param new_request The request to add to the queue.
 */
void LoadBalancer::addRequest(Request new_request) {
    if (ip_block.isIPBlocked(new_request.ip_in)) {
        incrementBlockedIP();
        std::cout << COLOR_RED << "IP " << new_request.ip_in << " blocked at cycle " << curr_sys_time << COLOR_RESET << "\n";
        log << "IP " << new_request.ip_in << " blocked at cycle " << curr_sys_time << "\n";
        return;
    }
    request_queue.push(new_request);
}

/**
 * @brief Increments the counter of blocked IP requests.
 *
 * Called when a request from a blocked IP address is rejected.
 */
void LoadBalancer::incrementBlockedIP() {
    blocked_count++;
}

/**
 * @brief Advances the simulation clock by one cycle.
 */
void LoadBalancer::incrementTime() {
    curr_sys_time++;
}

/**
 * @brief Executes the main simulation loop.
 *
 * The simulation runs for the specified duration, cycling through:
 * - Time advancement and request completion detection
 * - Server assignment of queued requests
 * - Random injection of new requests (~10% per cycle)
 * - Dynamic server scaling (every 30 cycles):
 *   - Add server if queue > 80*server_count
 *   - Remove server if queue < 50*server_count and server is idle
 *
 * All activity is logged to the log file and displayed with color output.
 */
void LoadBalancer::run() {
    const int waiting_time = 30;

    // Continue simulation until the configured runtime is reached.
    while (curr_sys_time < run_time) {
        // 1) Advance global simulation clock by one cycle.
        incrementTime();

        // 2) For each server, first check completion, then assign new work if available.
        for (auto& server : active_web_servers) {
            // If this busy server has finished its current request, mark stats and log it.
            if (!server.isServerAvailable() && server.hasRequestCompleted(curr_sys_time)) {
                server.availability = true;
                total_processed_requests++;

                std::cout << COLOR_YELLOW << "Server " << server.server_name << " finished request at cycle " << curr_sys_time << COLOR_RESET << "\n";
                log << "Server " << server.server_name << " finished request at cycle " << curr_sys_time << "\n";
            }

            // If server is idle and queue has pending requests, dispatch next request.
            if (server.isServerAvailable() && !request_queue.empty()) {
                Request next = request_queue.front();
                request_queue.pop();
                server.createRequest(next, curr_sys_time);

                std::cout << COLOR_GREEN << "Server " << server.server_name << " got request from " << next.ip_in << " | time=" << next.processing_time << COLOR_RESET << "\n";
                log << "Server " << server.server_name << " got request from " << next.ip_in << " | time=" << next.processing_time << "\n";
            }
        }

        // 3) Simulate incoming traffic: ~10% chance to add one new random request.
        if (rand() % 10 == 0) {
            addRequest(generateRandomRequest());
        }

        // 4) Periodically evaluate queue pressure and adjust server count.
        if (curr_sys_time % waiting_time == 0) {
            int num_servers = static_cast<int>(active_web_servers.size());
            int q_size      = static_cast<int>(request_queue.size());

            // Scale up when queue is above upper threshold (80 * active servers).
            if (q_size > (80 * num_servers)) {
                char new_name = 'A' + static_cast<int>(active_web_servers.size() + added_servers) % 26;
                active_web_servers.emplace_back(new_name);
                added_servers++;

                std::cout << COLOR_CYAN << "Added server " << new_name << " at time=" << curr_sys_time << ". Total active servers: " << active_web_servers.size() << COLOR_RESET << "\n";
                log << "Added server at time=" << curr_sys_time << ". Total: " << active_web_servers.size() << "\n";

            // Scale down when queue is below lower threshold (50 * active servers),
            // but keep at least one server alive and only remove if the last one is idle.
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

/**
 * @brief Prints and logs a summary of simulation statistics.
 *
 * Displays:
 * - Total clock cycles executed
 * - Requests blocked by firewall
 * - Requests successfully processed
 * - Requests remaining in queue
 * - Servers dynamically added
 * - Servers dynamically removed
 * - Final active server count
 */
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