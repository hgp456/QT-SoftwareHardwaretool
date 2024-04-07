#ifndef ESHC_H
#define ESHC_H

#include "haspa.h"
#include "ganntplot.h"

class eshc
{
public:
    std::vector<Coretype> cpu;
    std::vector<Coretype> fpga;
    taskAttribute taskattribute;
    std::vector<ProcessunitBlock> TaskList;
    std::vector<GanntInfo> Ganntinfo;
    Taspa taspa;
    eshc(int tasknum);
    int run();
    int task_init(std::vector<ProcessunitBlock> &Task);
};

#endif // ESHC_H
