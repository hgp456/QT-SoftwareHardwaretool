#include "ganntplot.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include "haspa_hg.h"
GanntPlot::GanntPlot()
{

}

void GanntPlot::GanntPlotFileWrite(QString coreinfomation,int endtime,std::vector<GanntInfo> ganntinfo)
{
    QFile file("input.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        // 写入核心信息
        out << coreinfomation << "\n";

        // 写入结束时间
        out <<endtime << "\n";

        // 写入Gannt信息
        for (const GanntInfo& info : ganntinfo)
        {
            out << "Task"+QString::number(info.taskNumber+1)+","
                << "Core" + QString::number(info.coreNumber) +","
                << QString::number(info.startTime) +","
                << QString::number(info.duration) + "\n";
//            out << "Start Time: " << info.startTime << "\n";
//            out << "Duration: " << info.duration << "\n";
//            out << "\n";
        }

        file.close();
    }
}


void GanntPlot::GanntPlotFileWriteHSPA(QString coreinfomation,int endtime,std::vector<GanntInfo> ganntinfo)
{
    QFile file("input2.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        // 写入核心信息
        out << coreinfomation << "\n";

        // 写入结束时间
        out <<endtime << "\n";

        // 写入Gannt信息
        for (const GanntInfo& info : ganntinfo)
        {
            if(info.hardwareuse==SOFTWARE)
            {
            out << "Task"+QString::number(info.taskNumber)+","
                << "Core" + QString::number(info.coreNumber) +","
                << QString::number(info.startTime) +","
                << QString::number(info.duration) + "\n";
            }
            else
            {
            out << "Task"+QString::number(info.taskNumber)+","
                << "FPGA" + QString::number(info.fpganumber) +","
                << QString::number(info.startTime) +","
                << QString::number(info.duration) + ","
                << QString::number(info.fpgaIndex)  + "\n";
            }
        }

        file.close();
    }
}
