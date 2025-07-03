#ifndef BTREEPERSISTENCE_H
#define BTREEPERSISTENCE_H

#include "BTree.h"
#include <string>

class BTreePersistence {
public:
    // Salva a Árvore B em um arquivo.
    static void saveToFile(BTree* tree, const std::string& filename);
    
    // Carrega uma Árvore B de um arquivo.
    // A assinatura foi corrigida para não receber mais o grau 't',
    // pois ele é lido de dentro do arquivo.
    static BTree* loadFromFile(const std::string& filename);

private:
    // Funções auxiliares recursivas para salvar e carregar os nós.
    static void saveNode(std::ofstream& out, BTreeNode* node);
    static BTreeNode* loadNode(std::ifstream& in, int minDegree);
};

#endif // BTREEPERSISTENCE_H
