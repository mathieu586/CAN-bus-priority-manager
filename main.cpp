#include <iostream>

#include "CANBus.h"
#include "Module.h"
#include "CANFrame.h"
#include "Config.h"

using namespace std;

int main() {
    unsigned long long current_time = 0;

    vector<Module> modules;
    int default_speed = 500000;
    int slow_speed = 10000;

    CANBus bus = CANBus(slow_speed);
    Module silnik = Module("Silnik", vector<MsgType>{
        {"Temperatura", 0x010, 4,ms_to_micro_s(10), ms_to_micro_s(50)},
        {"Ciśnienie", 0x020, 4, ms_to_micro_s(20), ms_to_micro_s(10)},
        {"Poziom paliwa", 0x050, 4, ms_to_micro_s(100)}});
    Module zasilanie = Module("Zasilanie", vector<MsgType>{
        {"Napiecie akumulatora", 0x100, 2, ms_to_micro_s(100)}});
    Module nawigacja = Module("Nawigacja", vector<MsgType>{
        {"Pozycja GPS", 0x200, 8, ms_to_micro_s(200), ms_to_micro_s(100)},
        {"Nachylenie", 0x150, 6, ms_to_micro_s(100), ms_to_micro_s(50)},
        {"Przyśpieszenie", 0x030, 4, ms_to_micro_s(30), ms_to_micro_s(20)}});
    Module diagnostyka = Module("Diagnostyka", vector<MsgType>{
        {"Status systemu", 0x300, 4, ms_to_micro_s(500)}});

    modules.push_back(silnik);
    modules.push_back(zasilanie);
    modules.push_back(nawigacja);
    modules.push_back(diagnostyka);

    vector<Module> optimized_id_modules;

    Module silnik_optimized = Module("Silnik", vector<MsgType>{
        {"Temperatura", 0x030, 4,ms_to_micro_s(10), ms_to_micro_s(50)},
        {"Ciśnienie", 0x010, 4, ms_to_micro_s(20), ms_to_micro_s(10)},
        {"Poziom paliwa", 0x120, 4, ms_to_micro_s(100)}});
    Module zasilanie_optimized = Module("Zasilanie", vector<MsgType>{
        {"Napiecie akumulatora", 0x200, 2, ms_to_micro_s(100)}});
    Module nawigacja_optimized = Module("Nawigacja", vector<MsgType>{
        {"Pozycja GPS", 0x110, 8, ms_to_micro_s(200), ms_to_micro_s(100)},
        {"Nachylenie", 0x100, 6, ms_to_micro_s(100), ms_to_micro_s(50)},
        {"Przyśpieszenie", 0x020, 4, ms_to_micro_s(30), ms_to_micro_s(20)}});
    Module diagnostyka_optimized = Module("Diagnostyka", vector<MsgType>{
        {"Status systemu", 0x300, 4, ms_to_micro_s(500)}});

    optimized_id_modules.push_back(silnik_optimized);
    optimized_id_modules.push_back(zasilanie_optimized);
    optimized_id_modules.push_back(nawigacja_optimized);
    optimized_id_modules.push_back(diagnostyka_optimized);

    while (current_time < 50000000) {
        // generating messages
        for (Module& module : optimized_id_modules) {
            module.update(bus, current_time);
        }


        // bus logic
        bus.update(current_time);

     //   cout << "=================\ntime: " << current_time << "\n";
     //   cout << "Bus queue size: " << bus.get_queue_size() << "\n";
     //   cout << "Late masseges count: " << bus.get_late_msg_counter() << "\n";
     //   cout  << "\n==================\n";

        // time unit: micro seconds
        current_time += 1;
    }
    bus.print_msg_stats();

    return 0;
}