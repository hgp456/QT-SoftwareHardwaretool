#ifndef TASPA_H
#define TASPA_H
#include <QVector>
#include <queue>

struct tasktype
{
    int num;          // 编号
    int value;        // 价值
    int privalue;     // 优先级值
    int retime;       // 释放时间
    int out;          // 出度
    int order;        // 执行顺序
    int sameprivalue; // 同样优先级值处理
    int softorhard;
};


class Taspa
{
public:
    Taspa(int max);
    void Run();
    QString order_print();
    std::vector<std::vector<int>> array;
    std::vector<int> val;
    std::vector<int> Retime;
    std::vector<int> Opri;
    std::vector<int> TempOpri1;
    std::vector<int> O;
    std::vector<int> MaxOpRiFlag;
    std::vector<tasktype> task;
//    std::vector<tasktype> tasktable;
    std::vector<tasktype> tasktable;
    int MaxOpri(int i);
    void order_output();
    void task_init();
    void out_calculate();
    void order_calculate();
    void out_calculatehaspa();
    void RunHaspa();
    int MaxOprihaspa(int i);
    int MAX;
    int resize(int max);
};

#endif // TASPA_H
