#include "../../include/structures/BTree.h"
#include "../../include/structures/BTreeNode.h"
#include <iostream>

// Construtor
BTree::BTree(int t) {
    root = nullptr;
    this->minDegree = t;
}

// Destrutor
BTree::~BTree() {
    delete root;
}

// Percorre a árvore (para depuração)
void BTree::traverse() {
    if (root != nullptr) {
        root->traverse();
    }
}

/**
 * @brief Busca por uma contagem de conquistas e retorna os IDs dos jogadores associados.
 * @param achievementCount A contagem de conquistas a ser buscada.
 * @return std::vector<int> Um vetor com os IDs dos jogadores. Retorna um vetor vazio se a chave não for encontrada.
 */
std::vector<int> BTree::search(int achievementCount) {
    if (root == nullptr) {
        return {}; // Retorna vetor vazio se a árvore estiver vazia.
    }
    BTreeNode* foundNode = root->search(achievementCount);
    if (foundNode != nullptr) {
        // Acesso permitido via 'friend'
        int i = foundNode->findKey(achievementCount);
        if (i < foundNode->keyCount && foundNode->keys[i].achievementCount == achievementCount) {
            return foundNode->keys[i].playerIds;
        }
    }
    return {}; // Retorna vetor vazio se a chave não for encontrada.
}

/**
 * @brief Busca por um intervalo de contagens de conquistas.
 * @param minKey O valor mínimo do intervalo.
 * @param maxKey O valor máximo do intervalo.
 * @return std::vector<int> Um vetor com todos os IDs de jogadores encontrados no intervalo.
 */
std::vector<int> BTree::searchRange(int minKey, int maxKey) {
    std::vector<int> result;
    if (root != nullptr) {
        root->searchRange(minKey, maxKey, result);
    }
    return result;
}

// Insere um jogador com sua contagem de conquistas
void BTree::insert(int achievementCount, int playerId) {
    if (root == nullptr) {
        root = new BTreeNode(minDegree, true);
        root->insertNonFull(achievementCount, playerId);
    } else {
        // Se a raiz estiver cheia, a árvore cresce em altura
        if (root->keyCount == 2 * minDegree - 1) { // Acesso via friend
            BTreeNode *s = new BTreeNode(minDegree, false);
            s->children[0] = root; // Acesso via friend
            s->splitChild(0, root);
            
            int i = 0;
            if (s->keys[0].achievementCount < achievementCount) { // Acesso via friend
                i++;
            }
            s->children[i]->insertNonFull(achievementCount, playerId);
            
            root = s;
        } else {
            root->insertNonFull(achievementCount, playerId);
        }
    }
}

// Remove um jogador ou uma chave da árvore
void BTree::remove(int achievementCount, int playerId) {
    if (!root) {
        return;
    }

    root->remove(achievementCount, playerId);

    // Se a raiz ficar vazia, seu primeiro filho se torna a nova raiz
    if (root->keyCount == 0) { // Acesso via friend
        BTreeNode *tmp = root;
        if (root->isLeaf) { // Acesso via friend
            root = nullptr;
        } else {
            root = root->children[0]; // Acesso via friend
            tmp->children[0] = nullptr; 
        }
        delete tmp;
    }
}

// Getter para o grau mínimo
int BTree::getMinDegree() {
    return minDegree;
}

// Getter para a raiz
BTreeNode* BTree::getRoot() {
    return root;
}
