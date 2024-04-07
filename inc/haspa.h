#ifndef HASPA_H
#define HASPA_H


#include <vector>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include "taspa.h"
#include "haspa_hg.h"
#include "ganntplot.h"
#define IDLE 0
#define BUSY 1

#define SOFT 0
#define HARD 1

struct ConstraintSolution {
    boost::dynamic_bitset<> variables;
    float gain=0;
    int s=0;
};


struct CoreAttribute
{
    int corenum;
    int hardwarenum;
};

struct taskAttribute
{
    int taskNum;
    int areaConstraints;
    int softwaretimeSum=0;
    std::vector<int> sortExectime;
    std::vector<int> hardExectime;
    std::vector<int> hardwareArea;
    std::vector<int> hardawaregain;
    std::vector<float> hardwaregainavg;
    std::vector<int> taskorder;
    std::vector<int> relesetime;
    std::vector<std::vector<int>> CommunicationCost;
};



typedef struct
{
    QString job_name;//作业名字
    int need_time;//需要时间
    int exec_time;//执行时间
    int wait_time;//等待时间
    int arrive_time;//到达时间(开始时间)
    int finish_time;//结束时间
    int level;//级别
    double arround_time;
    double weight_arround_time;

}PCB;

struct Coretype
{
    int runningtask;
    int status=IDLE;//状态idel busy 空闲或忙碌
    int busytime=0;//忙碌的时间
    int idletime=0;
    std::vector<int> finishtask;
};


class haspa
{
public:
    std::vector<Coretype> cpu;
    std::vector<Coretype> fpga;
    haspa(int tasknum);
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
    int runtime=0;
    QString result_output();
    QString output;
    std::vector<GanntInfo> Ganntinfo;

};

#endif // HASPA_H
