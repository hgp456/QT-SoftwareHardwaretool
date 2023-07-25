#include "muppa.h"
//#include <QVector>
#include "QDebug"
muppa::muppa(int max):t(max)
{
    MAX=max;
    time=0;
    task.resize(max);
    taskTable.resize(max);
    core.resize(max);
    for(int i=0;i<max;i++)
    task[i].relyFunction.resize(max);
}

void muppa::core_init()
{
    for(int i=0;i<CORENUM;i++)
    {
        core[i].status=IDLE;  //编号赋值
        core[i].busytime=0;//忙碌时间置0
    }
}

//int muppa::rely_anas()
//{
//    for(int i=0;i<max;i++)
//    {
//        int count=0;
//        for(int j=0;j<max;j++)
//        {
//            if(array[j][i]==1)
//            {
//                task[i].relyFunction[count++]=j;
//            }
//        }
//        task[i].relyNum=count;
//    }
//}

bool muppa::RelyFinish(int tasknum)
{
    // cout << tasknum << endl;
    // cout << "task" << tasknum << "relyNum" << task[tasknum].relyNum << endl;
    // cout << task[task[2].relyFunction[0]].finish << endl;
    for(int i=0;i<task[tasknum].relyNum;i++)
    {
        if(task[task[tasknum].relyFunction[i]].finish==false)
        {
            return false;
        }
    }
    return true;
}

int muppa::rely_anas()
{
    for(int i=0;i<MAX;i++)
    {
        int count=0;
        for(int j=0;j<MAX;j++)
        {
            if(t.array[j][i]==1)
            {
                task[i].relyFunction[count++]=j;
            }
        }
        task[i].relyNum=count;
    }
    return 0;
}

int muppa::order_print()
{
    for(int i=0;i<MAX;i++)
    {
        for(int j=0;j<MAX;j++)
        {
            if(task[j].order==i)
            {
                taskTable.push_back(task[j].num-1);
            }
        }
    }
    return 0;
}

int muppa::Scheduling()
{
    int allstatus=BUSY;
    while(allstatus==BUSY)
    {
        allstatus=IDLE;
        for(int i=0;i<CORENUM;i++)
        {
            if(core[i].status==BUSY)
            {
                core[i].busytime++;
                task[core[i].runningtask].value--;
                if(task[core[i].runningtask].value<0)
                {
                    task[core[i].runningtask].finish=true;
                    core[i].finishjob.push_back(core[i].runningtask);
                    core[i].status=IDLE;
                }
            }
        }
        for(int i=0;i<CORENUM;i++)
        {
            if(core[i].status==IDLE&&!taskTable.empty())//空闲且不为空
            {
                for(int taskNumNow=0;taskNumNow<taskTable.size();taskNumNow++)
                {
                    if(RelyFinish(taskTable[taskNumNow])==true && task[taskTable[taskNumNow]].finish==false &&task[taskTable[taskNumNow]].running==false)
                    {
                            core[i].runningtask=taskTable[taskNumNow];
                            task[taskTable[taskNumNow]].running=true;
                            core[i].status=BUSY;
                            break;
                    }
                }
            }
        }

        for(int i=0;i<CORENUM;i++)
        {
            allstatus=IDLE;
            if(core[i].status==BUSY)
            {
                allstatus=BUSY;
                break;
            }
        }
        if(allstatus==IDLE)
            break;
        time++;
    }
    return 0;

}


void muppa::task_init()
{
    for(int i=0;i<MAX;i++)
    {
        task[i].num=t.task[i].num;
        task[i].value=t.task[i].value;
        task[i].privalue=t.task[i].privalue;
        task[i].retime=t.task[i].retime;
        task[i].out=t.task[i].out;
        task[i].order=t.task[i].order;
//        qDebug()<<t.task[i].order;
    }
}


QString muppa::result_output()
{
    QString output;
    for(int i=0; i<CORENUM; i++)
    {
        output += "Core " + QString::number(i) + ":\n";
        output += "Busy Rate= " + QString::number(core[i].busytime) + "/" + QString::number(time) + "\n";
        for(int j=0; j<core[i].finishjob.size(); j++)
        {
            output += "task " + QString::number(core[i].finishjob[j]+1) + " ";
        }
        output += "\n";
    }
    return output;
}
int muppa::Run()
{
    qDebug()<<"test";
//    qDebug()<<t.order_print();
    core_init();
    task_init();
    order_print();
    Scheduling();
    result_output();
       //这里需要将 Taspa t装载进来
    return 0;
}
//int muppa::result_output()
//{
//    for(int i=0;i<CORENUM;i++)
//    {
//        cout << "Core" << i <<":" <<endl;
//        cout << "Busy Rate=" << core[i].busytime << "/" << time << endl;
//        for(int j=0;j<core[i].finishjob.size();j++)
//        {
//            cout <<  "task" <<core[i].finishjob[j]+1 << " ";
//        }
//        cout << endl;
//    }
//}
