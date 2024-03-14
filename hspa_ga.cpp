#include "hspa_ga.h"
#include <boost/dynamic_bitset.hpp>
#include "QDebug"
#include <random>
#include "cmath"
//#include "unordered_map"
std::vector<boost::dynamic_bitset<>> Group;

static bool fpgaIdle=true;
static bool cpuIdle=true;
static int squareused=0;
//static int runtime=0;

struct GApair
{
    float fitness;
    boost::dynamic_bitset<> group;
};

bool GAcompare(const GApair& a, const GApair& b) {
    return a.fitness > b.fitness; // Change the comparison logic if needed (e.g., a.fitness > b.fitness for descending order)
}


int hspa_ga::checkresourceCPUStatus()
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


int hspa_ga::checkresourceFPGAStatus()
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


int hspa_ga::logresourceCPUStatus()
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


int hspa_ga::logresourceFPGAStatus()
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



hspa_ga::hspa_ga(int tasknum):taspa(tasknum)
{
    taskattribute.taskorder.resize(tasknum,0);
    taskattribute.taskNum=tasknum;
    taskattribute.sortExectime.resize(tasknum,0);
    taskattribute.hardExectime.resize(tasknum,0);
    taskattribute.hardwareArea.resize(tasknum,0);
    taskattribute.hardawaregain.resize(tasknum,0);
    taskattribute.relesetime.resize(tasknum,0);
    constraintsolution.variables.resize(tasknum);
//    ScheduletimeResult.resize();
//     HardwareUseSituation;
    TaskList.resize(tasknum);
//    Ganntinfo.resize(tasknum);
    squreUsed=0;
    runtime=0;
//    GlobalEST=-1;
}


int hspa_ga::task_reinit(std::vector<ProcessunitBlock> &Task)
{
    for (int i = 0; i < Task.size(); i++) {
//        if(Task[i].hardwareuse==SOFTWARE)
        Task[i].time = taskattribute.sortExectime[i];
//        else
//            Task[i].time = taskattribute.hardExectime[i];
        Task[i].usedResouceIndex=0;
        Task[i].finfish=false;
        Task[i].running=false;
        Task[i].need_time = -1;
        Task[i].arrive_time = -1;
        Task[i].finish_time = -1;
        Task[i].wait_time = taskattribute.relesetime[i];
    }


    return 0;
}


int hspa_ga::task_init(std::vector<ProcessunitBlock> &Task)
{
    for (int i = 0; i < Task.size(); i++) {
        Task[i].id = i+1;
        Task[i].time = taskattribute.sortExectime[i];
        Task[i].releasetime=taskattribute.relesetime[i];
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
        taskattribute.softwaretimeSum+=taskattribute.sortExectime[i];
    }

    return 0;
}


//int runningtask;
//int status=IDLE;//状态idel busy 空闲或忙碌
//int busytime=0;//忙碌的时间
//int idletime=0;
//std::vector<int> finishtask;

int hspa_ga::Schedule(std::vector<ProcessunitBlock> &Task)
{
    int cpu_size=cpu.size();
    int fpga_size=fpga.size();

    cpu.clear();
    fpga.clear();

    cpu.resize(cpu_size);
    fpga.resize(fpga_size);


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


std::vector<boost::dynamic_bitset<>> hspa_ga::crossover(const std::vector<boost::dynamic_bitset<>>& group,int tasknum) {
    std::vector<boost::dynamic_bitset<>> newGroup = group;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, group.size() - 1);

    while (newGroup.size() < tasknum*2) {
        // Select two random individuals from the group
        int parent1Index = dis(gen);
        int parent2Index = dis(gen);
        const boost::dynamic_bitset<>& parent1 = group[parent1Index];
        const boost::dynamic_bitset<>& parent2 = group[parent2Index];

        // Select two random crossover points
        std::uniform_int_distribution<> disPoint(1, parent1.size() - 2);
        int point1 = disPoint(gen);
        int point2 = disPoint(gen);
        if (point1 > point2) {
            std::swap(point1, point2);
        }

        // Perform crossover
        boost::dynamic_bitset<> child1 = parent1;
        boost::dynamic_bitset<> child2 = parent2;
        for (int i = point1; i <= point2; ++i) {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }

        // Add the new individuals to the new group
        newGroup.push_back(child1);
        if(newGroup.size()<tasknum*2)
        newGroup.push_back(child2);
    }

    return newGroup;
}

