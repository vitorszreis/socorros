#ifndef BTREENODE_H
#define BTREENODE_H

#include <algorithm>  // Para std::find
#include <vector>
#include <fstream>

struct AchievementKey {
    int achievementCount;
    std::vector<int> playerIds;
};

// Forward-declarations
class BTree;
class SaveManager;

class BTreeNode {
private:
    int minDegree;
    bool isLeaf;
    int keyCount;
    std::vector<AchievementKey> keys;
    std::vector<BTreeNode*> children;

    // Métodos privados auxiliares
    int findKey(int k);
    void splitChild(int i, BTreeNode* y);
    void removeFromLeaf(int idx, int playerId);
    void removeFromNonLeaf(int idx, int playerId);
    AchievementKey getPredecessor(int idx);
    AchievementKey getSuccessor(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);

public:
    // Construtor e Destrutor
    BTreeNode(int t, bool leaf);
    ~BTreeNode();

    // Métodos públicos
    void traverse();
    BTreeNode* search(int k);
    void searchRange(int minKey, int maxKey, std::vector<int>& result);
    void insertNonFull(int k, int playerId);
    void remove(int k, int playerId);

    // Concede acesso aos membros privados para as classes BTree e SaveManager
    friend class BTree;
    friend class SaveManager;

     
    void getTopAchievements(std::vector<int>& result, int& remaining) {
        if (!isLeaf) {
            children[keyCount]->getTopAchievements(result, remaining);
        }
        
        for (int i = keyCount - 1; i >= 0 && remaining > 0; --i) {
            for (int pid : keys[i].playerIds) {
                if (remaining-- > 0) result.push_back(pid);
                else return;
            }
            if (!isLeaf) children[i]->getTopAchievements(result, remaining);
        }
    }
    
    int getAchievementCount(int playerId) {
        for (int i = 0; i < keyCount; ++i) {
            if (std::find(keys[i].playerIds.begin(), keys[i].playerIds.end(), playerId) 
                != keys[i].playerIds.end()) {
                return keys[i].achievementCount;
            }
        }
        return 0;
    }
};

#endif // BTREENODE_H
