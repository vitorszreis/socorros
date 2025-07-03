#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <fstream> // Necessário para std::ofstream/ifstream

// Forward declarations para evitar dependências circulares e permitir o uso de ponteiros
class BTree;
class BTreeNode;
class HashBase;

/**
 * @class SaveManager
 * @brief Classe estática para gerenciar a persistência de estruturas de dados em ficheiro.
 * É responsável por salvar e carregar a Árvore B e a Tabela Hash em formato binário.
 */
class SaveManager {
public:
    // --- Funções Públicas ---
    static void saveHash(HashBase* tabela, const std::string& filename);
    static void loadHash(HashBase* tabela, const std::string& filename);
    static void saveBTree(BTree* tree, const std::string& filename);
    static BTree* loadBTree(const std::string& filename);

    static void saveAll(HashBase* hash, BTree* tree);

private:
    // --- Funções Auxiliares Privadas para a Árvore B ---
    // Estas funções são chamadas recursivamente para percorrer a árvore.
    static void saveBTreeNode_recursive(std::ofstream& file, BTreeNode* node);
    static BTreeNode* loadBTreeNode_recursive(std::ifstream& file, int minDegree);
};

#endif // SAVEMANAGER_H
