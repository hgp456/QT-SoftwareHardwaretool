#include "mmm.h"
#include <iostream>
#include <vector>
#include <climits>
#include <QDebug>


MMM::MMM(int tasknum)
{
    Kc.resize(tasknum,std::vector<int>(tasknum, 0));
    clusterarray.resize(tasknum,std::vector<int>(tasknum, 0));
    clusterarrayOri.resize(tasknum,std::vector<int>(tasknum, 0));
    TASKNUM=tasknum;
    cost=0;
}

int MMM::Kc_init()
{
    int count=0;
    std::vector<int> a;
    Kc.clear();
    for(int i=0;i<TASKNUM;i++)
    {
        a.push_back(i);
        Kc.push_back(a);
        a.clear();
    }
    return 0;
}


int MMM::MaxAjacent(){
    int max=0;
    for(int i=0;i<TASKNUM;i++){
        for(int j=0;j<TASKNUM;j++){
            if(commCost[i][j]>max)
                max=commCost[i][j];
        }
    }
    return max;
}
void MMM::clusterarrayinit(){
    for(int i=0;i<TASKNUM;i++){
        for(int j=0;j<TASKNUM;j++){
            if(i==j){
                clusterarray[i][j]=1;
            }
        }
    }
}
QString MMM::result_print() {
    QString output;
//    output += "Kc.size() = " + QString::number(Kc.size()) + "\n";
    for (int i = 0; i < Kc.size(); i++) {
        output += "cluster " + QString::number(i) + "\n";
        for (int j = 0; j < Kc[i].size(); j++) {
            output += QString::number(Kc[i][j] + 1) + " ";
        }
        output += "\n";
    }
    output += "cost="  + QString::number(cost) + "\n";
    return output;
}

void MMM::Run()
{
    int c=0;
    int k=0;
    int larc=0;
    int clusternum=0;
    int cluster0=0;
    int cluster1=0;
    int temp=0;
    int temp1=0;
    int number=0;
    int number1=0;

    c=1+MaxAjacent();
    k=TASKNUM;

    Kc_init();
    clusterarrayinit(); //初始化聚类矩阵
    qDebug() << "test";
    while(1)
    {
        for(int i=0;i<Kc.size();i++)//选集合
        {
            for(int j=i+1;j<Kc.size();j++)//选集合
            {
                int max=0;
                for(int q=0;q<Kc[i].size();q++)//求笛卡尔积
                {
                    temp=Kc[i][q];//第一个集合的数
                    for(int p=0;p<Kc[j].size();p++)//求笛卡尔积
                    {
                        temp1=Kc[j][p];//第二个集合的数字
                        // cout << temp << " " << temp1 << endl;
                        if(max<commCost[temp][temp1])
                        {
                            max=commCost[temp][temp1];
                            cluster0=i;
                            cluster1=j;
                            number=temp;
                            number1=temp1;
                        }
                    }
                }//此时这俩集合的笛卡尔积已经求完了
                if(max>=c && Kc[cluster0].size()+Kc[cluster1].size()<=maxTaskNum)
                {
//                    cout << number+1 << " " << number1+1 << endl;
                    commCost[number][number1]=0;
                    for(int i=0;i<Kc[cluster1].size();i++)
                    {
                        Kc[cluster0].push_back(Kc[cluster1][i]);
                    }
                    Kc.erase(Kc.begin()+cluster1);
//                    if(Kc.size()==modulesNum)
//                    result_print();
                }
            }
        }
        c=c-1;
        k=Kc.size();
        qDebug() <<"k=" <<k;
        if(k==modulesNum)
        {
            break;
        }
        else if(c==0 && k > modulesNum)
        {
            c=1+MaxAjacent();
        }
        // cout << "threshold= " << c << endl;
    }
    cost=0;
    for(int i=0;i<Kc.size();i++)//选集合
    {
        for(int j=i+1;j<Kc.size();j++)//选集合
        {
            for(int k=0;k<Kc[i].size();k++)//求笛卡尔积
            {
                temp=Kc[i][k];//第一个集合的数
                for(int p=0;p<Kc[j].size();p++)//求笛卡尔积
                {
                    temp1=Kc[j][p];//第二个集合的数字
                    cost+=commCost[temp][temp1];
                }
            }//此时这俩集合的笛卡尔积已经求完了
        }
    }

//    cout << "cost="<<cost << endl;
}



