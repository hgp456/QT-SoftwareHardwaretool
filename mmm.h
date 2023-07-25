#ifndef MMM_H
#define MMM_H
#include <QVector>


class MMM
{
public:

    MMM(int tasknum);
    int MaxAjacent();
    void clusterarrayinit();
    QString result_print();
    void Run();
    int Kc_init();
    int TASKNUM;
    std::vector<std::vector<int>> Kc;
    std::vector<std::vector<int>> clusterarray;
    std::vector<std::vector<int>> clusterarrayOri;
    std::vector<std::vector<int>> commCost;
    int modulesNum;
    int maxTaskNum;
    int cost;
};

#endif // MMM_H
