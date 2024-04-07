#include "haspa_criticalpath.h"
#include "haspa.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "haspa.h"
#include <QDebug>
#include <boost/dynamic_bitset.hpp>
#include "taspa.h"
#include <queue>
#include <deque>
#include <qstring.h>
#include "ganntplot.h"

#include <QDateTime>




static bool fpgaIdle=true;
static bool cpuIdle=true;


#define SOFT 0
#define HARD 1


int GlobalEST=-1;

int HASPA_CRITICALPATH::checkresourceCPUStatus()
{
    cpuIdle=false;
    for(int i=0;i<cpu.size();i++)
    {
        if(cpu[i].status==IDLE)
        {
            //            cpu[i].busytime++;
            cpuIdle=true;
        }
        //        else
        //        {
        //            cpu[i].idletime++;
        //        }
    }

    return 0;
}


int HASPA_CRITICALPATH::checkresourceFPGAStatus()
{
    fpgaIdle=false;
    for(int i=0;i<fpga.size();i++)
    {
        if(fpga[i].status==IDLE)
        {
            //            fpga[i].busytime++;
            fpgaIdle=true;
        }
    }

    return 0;
}


int HASPA_CRITICALPATH::logresourceCPUStatus()
{
    cpuIdle=false;
    for(int i=0;i<cpu.size();i++)
    {
        if(cpu[i].status==IDLE)
        {

            cpuIdle=true;
        }
        else
        {
             cpu[i].busytime++;
        }
    }

    return 0;
}


int HASPA_CRITICALPATH::logresourceFPGAStatus()
{
    fpgaIdle=false;
    for(int i=0;i<fpga.size();i++)
    {
        if(fpga[i].status==IDLE)
        {

            fpgaIdle=true;
        }
        else
        {
            fpga[i].busytime++;
        }
    }

    return 0;
}
HASPA_CRITICALPATH::HASPA_CRITICALPATH(int tasknum):taspa(tasknum)
{
    taskattribute.taskorder.resize(tasknum,0);
    taskattribute.taskNum=tasknum;
    taskattribute.sortExectime.resize(tasknum,0);
    taskattribute.hardExectime.resize(tasknum,0);
    taskattribute.hardwareArea.resize(tasknum,0);
    taskattribute.hardawaregain.resize(tasknum,0);
    taskattribute.relesetime.resize(tasknum,0);
    constraintsolution.variables.resize(tasknum);
    TaskList.resize(tasknum);
    Ganntinfo.resize(tasknum);
    squreUsed=0;
    runtime=0;
    GlobalEST=-1;
}

int HASPA_CRITICALPATH::hardwaregainCalculation()
{
    for(int i=0;i<taskattribute.taskNum;i++)
    {
        taskattribute.hardawaregain[i]=taskattribute.sortExectime[i]-taskattribute.hardExectime[i];
    }
    return 0;
}


int HASPA_CRITICALPATH::task_init(std::vector<ProcessunitBlock> &Task)
{
    for (int i = 0; i < Task.size(); i++) {
        Task[i].id = i+1;
        Task[i].time = taskattribute.sortExectime[i];
        Task[i].earliestStartTime = taskattribute.relesetime[i];
        Task[i].latestStartTime = -1;
        Task[i].ESTCheckTime = 0;
        Task[i].need_time = -1;
        Task[i].exec_time = -1;
        Task[i].wait_time = taskattribute.relesetime[i]; //加入释放时间
        Task[i].arrive_time = -1;
        Task[i].finish_time = -1;
        for (int j = 0; j < Task.size(); j++){
            if (taskattribute.CommunicationCost[i][j] > 0) {
                Task[i].succ.push_back(j);
            }
            if (taskattribute.CommunicationCost[j][i] > 0) {
                Task[i].pre.push_back(j);
            }
        }
    }

    return 0;
}


