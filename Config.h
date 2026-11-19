#pragma once
#ifndef ZADANIE_2_CONFIG_H
#define ZADANIE_2_CONFIG_H

#define FRAME_OVERHEAD 44
#define FRAME_ISF 3
#define CAN_BITRATE 500000 // bits/s
#define TIME_TO_PROCESS_1_BIT (1000000 / CAN_BITRATE) // result: 2 micro s

#endif //ZADANIE_2_CONFIG_H