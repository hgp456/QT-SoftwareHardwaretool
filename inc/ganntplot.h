#ifndef GANNTPLOT_H
#define GANNTPLOT_H
#include <QVector>
#include <string>
struct GanntInfo
{
    bool hardwareuse;
    int fpgaIndex;
    int fpganumber;
    int coreNumber;
    int taskNumber;
    int startTime;
    int duration;
};
class GanntPlot
{
public:
    GanntPlot();
    void GanntPlotFileWrite(QString coreinfomation,int endtime,std::vector<GanntInfo> ganntinfo);
    void GanntPlotFileWriteHSPA(QString coreinfomation,int endtime,std::vector<GanntInfo> ganntinfo);
};

#endif // GANNTPLOT_H