int HASPA_CRITICALPATH::task_reinit(std::vector<ProcessunitBlock> &Task)
{
    for (int i = 0; i < Task.size(); i++) {
        if(Task[i].hardwareuse==SOFTWARE)
        Task[i].time = taskattribute.sortExectime[i];
        else
        Task[i].time = taskattribute.hardExectime[i];

        Task[i].earliestStartTime = taskattribute.relesetime[i];
        Task[i].latestStartTime = -1;
        Task[i].ESTCheckTime = 0;
    }

    return 0;
}


int HASPA_CRITICALPATH::Schedule(std::vector<ProcessunitBlock> &Task)
{
    std::vector<int> q;
    int ordertemp=0;
    for(int i=0;i<Task.size();i++)  //这里可以用haspmap优化
    {
        for(int j=0;j<Task.size();j++)
        {
            if(taskattribute.taskorder[j]==ordertemp)
            {
                q.push_back(j);
                ordertemp++;
                continue;
            }
        }
    }

    //    cpu.resize(2);//2核心
    //    fpga.resize(2);//2并行

    int time=0;
    while(!q.empty())
    {

        for (int i = 0; i < q.size(); ) {
            int tempNode = q[i];

            if (Task[tempNode].finish_time == time) {
                Task[tempNode].finfish = true;
                if (Task[tempNode].hardwareuse == true) {
                    int tempindex = Task[tempNode].usedResouceIndex;
                    fpga[tempindex].status = IDLE;
                }
                else {
                    int tempindex = Task[tempNode].usedResouceIndex;
                    cpu[tempindex].status = IDLE;
                }
                for (int i = 0; i < Task[tempNode].succ.size(); i++) {
                    int succNode = Task[tempNode].succ[i];
                    Task[succNode].wait_time = std::max(Task[succNode].wait_time, taskattribute.CommunicationCost[tempNode][succNode]);
                }
                q.erase(q.begin() + i);
            }
            else {
                i++;
            }
        }

        checkresourceCPUStatus();//确认CPU的资源使用情况
        checkresourceFPGAStatus();//确认FPGA的资源使用情况

        if(q.empty())
        {
            break;
        }

        int currNode;   //
        std::vector<int> fesibleNode;
        for(int i=0;i<q.size();i++)
        {
            currNode=q[i];//选出当前列表中优先级最高的任务
            if(Task[currNode].wait_time!=0) //如果在等待通讯,那直接切换到下一个节点
            {
                continue;
            }
            bool allprefinish=true;
            if(!Task[currNode].pre.empty()) //如果他是一个有前继的任务
            {
                for(int i=0;i<Task[currNode].pre.size();i++) //判断所有前继节点是否均完成
                {
                    int preNode=Task[currNode].pre[i];
                    if(Task[preNode].finfish==false)
                    {
                        allprefinish=false;
                        break;
                    }
                }
            }
            if(allprefinish==true) //如果所有前继节点都完成了的话,则结束选择
            {
                fesibleNode.push_back(currNode);
            }
        }

        for(int i=0;i<fesibleNode.size();i++)
        {
            currNode=fesibleNode[i];
            if(Task[currNode].running==true)
            {
                continue;
            }
            if(Task[currNode].hardwareuse==HARDWARE&&fpgaIdle==true) //第四步 如果硬件空闲,资源不超过限制,交给硬件执行
            {
                //                squareUsed+=SquareUsed[currNode];
                //            Task[currNode].need_time=hardwareExecutionTimes[currNode];
                Task[currNode].exec_time=taskattribute.hardExectime[currNode];
                Task[currNode].arrive_time=time;
                Task[currNode].finish_time=time+Task[currNode].exec_time;

                //                Task[currNode].hardwareuse=true;
                Task[currNode].running=true;

                int tempindex=Task[currNode].usedResouceIndex;
                while(fpga[tempindex].status!=IDLE)
                {
                    tempindex++;
                }
                fpga[tempindex].status=BUSY;
                fpga[tempindex].finishtask.push_back(currNode);
                Task[currNode].usedResouceIndex=tempindex;
            }
            else if( (Task[currNode].hardwareuse==SOFTWARE || Task[currNode].hardwareuse==HARDWARE && fpga.size()==0 ) && cpuIdle==true)  //如果处理器空闲
            {
                Task[currNode].exec_time=taskattribute.sortExectime[currNode];
                Task[currNode].arrive_time=time;
                Task[currNode].finish_time=time+Task[currNode].exec_time;

                Task[currNode].hardwareuse=false;
                Task[currNode].running=true;

                //                int tempindex=Task[currNode].usedResouceIndex;
                int maxIdleTime=INT_MIN;
                int cpuindex=0;
                for(int i=0;i<cpu.size();i++)
                {
                    if(cpu[i].status==BUSY)
                        continue;
                    else
                    {
                        if(cpu[i].idletime>maxIdleTime)
                        {
                            maxIdleTime=cpu[i].idletime;
                            cpuindex=i;
                        }
                    }
                }
                cpu[cpuindex].status=BUSY;
                cpu[cpuindex].finishtask.push_back(currNode);
                cpu[cpuindex].idletime=0;
                Task[currNode].usedResouceIndex=cpuindex;
            }
            checkresourceCPUStatus();//确认CPU的资源使用情况
            checkresourceFPGAStatus();//确认FPGA的资源使用情况
        }

        //确认资源情况
        logresourceFPGAStatus();
        logresourceCPUStatus();


        time++;

        for (int i=0;i< q.size();i++)
        {
            int tempNode = q[i];
            if (Task[tempNode].wait_time > 0) {
                Task[tempNode].wait_time--;
            }
        }

    }
    runtime=time;
//    qDebug()<<time;

    return 0;
}