std::vector<boost::dynamic_bitset<>>  hspa_ga::mutate(const std::vector<boost::dynamic_bitset<>>& group) {
    std::vector<boost::dynamic_bitset<>> mutatedGroup = group;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (auto& individual : mutatedGroup) {
        for (size_t i = 0; i < individual.size(); ++i) {
        if (dis(gen) < 0.005) { // Mutation probability (Pm=0.005)
//                qDebug() << "Mutate!";
                int numGenesToFlip = std::uniform_int_distribution<>(1, 3)(gen); // Randomly select 1 to 3 genes to flip
                for (int j = 0; j < numGenesToFlip; ++j) {
                    size_t geneIndex = std::uniform_int_distribution<>(0, individual.size() - 1)(gen);
                    individual.flip(geneIndex); // Flip the selected gene (0 to 1 or 1 to 0)
                }
        }
        }
    }

//    qDebug() << "Mutate Finish!";
    return mutatedGroup;
}

int hspa_ga::geneticAlgorithm()
{
    GeneAlpha();
    std::random_device rd;  // Seed for the random number generator
    std::mt19937 gen(rd()); // Mersenne Twister engine for random numbers
    std::uniform_int_distribution<> dis(0, 1); // Distribution for generating random 0 or 1

    for(int i=0;i<2*taskattribute.taskNum-1;i++)
    {
        boost::dynamic_bitset<> tempvariables(taskattribute.taskNum);
        for(size_t j = 0; j < taskattribute.taskNum; j++)
        {
            bool randomBit = dis(gen); // Generate a l  random bit using the distribution and generator
            tempvariables[j] = randomBit;
        }
        Group.push_back(tempvariables);
    }
    //
    for(int i=0;i<Group.size();i++)
    {
//        QString test;
//        for (int k = 0; k < Group[i].size(); k++) {
//            test += std::to_string(Group[i][k]) + " ";
//        }
//        qDebug() << test;
        //选出每一个Group里的元素进行计算
        for(int j=0;j<Group[i].size();j++)
        {
            if(Group[i][j]==1)
            {
                TaskList[j].hardwareuse=HARDWARE;//硬件做
                squreUsed+=taskattribute.hardwareArea[j];
            }
            else
            {
                TaskList[j].hardwareuse=SOFTWARE;//软件做
            }
        }
        HardwareUseSituation.push_back(squreUsed);
        for(int l=0;l<taskattribute.taskNum;l++)
        {
            if(TaskList[l].hardwareuse==SOFTWARE || fpga.size()==0)
            {
                taspa.val[l]=taskattribute.sortExectime[l];  //给执行时间
                taspa.task[l].softorhard=SOFT;
                TaskList[l].hardwareuse=SOFTWARE;
            }
            else
            {
                taspa.val[l]=taskattribute.hardExectime[l];
                taspa.task[l].softorhard=HARD;
                TaskList[l].hardwareuse=HARDWARE;
                //            qDebug() << "T" << i+1;

            }
        }
        taspa.Retime.assign(taskattribute.relesetime.begin(),taskattribute.relesetime.end()); //给通讯时间
        for (const std::vector<int>& subVector : taskattribute.CommunicationCost) {  //给依赖矩阵
            taspa.array.emplace_back(subVector.begin(), subVector.end());
        }

        taspa.RunHaspa();

        for(int m=0;m<taskattribute.taskNum;m++)
        {
            taskattribute.taskorder[m]=taspa.task[m].order;
        }




        //这个时候开始调度
        Schedule(TaskList);

        ScheduletimeResult.push_back(runtime);

        task_reinit(TaskList);
        squreUsed=0;
        runtime=0;
        taspa.resize(taskattribute.taskNum);
    }

    //计算适应度
    for(int i=0;i<2*taskattribute.taskNum;i++)
    {
//        if(HardwareUseSituation[i]>taskattribute.areaConstraints)
//        {
////            qDebug() << "It happens!";
//            fitnessUnion.push_back(-100);
//        }
//        else
//        {
        float temp1=(float)(HardwareUseSituation[i]-taskattribute.areaConstraints)/taskattribute.areaConstraints;
        float temp2=(float)abs(HardwareUseSituation[i]-taskattribute.areaConstraints)/taskattribute.areaConstraints;
        float fitnessTemp=1/( (0.7*pow(M_E,temp1)*temp2)+(0.3*(float)ScheduletimeResult[i]/taskattribute.softwaretimeSum) );
//        qDebug() << HardwareUseSituation[i] << " " <<  taskattribute.areaConstraints <<  " " << temp1 <<  " " << temp2 << " " <<   fitnessTemp << " " << ScheduletimeResult[i] << " " << taskattribute.softwaretimeSum;
        fitnessUnion.push_back(fitnessTemp);
//        }
//        qDebug() <<temp1 << " " << temp2 << " " <<fitnessTemp;
    }
    HardwareUseSituation.clear();

    ScheduletimeResult.clear();
    //此时开始计算适应度 fitness_calculation(),制造新的种群

    std::vector<GApair> data(2*taskattribute.taskNum);
    for(int i=0;i<data.size();i++)
    {
        data[i].fitness=fitnessUnion[i];
        data[i].group=Group[i];
    }
    fitnessUnion.clear();
    int gen1=0;
    int Gen=3*taskattribute.taskNum;

//    unordered_map<
    //选择前四分之一直接进入下一代,剩下的开赌

    //group 和 fitness的i是挂钩的


//    qDebug() << "Run here";
    while(gen1<Gen)
    {
        Group.clear();
        std::sort(data.begin(), data.end(), GAcompare);

        for(int i=0;i<taskattribute.taskNum/2;i++) //直接选择前1/4;
        {
            Group.push_back(data[i].group);
        }


        for(int i=taskattribute.taskNum/2;i<data.size();i++) //后3/4进行轮盘赌
        {
            bool randomBit = dis(gen);
            if(randomBit>0.5)
            {
                Group.push_back(data[i].group);
            }
        }
//        qDebug() << "Run here...";

        while (Group.size() < taskattribute.taskNum * 2) {
//            qDebug() << "Run here~";
            Group = crossover(Group,taskattribute.taskNum);
//            qDebug() << "Run here!";
        }

        // Print the resulting group

//        qDebug() << "After Crossover:"<< Group.size();
//        for(int i=0;i< Group.size(); i++)
//        {
////            QString test;
//            for (int k = 0; k < Group[i].size(); k++) {
//                test += std::to_string(Group[i][k]) + " ";
//            }
////            qDebug() << test;
//        }
        //进行变异
        Group = mutate(Group);



        //重新开始调度

        for(int i=0;i<Group.size();i++)
        {
//            QString test;
//            for (int k = 0; k < Group[i].size(); k++) {
//                test += std::to_string(Group[i][k]) + " ";
//            }
//            qDebug() << test;
            //选出每一个Group里的元素进行计算
            for(int j=0;j<Group[i].size();j++)
            {
                if(Group[i][j]==1)
                {
                    TaskList[j].hardwareuse=HARDWARE;//硬件做
                    squreUsed+=taskattribute.hardwareArea[j];
                }
                else
                {
                    TaskList[j].hardwareuse=SOFTWARE;//软件做
                }
            }
            HardwareUseSituation.push_back(squreUsed);
            for(int l=0;l<taskattribute.taskNum;l++)
            {
                if(TaskList[l].hardwareuse==SOFTWARE || fpga.size()==0)
                {
                    taspa.val[l]=taskattribute.sortExectime[l];  //给执行时间
                    taspa.task[l].softorhard=SOFT;
                    TaskList[l].hardwareuse=SOFTWARE;
                }
                else
                {
                    taspa.val[l]=taskattribute.hardExectime[l];
                    taspa.task[l].softorhard=HARD;
                    TaskList[l].hardwareuse=HARDWARE;
                    //            qDebug() << "T" << i+1;

                }
            }
            taspa.Retime.assign(taskattribute.relesetime.begin(),taskattribute.relesetime.end()); //给通讯时间
            for (const std::vector<int>& subVector : taskattribute.CommunicationCost) {  //给依赖矩阵
                taspa.array.emplace_back(subVector.begin(), subVector.end());
            }

            taspa.RunHaspa();

            for(int m=0;m<taskattribute.taskNum;m++)
            {
                taskattribute.taskorder[m]=taspa.task[m].order;
            }




            //这个时候开始调度
            Schedule(TaskList);

            ScheduletimeResult.push_back(runtime);

            task_reinit(TaskList);
            squreUsed=0;
            runtime=0;
            taspa.resize(taskattribute.taskNum);
        }

        //计算适应度
        for(int i=0;i<2*taskattribute.taskNum;i++)
        {
//            if(HardwareUseSituation[i]>taskattribute.areaConstraints)
//            {
////                qDebug() << "It happens!";
//                fitnessUnion.push_back(-100);
//            }
//            else
//            {
            float temp1=(float)(HardwareUseSituation[i]-taskattribute.areaConstraints)/taskattribute.areaConstraints;
            float temp2=(float)abs(HardwareUseSituation[i]-taskattribute.areaConstraints)/taskattribute.areaConstraints;
            float fitnessTemp=1/( (0.7*pow(M_E,temp1)*temp2)+(0.3*(float)ScheduletimeResult[i]/taskattribute.softwaretimeSum) );
//            qDebug() << HardwareUseSituation[i] << " " <<  taskattribute.areaConstraints <<  " " << temp1 <<  " " << temp2 << " " <<   fitnessTemp << " " << ScheduletimeResult[i] << " " << taskattribute.softwaretimeSum;
            fitnessUnion.push_back(fitnessTemp);
//            qDebug() <<temp1 << " " << temp2 << " " <<fitnessTemp;
//            }
        }
        HardwareUseSituation.clear();
//        HardwareUseSituation.clear();

        ScheduletimeResult.clear();
        //此时开始计算适应度 fitness_calculation(),制造新的种群

        std::vector<GApair> data(2*taskattribute.taskNum);
        for(int i=0;i<data.size();i++)
        {
            data[i].fitness=fitnessUnion[i];
            data[i].group=Group[i];
        }
        fitnessUnion.clear();
        gen1++;
//        return 0;

    }

    //此时data中最高的应该就是最终结果了
    std::sort(data.begin(), data.end(), GAcompare);

    QString test;
    for (int k = 0; k < data[0].group.size(); k++) {
        test += std::to_string(data[0].group[k]) + " ";
    }
    qDebug() << test;
    qDebug() << data[0].fitness;

    for(int j=0;j<data[0].group.size();j++)
    {
        if(data[0].group[j]==1)
        {
            TaskList[j].hardwareuse=HARDWARE;//硬件做
            squreUsed+=taskattribute.hardwareArea[j];
        }
        else
        {
            TaskList[j].hardwareuse=SOFTWARE;//软件做
        }
    }

    qDebug() << "SquareUsed=" << squreUsed;
    for(int l=0;l<taskattribute.taskNum;l++)
    {
        if(TaskList[l].hardwareuse==SOFTWARE || fpga.size()==0)
        {
            taspa.val[l]=taskattribute.sortExectime[l];  //给执行时间
            taspa.task[l].softorhard=SOFT;
            TaskList[l].hardwareuse=SOFTWARE;
        }
        else
        {
            taspa.val[l]=taskattribute.hardExectime[l];
            taspa.task[l].softorhard=HARD;
            TaskList[l].hardwareuse=HARDWARE;
            //            qDebug() << "T" << i+1;

        }
    }
    taspa.Retime.assign(taskattribute.relesetime.begin(),taskattribute.relesetime.end()); //给通讯时间
    for (const std::vector<int>& subVector : taskattribute.CommunicationCost) {  //给依赖矩阵
        taspa.array.emplace_back(subVector.begin(), subVector.end());
    }

    taspa.RunHaspa();

    for(int m=0;m<taskattribute.taskNum;m++)
    {
        taskattribute.taskorder[m]=taspa.task[m].order;
    }




    //这个时候开始调度
    Schedule(TaskList);

    output+="Orange means it's done by software. Red means it's done by hardware.\n";
    output+="SquareUsed:" +  QString::number(squreUsed) + "\n" + "Runtime:" + QString::number(runtime)+"\n";
    qDebug()<<runtime;


    return 0;
}

