#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits> // For INT_MAX constant
#include <chrono>
using namespace std;
// std::chrono::steady_clock::time_point start_time, end_time;
vector<int> executionTimes = {4, 5, 4, 5, 3, 16, 3, 4, 14, 5, 20};
vector<int> hardwareExecutionTimes ={2,3,1,2,1,10,2,2,7,2,9};
vector<int> SquareUsed={4,2,4,7,5,3,4,2,3,5,5};
int SquareLimit=18;

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

// vector<int> executionTimes = {10, 4,7,8,12,6,5,10};
// vector<int> hardwareExecutionTimes ={8,2,5,4,5,3,5,4};
// vector<int> SquareUsed={4,3,1,1,6,5,2,3};
// int SquareLimit=11;

// // DAG dependencies
// vector<vector<int>> dependencies = {
//     {0,0,0,0,4,0,0,0},
//     {0,0,1,2,0,0,0,0},
//     {0,0,0,0,1,4,0,0},
//     {0,0,0,0,0,2,4,0},
//     {0,0,0,0,0,0,0,1},
//     {0,0,0,0,0,0,0,3},
//     {0,0,0,0,0,0,0,0},
//     {0,0,0,0,0,0,0,0}
// };


// Structure to represent a node in the DAG
struct Node {
    int id;
    int time;
    vector<int> dependencies;
    vector<int> pre;
    vector<int> succ;
    int ESTCheckTime;
    int earliestStartTime;
    int latestStartTime;
};

int GlobalEST=-1;
// Function to calculate the earliest start time for each node
void calculateEarliestStartTime(vector<Node>& nodes) {
    queue<int> q;
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i].pre.empty()) {
            // nodes[i].earliestStartTime = nodes[i].time;
            nodes[i].earliestStartTime = 0;
            q.push(i);
        }
    }

    while (!q.empty()) {
        int currNode = q.front();
        q.pop();
        if(nodes[currNode].succ.empty())
        {
            GlobalEST=max(GlobalEST,nodes[currNode].earliestStartTime+nodes[currNode].time);
        }

        for (int i = 0; i < nodes[currNode].succ.size(); i++) {
            int dependency = nodes[currNode].succ[i];
            nodes[dependency].earliestStartTime = max(nodes[dependency].earliestStartTime, nodes[currNode].earliestStartTime+nodes[currNode].time);

            nodes[dependency].ESTCheckTime++;
            // Check if all dependencies of the current node are satisfied
            bool allDependenciesSatisfied = false;
            if(nodes[dependency].ESTCheckTime==nodes[dependency].pre.size())
            {
                allDependenciesSatisfied = true;
            }
            if (allDependenciesSatisfied) {
                // cout << nodes[dependency].id <<  " " << nodes[dependency].earliestStartTime << endl;
                q.push(dependency);
            }
        }
    }
}

// Function to calculate the latest start time for each node
void calculateLatestStartTime(vector<Node>& nodes) {
    int lastNode = nodes.size() - 1;

    for (int i = lastNode; i >= 0; i--) {
        int minLatestStartTime = INT_MAX;
        if (nodes[i].succ.empty())
        {
           nodes[i].latestStartTime = GlobalEST-nodes[i].time;
           continue;
        }
        for (int j = 0; j < nodes[i].succ.size(); j++) {
            int dependency = nodes[i].succ[j];
            minLatestStartTime = min(minLatestStartTime, nodes[dependency].latestStartTime - nodes[i].time);
        }
        nodes[i].latestStartTime = minLatestStartTime;
    }
    // for(int i=0;i<nodes.size();i++)
    // {
    //     cout << nodes[i].id << ":" << nodes[i].latestStartTime << endl;
    // }
}

// Function to calculate the critical path
vector<int> calculateCriticalPath(vector<Node>& nodes) {
    vector<int> criticalPath;

    for(int i=0;i<nodes.size();i++)
    {
        if(nodes[i].earliestStartTime==nodes[i].latestStartTime)
        {
            criticalPath.push_back(i);
        }
    }
    // int lastNode = nodes.size() - 1;
    // criticalPath.push_back(lastNode); // Add the last node to the critical path

    // queue<int> q;
    // q.push(lastNode);

    // while (!q.empty()) {
    //     int currNode = q.front();
    //     q.pop();

    //     for (int i = 0; i < nodes[currNode].dependencies.size(); i++) {
    //         int dependency = nodes[currNode].dependencies[i];
    //         if (nodes[dependency].latestStartTime - nodes[dependency].time == nodes[currNode].latestStartTime) {
    //             criticalPath.push_back(dependency);
    //             q.push(dependency);
    //         }
    //     }
    // }

    // reverse(criticalPath.begin(), criticalPath.end()); // Reverse the critical path

    return criticalPath;
}


vector<int> critical_path_calculate(vector<Node>& nodes)
{
    GlobalEST=-1;
    calculateEarliestStartTime(nodes);
    // Calculate latest start time
    calculateLatestStartTime(nodes);
    // Calculate critical path
    vector<int> criticalPath = calculateCriticalPath(nodes);

    cout << "criticalpath:";
    for(int i=0;i<criticalPath.size();i++)
    {
        cout << criticalPath[i]+1 << " ";
    }
    cout << endl;
    return criticalPath;
}

void node_init(vector<Node>& nodes)
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


int main() {
    // Node execution times
    auto start = std::chrono::high_resolution_clock::now();

    // start_time = std::chrono::steady_clock::now();
    int numNodes = executionTimes.size();
    int squareUsed=0;
    vector<Node> nodes(numNodes);
    // Initialize nodes
    node_init(nodes);
    // nodes[10].time=9; //11
    // nodes[1].time=3;  //2
    // nodes[8].time=7;  //9
    // nodes[5].time=10;
    bool optimise=true;
    vector<int> criticalPath = critical_path_calculate(nodes);
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
            gain=executionTimes[currentNode]-hardwareExecutionTimes[currentNode];
            // if(gain/SquareUsed[currentNode]>=max_gain&&squareUsed+SquareUsed[currentNode]<=SquareLimit&&SquareUsed[currentNode]<tempsqureused)
            // {
            //     optimise=true;
            //     current_acc_node=currentNode;
            //     max_gain=gain/SquareUsed[currentNode];
            //     tempsqureused=SquareUsed[currentNode];
            // }

            if(gain>=max_gain&&squareUsed+SquareUsed[currentNode]<=SquareLimit&&SquareUsed[currentNode]<tempsqureused)
            {
                optimise=true;
                current_acc_node=currentNode;
                max_gain=gain;
                tempsqureused=SquareUsed[currentNode];
            }

            
        }
        if(optimise==true)
        {
            squareUsed+=SquareUsed[current_acc_node];
            executionTimes[current_acc_node]=hardwareExecutionTimes[current_acc_node];
            cout <<  "current_acc_node:T"<<current_acc_node+1 << " " << max_gain << endl;
            nodes.clear();
            nodes.resize(numNodes);
            node_init(nodes);
            criticalPath.clear();
            criticalPath = critical_path_calculate(nodes);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Execution time: " << duration << " microseconds" << std::endl;

    // end_time = std::chrono::steady_clock::now();
    // std::chrono::duration<double> duration = end_time - start_time;
    // std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
    return 0;
}