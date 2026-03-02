#include "WebServer.h"

WebServer::WebServer(char name) {
    server_name = name;
    availability = true;
    start_time = 0;
}

void WebServer::createRequest(Request new_request, int current_time) {
    request = new_request;
    start_time = current_time;
    availability = false;
}

bool WebServer::hasRequestCompleted(int current_time) {
    if (!availability && current_time >= (start_time + request.processing_time)) {
        availability = true;
        return true;
    }
    return false;
}

bool WebServer::isServerAvailable() {
    return availability;
}