void MMM::Run_MMMCAA()
{
        int c=0;
        int k=0;
        int larc=0;
        int clusternum=0;
        int cluster0=0;
        int cluster1=0;
        int temp=0;
        int temp1=0;
        int number=0;
        int number1=0;
        c=1+MaxAjacent();
        k=TASKNUM;

        Kc_init();
        clusterarrayinit(); //初始化聚类矩阵
        while(1)
        {
            cost=0;
            for(int i=0;i<Kc.size();i++)//选集合
            {
                for(int j=i+1;j<Kc.size();j++)//选集合
                {
                    int max=0;
                    int small=INT8_MAX;
                    for(int q=0;q<Kc[i].size();q++)//求笛卡尔积
                    {
                        temp=Kc[i][q];//第一个集合的数
                        for(int p=0;p<Kc[j].size();p++)//求笛卡尔积
                        {
                            cost+=commCost[temp][temp1];
                            temp1=Kc[j][p];//第二个集合的数字
                            // cout << temp << " " << temp1 << endl;
                            if(small>commCost[temp][temp1])
                            {
                                small=commCost[temp][temp1];
                                cluster0=i;
                                cluster1=j;
                                number=temp;
                                number1=temp1;
                            }
                        }
                    }//此时这俩集合的笛卡尔积已经求完了
                    if(small>=c && Kc[cluster0].size()+Kc[cluster1].size()<=maxTaskNum)
                    {
                        commCost[number][number1]=0;
                        for(int i=0;i<Kc[cluster1].size();i++)
                        {
                            Kc[cluster0].push_back(Kc[cluster1][i]);
                        }
                        Kc.erase(Kc.begin()+cluster1);
                        // result_print();
                    }
                }
            }
            c=c-1;
            k=Kc.size();
            if(c==0||k==modulesNum)
            {
                break;
            }
        }
        if(c==0 && k>modulesNum)
        {
            c=1+MaxAjacent();
            while(1)
            {
                cost=0;
                for(int i=0;i<Kc.size();i++)//选集合
                {
                    for(int j=i+1;j<Kc.size();j++)//选集合
                    {
                        int max=0;
                        for(int k=0;k<Kc[i].size();k++)//求笛卡尔积
                        {
                            temp=Kc[i][k];//第一个集合的数
                            for(int p=0;p<Kc[j].size();p++)//求笛卡尔积
                            {
                                temp1=Kc[j][p];//第二个集合的数字
                                // cout << temp << " " << temp1 << endl;
                                cost+=commCost[temp][temp1];
                                if(max<commCost[temp][temp1])
                                {
                                    max=commCost[temp][temp1];
                                    cluster0=i;
                                    cluster1=j;
                                    number=temp;
                                    number1=temp1;
                                }
                            }
                        }//此时这俩集合的笛卡尔积已经求完了
                        if(max>=c && Kc[cluster0].size()+Kc[cluster1].size()<=maxTaskNum)
                        {
                            commCost[number][number1]=0;
                            for(int i=0;i<Kc[cluster1].size();i++)
                            {
                                Kc[cluster0].push_back(Kc[cluster1][i]);
                            }
                            Kc.erase(Kc.begin()+cluster1);
                        }
                    }
                }
                c=c-1;
                qDebug() <<"cost=" << cost;
                // cout << "threshold= " << c << endl;
                k=Kc.size();
                if(k==modulesNum)
                {
                    break;
                }
                else if(c==0 && k > modulesNum)
                {
                    c=1+MaxAjacent();
                }
            }
        }
//        result_print();
        cost=0;
        for(int i=0;i<Kc.size();i++)//选集合
        {
            for(int j=i+1;j<Kc.size();j++)//选集合
            {
                for(int k=0;k<Kc[i].size();k++)//求笛卡尔积
                {
                    temp=Kc[i][k];//第一个集合的数
                    for(int p=0;p<Kc[j].size();p++)//求笛卡尔积
                    {
                        temp1=Kc[j][p];//第二个集合的数字
                        cost+=commCost[temp][temp1];
                    }
                }//此时这俩集合的笛卡尔积已经求完了
            }
        }

}


