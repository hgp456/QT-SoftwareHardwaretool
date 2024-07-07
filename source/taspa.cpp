#include "taspa.h"
#include <QDebug>
#include <QVector>




Taspa::Taspa(int max)
{
    MAX=max;
    val.resize(max,0);
    Opri.resize(max, 0);
    TempOpri1.resize(max, 0);
    O.resize(max, 0);
    MaxOpRiFlag.resize(max, 0);
    Retime.resize(max, 0);
    task.resize(max);
}


int Taspa::resize(int max)
{
    MAX=max;
    val.resize(max,0);
    Opri.resize(max, 0);
    TempOpri1.resize(max, 0);
    O.resize(max, 0);
    MaxOpRiFlag.resize(max, 0);
    Retime.resize(max, 0);
    task.resize(max);

    return 0;
}

void Taspa::out_calculate()
{
//    qDebug() << O.size();
    for(int i=0;i<MAX;i++)
    {
        for(int j=0;j<MAX;j++)
        {
            if(array[i][j]!=0)
            {
                O[i]+=1;
            }
//            O[i]+=array[i][j]; //!出度计算
        }
        Opri[i]=O[i]+val[i];
    }
    for(int i=0;i<MAX;i++) //!优先级值计算
    {
        if(O[i]!=0)
        {
//            qDebug() << i;
            MaxOpri(i);
        }
    }
}


int Taspa::MaxOprihaspa(int i)
{
    int temp=0;
    int max=0;
    if(O[i]==0)
    {
        MaxOpRiFlag[i]=1;
        return Opri[i];
    }
    for(int j=0;j<MAX;j++)
    {
        if(array[i][j]!=0)
        {
            temp=MaxOprihaspa(j)+array[i][j];
            if(max<temp)
                max=temp;
        }
    }
    if(MaxOpRiFlag[i]==0)
    {
        Opri[i]+=max;
        MaxOpRiFlag[i]=1;
    }
    return Opri[i];
}



void Taspa::out_calculatehaspa()
{
    //    qDebug() << O.size();
    for(int i=0;i<MAX;i++)
    {
        for(int j=0;j<MAX;j++)
        {
            if(array[i][j]!=0)
            {
                O[i]+=1;
            }
            //            O[i]+=array[i][j]; //!出度计算
        }
        Opri[i]=O[i]+val[i];
    }
    for(int i=0;i<MAX;i++) //!优先级值计算
    {
        if(O[i]!=0)
        {
            //            qDebug() << i;
            MaxOprihaspa(i);
        }
    }
}




void Taspa::task_init()
{
    for(int i=0;i<MAX;i++)
    {
        task[i].num=i+1;  //编号赋值
        task[i].retime=Retime[i];   //释放时间赋值
        task[i].value=val[i]; //价值赋值
        task[i].out=O[i];  //出度赋值
        task[i].privalue=Opri[i]; //优先级值
    }
}


void Taspa::order_calculate()
{
    for(int i=0;i<MAX;i++)
    {
        for(int j=i+1;j<MAX;j++)
        {
            if(task[i].privalue==task[j].privalue)//优先值级别
            {
                if(task[i].retime>task[j].retime) //释放时间
                    task[j].sameprivalue++;
                else if(task[i].retime==task[j].retime) //施放时间相等
                {
                    if(task[i].value<task[j].value) //执行时间大的先运行
                        task[j].sameprivalue++;
                    else if(task[i].num>task[j].num)  //按照编号从小到大排序
                        task[j].sameprivalue++;
                    else
                        task[i].sameprivalue++;
                }
                else
                    task[i].sameprivalue++;
            }
        }
    }
}


int Taspa::MaxOpri(int i)
{
    int temp=0;
    int max=0;
    if(O[i]==0)
    {
        MaxOpRiFlag[i]=1;
        return Opri[i];
    }
    for(int j=0;j<MAX;j++)
    {
        if(array[i][j]!=0)
        {
            temp=MaxOpri(j);
            if(max<temp)
                max=temp;
        }
    }
    if(MaxOpRiFlag[i]==0)
    {
        Opri[i]+=max;
        MaxOpRiFlag[i]=1;
    }
    return Opri[i];
}



void Taspa::order_output()
{
    int order=0;
    int count1=0;
    copy(begin(Opri),end(Opri),begin(TempOpri1));
    for(int i=0;i<MAX;i++)
    {
        int max=-1;
        for(int j=0;j<MAX;j++)
        {
            if(TempOpri1[j]==max)
            {
                if(task[j].sameprivalue>task[order].sameprivalue)
                {
                    order=j;
                }
            }
            else if(TempOpri1[j]>max)
            {
                max=TempOpri1[j];
                order=j;
            }
        }
        task[order].order=count1++;//最大的
        TempOpri1[order]=0;
    }
}


QString Taspa::order_print()
{
    QString output;
    for(int i=0;i<MAX;i++)
    {
        for(int j=0;j<MAX;j++)
        {
            if(task[j].order==i)
            {
                output += QString("T%1 %2 ").arg(task[j].num).arg(Opri[j]);

                tasktable.push_back(task[j]);
            }
        }
    }
//    qDebug().noquote().nospace() << output;
    return output;

}


void Taspa::Run()
{
//    qDebug() << "a";
    out_calculate();
//    qDebug() << "b";
    task_init();
    order_calculate();
    order_output();
    order_print();
}


void Taspa::RunHaspa()
{
    out_calculatehaspa();
    task_init();
    order_calculate();
    order_output();
    order_print();
}

