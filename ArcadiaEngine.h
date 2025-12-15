#ifndef ARCADIAENGINE_H
#define ARCADIAENGINE_H

#include <vector>
using namespace std;
class Leaderboard {
public:
    virtual ~Leaderboard() {}
    virtual void addScore(int playerID, int score) = 0;
    virtual void removePlayer(int playerID) = 0;
    virtual std::vector<int> getTopN(int n) = 0;
};

class ConcreteLeaderboard : public Leaderboard {
public:
    ConcreteLeaderboard(int max_lvl = 16);
    ~ConcreteLeaderboard();

    void addScore(int playerID, int score) override;
    void removePlayer(int playerID) override;
    std::vector<int> getTopN(int n) override;

private:
    class SkipListNode;
    int max_level;
    int level;
    SkipListNode* header;

    int random_level();
};


class InventorySystem {
public:
    static int maximizeCarryValue(int capacity, vector<pair<int, int>>& items);

};

#endif
