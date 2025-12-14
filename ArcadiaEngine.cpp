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
    enum Color { RED, BLACK };

    struct Node{
        int itemID;
        int price;
        Color color;
        Node* left;
        Node* right;
        Node* parent;
        Node(int id=0, int p=0) : itemID(id), price(p), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;
    Node* NIL;

    // Create a new node
    Node* createNode(int id, int price) {
        Node* n = new Node(id, price);
        n->left = n->right = n->parent = NIL;
        n->color = RED;
        return n;
    }

    // Left rotate function
    void leftRotate(Node* x) {
        Node* y = x->right; // let y be x's right child

        x->right = y->left; // turn y's left subtree into x's right subtree

        // update parent pointer of y's left child
        if (y->left != NIL)
            y->left->parent = x;

        // let x's parent be y's parent
        y->parent = x->parent;

        // update root or x's parent's child pointer
        if (x->parent == NIL)
            root = y;

        // if x is a left child
        else if (x == x->parent->left)
            // let y be the left child of x's parent
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // Right rotate function
    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;

        if (y->right != NIL)
            y->right->parent = x;

        y->parent = x->parent;

        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    // insert fix function
    void insertFix(Node* z) {
        // fix the red-black tree that will only happen if z's parent is red also
        while (z->parent->color == RED) {
            // case A: z's parent is a left child
            if (z->parent == z->parent->parent->left) {
                // y is z's uncle
                Node* y = z->parent->parent->right;

                // case 1: uncle is red
                if (y->color == RED) {
                    // recolor parent and uncle to black and grandparent to red
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    // move z up the tree to check if there is any violations
                    z = z->parent->parent;
                }
                // case 2 and 3: uncle is black
                else {
                    // case 2: z is a right child
                    if (z == z->parent->right) {
                        // move z up the tree and make left rotation on parent
                        z = z->parent;
                        leftRotate(z);
                    }
                    // case 3: z is a left child
                    // recolor parent to black and grandparent to red
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    // right rotate on grandparent
                    rightRotate(z->parent->parent);
                }
            }
            // case B: z's parent is a right child
            else {
                // y is z's uncle
                Node* y = z->parent->parent->left;

                // case 1: uncle is red
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                // case 2 and 3: uncle is black
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    // case 3: z is a right child
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    // left rotate on grandparent
                    leftRotate(z->parent->parent);
                }
            }
        }
        // to make sure that the root is always black
        root->color = BLACK;
    }

    // transplant function for deletion
    void transplant(Node* u, Node* v) {
        // if u is root
        if (u->parent == NIL)
            // make v the new root
            root = v;
        // if u is a left child
        else if (u == u->parent->left)
            // make v the left child of u's parent
            u->parent->left = v;
        else
            // make v the right child of u's parent
            u->parent->right = v;

        // make u's parent be v's parent
        v->parent = u->parent;
    }

    // minimum function for deletion
    Node* minimum(Node* x) {
        while (x->left != NIL)
            x = x->left;
        return x;
    }

    // delete fix function
    void deleteFix(Node* x) {
        // fix the red-black tree that will only happen if x is black
        // x is the node that came to replace the deleted node and may carry the double black property
        while (x != root && x->color == BLACK) {

            // case A: x is a left child
            if (x == x->parent->left) {
                // w is x's sibling
                Node* w = x->parent->right;

                // case 1: w is red
                if (w->color == RED) {
                    // recolor w to black and parent to red then left rotate on parent and update w
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }

                // case 2 and 3: w is black
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    // case 2: both of w's children are black
                    // recolor w to red and move x up the tree to check for further violations
                    w->color = RED;
                    x = x->parent;
                }
                // case 3: at least one of w's children is red
                else {
                    // case 3a: w's right child is black
                    if (w->right->color == BLACK) {
                        // recolor w's left child to black and w to red then right rotate on w and update w
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    // case 3b: w's right child is red
                    // recolor w to parent's color and parent to black and w's right child to black
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            // case B: x is a right child
            else {
                Node* w = x->parent->left;

                // case 1: w is red
                if (w->color == RED) {
                    // recolor w to black and parent to red then right rotate on parent and update w
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }

                // case 2 and 3: w is black
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    // case 2: both of w's children are black
                    // recolor w to red and move x up the tree to check for further violations
                    w->color = RED;
                    x = x->parent;
                }
                // case 3: at least one of w's children is red
                else {
                    // case 3a: w's left child is black
                    // recolor w's right child to black and w to red then left rotate on w and update w
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    // case 3b: w's left child is red
                    // recolor w to parent's color and parent to black and w's left child to black
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        // to remove double black property from x
        x->color = BLACK;
    }

    // search node by itemID
    Node* searchByID(Node* x, int itemID) {
        // base case
        if (x == NIL) return NIL;
        if (x->itemID == itemID) return x;

        // search in left subtree
        Node* left = searchByID(x->left, itemID);
        if (left != NIL) return left;

        // search in right subtree
        return searchByID(x->right, itemID);
    }

    // helper function to delete all nodes (used in destructor)
    void deleteAll(Node* x) {
        if (x != NIL) {
            deleteAll(x->left);
            deleteAll(x->right);
            delete x;
        }
    }

public:
    // Constructor and Destructor
    ConcreteAuctionTree() {
        NIL = new Node();
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }

    ~ConcreteAuctionTree() {
        deleteAll(root);
        delete NIL;
    }

    // insert item
    void insertItem(int itemID, int price) override {
        Node* z = createNode(itemID, price);
        Node* y = NIL;
        Node* x = root;

        while (x != NIL) {
            // find the correct position to insert
            y = x;
            if (price < x->price)
                x = x->left;
            else if (price > x->price)
                x = x->right;
            else if (itemID < x->itemID)
                x = x->left;
            else
                x = x->right;
        }

        // insert z
        z->parent = y;

        if (y == NIL)
            root = z;
        else if (price < y->price)
            y->left = z;
        else if (price > y->price)
            y->right = z;
        else if (itemID < y->itemID)
            y->left = z;
        else
            y->right = z;

        insertFix(z);
    }

    // delete item
    void deleteItem(int itemID) override {
        Node* z = searchByID(root, itemID);
        if (z == NIL) return;

        Node* y = z;
        Node* x;
        Color yOriginalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z)
                x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        if (yOriginalColor == BLACK)
            deleteFix(x);
    }

//    // for testing
//    void inorderPrint(Node* x) {
//        if (x == NIL) return;
//        inorderPrint(x->left);
//        cout << "(" << x->itemID << "," << x->price
//             << "," << (x->color == RED ? "R" : "B") << ") ";
//        inorderPrint(x->right);
//    }

//    void debugPrintAll() {
//        inorderPrint(root);
//        cout << endl;
//    }

    void printTree(Node* node, string indent, bool last) {
        if (node == NIL) return;

        cout << indent;
        if (last) {
            cout << "R----";
            indent += "     ";
        } else {
            cout << "L----";
            indent += "|    ";
        }

        cout << "(" << node->itemID << ","
             << node->price << ","
             << (node->color == RED ? "R" : "B") << ")\n";

        printTree(node->left, indent, false);
        printTree(node->right, indent, true);
    }

    void visualize() {
        if (root == NIL) {
            cout << "[empty tree]\n";
            return;
        }
        printTree(root, "", true);
    }

};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================

int InventorySystem::optimizeLootSplit(int n, vector<int>& coins) {
    if (n==0) return 0; // base cases
    if (n==1) return coins[0]; // base cases
    if (n==2) return abs(coins[0]-coins[1]); // base cases
    int totalCoins = 0;
    for (int i = 0; i < n; ++i) totalCoins+=coins[i]; //getting total amount of coins
    int bestSplitNum = totalCoins / 2; // getting optimal split or very close to it
    vector<bool> subsetArr(bestSplitNum+1,false); subsetArr[0] = true; // boolean array representing values that can be summed up to using coins in subset
    for (int i = 0; i < n; ++i) {
        for (int j = bestSplitNum; j >= coins[i]; --j) { //update reachable sums by including the current coin
            if (subsetArr[j - coins[i]] == true) subsetArr[j] = true; // checks if this number can be reached with coins < bestSplitNum
        }
    }
    int closestSum = 0;
    for (int i = bestSplitNum; i >= 0; --i) { // iterating backwards to get largest subset number in bool array
        if (subsetArr[i]) {
            closestSum = i;
            break;
        }
    }
    int difference = abs(closestSum - (totalCoins-closestSum)); // getting difference between 2 subsets
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
    const long long mod = 1e9 + 7;
    int n = s.length();
    // if string is empty
    if (n==0) return 1;

    // dp array to store the number of ways to decode up to index i
    vector<long long> dp(n+1,0);
    dp[0] = 1;

    for(int i=1; i<=n ; i++){
        dp[i] = dp[i-1]; // single character decoding

        // check for double character decoding
        if(i>1){
            string pair = s.substr(i-2,2);
            if(pair == "uu" || pair == "nn"){
                // add the ways to decode up to i-2
                dp[i] = (dp[i] + dp[i-2]) % mod;
            }
        }

    }

    return dp[n];
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
    const long long INF = 9999999999999; //const large value representing infinity/unreachable nodes
    vector<vector<long long>> distance(n, vector<long long>(n,INF)); // 2d array calculating shortest distances in graph
    for(int i = 0; i < n; ++i) {
        distance[i][i] = 0; // initializes diagonal points as 0
    }
    for(int i = 0; i < roads.size(); ++i) {
        if (roads[i][2]<distance[roads[i][0]][roads[i][1]]) { // adds edges from roads into distance 2d array
            distance[roads[i][0]][roads[i][1]] = roads[i][2];
            //distance[roads[i][1]][roads[i][0]] = roads[i][2];
        }
    }
    for (int k = 0; k < n; ++k) { // performs Floyd Warshall algorithm
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (distance[i][k]!=INF && distance[k][j]!=INF){ // checks if two indices are not infinity and there's a direct path since algorithm calculates shortest distance between every pair of node
                    if (distance[i][j] > distance[i][k] + distance[k][j]) { //checks if path going through k gives shorter distance
                        distance[i][j] = distance[i][k] + distance[k][j];
                    }
                }
            }
        }
    }
    long long sum = 0;
    for (int i = 0; i < n; ++i) { // adding elems from distance 2d arr into total sum
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
//    vector<vector<int>> graph= {{0,1,1},{1,2,2}};
//    vector<vector<int>> graph2= {{0,1,2},{0,2,8}};
//    vector<vector<int>> graph3= {{0,1,4}};
//    cout << WorldNavigator::sumMinDistancesBinary(3,graph) << endl;
//    cout << WorldNavigator::sumMinDistancesBinary(2,graph3) << endl;
//    cout << WorldNavigator::sumMinDistancesBinary(3,graph2) << endl;

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


//   //----------------------------------------------------------------------------------
    //goz2 Youki
//    AuctionTree* tree = new ConcreteAuctionTree();
//
//    cout << "===== INSERTIONS =====\n";
//    tree->insertItem(10, 50);
//    tree->insertItem(20, 40);
//    tree->insertItem(30, 60);
//    tree->insertItem(15, 40);
//    tree->insertItem(25, 55);
//    tree->insertItem(5, 50);
//
//    cout << "\nTree after inserts:\n";
//    ((ConcreteAuctionTree*)tree)->visualize();
//
//    cout << "\n===== DELETE itemID 20 =====\n";
//    tree->deleteItem(20);
//    ((ConcreteAuctionTree*)tree)->visualize();
//
//    cout << "\n===== DELETE itemID 10 =====\n";
//    tree->deleteItem(10);
//    ((ConcreteAuctionTree*)tree)->visualize();
//
//    cout << "\n===== DELETE itemID 30 =====\n";
//    tree->deleteItem(30);
//    ((ConcreteAuctionTree*)tree)->visualize();
//
//    cout << "\n===== FINAL TREE =====\n";
//    ((ConcreteAuctionTree*)tree)->visualize();

//    delete tree;
    // InventorySystem system;
    //
    // vector<string> testStrings = {"uu", "uunn", "u", "nnuu", "uuuunn"};
    //
    // for (const string& testStr : testStrings) {
    //     long long count = system.countStringPossibilities(testStr);
    //     cout << "String: " << testStr << " => Possibilities: " << count << endl;
    // }
    //===================================================================================
    // goz2 aboda
    return 0;
}