void HASPA_CRITICALPATH::calculateEarliestStartTime(std::vector<ProcessunitBlock> &Task) {
    std::queue<int> q;
    for (int i = 0; i < Task.size(); i++) {
        if (Task[i].pre.empty()) {
            // nodes[i].earliestStartTime = nodes[i].time;
//            Task[i].earliestStartTime = taskattribute.relesetime[i];
//            Task[i].earliestStartTime = Tas;
            q.push(i);
        }
    }

    while (!q.empty()) {
        int currNode = q.front();
        q.pop();
        if(Task[currNode].succ.empty())
        {
            GlobalEST=std::max(GlobalEST,Task[currNode].earliestStartTime+Task[currNode].time);
        }

        for (int i = 0; i < Task[currNode].succ.size(); i++) {
            int dependency = Task[currNode].succ[i];
            Task[dependency].earliestStartTime = std::max(Task[dependency].earliestStartTime, Task[currNode].earliestStartTime+Task[currNode].time/*+taskattribute.CommunicationCost[currNode][dependency]*/);

            Task[dependency].ESTCheckTime++;
            // Check if all dependencies of the current node are satisfied
            bool allDependenciesSatisfied = false;
            if(Task[dependency].ESTCheckTime==Task[dependency].pre.size())
            {
                allDependenciesSatisfied = true;
            }
            if (allDependenciesSatisfied) {
//                qDebug() << Task[dependency].id <<  " " << Task[dependency].earliestStartTime;
                q.push(dependency);
            }
        }
    }
}

void HASPA_CRITICALPATH::calculateLatestStartTime(std::vector<ProcessunitBlock> &Task) {
    int lastNode = Task.size() - 1;

    for (int i = lastNode; i >= 0; i--) {
        int minLatestStartTime = INT_MAX;
        if (Task[i].succ.empty())
        {
            Task[i].latestStartTime = GlobalEST-Task[i].time;
            continue;
        }
        for (int j = 0; j < Task[i].succ.size(); j++) {
            int dependency = Task[i].succ[j];
            minLatestStartTime = std::min(minLatestStartTime, Task[dependency].latestStartTime - Task[i].time /*- taskattribute.CommunicationCost[i][dependency]*/);
        }
        Task[i].latestStartTime = minLatestStartTime;
    }
    // for(int i=0;i<nodes.size();i++)
    // {
    //     cout << nodes[i].id << ":" << nodes[i].latestStartTime << endl;
    // }
}

