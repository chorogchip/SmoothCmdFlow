#include <iostream>
#include <string>

#include "SmoothCmdFlowDevice.h"
#include "testexamples.h"

int main() {
    scf::SmoothCmdFlowSetup setup = scf::testexamples::make_minecraft_like_setup();

    scf::SmoothCmdFlowDevice device;
    device.bind_setup(&setup);

    std::cout << "SmoothCmdFlow terminal\n";
    std::cout << "type 'exit' or 'quit' to close\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line == "exit" || line == "quit") {
            break;
        }

        device.set_string(line);

        if (device.execute()) {
            std::cout << "ok\n";
        } else {
            std::cout << "error: " << device.get_last_error_msg() << "\n";
        }
    }

    return 0;
}
