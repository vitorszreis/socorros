#ifndef BTREE_H
#define BTREE_H

#include "BTreeNode.h"
#include <vector>

// Forward declaration para a classe SaveManager, permitindo a amizade (friend)
class SaveManager;

/**
 * @class BTree
 * @brief Classe que gerencia a Árvore B de conquistas.
 */
class BTree {
private:
    BTreeNode *root;
    int minDegree; // Grau mínimo

public:
    // Construtor e Destrutor
    BTree(int t);
    ~BTree();

    // Métodos públicos
    void traverse();
    std::vector<int> search(int achievementCount); // Retorna os IDs dos jogadores
    std::vector<int> searchRange(int minKey, int maxKey); // Retorna os IDs no intervalo
    void insert(int achievementCount, int playerId);
    void remove(int achievementCount, int playerId = -1);

     
    std::vector<int> getTopAchievements(int n) {
        std::vector<int> result;
        if (root) root->getTopAchievements(result, n);
        return result;
    }
    
    int getAchievementCount(int playerId) {
        return root ? root->getAchievementCount(playerId) : 0;
    }
    
    // Getters
    int getMinDegree();
    BTreeNode* getRoot();

    // Concede acesso ao SaveManager para salvar e carregar a árvore.
    friend class SaveManager;
};

#endif // BTREE_H
