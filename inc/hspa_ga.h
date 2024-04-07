#ifndef HSPA_GA_H
#define HSPA_GA_H

#include "haspa.h"
//#include "ganntplot.h"
class hspa_ga
{
public:
    hspa_ga(int tasknum);
    std::vector<Coretype> cpu;
    std::vector<Coretype> fpga;
    taskAttribute taskattribute;
    CoreAttribute Core;
    ConstraintSolution constraintsolution;
    ConstraintSolution bestSolution;
    Taspa taspa;
    std::vector<ProcessunitBlock> TaskList;
    std::vector<int> relytasklist;
    std::vector<int> ScheduletimeResult;
    std::vector<int> HardwareUseSituation;
    std::vector<float> fitnessUnion;
    int task_init(std::vector<ProcessunitBlock> &Task);
    int hardwaregainCalculation();
    int constraint_calculation();
    int scheduling();
    int run();
    int datastructtrans();
    int Schedule(std::vector<ProcessunitBlock> &Task);
    int checkresourceCPUStatus();
    int checkresourceFPGAStatus();
    int logresourceCPUStatus();
    int logresourceFPGAStatus();
    int GetHardWareParition();
    int squreUsed=0;
    int task_reinit(std::vector<ProcessunitBlock> &Task);
    int runtime=0;
    int geneticAlgorithm();
    int GeneAlpha();
    std::vector<GanntInfo> Ganntinfo;
//    std::vector<boost::dynamic_bitset<>> Group;
    std::vector<boost::dynamic_bitset<>> crossover(const std::vector<boost::dynamic_bitset<>>& group,int tasknum);
    std::vector<boost::dynamic_bitset<>> mutate(const std::vector<boost::dynamic_bitset<>>& group);

    QString result_output();
    QString output;

};

#endif // HSPA_GA_H
