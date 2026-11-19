#include <iostream>

#include "CANBus.h"
#include "Module.h"
#include "CANFrame.h"
#include "Config.h"

using namespace std;

int main() {
    unsigned long long current_time = 0;

    vector<Module> modules;

    CANBus bus = CANBus(500000);
    Module silnik = Module("Silnik", vector<MsgType>{{"Temperatura", 0x010, 4, 10}, MsgType {"Ciśnienie", 0x020, 4, 20}});

    modules.push_back(silnik);

    while (true) {
        // generating messages
        for (Module& module : modules) {
            module.update(bus);
        }

        // bus handling the messages
        bus.update();


        cout << bus.get_queue_size() << "\n";
        cout << "=================\ntime: " << current_time << "\n==================\n";

        // time unit: micro seconds
        current_time += 1;
    }
    return 0;
}