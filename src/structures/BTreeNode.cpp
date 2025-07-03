#include "../../include/structures/BTreeNode.h"
#include <algorithm>
#include <iostream>

// Construtor do Nó.
BTreeNode::BTreeNode(int t, bool leaf) : minDegree(t), isLeaf(leaf), keyCount(0) {
    keys.resize(2 * minDegree - 1);
    children.resize(2 * minDegree, nullptr);
}

// Destrutor do Nó (libera a memória dos filhos recursivamente).
BTreeNode::~BTreeNode() {
    for (BTreeNode* child : children) {
        delete child;
    }
}

// Encontra o índice da primeira chave que é maior ou igual a k.
int BTreeNode::findKey(int k) {
    int idx = 0;
    while (idx < keyCount && keys[idx].achievementCount < k)
        ++idx;
    return idx;
}

// Atravessa a subárvore a partir deste nó (para depuração).
void BTreeNode::traverse() {
    int i;
    for (i = 0; i < keyCount; i++) {
        if (!isLeaf) children[i]->traverse();
        std::cout << " " << keys[i].achievementCount << " (PIDs: ";
        for(size_t j = 0; j < keys[i].playerIds.size(); ++j) {
            std::cout << keys[i].playerIds[j] << (j == keys[i].playerIds.size() - 1 ? "" : ",");
        }
        std::cout << ")";
    }
    if (!isLeaf) children[i]->traverse();
}

// Busca por uma chave na subárvore. Retorna o nó que a contém.
BTreeNode* BTreeNode::search(int k) {
    int i = findKey(k);
    if (i < keyCount && keys[i].achievementCount == k) return this;
    if (isLeaf) return nullptr;
    return children[i]->search(k);
}

// Busca por chaves dentro de um intervalo.
void BTreeNode::searchRange(int minKey, int maxKey, std::vector<int>& result) {
    int i = 0;
    while (i < keyCount && keys[i].achievementCount < minKey) i++;
    for (; i < keyCount; i++) {
        if (!isLeaf) children[i]->searchRange(minKey, maxKey, result);
        if (keys[i].achievementCount > maxKey) return;
        if (keys[i].achievementCount >= minKey) {
            result.insert(result.end(), keys[i].playerIds.begin(), keys[i].playerIds.end());
        }
    }
    if (!isLeaf) children[keyCount]->searchRange(minKey, maxKey, result);
}

// Insere uma nova chave em um nó que não está cheio.
void BTreeNode::insertNonFull(int k, int playerId) {
    int i = keyCount - 1;
    if (isLeaf) {
        while (i >= 0 && keys[i].achievementCount > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        if (i >= 0 && keys[i].achievementCount == k) {
            keys[i].playerIds.push_back(playerId);
            return;
        }
        keys[i + 1].achievementCount = k;
        keys[i + 1].playerIds = {playerId};
        keyCount++;
    } else {
        while (i >= 0 && keys[i].achievementCount > k) i--;
        if (i >= 0 && keys[i].achievementCount == k) {
            keys[i].playerIds.push_back(playerId);
            return;
        }
        i++;
        if (children[i]->keyCount == 2 * minDegree - 1) {
            splitChild(i, children[i]);
            if (keys[i].achievementCount < k) i++;
        }
        children[i]->insertNonFull(k, playerId);
    }
}

// Divide o filho y deste nó. y deve estar cheio quando esta função é chamada.
void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->minDegree, y->isLeaf);
    z->keyCount = minDegree - 1;
    for (int j = 0; j < minDegree - 1; j++) z->keys[j] = y->keys[j + minDegree];
    if (!y->isLeaf) {
        for (int j = 0; j < minDegree; j++) z->children[j] = y->children[j + minDegree];
    }
    y->keyCount = minDegree - 1;
    for (int j = keyCount; j >= i + 1; j--) children[j + 1] = children[j];
    children[i + 1] = z;
    for (int j = keyCount - 1; j >= i; j--) keys[j + 1] = keys[j];
    keys[i] = y->keys[minDegree - 1];
    keyCount++;
}

// Função principal para remover uma chave desta subárvore.
void BTreeNode::remove(int k, int playerId) {
    int idx = findKey(k);
    if (idx < keyCount && keys[idx].achievementCount == k) {
        if (isLeaf) removeFromLeaf(idx, playerId);
        else removeFromNonLeaf(idx, playerId);
    } else {
        if (isLeaf) return;
        bool flag = (idx == keyCount);
        if (children[idx]->keyCount < minDegree) fill(idx);
        if (flag && idx > keyCount) children[idx - 1]->remove(k, playerId);
        else children[idx]->remove(k, playerId);
    }
}

