#include "kl.h"
#include <QDebug>
KL::KL(int tasknum)
{
    TASKNUM=tasknum;
    D.resize(tasknum);
    output1.clear();
//    commCost.resize(tasknum,std::vector<int>(tasknum, 0));
}

int KL::dCalulator()
{
    int temp0=0;
    int temp1=0;
    int temp2=0;
    int temp3=0;
    int insideD1=0;
    int insideD2=0;
    int outsideD1=0;
    int outsideD2=0;
    int allD1=0;
    int allD2=0;
    for(int i=0;i<union1.size();i++)
    {
        temp0=0;
        temp1=0;
        temp2=0;
        temp3=0;
        insideD1=0;
        insideD2=0;
        outsideD1=0;
        outsideD2=0;
        for(int j=0;j<union1.size();j++)
        {
            temp0=union1[i];//!当前元素
            temp1=union1[j];//!当前元素内部元素
            temp2=union2[i];//!当前元素1
            temp3=union2[j];//!当前元素1内部元素
            insideD1+=commCost[temp0][temp1];
            outsideD1+=commCost[temp0][temp3];
            insideD2+=commCost[temp2][temp3];
            outsideD2+=commCost[temp2][temp1];
        }
        // for(int k=0;k<TASKNUM;k++)
        // {
        //     allD1+=commCost[temp0][k];
        //     allD2+=commCost[temp2][k];
        // }
        //! Outside-inside
        D[temp0]=outsideD1-insideD1;
        D[temp2]=outsideD2-insideD2;
    }
    return 0;
}

int KL::gcalculator()
{
    int g=0;
    int max=INT8_MIN;
    int a1=0;
    int b1=0;

    for(int i=0;i<union1.size();i++)
    {
        int union1number=0;
        int union2number=0;
        int Cab=0;
        int Da=0;
        int Db=0;

        for(int j=0;j<union2.size();j++)
        {
            union1number=union1[i];
            union2number=union2[j];
            Da=D[union1number];
            Db=D[union2number];
            Cab=commCost[union1number][union2number];
            g=Da+Db-2*Cab;
            if(g>max)
            {
                max=g;
                a1=union1number;
                b1=union2number;
            }
        }
    }
    // cout << "max:" << max <<endl;
    // cout <<"a1: " << char('a'+a1) << endl;
    // cout <<"b1: " << char('a'+b1) << endl;
    G.push_back(max);//把G存起来
    A.push_back(a1);//把a1存起来
    B.push_back(b1);//把a2存起来

    //! 让我们来删掉a1和b1
    for(int i=0;i<union1.size();i++)
    {
        if(union1[i]==a1)
        {
            union1.erase(union1.begin()+i);
            break;
        }
    }

    for(int i=0;i<union2.size();i++)
    {
        if(union2[i]==b1)
        {
            union2.erase(union2.begin()+i);
            break;
        }
    }
    return 0;
}


int KL::Run()
{
    output1.clear();
    for(int i=0;i<TASKNUM;i++)
    {
        union1.push_back(i);
    }
    // cout << union1.size() << endl;

    srand((unsigned)time(NULL));
    while(union2.size()!=TASKNUM/2)
    {
        int temp=rand() % (TASKNUM-1);
        int flag=0;

        for(int i=0;i<union2.size();i++)
        {
            if(temp==union2[i])
            {
                flag=1;
                break;
            }
        }
        if(flag==0)
        {
            // cout << temp<< endl;   //!选出四个倒霉蛋 作为初始划分
            union2.push_back(temp);
        }
    }

    for(int i=0;i<union2.size();i++)
    {
        for(int j=0;j<union1.size();j++)
        {
            if(union2[i]==union1[j])
            {
                union1.erase(union1.begin()+j);
                break;
            }
        }
    }

    output1+= "union1: ";
    for(int i=0;i<union1.size();i++)
    {
        qDebug() <<"union1:" << char('a'+union1[i]);
        output1+= QString(char('a'+union1[i]))+' ';
    }
    output1+= '\n';
    output1+="union2: ";
    for(int i=0;i<union2.size();i++)
    {
        qDebug() <<"union2:" << char('a'+union2[i]);
        output1+= QString(char('a'+union2[i]))+' ';
    }
    output1+= '\n';

    int Cab2=0;
    for(int i=0;i<union1.size();i++)
    {
        int union1number=union1[i];
        // cout <<"union1:" << union1[i] << endl;
        for(int j=0;j<union2.size();j++)
        {
            int union2number=union2[j];
            Cab2+=commCost[union1number][union2number];
        }
    }
    qDebug() <<"OriCommCost:" << Cab2;
    output1+= "OriCommCost: " + QString::number(Cab2) + '\n';



    int sumG=0;
    int maxG=0;
    int k=0;
    do
    {
        sumG=0;
        maxG=-255;
        k=0;
        while(union1.size()!=0)
        {
            dCalulator();
            gcalculator();
            // for(int i=0;i<union1.size();i++)
            // {
            //     cout <<"union1:" << char('a'+union1[i]) << endl;
            // }
            // for(int i=0;i<union2.size();i++)
            // {
            //     cout <<"union2:" << char('a'+union2[i]) << endl;
            // }
        }

        for(int i=0;i<G.size();i++)
        {
            sumG+=G[i];
            if(sumG>maxG)
            {
                k=i;
                maxG=sumG;
            }
        }

        for(int i=0;i<G.size();i++)
        {
            // cout <<"i am in " << endl;
            if(i<=k && maxG>0)
            {
                union1.push_back(B[i]);
                union2.push_back(A[i]);
            }
            else
            {
                union1.push_back(A[i]);
                union2.push_back(B[i]);
            }
        }
        A.clear();
        B.clear();
        G.clear();
        // system("pause");
    }while(maxG>0);

    qDebug() << "maxG:" << maxG;
    output1+= "maxG:" + QString::number(maxG) + '\n';
    output1+= "union1: ";
    for(int i=0;i<union1.size();i++)
    {
        qDebug() <<"union1:" << char('a'+union1[i]);
        output1+= QString(char('a'+union1[i]))+' ';
    }
    output1+= '\n';
    output1+="union2: ";
    for(int i=0;i<union2.size();i++)
    {
        qDebug() <<"union2:" << char('a'+union2[i]);
        output1+= QString(char('a'+union2[i]))+' ';
    }
    output1+= '\n';

    int Cab1=0;
    for(int i=0;i<union1.size();i++)
    {
        int union1number=union1[i];
        // cout <<"union1:" << union1[i] << endl;
        for(int j=0;j<union2.size();j++)
        {
            int union2number=union2[j];
            Cab1+=commCost[union1number][union2number];
        }
    }
    qDebug() <<"CommCost:" << Cab1;
    output1+= "CommCost: " + QString::number(Cab1) + '\n';
    // system("pause");
    return 0;
}