std::vector<int> HASPA_CRITICALPATH::calculateCriticalPath(std::vector<ProcessunitBlock> &Task) {
    std::vector<int> criticalPath;

    for(int i=0;i<Task.size();i++)
    {
        if(Task[i].earliestStartTime==Task[i].latestStartTime)
        {
            criticalPath.push_back(i);
        }
    }

    return criticalPath;
}

std::vector<int> HASPA_CRITICALPATH::critical_path_calculate(std::vector<ProcessunitBlock> &Task)
{
    GlobalEST=-1;
    calculateEarliestStartTime(Task);
    // Calculate latest start time
    calculateLatestStartTime(Task);
    // Calculate critical path
    std::vector<int> criticalPath = calculateCriticalPath(Task);

    QString a;
    for(int i=0; i<criticalPath.size();i++)
    {
        a+= "T" + QString::number(criticalPath[i]+1) + " ";
    }
//    qDebug() << a;

    return criticalPath;
}

int HASPA_CRITICALPATH::GetHardWareParition()
{
//

    bool optimise=true;
    std::vector<int> criticalPath = critical_path_calculate(TaskList);
    while(optimise)
    {
        optimise=false;
        float max_gain=INT_MIN;
        int current_acc_node;
        // cout << criticalPath.size() << endl;
        for(int i=0;i<criticalPath.size();i++)
        {
            float gain;
            int tempsqureused=INT_MAX;
            int currentNode=criticalPath[i];
            gain=taskattribute.hardawaregain[currentNode];
            //取平均收益最高
//            if(gain>0 && (float)gain>=max_gain&&squreUsed+taskattribute.hardwareArea[currentNode]<=taskattribute.areaConstraints&&taskattribute.hardwareArea[currentNode]<tempsqureused)
//             {
//                 optimise=true;
//                 current_acc_node=currentNode;
//                 max_gain=(float)gain/taskattribute.hardwareArea[currentNode];
//                 tempsqureused=taskattribute.hardwareArea[currentNode];
//             }

             //取直接收益最高
            if(gain>0 && gain>=max_gain&&squreUsed+taskattribute.hardwareArea[currentNode]<=taskattribute.areaConstraints/*taskattribute.hardwareArea[currentNode]<tempsqureused*/)
            {
                if(gain>max_gain || gain==max_gain&&taskattribute.hardwareArea[currentNode]<tempsqureused)
                {
                    optimise=true;
                    current_acc_node=currentNode;
                    max_gain=gain;
                    tempsqureused=taskattribute.hardwareArea[currentNode];
                }
            }

        }
        if(optimise==true)
        {
            squreUsed+=taskattribute.hardwareArea[current_acc_node];
//             qDebug() << "SquareUsed:" << squreUsed;
            taskattribute.hardawaregain[current_acc_node]=0;
//            taskattribute.sortExectime[current_acc_node]=taskattribute.hardExectime[current_acc_node];
            TaskList[current_acc_node].hardwareuse=HARDWARE;
//            qDebug() <<  "current_acc_node:T"<<current_acc_node+1 << " " << max_gain;
//            TaskList.clear();
//            TaskList.resize(taskattribute.taskNum);
            task_reinit(TaskList);
            criticalPath.clear();
            criticalPath = critical_path_calculate(TaskList);
        }
    }
    //先从上到下给
    for(int i=0;i<taskattribute.taskNum;i++)
    {
        if(squreUsed==taskattribute.areaConstraints)
            break;
        else if(TaskList[i].hardwareuse==SOFTWARE&&squreUsed+taskattribute.hardwareArea[i]<=taskattribute.areaConstraints)
        {
            TaskList[i].hardwareuse=HARDWARE;
            taskattribute.hardawaregain[i]=0;
//            taskattribute.sortExectime[i]=taskattribute.hardExectime[i];
            squreUsed+=taskattribute.hardwareArea[i];
//            qDebug() << "add T" <<i+1;
        }
    }

}