// Remove uma chave que está em um nó folha.
void BTreeNode::removeFromLeaf(int idx, int playerId) {
    if (playerId == -1 || keys[idx].playerIds.size() <= 1) {
        for (int i = idx + 1; i < keyCount; ++i) keys[i - 1] = keys[i];
        keyCount--;
    } else {
        auto& ids = keys[idx].playerIds;
        ids.erase(std::remove(ids.begin(), ids.end(), playerId), ids.end());
    }
}

// Remove uma chave que está em um nó interno.
void BTreeNode::removeFromNonLeaf(int idx, int playerId) {
    AchievementKey k = keys[idx];
    if (playerId != -1 && k.playerIds.size() > 1) {
        auto& ids = keys[idx].playerIds;
        ids.erase(std::remove(ids.begin(), ids.end(), playerId), ids.end());
        return;
    }
    if (children[idx]->keyCount >= minDegree) {
        AchievementKey pred = getPredecessor(idx);
        keys[idx] = pred;
        children[idx]->remove(pred.achievementCount, -1);
    } else if (children[idx + 1]->keyCount >= minDegree) {
        AchievementKey succ = getSuccessor(idx);
        keys[idx] = succ;
        children[idx + 1]->remove(succ.achievementCount, -1);
    } else {
        merge(idx);
        children[idx]->remove(k.achievementCount, -1);
    }
}

// Pega a chave predecessora (maior chave na subárvore esquerda).
AchievementKey BTreeNode::getPredecessor(int idx) {
    BTreeNode* cur = children[idx];
    while (!cur->isLeaf) cur = cur->children[cur->keyCount];
    return cur->keys[cur->keyCount - 1];
}

// Pega a chave sucessora (menor chave na subárvore direita).
AchievementKey BTreeNode::getSuccessor(int idx) {
    BTreeNode* cur = children[idx + 1];
    while (!cur->isLeaf) cur = cur->children[0];
    return cur->keys[0];
}

// Preenche o filho na posição idx que tem menos que minDegree-1 chaves.
void BTreeNode::fill(int idx) {
    if (idx != 0 && children[idx - 1]->keyCount >= minDegree) borrowFromPrev(idx);
    else if (idx != keyCount && children[idx + 1]->keyCount >= minDegree) borrowFromNext(idx);
    else {
        if (idx != keyCount) merge(idx);
        else merge(idx - 1);
    }
}

// Pega uma chave emprestada do filho anterior.
void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx - 1];
    for (int i = child->keyCount - 1; i >= 0; --i) child->keys[i + 1] = child->keys[i];
    if (!child->isLeaf) {
        for (int i = child->keyCount; i >= 0; --i) child->children[i + 1] = child->children[i];
    }
    child->keys[0] = keys[idx - 1];
    if (!child->isLeaf) child->children[0] = sibling->children[sibling->keyCount];
    keys[idx - 1] = sibling->keys[sibling->keyCount - 1];
    child->keyCount++;
    sibling->keyCount--;
}

// Pega uma chave emprestada do próximo filho.
void BTreeNode::borrowFromNext(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];
    child->keys[child->keyCount] = keys[idx];
    if (!child->isLeaf) child->children[child->keyCount + 1] = sibling->children[0];
    keys[idx] = sibling->keys[0];
    for (int i = 1; i < sibling->keyCount; ++i) sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->isLeaf) {
        for (int i = 1; i <= sibling->keyCount; ++i) sibling->children[i - 1] = sibling->children[i];
    }
    child->keyCount++;
    sibling->keyCount--;
}

// Funde o filho na posição idx com o próximo filho.
void BTreeNode::merge(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];
    child->keys[minDegree - 1] = keys[idx];
    for (int i = 0; i < sibling->keyCount; ++i) child->keys[i + minDegree] = sibling->keys[i];
    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->keyCount; ++i) child->children[i + minDegree] = sibling->children[i];
    }
    for (int i = idx + 1; i < keyCount; ++i) keys[i - 1] = keys[i];
    for (int i = idx + 2; i <= keyCount; ++i) children[i - 1] = children[i];
    child->keyCount += sibling->keyCount + 1;
    keyCount--;
    sibling->children.assign(2 * minDegree, nullptr);
    delete sibling;
}
