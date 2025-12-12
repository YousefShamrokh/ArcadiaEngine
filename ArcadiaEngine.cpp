// ArcadiaEngine.cpp - STUDENT TEMPLATE
// TODO: Implement all the functions below according to the assignment requirements

#include "ArcadiaEngine.h"
#include <algorithm>
#include <queue>
#include <numeric>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>

using namespace std;

// =========================================================
// PART A: DATA STRUCTURES (Concrete Implementations)
// =========================================================

// --- 1. PlayerTable (Double Hashing) ---

class ConcretePlayerTable : public PlayerTable {
private:
    // TODO: Define your data structures here
    // Hint: You'll need a hash table with double hashing collision resolution

public:
    ConcretePlayerTable() {
        // TODO: Initialize your hash table
    }

    void insert(int playerID, string name) override {
        // TODO: Implement double hashing insert
        // Remember to handle collisions using h1(key) + i * h2(key)
    }

    string search(int playerID) override {
        // TODO: Implement double hashing search
        // Return "" if player not found
        return "";
    }
};

// --- 2. Leaderboard (Skip List) ---

class ConcreteLeaderboard : public Leaderboard {
private:
    // TODO: Define your skip list node structure and necessary variables
    // Hint: You'll need nodes with multiple forward pointers

public:
    ConcreteLeaderboard() {
        // TODO: Initialize your skip list
    }

    void addScore(int playerID, int score) override {
        // TODO: Implement skip list insertion
        // Remember to maintain descending order by score
    }

    void removePlayer(int playerID) override {
        // TODO: Implement skip list deletion
    }

    vector<int> getTopN(int n) override {
        // TODO: Return top N player IDs in descending score order
        return {};
    }
};

// --- 3. AuctionTree (Red-Black Tree) ---

class ConcreteAuctionTree : public AuctionTree {
private:
    // TODO: Define your Red-Black Tree node structure
    // Hint: Each node needs: id, price, color, left, right, parent pointers

public:
    ConcreteAuctionTree() {
        // TODO: Initialize your Red-Black Tree
    }

    void insertItem(int itemID, int price) override {
        // TODO: Implement Red-Black Tree insertion
        // Remember to maintain RB-Tree properties with rotations and recoloring
    }

    void deleteItem(int itemID) override {
        // TODO: Implement Red-Black Tree deletion
        // This is complex - handle all cases carefully
    }
};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================

int InventorySystem::optimizeLootSplit(int n, vector<int>& coins) {
    if (n==0) return 0; // base cases
    if (n==1) return coins[0]; // base cases
    if (n==2) return abs(coins[0]-coins[1]); // base cases
    int totalCoins = 0; //
    for (int i = 0; i < n; ++i) totalCoins+=coins[i];
    int bestSplitNum = totalCoins / 2;
    vector<bool> subsetArr(bestSplitNum+1,false); subsetArr[0] = true;
    for (int i = 0; i < n; ++i) {
        for (int j = bestSplitNum; j >= coins[i]; --j) {
            if (subsetArr[j - coins[i]] == true) subsetArr[j] = true;
        }
    }
    int closestSum = 0;
    for (int i = bestSplitNum; i >= 0; --i) {
        if (subsetArr[i]) {
            closestSum = i;
            break;
        }
    }
    int difference = totalCoins - 2 * closestSum;
    return difference;
    // TODO: Implement partition problem using DP
    // Goal: Minimize |closestSum(subset1) - closestSum(subset2)|
    // Hint: Use subset closestSum DP to find closest closestSum to totalCoins/2
}

int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items) {
    // TODO: Implement 0/1 Knapsack using DP
    // items = {weight, value} pairs
    // Return maximum value achievable within capacity
    return 0;
}

long long InventorySystem::countStringPossibilities(string s) {
    // TODO: Implement string decoding DP
    // Rules: "uu" can be decoded as "w" or "uu"
    //        "nn" can be decoded as "m" or "nn"
    // Count total possible decodings
}

// =========================================================
// PART C: WORLD NAVIGATOR (Graphs)
// =========================================================

bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest) {
    // TODO: Implement path existence check using BFS or DFS
    // edges are bidirectional
    //first, set up the adjacency list
    vector<vector<int>> adjacencyList(n);
    //let u and v be two adjacent nodes
    int u,v;
    //iterate over each row in the 2d vector, and get the two nodes that are in there
    //then add them to each other's adjacency list
    for (int i = 0; i < edges.size(); ++i) {
        u = edges[i][0];
        v = edges[i][1];
        adjacencyList[u].push_back(v);
        adjacencyList[v].push_back(u);
    }
    //APPLYING BFS
    //create a queue to keep track of nodes to visit
    queue<int> q;
    //create a bool vector to keep track of nodes we have visited before to avoid infinite loops
    vector<bool> seen(n, false);

    q.push(source);
    seen[source] = true;
    //BFS logic
    int currentVertex, connectedVertex;
    while (!q.empty()) {
        //take the first element in the queue
        currentVertex = q.front();
        q.pop();
        if (currentVertex == dest) return true;
        //iterate over all the vertices that are connected to the current vertex and add them
        //to 'seen' vector and the queue only if we have not seen them before
        for (int i = 0; i < adjacencyList[currentVertex].size(); ++i) {
            connectedVertex = adjacencyList[currentVertex][i];
            if (!seen[connectedVertex]) {
                seen[connectedVertex] = true;
                q.push(connectedVertex);
            }
        }
        //now on the next iteration in the while loop, we will check the next node that was pushed into the q
    }
    return false;
}


long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate,
                                       vector<vector<int>>& roadData)
{
    // TODO: Implement Minimum Spanning Tree (Kruskal's or Prim's)
    // roadData[i] = {u, v, goldCost, silverCost}
    // Total cost = goldCost * goldRate + silverCost * silverRate
    // Return -1 if graph cannot be fully connected
    // Build adjacency list: each entry -> (neighbor, weight)


    //adjacency list, each entry has a connected vertex and the weight to get there from a pair
    vector<vector<pair<int,long long>>> adjacencyList(n);

    //start by setting up the adjacency list
    for (int i =0 ; i < roadData.size(); ++i) {
        //first two entries in road data are the two vertices
        int u = roadData[i][0];
        int v = roadData[i][1];
        //third entry is the gold value
        long long gold  = roadData[i][2];
        //forth entry is the silver value
        long long silver = roadData[i][3];

        //calculate the total cost of the road/edge
        long long w = gold * goldRate + silver * silverRate;

        //add the connected vertex and weight info to the adjacency list
        adjacencyList[u].push_back({v, w});
        adjacencyList[v].push_back({u, w});
    }

    // vector to keep track of the min key to reach a vertex
    vector<long long> key(n, LLONG_MAX);
    //vector of vertices we have visited
    vector<bool> seen(n, false);

    //prio queue of (key, vertex)
    //priority_queue<Type, Container, Comparator>
    priority_queue<pair<long long,int>,
                   vector<pair<long long,int>>,
                   greater<pair<long long,int>>> pq;

    //start from vertex 0 (or any vertex)
    key[0] = 0;
    pq.push({0, 0});

    long long mstCost = 0;
    int visitedCount = 0;

    //loop until the queue is empty (we have checked all nodes)
    while (!pq.empty()) {
        //get the lowest key element from the queue
        pair<long long,int> currNode = pq.top();
        pq.pop();

        //node index is the second value in the pair
        int nodeIndex = currNode.second;
        //node wight is the first value in the pair
        int nodeWeight = currNode.first;

        //skip stale/repeated entries
        if (seen[nodeIndex]) continue;

        seen[nodeIndex] = true;
        mstCost += nodeWeight;
        visitedCount++;

        //check all adjacent nodes to the current node we are on, and add it to the queue
        for (int i =0; i < adjacencyList[nodeIndex].size(); ++i) {
            int vertex = adjacencyList[nodeIndex][i].first;
            long long weight = adjacencyList[nodeIndex][i].second;

            //if vertex not in MST and weight < the last updated weight: update the key and add to queue
            if (!seen[vertex] && weight < key[vertex]) {
                key[vertex] = weight; //update weight
                pq.push({weight, vertex}); // add to queue
            }
        }
    }

    //if we visited all vertices then MST completed
    if (visitedCount == n)
        return mstCost;

    return -1; //graph is disconnected
}


