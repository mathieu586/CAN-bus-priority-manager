#ifndef ZADANIE_2_CANFRAME_H
#define ZADANIE_2_CANFRAME_H
#include <cstdint>

class CANFrame {
private:
    int size;
    int id;
    unsigned long long created_at;
    unsigned long long deadline_at;
    unsigned long long delay;
public:
    CANFrame(int size, int id, unsigned long long created_at, unsigned long long deadline);
    bool operator<(const CANFrame& other) const;
    int get_size() const;
    int get_deadline_at() const;
    void set_delay(unsigned long long delay_val);
    unsigned long long get_created_at();
};

#endif //ZADANIE_2_CANFRAME_H