int HASPA_CRITICALPATH::run()
{

    //执行开始时间记录点


    task_init(TaskList);
    hardwaregainCalculation();
    GetHardWareParition();
//    constraint_calculation*();


    for(int i=0;i<taskattribute.taskNum;i++)
    {
        if(TaskList[i].hardwareuse==SOFTWARE || fpga.size()==0)
        {
            taspa.val[i]=taskattribute.sortExectime[i];  //给执行时间
            taspa.task[i].softorhard=SOFT;
            TaskList[i].hardwareuse=SOFTWARE;
        }
        else
        {
            taspa.val[i]=taskattribute.hardExectime[i];
            taspa.task[i].softorhard=HARD;
            TaskList[i].hardwareuse=HARDWARE;
//            qDebug() << "T" << i+1;
        }
    }
    taspa.Retime.assign(taskattribute.relesetime.begin(),taskattribute.relesetime.end()); //给通讯时间
    for (const std::vector<int>& subVector : taskattribute.CommunicationCost) {  //给依赖矩阵
        taspa.array.emplace_back(subVector.begin(), subVector.end());
    }

    taspa.RunHaspa();
//    qDebug() << taspa.order_print();

    for(int i=0;i<taskattribute.taskNum;i++)
    {
        taskattribute.taskorder[i]=taspa.task[i].order;
    }

    Schedule(TaskList);

//    for(int i=0;i<TaskList.size();i++)
//    {
//        qDebug() <<"T" << i+1 << ":"<<  TaskList[i].arrive_time << "->" << TaskList[i].finish_time << " " << TaskList[i].hardwareuse;
//    }

    //执行结束时间记录点


    for(int i=0;i<TaskList.size();i++)
    {
        Ganntinfo[i].hardwareuse=TaskList[i].hardwareuse;
        Ganntinfo[i].fpgaIndex=TaskList[i].usedResouceIndex;
        Ganntinfo[i].taskNumber=TaskList[i].id;
        Ganntinfo[i].startTime=TaskList[i].arrive_time;
        Ganntinfo[i].duration=TaskList[i].exec_time;
        Ganntinfo[i].coreNumber=TaskList[i].usedResouceIndex;
        Ganntinfo[i].fpganumber=0;
    }

    QString coreinfomation;
    int endtime=0;
    for(int i=0;i<cpu.size()-1;i++)
    {
        coreinfomation+="Core" +QString::number(i)+",";
    }
    coreinfomation+="Core" + QString::number(cpu.size()-1);
    coreinfomation+="\n";

    coreinfomation+="FPGA" + QString::number(0);
//    for(int i=0;i<fpga.size()-1;i++)
//    {
//        coreinfomation+="FPGA" +QString::number(i)+",";
//    }
//    coreinfomation+="FPGA" + QString::number(fpga.size()-1);
    endtime=runtime;

    qDebug() <<"Runtime=" << runtime;

    output+="Orange means it's done by software. Red means it's done by hardware.\n";
    output+="SquareUsed:" +  QString::number(squreUsed) + "\n" + "Runtime:" + QString::number(runtime)+"\n";


    GanntPlot ganntplot;
    ganntplot.GanntPlotFileWriteHSPA(coreinfomation,endtime,Ganntinfo);

    return 0;
}

QString HASPA_CRITICALPATH::result_output()
{
    QString output;
    for(int i=0; i<cpu.size(); i++)
    {
        output += "Core " + QString::number(i) + ":\n";
        output += "Busy Rate= " + QString::number(cpu[i].busytime) + "/" + QString::number(runtime) + "\n";
        for(int j=0; j<cpu[i].finishtask.size(); j++)
        {
            output += "task " + QString::number(cpu[i].finishtask[j]+1) + " ";
        }
        output += "\n";
    }

    for(int i=0; i<fpga.size(); i++)
    {
        output += "FPGA Channel" + QString::number(i) + ":\n";
        output += "FPGA Channel Busy Rate= " + QString::number(fpga[i].busytime) + "/" + QString::number(runtime) + "\n";
        for(int j=0; j<fpga[i].finishtask.size(); j++)
        {
            output += "task " + QString::number(fpga[i].finishtask[j]+1) + " ";
        }
        output += "\n";
    }
    return output;
}

