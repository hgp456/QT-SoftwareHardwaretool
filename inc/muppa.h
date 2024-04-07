#ifndef MUPPA_H
#define MUPPA_H
#include <QVector>
#define IDLE 0
#define BUSY 1
#include "taspa.h"
#include "ganntplot.h"

struct muppatasktype
{
    int num;//编号
    int value;//价值
    int privalue;//优先级值
    int retime;//释放时间
    int out;//出度
    int order;//执行顺序
    int sameprivalue;//同样优先级值处理
    bool finish=false;   //是否完成
    bool running=false;
    std::vector<int> relyFunction;
//    int relyFunction[MAX]; //依赖的前置函数
    int relyNum; //依赖的个数
};

struct coretype
{
    int runningtask;
    int status;//状态idel busy 空闲或忙碌
    int lefttime;//剩下的时间
    int busytime;//忙碌的时间
    std::vector<int> finishjob;
};

class muppa
{
public:
    muppa(int max);
    void core_init();
    int CORENUM;
    int MAX;
    std::vector<coretype> core;
    std::vector<int> taskTable;
    std::vector<muppatasktype> task;
    bool RelyFinish(int tasknum);
    int order_print();
    int Scheduling();
    int time;
    int Run();
    void task_init();
    int rely_anas();
    Taspa t;
    std::vector<GanntInfo> Ganntinfo;
    QString result_output();
};

#endif // MUPPA_H
