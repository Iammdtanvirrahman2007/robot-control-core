#include "../manager/multi_robot_manager.h"

#include <iostream>

int main() {
    MultiRobotManager manager;

    manager.add_robot("R1", "192.168.1.101");
    manager.add_robot("R2", "192.168.1.102");
    manager.add_robot("R3", "192.168.1.103");
    manager.add_robot("R4", "192.168.1.104");
    manager.add_robot("R5", "192.168.1.105");

    if (!manager.connect_all()) {
        std::cerr << "One or more robots failed to connect.\n";
    }

    if (auto* r1 = manager.robot("R1")) {
        r1->forward(50);
    }

    if (auto* r2 = manager.robot("R2")) {
        r2->stop();
    }

    manager.stop_all();

    return 0;
}
