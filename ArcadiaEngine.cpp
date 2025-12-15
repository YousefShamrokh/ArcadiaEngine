#include "ArcadiaEngine.h"
#include <cstdlib>
#include <vector>
using namespace std;

class ConcreteLeaderboard::SkipListNode {
public:
    int player_id;
    int score;
    vector<SkipListNode*> forward;

    SkipListNode(int id, int s, int level)
        : player_id(id), score(s), forward(level + 1, nullptr) {}
};


ConcreteLeaderboard::ConcreteLeaderboard(int max_lvl)
    : max_level(max_lvl), level(0) {
    header = new SkipListNode(-1, -1, max_level);
}

ConcreteLeaderboard::~ConcreteLeaderboard() {
    SkipListNode* current = header;
    while (current) {
        SkipListNode* next = current->forward[0];
        delete current;
        current = next;
    }
}



int ConcreteLeaderboard::random_level() {
    int l = 0;
    while ((rand() % 2) == 0 && l < max_level) {
        ++l;
    }
    return l;
}


void ConcreteLeaderboard::addScore(int player_id, int score) {
   vector<SkipListNode*> update(max_level + 1);
        SkipListNode* current = header;


        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   (current->forward[i]->score > score ||
                    (current->forward[i]->score == score &&
                     current->forward[i]->player_id < player_id))) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        /*current = current->forward[0];


        if (current != nullptr && current->player_id == player_id) {
            removePlayer(player_id);

            current = header;
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] != nullptr &&
                       (current->forward[i]->score > score ||
                        (current->forward[i]->score == score &&
                         current->forward[i]->player_id < player_id))) {
                    current = current->forward[i];
                }
                update[i] = current;
            }
        }*/


        int new_level = random_level();
        if (new_level > level) {
            for (int i = level + 1; i <= new_level; i++) {
                update[i] = header;
            }
            level = new_level;
        }

        SkipListNode* new_node = new SkipListNode(player_id, score, new_level);
        for (int i = 0; i <= new_level; i++) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
}
/*
void ConcreteLeaderboard::addScore(int player_id, int score) {

    SkipListNode* current = header;
    bool found = false;

    for (int i = level; i >= 0; i--) {
        while (current->forward[i] != nullptr) {
            if (current->forward[i]->player_id == player_id) {
                found = true;
                break;
            }
            if (current->forward[i]->player_id > player_id) {
                break;
            }
            current = current->forward[i];
        }
        if (found) break;
    }

    if (found) {
        removePlayer(player_id);
    }


    vector<SkipListNode*> update(max_level + 1);
    current = header;

    for (int i = level; i >= 0; i--) {
        while (current->forward[i] != nullptr &&
               (current->forward[i]->score > score ||
                (current->forward[i]->score == score &&
                 current->forward[i]->player_id < player_id))) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    int new_level = random_level();
    if (new_level > level) {
        for (int i = level + 1; i <= new_level; i++) {
            update[i] = header;
        }
        level = new_level;
    }

    SkipListNode* new_node = new SkipListNode(player_id, score, new_level);
    for (int i = 0; i <= new_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}
*/
void ConcreteLeaderboard::removePlayer(int player_id) {

    SkipListNode* current = header->forward[0];
    SkipListNode* prev = header;


    while (current != nullptr && current->player_id != player_id) {
        prev = current;
        current = current->forward[0];
    }


    if (current == nullptr || current->player_id != player_id) {
        return;
    }


    for (int i = 0; i <= level; i++) {
        SkipListNode* temp = header;


        while (temp->forward[i] != nullptr && temp->forward[i] != current) {
            temp = temp->forward[i];
        }


        if (temp->forward[i] == current) {
            temp->forward[i] = current->forward[i];
        }
    }


    delete current;


    while (level > 0 && header->forward[level] == nullptr) {
        level--;
    }
}



vector<int> ConcreteLeaderboard::getTopN(int n) {
vector<int> result(n);
        SkipListNode* current = header->forward[0];
        int count = 0;

        while (current != nullptr && count < n) {
                result[count++]=current->player_id;

            current = current->forward[0];

        }


        return result;
}


int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items) {
      if (items.empty() || capacity <= 0) return 0;

    int n = items.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; i++) {
        int weight = items[i - 1].first;
        int value = items[i - 1].second;

        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w];
            if (w >= weight) {
                dp[i][w] = max(dp[i][w], dp[i - 1][w - weight] + value);
            }
        }
    }

    return dp[n][capacity];

}



































