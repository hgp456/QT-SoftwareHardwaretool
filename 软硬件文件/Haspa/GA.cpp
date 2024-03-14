#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits> // For INT_MAX constant
#include <chrono>
using namespace std;

struct ProcessunitBlock
{
    int id;
    int time;
    vector<int> pre;
    vector<int> succ;
    int ESTCheckTime;
    int earliestStartTime;
    int latestStartTime;
    int need_time;//需要时间
    int exec_time;//执行时间
    int wait_time;//等待时间
    int arrive_time;//到达时间(开始时间)
    int finish_time;//结束时间
};

vector<int> executionTimes = {4, 5, 4, 5, 3, 16, 3, 4, 14, 5, 20};
vector<int> hardwareExecutionTimes ={2,3,1,2,1,10,2,2,7,2,9};
vector<int> SquareUsed={4,2,4,7,5,3,4,2,3,5,5};
int SquareLimit=18;

// DAG dependencies
vector<vector<int>> dependencies = {
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

void node_init(vector<ProcessunitBlock>& nodes)
{
    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].id = i+1;
        nodes[i].time = executionTimes[i];
        nodes[i].earliestStartTime = -1;
        nodes[i].latestStartTime = -1;
        nodes[i].ESTCheckTime = 0;
        for (int j = 0; j < nodes.size(); j++){
            if (dependencies[i][j] > 0) {
                nodes[i].succ.push_back(j);
            }
            if (dependencies[j][i] > 0) {
                nodes[i].pre.push_back(j);
            }
        }
    }
}

int main()
{

}