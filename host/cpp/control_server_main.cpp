#include "control_http_server.h"

int main() {
    ControlHttpServer server(8080);
    server.register_default_robots();
    server.run();
    return 0;
}
