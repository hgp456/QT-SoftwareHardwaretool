#include "haspa.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "haspa.h"
#include <QDebug>
#include <QString>
#include <boost/dynamic_bitset.hpp>
#include "taspa.h"
#include <queue>
#include <deque>





static bool fpgaIdle=true;
static bool cpuIdle=true;
static int squareused=0;
//static int runtime=0;
int haspa::checkresourceCPUStatus()
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


int haspa::checkresourceFPGAStatus()
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


int haspa::logresourceCPUStatus()
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


int haspa::logresourceFPGAStatus()
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

haspa::haspa(int tasknum):taspa(tasknum)
{
    taskattribute.taskorder.resize(tasknum,0);
    taskattribute.taskNum=tasknum;
    taskattribute.sortExectime.resize(tasknum,0);
    taskattribute.hardExectime.resize(tasknum,0);
    taskattribute.hardwareArea.resize(tasknum,0);
    taskattribute.hardawaregain.resize(tasknum,0);
    taskattribute.hardwaregainavg.resize(tasknum,0);
    taskattribute.relesetime.resize(tasknum,0);
    constraintsolution.variables.resize(tasknum);
    TaskList.resize(tasknum);
    Ganntinfo.resize(tasknum);
    runtime=0;
    squareused=0;
}


bool compareSolutions(const ConstraintSolution& a, const ConstraintSolution& b) {
    return a.gain > b.gain;
}


int haspa::constraint_calculation()
{
    hardwaregainCalculation();
    std::vector<ConstraintSolution> solutions;


    for(int i=0;i<taskattribute.taskNum;i++)
    {
        taskattribute.hardwaregainavg[i]=(float)taskattribute.hardawaregain[i]/taskattribute.hardwareArea[i];
    }

    for(int j=0;j<taskattribute.taskNum;j++)
    {
        int tempNode=-1;
        float maxhardwaregainavg=INT_MIN;
        for(int i=0;i<taskattribute.taskNum;i++)
        {
            //选出增益最大的节点
            if(taskattribute.hardwaregainavg[i]>0&&taskattribute.hardwaregainavg[i]>maxhardwaregainavg && taskattribute.hardwareArea[i]+squareused<=taskattribute.areaConstraints)
            {
                maxhardwaregainavg=taskattribute.hardwaregainavg[i];

                tempNode=i;
            }
        }
        if(tempNode==-1)
        {
            continue;
        }
        else
        {
            squareused+=taskattribute.hardwareArea[tempNode];
            TaskList[tempNode].hardwareuse=HARDWARE;
            taskattribute.hardwaregainavg[tempNode]=0;
        }
    }


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

//    qDebug() << squareused;
//    for (int i = 0; i < (1 << taskattribute.taskNum); i++) {
//        ConstraintSolution solution;
//        solution.variables = boost::dynamic_bitset<>(taskattribute.taskNum, i);





//        for(int j=0;j<taskattribute.taskNum;j++)
//        {
//            solution.gain+=(float)solution.variables[j]/taskattribute.hardwareArea[j] * taskattribute.hardawaregain[j];
//            solution.s+=solution.variables[j] * taskattribute.hardwareArea[j];
//        }
////        qDebug() << solution.gain << " " << solution.s;

//        // Check if the solution satisfies the constraint
//        if (solution.s <= taskattribute.areaConstraints) {
//            solutions.push_back(solution);
//        }
//    }

//    std::sort(solutions.begin(), solutions.end(), compareSolutions);

//    if (!solutions.empty()) {
//        bestSolution = solutions[0];
//        qDebug() << "Best Solution: ";
//        QString test;
//        for (int i = 0; i < bestSolution.variables.size(); i++) {
//            test += std::to_string(bestSolution.variables[i]) + " ";
//        }
//        qDebug() << test;


//        qDebug() << "Gain: " << bestSolution.gain;
//        qDebug() << "S: " << bestSolution.s;
//    } else {
//        qDebug() << "No feasible solution found.";
//    }

//    //此时开始计算任务优先级
////    Taspa taspa(taskattribute.taskNum);
//    for(int i=0;i<taskattribute.taskNum;i++)
//    {
//        if(bestSolution.variables[i]==0)
//        {
//            taspa.val[i]=taskattribute.sortExectime[i];  //给执行时间
//            taspa.task[i].softorhard=SOFT;
//            TaskList[i].hardwareuse=SOFTWARE;
//        }
//        else
//        {
//            taspa.val[i]=taskattribute.hardExectime[i];
//            taspa.task[i].softorhard=HARD;
//            TaskList[i].hardwareuse=HARDWARE;
//        }
//    }
//    taspa.Retime.assign(taskattribute.relesetime.begin(),taskattribute.relesetime.end()); //给通讯时间
//    for (const std::vector<int>& subVector : taskattribute.CommunicationCost) {  //给依赖矩阵
//        taspa.array.emplace_back(subVector.begin(), subVector.end());
//    }

//    taspa.RunHaspa();
//    qDebug() << taspa.order_print();

    for(int i=0;i<taskattribute.taskNum;i++)
    {
        taskattribute.taskorder[i]=taspa.task[i].order;
    }

    return 0;
}

int haspa::hardwaregainCalculation()
{
    for(int i=0;i<taskattribute.taskNum;i++)
    {
        taskattribute.hardawaregain[i]=taskattribute.sortExectime[i]-taskattribute.hardExectime[i];
    }
    return 0;
}

int haspa::task_init(std::vector<ProcessunitBlock> &Task)
{
    for (int i = 0; i < Task.size(); i++) {
        Task[i].id = i+1;
        Task[i].time = taskattribute.sortExectime[i];
        Task[i].earliestStartTime = -1;
        Task[i].latestStartTime = -1;
        Task[i].ESTCheckTime = 0;
        Task[i].need_time = -1;
        Task[i].exec_time = -1;
        Task[i].wait_time = taskattribute.relesetime[i];
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


int haspa::Schedule(std::vector<ProcessunitBlock> &Task)
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
                cpu[cpuindex].finishtask.push_back(currNode);
                cpu[cpuindex].status=BUSY;
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

int haspa::run()
{

    task_init(TaskList);
    constraint_calculation();
    Schedule(TaskList);

//    for(int i=0;i<TaskList.size();i++)
//    {
//        qDebug() <<"T" << i+1 << ":"<<  TaskList[i].arrive_time << "->" << TaskList[i].finish_time << " " << TaskList[i].hardwareuse;
//    }

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

    output+="Orange means it's done by software. Red means it's done by hardware.\n";
    output+="SquareUsed:" +  QString::number(squareused) + "\n" + "Runtime:" + QString::number(runtime);


    GanntPlot ganntplot;
    ganntplot.GanntPlotFileWriteHSPA(coreinfomation,endtime,Ganntinfo);
    return 0;
}


QString haspa::result_output()
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
