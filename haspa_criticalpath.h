#ifndef HASPA_CRITICALPATH_H
#define HASPA_CRITICALPATH_H

#include "haspa.h"
#include "ganntplot.h"
class HASPA_CRITICALPATH
{
public:
    HASPA_CRITICALPATH(int tasknum);
    std::vector<Coretype> cpu;
    std::vector<Coretype> fpga;
    taskAttribute taskattribute;
    CoreAttribute Core;
    ConstraintSolution constraintsolution;
    ConstraintSolution bestSolution;
    Taspa taspa;
    std::vector<ProcessunitBlock> TaskList;
    std::vector<int> relytasklist;
    int task_init(std::vector<ProcessunitBlock> &Task);
    int hardwaregainCalculation();
    int constraint_calculation();
    int scheduling();
    int run();
    int datastructtrans();
    int critical_path();
    int Schedule(std::vector<ProcessunitBlock> &Task);
    int checkresourceCPUStatus();
    int checkresourceFPGAStatus();
    int logresourceCPUStatus();
    int logresourceFPGAStatus();
    int GetHardWareParition();
    int squreUsed=0;
    int task_reinit(std::vector<ProcessunitBlock> &Task);
    std::vector<GanntInfo> Ganntinfo;

    int runtime=0;
    void calculateEarliestStartTime(std::vector<ProcessunitBlock> &Task);
    void calculateLatestStartTime(std::vector<ProcessunitBlock> &Task);
    std::vector<int> critical_path_calculate(std::vector<ProcessunitBlock> &Task);
    std::vector<int> calculateCriticalPath(std::vector<ProcessunitBlock> &Task);
    QString result_output();
    QString output;

};

#endif // HASPA_CRITICALPATH_H
