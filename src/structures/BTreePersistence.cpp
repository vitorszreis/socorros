#include "../../include/structures/BTreePersistence.h"
#include "../../include/structures/BTree.h"
#include "../../include/structures/BTreeNode.h"
#include <fstream>
#include <iostream>

void BTreePersistence::saveToFile(BTree* tree, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo para escrita: " << filename << std::endl;
        return;
    }
    // Acesso permitido pois BTreePersistence é 'friend' de BTree.
    int t = tree->minDegree;
    out.write((char*)&t, sizeof(int)); // Salva o grau mínimo primeiro.
    if (tree->root) {
        saveNode(out, tree->root);
    }
    out.close();
}

// Assinatura corrigida para corresponder à declaração no .h
BTree* BTreePersistence::loadFromFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        // Não é um erro, o arquivo pode não existir na primeira execução.
        return nullptr;
    }
    int minDegree;
    in.read((char*)&minDegree, sizeof(int));
    if (in.gcount() != sizeof(int)) {
        return nullptr; // Arquivo vazio ou corrompido.
    }
    BTree* tree = new BTree(minDegree);
    if (in.peek() != std::ifstream::traits_type::eof()) {
        tree->root = loadNode(in, minDegree);
    }
    in.close();
    return tree;
}

void BTreePersistence::saveNode(std::ofstream& out, BTreeNode* node) {
    if (!node) return;
    // Acesso permitido pois BTreePersistence é 'friend' de BTreeNode.
    out.write((char*)&node->keyCount, sizeof(int));
    out.write((char*)&node->isLeaf, sizeof(bool));
    for (int i = 0; i < node->keyCount; ++i) {
        out.write((char*)&node->keys[i].achievementCount, sizeof(int));
        size_t vec_size = node->keys[i].playerIds.size();
        out.write((char*)&vec_size, sizeof(size_t));
        if (vec_size > 0) {
            out.write((char*)node->keys[i].playerIds.data(), vec_size * sizeof(int));
        }
    }
    if (!node->isLeaf) {
        for (int i = 0; i <= node->keyCount; ++i) {
            saveNode(out, node->children[i]);
        }
    }
}

BTreeNode* BTreePersistence::loadNode(std::ifstream& in, int minDegree) {
    if (in.peek() == std::ifstream::traits_type::eof()) return nullptr;
    int n;
    bool leaf;
    in.read((char*)&n, sizeof(int));
    in.read((char*)&leaf, sizeof(bool));
    BTreeNode* node = new BTreeNode(minDegree, leaf);
    node->keyCount = n;
    for (int i = 0; i < n; ++i) {
        in.read((char*)&node->keys[i].achievementCount, sizeof(int));
        size_t vec_size;
        in.read((char*)&vec_size, sizeof(size_t));
        if (vec_size > 0) {
            node->keys[i].playerIds.resize(vec_size);
            in.read((char*)node->keys[i].playerIds.data(), vec_size * sizeof(int));
        }
    }
    if (!leaf) {
        for (int i = 0; i <= n; ++i) {
            node->children[i] = loadNode(in, minDegree);
        }
    }
    return node;
}
