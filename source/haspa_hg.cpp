#include "haspa_hg.h"
#include <vector>
#include "taspa.h"
#include "QDebug"

#include <qstring.h>
static std::vector<int> executionTimes = {4, 5, 4, 5, 3, 16, 3, 4, 14, 5, 20};
static std::vector<int> softwareExecutionTimes = {4, 5, 4, 5, 3, 16, 3, 4, 14, 5, 20};
static std::vector<int> hardwareExecutionTimes = {2, 3, 1, 2, 1, 10, 2, 2,  7, 2,  9};
static std::vector<int> taskorder;
static std::vector<int> hardwaregain;
static std::vector<int> relesetime;
static std::vector<int> SquareUsed={4,2,4,7,5,3,4,2,3,5,5};
static int SquareLimit=18;

// DAG dependencies
static std::vector<std::vector<int>> dependencies = {
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 4, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


HASPA_HG::HASPA_HG()
{

}

static void task_init(std::vector<ProcessunitBlock> &Task)
{
    for (int i = 0; i < Task.size(); i++) {
        Task[i].id = i+1;
        Task[i].time = executionTimes[i];
        Task[i].earliestStartTime = -1;
        Task[i].latestStartTime = -1;
        Task[i].ESTCheckTime = 0;
        Task[i].need_time = -1;
        Task[i].exec_time = -1;
        Task[i].wait_time = 0;
        Task[i].arrive_time = -1;
        Task[i].finish_time = -1;
        for (int j = 0; j < Task.size(); j++){
            if (dependencies[i][j] > 0) {
                Task[i].succ.push_back(j);
            }
            if (dependencies[j][i] > 0) {
                Task[i].pre.push_back(j);
            }
        }
    }
}

static void hardwaregainCalculate(int tasknum)
{
    hardwaregain.resize(tasknum);
    for(int i=0;i<tasknum;i++)
    {
        hardwaregain[i]=softwareExecutionTimes[i]-hardwareExecutionTimes[i];
    }
}


static int taspaPrivalueCalculate(int tasknum)
{
    Taspa taspa(tasknum);
    taskorder.resize(tasknum,0);
    relesetime.resize(tasknum,0);
    for(int i=0;i<tasknum;i++)
    {
        taspa.val[i]=hardwaregain[i];  //给执行时间
    }
    taspa.Retime.assign(relesetime.begin(),relesetime.end()); //给通讯时间
    for (const std::vector<int>& subVector : dependencies) {  //给依赖矩阵
        taspa.array.emplace_back(subVector.begin(), subVector.end());
    }

    taspa.Run();
//    qDebug() << taspa.order_print();

    for(int i=0;i<tasknum;i++)
    {
        taskorder[i]=taspa.task[i].order;
//        qDebug() << taspa.task[i].num <<":" << taskorder[i];
    }

    return 0;
}

#define IDLE 0
#define BUSY 1
struct coretype
{
    int runningtask;
    int status=IDLE;//状态idel busy 空闲或忙碌
    int busytime=0;//忙碌的时间
    int idletime=0;
};

static std::vector<coretype> cpu;
static std::vector<coretype> fpga;



static bool fpgaIdle=true;
static bool cpuIdle=true;

static int checkresourceCPUStatus()
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


static int checkresourceFPGAStatus()
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


static int logresourceCPUStatus()
{
    cpuIdle=false;
    for(int i=0;i<cpu.size();i++)
    {
        if(cpu[i].status==IDLE)
        {
            cpu[i].busytime++;
            cpuIdle=true;
        }
    }

    return 0;
}


static int logresourceFPGAStatus()
{
    fpgaIdle=false;
    for(int i=0;i<fpga.size();i++)
    {
        if(fpga[i].status==IDLE)
        {
            fpga[i].busytime++;
            fpgaIdle=true;
        }
    }

    return 0;
}


int HASPA_HG::Schedule(std::vector<ProcessunitBlock> &Task)
{
    int squareUsed=0;
    std::vector<int> q;
    int ordertemp=0;
    for(int i=0;i<Task.size();i++)  //这里可以用haspmap优化
    {
        for(int j=0;j<Task.size();j++)
        {
            if(taskorder[j]==ordertemp)
            {
                q.push_back(j);
                ordertemp++;
                continue;
            }
        }
        if(ordertemp==Task.size()-1) //因为顺序从0开始排序的
        {
            break;
        }
    }

    int hardware_used=0;
    int software_used=0;


    cpu.resize(2);//2核心
    fpga.resize(2);//2并行

    int time=0;
    while(!q.empty())
    {
        for (int i=0;i< q.size();i++)
        {
            int tempNode = q[i];
            if (Task[tempNode].wait_time > 0) {
                Task[tempNode].wait_time--;
            }
        }
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
                    Task[succNode].wait_time = std::max(Task[succNode].wait_time, dependencies[tempNode][succNode]);
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
            if(squareUsed+SquareUsed[currNode]<=SquareLimit && fpgaIdle==true) //第四步 如果硬件空闲,资源不超过限制,交给硬件执行
            {
                squareUsed+=SquareUsed[currNode];
                //            Task[currNode].need_time=hardwareExecutionTimes[currNode];
                Task[currNode].exec_time=hardwareExecutionTimes[currNode];
                Task[currNode].arrive_time=time;
                Task[currNode].finish_time=time+Task[currNode].exec_time;

                Task[currNode].hardwareuse=true;
                Task[currNode].running=true;

                int tempindex=Task[currNode].usedResouceIndex;
                while(fpga[tempindex].status!=IDLE)
                {
                    tempindex++;
                }
                fpga[tempindex].status=BUSY;
                Task[currNode].usedResouceIndex=tempindex;
            }
            else if(cpuIdle==true)  //如果处理器空闲
            {
                Task[currNode].exec_time=softwareExecutionTimes[currNode];
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
                //                while(cpu[tempindex].status!=IDLE)
                //                {
                //                    tempindex++;
                //                }
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

    }
    qDebug()<<time;

    return 0;
}

int HASPA_HG::run()
{
    int tasknum=softwareExecutionTimes.size();

    std::vector<ProcessunitBlock> Task(tasknum);
    task_init(Task);
    hardwaregainCalculate(tasknum); //计算硬件增益
    taspaPrivalueCalculate(tasknum); //根据taspa计算基于硬件增益的优先级值

    Schedule(Task);

    return 0;
}