int hspa_ga::GeneAlpha()
{

    boost::dynamic_bitset<> variables(taskattribute.taskNum,0);
    for(int i=0;i<taskattribute.taskNum;i++)
    {
        taskattribute.hardawaregain[i]=taskattribute.sortExectime[i]-taskattribute.hardExectime[i];
    }
    squreUsed=0;
    while(squreUsed<taskattribute.areaConstraints)
    {
        auto maxPosition = max_element(taskattribute.hardawaregain.begin(), taskattribute.hardawaregain.end());
        int maxposition=maxPosition-taskattribute.hardawaregain.begin();
        bool optimise=false;
        if(squreUsed+taskattribute.hardwareArea[maxposition]<=taskattribute.areaConstraints)
        {
            optimise=true;
            TaskList[maxposition].hardwareuse=HARDWARE;
            squreUsed+=taskattribute.hardwareArea[maxposition];
            qDebug() << maxposition+1  << " " << taskattribute.hardwareArea[maxposition] << " " << squreUsed;
            taskattribute.hardawaregain[maxposition]=0;
        }
        if(optimise==false)
        {
            break;
        }

    }
    for(int i=0;i<taskattribute.taskNum;i++)
    {
        if(TaskList[i].hardwareuse==HARDWARE)
        {
            variables[i]=1;
        }
    }
    Group.push_back(variables);
//    QString test;
//    for (int i = 0; i < variables.size(); i++) {
//        test += std::to_string(variables[i]) + " ";
//    }
//    qDebug() << test;
    return 0;
}

int hspa_ga::run()
{
    task_init(TaskList);

    geneticAlgorithm();
    //遗传+调度

    return 0;
}

QString hspa_ga::result_output()
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

