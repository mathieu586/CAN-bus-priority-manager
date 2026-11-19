#ifndef ZADANIE_2_CANFRAME_H
#define ZADANIE_2_CANFRAME_H
#include <cstdint>

class CANFrame {
private:
    int size;
    int id;
public:
    CANFrame(int size, int id);
};

#endif //ZADANIE_2_CANFRAME_H