#include "eshc.h"

static int runtime=0;
static int GlobalEST=-1;
static int squreUsed=0;

eshc::eshc(int tasknum):taspa(tasknum)
{
    taskattribute.taskorder.resize(tasknum,0);
    taskattribute.taskNum=tasknum;
    taskattribute.sortExectime.resize(tasknum,0);
    taskattribute.hardExectime.resize(tasknum,0);
    taskattribute.hardwareArea.resize(tasknum,0);
    taskattribute.hardawaregain.resize(tasknum,0);
    taskattribute.relesetime.resize(tasknum,0);
    TaskList.resize(tasknum);
    Ganntinfo.resize(tasknum);
    squreUsed=0;
    runtime=0;
    GlobalEST=-1;
}

int eshc::task_init(std::vector<ProcessunitBlock> &Task)
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
int eshc::run()
{
    task_init(TaskList);
    //得到软硬件划分结果

    //调度

    return 0;
}



