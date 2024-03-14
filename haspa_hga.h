#ifndef HASPA_HGA_H
#define HASPA_HGA_H
#include "haspa_hg.h"
class haspa_hga
{
public:
    haspa_hga();
    int run();
    int Schedule(std::vector<ProcessunitBlock> &Task);
};

#endif // HASPA_HGA_H