void MMM::Run_MMMAAA()
{
        int c=0;
        int k=0;
        int larc=0;
        int clusternum=0;
        int cluster0=0;
        int cluster1=0;
        int temp=0;
        int temp1=0;
        int number=0;
        int number1=0;
        c=1+MaxAjacent();
        k=TASKNUM;

        Kc_init();
        clusterarrayinit(); //初始化聚类矩阵
        while(1)
        {
            for(int i=0;i<Kc.size();i++)//选集合
            {
                for(int j=i+1;j<Kc.size();j++)//选集合
                {
                    int max=0;
                    int small=INT8_MAX;
                    float avg=0;
                    for(int k=0;k<Kc[i].size();k++)//求笛卡尔积
                    {
                        temp=Kc[i][k];//第一个集合的数
                        for(int p=0;p<Kc[j].size();p++)//求笛卡尔积
                        {
                            temp1=Kc[j][p];//第二个集合的数字
                            // cout << temp << " " << temp1 << endl;
                            avg+=commCost[temp][temp1];
                            cost+=commCost[temp][temp1];
                            if(small>commCost[temp][temp1])
                            {
                                small=commCost[temp][temp1];
                                cluster0=i;
                                cluster1=j;
                                number=temp;
                                number1=temp1;
                            }
                        }
                    }//此时这俩集合的笛卡尔积已经求完了
                    avg=avg/(Kc[i].size()*Kc[j].size());
                    if(avg>=c && Kc[cluster0].size()+Kc[cluster1].size()<=maxTaskNum)
                    {
                        commCost[number][number1]=0;
                        for(int i=0;i<Kc[cluster1].size();i++)
                        {
                            Kc[cluster0].push_back(Kc[cluster1][i]);
                        }
                        Kc.erase(Kc.begin()+cluster1);
                        result_print();
                    }
                }
            }
            c=c-1;
            k=Kc.size();
            if(c==0||k==modulesNum)
            {
                break;
            }
        }
        if(c==0 && k>modulesNum)
        {
            c=1+MaxAjacent();
            while(1)
            {
                cost=0;
                for(int i=0;i<Kc.size();i++)//选集合
                {
                    for(int j=i+1;j<Kc.size();j++)//选集合
                    {
                        int max=0;
                        for(int k=0;k<Kc[i].size();k++)//求笛卡尔积
                        {
                            temp=Kc[i][k];//第一个集合的数
                            for(int p=0;p<Kc[j].size();p++)//求笛卡尔积
                            {
                                temp1=Kc[j][p];//第二个集合的数字
                                cost+=commCost[temp][temp1];
                                // cout << temp << " " << temp1 << endl;
                                if(max<commCost[temp][temp1])
                                {
                                    max=commCost[temp][temp1];
                                    cluster0=i;
                                    cluster1=j;
                                    number=temp;
                                    number1=temp1;
                                }
                            }
                        }//此时这俩集合的笛卡尔积已经求完了
                        if(max>=c && Kc[cluster0].size()+Kc[cluster1].size()<=maxTaskNum)
                        {
                            commCost[number][number1]=0;
                            for(int i=0;i<Kc[cluster1].size();i++)
                            {
                                Kc[cluster0].push_back(Kc[cluster1][i]);
                            }
                            Kc.erase(Kc.begin()+cluster1);
                            result_print();
                        }
                    }
                }
                c=c-1;
                k=Kc.size();
                if(k==modulesNum)
                {
                    break;
                }
                else if(c==0 && k > modulesNum)
                {
                    c=1+MaxAjacent();
                }
            }
        }
        cost=0;
        for(int i=0;i<Kc.size();i++)//选集合
        {
            for(int j=i+1;j<Kc.size();j++)//选集合
            {
                for(int k=0;k<Kc[i].size();k++)//求笛卡尔积
                {
                    temp=Kc[i][k];//第一个集合的数
                    for(int p=0;p<Kc[j].size();p++)//求笛卡尔积
                    {
                        temp1=Kc[j][p];//第二个集合的数字
                        cost+=commCost[temp][temp1];
                    }
                }//此时这俩集合的笛卡尔积已经求完了
            }
        }
}
