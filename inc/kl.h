#ifndef KL_H
#define KL_H
#include <QVector>
#include <QString>
class KL
{
public:
    KL(int tasknum);
    std::vector<int>union1;
    std::vector<int>union2;
    std::vector<int> D;
    int TASKNUM;
    int dCalulator();
    int gcalculator();
    std::vector<std::vector<int>> commCost;
    int Run();
    QString output1;
    std::vector<int> G;
    std::vector<int> A;
    std::vector<int> B;
};

#endif // KL_H
