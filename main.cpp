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
    Module silnik = Module("Silnik", vector<MsgType>{{"Temperatura", 0x010, 4,ms_to_micro_s(10), ms_to_micro_s(50)},
            {"Ciśnienie", 0x020, 4, ms_to_micro_s(20), ms_to_micro_s(10)}});
    Module zasilanie = Module("Zasilanie", vector<MsgType>{{"Napiecie akumulatora", 0x100, 2, ms_to_micro_s(100)}});
    Module nawigacja = Module("Nawigacja", vector<MsgType>{{"Pozycja GPS", 0x200, 8, ms_to_micro_s(200)}});
    Module diagnostyka = Module("Diagnostyka", vector<MsgType>{{"Status systemu", 0x300, 4, ms_to_micro_s(500)}});


    modules.push_back(silnik);
    modules.push_back(zasilanie);
    modules.push_back(nawigacja);
    modules.push_back(diagnostyka);

    while (true) {
        // generating messages
        for (Module& module : modules) {
            module.update(bus, current_time);
        }

        // bus logic
        bus.update(current_time);

        cout << "=================\ntime: " << current_time << "\n";
        cout << "Bus queue size: " << bus.get_queue_size() << "\n";
        cout << "Late masseges count: " << bus.get_late_msg_counter() << "\n";
        cout  << "\n==================\n";

        // time unit: micro seconds
        current_time += 1;
    }
    return 0;
}