/*#include "ArcadiaEngine.h"
#include <cstdlib>

class ConcreteLeaderboard : public Leaderboard {
private:
    class SkipListNode {
public:
    int player_id;
    int score;
    vector<SkipListNode*> forward;

    SkipListNode(int id, int s, int level)
        : player_id(id), score(s), forward(level + 1, nullptr) {}
};
 int max_level;
    int level;
    SkipListNode* header;
int random_level() {
   int l = 0;

        while ((rand() % 2) == 0 && l < max_level) {
            l++;
        }
        return l;
}
public:
    ConcreteLeaderboard(int max_lvl=16): max_level(max_lvl), level(0)  {
       header = new SkipListNode(-1, -1, max_level);
    }

    void addScore(int player_id, int score) override {
     vector<SkipListNode*> update(max_level + 1);
        SkipListNode* current = header;


        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   (current->forward[i]->score > score ||
                    (current->forward[i]->score == score &&
                     current->forward[i]->player_id < player_id))) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];


        if (current != nullptr && current->player_id == player_id) {
            removePlayer(player_id);

            current = header;
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] != nullptr &&
                       (current->forward[i]->score > score ||
                        (current->forward[i]->score == score &&
                         current->forward[i]->player_id < player_id))) {
                    current = current->forward[i];
                }
                update[i] = current;
            }
        }


        int new_level = random_level();
        if (new_level > level) {
            for (int i = level + 1; i <= new_level; i++) {
                update[i] = header;
            }
            level = new_level;
        }

        SkipListNode* new_node = new SkipListNode(player_id, score, new_level);
        for (int i = 0; i <= new_level; i++) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    }

  void removePlayer(int player_id) override {
    SkipListNode* current = header->forward[0];
    SkipListNode* prev = header;


    while (current != nullptr && current->player_id != player_id) {
        prev = current;
        current = current->forward[0];
    }


    if (current == nullptr || current->player_id != player_id) {
        return;
    }


    for (int i = 0; i <= level; i++) {
        SkipListNode* temp = header;


        while (temp->forward[i] != nullptr && temp->forward[i] != current) {
            temp = temp->forward[i];
        }


        if (temp->forward[i] == current) {
            temp->forward[i] = current->forward[i];
        }
    }


    delete current;

    while (level > 0 && header->forward[level] == nullptr) {
        level--;
    }
}

    vector<int> getTopN(int n) override {
        vector<int> result(n);
        SkipListNode* current = header->forward[0];
        int count = 0;

        while (current != nullptr && count < n) {
                result[count++]=current->player_id;

            current = current->forward[0];

        }


        return result;
    }
    ~ConcreteLeaderboard() {
 SkipListNode* current = header;
        while (current) {
            SkipListNode* next = current->forward[0];
            delete current;
            current = next;
        }
}
};





















/*
ConcreteLeaderboard::ConcreteLeaderboard(int max_lvl)
    : max_level(max_lvl), level(0) {
  header = new SkipListNode(-1, -1, max_level);
}


ConcreteLeaderboard::~ConcreteLeaderboard() {
 SkipListNode* current = header;
        while (current) {
            SkipListNode* next = current->forward[0];
            delete current;
            current = next;
        }
}


int ConcreteLeaderboard::random_level() {
   int l = 0;

        while ((rand() % 2) == 0 && l < max_level) {
            l++;
        }
        return l;
}
void ConcreteLeaderboard::addScore(int player_id, int score) {
   vector<SkipListNode*> update(max_level + 1);
        SkipListNode* current = header;


        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   (current->forward[i]->score > score ||
                    (current->forward[i]->score == score &&
                     current->forward[i]->player_id < player_id))) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];


        if (current != nullptr && current->player_id == player_id) {
            remove(player_id);

            current = header;
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] != nullptr &&
                       (current->forward[i]->score > score ||
                        (current->forward[i]->score == score &&
                         current->forward[i]->player_id < player_id))) {
                    current = current->forward[i];
                }
                update[i] = current;
            }
        }


        int new_level = random_level();
        if (new_level > level) {
            for (int i = level + 1; i <= new_level; i++) {
                update[i] = header;
            }
            level = new_level;
        }

        SkipListNode* new_node = new SkipListNode(player_id, score, new_level);
        for (int i = 0; i <= new_level; i++) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
}
*/
/*void ConcreteLeaderboard::addScore(int player_id, int score) {

        vector<SkipListNode*> update(max_level + 1);
        SkipListNode* current = header;


        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   (current->forward[i]->score > score ||
                    (current->forward[i]->score == score &&
                     current->forward[i]->player_id < player_id))) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];


        if (current != nullptr && current->player_id == player_id) {
           removePlayer(player_id);

            current = header;
            for (int i = level; i >= 0; i--) {
                while (current->forward[i] != nullptr &&
                       (current->forward[i]->score > score ||
                        (current->forward[i]->score == score &&
                         current->forward[i]->player_id < player_id))) {
                    current = current->forward[i];
                }
                update[i] = current;
            }
        }


        int new_level = random_level();
        if (new_level > level) {
            for (int i = level + 1; i <= new_level; i++) {
                update[i] = header;
            }
            level = new_level;
        }

        SkipListNode* new_node = new SkipListNode(player_id, score, new_level);
        for (int i = 0; i <= new_level; i++) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
}

*//*
void ConcreteLeaderboard::removePlayer(int player_id) {
    vector<SkipListNode*> update(max_level + 1);

    for (int i = level; i >= 0; i--) {
        SkipListNode* current = header;  // RESET for each level!

        while (current->forward[i] != nullptr &&
               current->forward[i]->player_id != player_id) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    SkipListNode* target = update[0]->forward[0];

    if (target == nullptr || target->player_id != player_id) {
        return;
    }

    for (int i = 0; i <= level; i++) {
        if (update[i]->forward[i] != target) {
            break;
        }
        update[i]->forward[i] = target->forward[i];
    }

    delete target;

    while (level > 0 && header->forward[level] == nullptr) {
        level--;
    }
}



vector<int> ConcreteLeaderboard::getTopN(int n) {
     vector<int> result(n);
        SkipListNode* current = header->forward[0];
        int count = 0;

        while (current != nullptr && count < n) {
                result[count++]=current->player_id;

            current = current->forward[0];

        }


        return result;

}
*/
