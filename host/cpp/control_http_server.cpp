#include "control_http_server.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <string>

namespace {

std::string query_value(const std::string& query, const std::string& key) {
    const std::string prefix = key + "=";
    std::size_t start = 0;

    while (start < query.size()) {
        std::size_t end = query.find('&', start);
        if (end == std::string::npos) end = query.size();

        const std::string item = query.substr(start, end - start);
        if (item.rfind(prefix, 0) == 0) {
            return item.substr(prefix.size());
        }

        start = end + 1;
    }

    return {};
}

} // namespace

ControlHttpServer::ControlHttpServer(uint16_t port)
    : port_(port) {}

void ControlHttpServer::register_default_robots() {
    bridge_.register_robot("R1", "192.168.1.101");
    bridge_.register_robot("R2", "192.168.1.102");
    bridge_.register_robot("R3", "192.168.1.103");
    bridge_.register_robot("R4", "192.168.1.104");
    bridge_.register_robot("R5", "192.168.1.105");
}

std::string ControlHttpServer::json_response(
    bool ok,
    const std::string& message) const {

    std::ostringstream body;
    body << "{\"ok\":" << (ok ? "true" : "false")
         << ",\"message\":\"" << message << "\"}";

    std::ostringstream response;
    response << "HTTP/1.1 " << (ok ? "200 OK" : "400 Bad Request") << "\r\n"
             << "Content-Type: application/json\r\n"
             << "Access-Control-Allow-Origin: *\r\n"
             << "Access-Control-Allow-Methods: GET, OPTIONS\r\n"
             << "Content-Length: " << body.str().size() << "\r\n"
             << "Connection: close\r\n\r\n"
             << body.str();

    return response.str();
}

std::string ControlHttpServer::handle_request(
    const std::string& request) {

    const std::size_t line_end = request.find("\r\n");
    if (line_end == std::string::npos) {
        return json_response(false, "invalid request");
    }

    const std::string line = request.substr(0, line_end);
    std::istringstream parser(line);

    std::string method;
    std::string target;
    parser >> method >> target;

    if (method == "OPTIONS") {
        return "HTTP/1.1 204 No Content\r\n"
               "Access-Control-Allow-Origin: *\r\n"
               "Access-Control-Allow-Methods: GET, OPTIONS\r\n"
               "Access-Control-Allow-Headers: Content-Type\r\n"
               "Connection: close\r\n\r\n";
    }

    if (method != "GET") {
        return json_response(false, "only GET is supported");
    }

    if (target == "/api/emergency-stop") {
        bridge_.emergency_stop_all();
        return json_response(true, "emergency stop sent");
    }

    const std::size_t query_pos = target.find('?');
    const std::string path = target.substr(0, query_pos);
    const std::string query =
        query_pos == std::string::npos ? "" : target.substr(query_pos + 1);

    if (path == "/api/command") {
        const std::string robot = query_value(query, "robot");
        const std::string action = query_value(query, "action");
        const std::string speed_text = query_value(query, "speed");

        if (robot.empty() || action.empty()) {
            return json_response(false, "robot and action are required");
        }

        int speed = speed_text.empty() ? 0 : std::atoi(speed_text.c_str());
        speed = std::clamp(speed, 0, 100);

        const bool ok = bridge_.command(
            robot,
            action,
            static_cast<uint8_t>(speed));

        return json_response(ok, ok ? "command sent" : "command failed");
    }

    return json_response(false, "unknown endpoint");
}

void ControlHttpServer::run() {
    const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        return;
    }

    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port_);

    if (bind(server_fd,
             reinterpret_cast<sockaddr*>(&address),
             sizeof(address)) < 0 ||
        listen(server_fd, 8) < 0) {
        close(server_fd);
        return;
    }

    while (true) {
        const int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) continue;

        char buffer[4096]{};
        const ssize_t received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (received > 0) {
            const std::string request(buffer, static_cast<std::size_t>(received));
            const std::string response = handle_request(request);
            send(client_fd, response.data(), response.size(), 0);
        }

        close(client_fd);
    }
}
