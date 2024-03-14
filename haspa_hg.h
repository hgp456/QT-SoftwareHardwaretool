#ifndef HASPA_HG_H
#define HASPA_HG_H

#include <vector>

#define HARDWARE 1
#define SOFTWARE 0
#define UNDICIED 2
struct ProcessunitBlock
{
    int id;
    int time;
    std::vector<int> pre;
    std::vector<int> succ;
    int ESTCheckTime;
    int releasetime;
    int earliestStartTime;
    int latestStartTime;
    int need_time;//需要时间
    int exec_time;//执行时间
    int wait_time;//等待时间
    int arrive_time;//到达时间(开始时间)
    int finish_time;//结束时间
    bool finfish=false;
    int hardwareuse=SOFTWARE;
    bool running=false;
    int  usedResouceIndex=0;
};


class HASPA_HG
{
public:
    HASPA_HG();
    int run();
    int Schedule(std::vector<ProcessunitBlock> &Task);
};

#endif // HASPA_HG_H