string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
    const long long INF = 9999999999999;
    vector<vector<long long>> distance(n, vector<long long>(n,INF));
    vector<vector<bool>> connectedVertex(n,vector<bool>(n, false));
    for(int i = 0; i < n; ++i) {
        distance[i][i] = 0;
        connectedVertex[i][i]= true;
    }
    for(int i = 0; i < roads.size(); ++i) {
        connectedVertex[roads[i][0]][roads[i][1]] = true;
        connectedVertex[roads[i][1]][roads[i][0]] = true;
        if (roads[i][2]<distance[roads[i][0]][roads[i][1]]) {
            distance[roads[i][0]][roads[i][1]] = roads[i][2];
            distance[roads[i][1]][roads[i][0]] = roads[i][2];
        }
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (distance[i][k]!=INF && distance[k][j]!=INF){
                    if (distance[i][j] > distance[i][k] + distance[k][j]) {
                        distance[i][j] = distance[i][k] + distance[k][j];
                    }
                }
            }
        }
    }
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (distance[i][j]!=INF) sum += distance[i][j];
        }
    }
//    cout << sum << "\n";
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j) {
//            cout << to_string(distance[i][j]) + " ";
//        }
//        cout << endl;
//    }
    string binaryConversion = "";
    if (sum == 0) return "0";
    while (sum>0){
        if (sum%2==0) binaryConversion = "0" + binaryConversion;
        else binaryConversion = "1" + binaryConversion;
        sum/=2;
    }
    return binaryConversion;
    // TODO: Implement All-Pairs Shortest Path (Floyd-Warshall)
    // Sum all shortest distances between unique pairs (i < j)
    // Return the sum as a binary string
    // Hint: Handle large numbers carefully
}

// =========================================================
// PART D: SERVER KERNEL (Greedy)
// =========================================================

int ServerKernel::minIntervals(vector<char>& tasks, int n) {
    // TODO: Implement task scheduler with cooling time
    // Same task must wait 'n' intervals before running again
    // Return minimum total intervals needed (including idle time)
    // Hint: Use greedy approach with frequency counting
    return 0;
}

// =========================================================
// FACTORY FUNCTIONS (Required for Testing)
// =========================================================

extern "C" {
PlayerTable* createPlayerTable() {
    return new ConcretePlayerTable();
}

Leaderboard* createLeaderboard() {
    return new ConcreteLeaderboard();
}

AuctionTree* createAuctionTree() {
    return new ConcreteAuctionTree();
}
}

int main(){
    //goz2 Essam
//    vector<int> v = {1,2,3,4,7};
//    InventorySystem::optimizeLootSplit(v.size(),v);
    vector<vector<int>> graph= {{0,1,1},{1,2,2}};
    vector<vector<int>> graph2= {{0,1,2},{0,2,8}};
    vector<vector<int>> graph3= {{0,1,4}};
    cout << WorldNavigator::sumMinDistancesBinary(3,graph) << endl;
    cout << WorldNavigator::sumMinDistancesBinary(2,graph3) << endl;
    cout << WorldNavigator::sumMinDistancesBinary(3,graph2) << endl;

    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------
    //goz2 seif :D
    //TESTING PATH EXISTS
//    vector<vector<int>> graph3= {{0,1},{0,2}, {3,4}};
//    cout << WorldNavigator::pathExists(5, graph3, 0, 4  ) << endl; // 1--->4 should return false
//    cout << WorldNavigator::pathExists(5, graph3, 1, 2  ) << endl; // 1--->2 should return true
//
//    //TESTING MIN BRIBE COST
//    //example 1: connected graph
//    //roadData: {u, v, goldCost, silverCost}
//    vector<vector<int>> roads = {{0,1,10,0},{1,2,5,0},{0,2,20,0}};
//
//    long long cost = WorldNavigator::minBribeCost(3, 3, 1, 1, roads);
//    cout << "MST cost (Example 1): " << cost << endl; //expected: 15
//
//    //example 2: disconnected graph
//    vector<vector<int>> roads2 = {{0,1,5,0},{2,3,7,0}};
//
//    cost = WorldNavigator::minBribeCost(4, 2, 1, 1, roads2);
//    cout << "MST cost (Example 2): " << cost << endl; //expected: -1 (disconnected)
//
//    //example 3: graph with both gold and silver costs
//    vector<vector<int>> roads3 = {
//        {0,1,1,2}, // cost = 1*2 + 2*3 = 2 + 6 = 8
//        {1,2,2,1}, // cost = 2*2 + 1*3 = 4 + 3 = 7
//        {0,2,3,1}  // cost = 3*2 + 1*3 = 6 + 3 = 9
//    };
//
//    cost = WorldNavigator::minBribeCost(3, 3, 2, 3, roads3);
//    cout << "MST cost (Example 3): " << cost << endl; //expected: 8 + 7 = 15

    return 0;
}