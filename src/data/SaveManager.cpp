#include "../../include/data/SaveManager.h"
#include "../../include/structures/BTree.h"
#include "../../include/structures/BTreeNode.h"
#include "../../include/structures/HashBase.h"
#include <fstream>
#include <iostream>

// --- Implementação dos métodos privados e auxiliares do SaveManager ---

/**
 * @brief Salva recursivamente um nó da Árvore B e todos os seus descendentes em um ficheiro binário.
 * @param file O stream do ficheiro de saída (ofstream) aberto em modo binário.
 * @param node O ponteiro para o nó a ser salvo.
 */
void SaveManager::saveBTreeNode_recursive(std::ofstream& file, BTreeNode* node) {
    // Caso base: Se o nó for nulo, escreve um marcador 'true' para indicar o fim deste ramo.
    if (node == nullptr) {
        bool is_null = true;
        file.write(reinterpret_cast<const char*>(&is_null), sizeof(is_null));
        return;
    }
    // Se o nó não for nulo, escreve 'false' e continua a salvar os dados do nó.
    bool is_null = false;
    file.write(reinterpret_cast<const char*>(&is_null), sizeof(is_null));

    // O acesso aos membros privados é permitido porque SaveManager é 'friend' de BTreeNode.
    // Salva as propriedades básicas do nó.
    file.write(reinterpret_cast<const char*>(&node->isLeaf), sizeof(node->isLeaf));
    file.write(reinterpret_cast<const char*>(&node->keyCount), sizeof(node->keyCount));

    // Itera sobre todas as chaves (AchievementKey) do nó e as salva.
    for (int i = 0; i < node->keyCount; ++i) {
        const AchievementKey& key = node->keys[i];
        // Salva a contagem de conquistas.
        file.write(reinterpret_cast<const char*>(&key.achievementCount), sizeof(key.achievementCount));
        
        // Salva o vetor de IDs de jogadores associado a esta chave.
        size_t playerIdsSize = key.playerIds.size();
        file.write(reinterpret_cast<const char*>(&playerIdsSize), sizeof(playerIdsSize)); // Primeiro, o tamanho do vetor.
        if (playerIdsSize > 0) {
            // Depois, os dados do vetor.
            file.write(reinterpret_cast<const char*>(key.playerIds.data()), playerIdsSize * sizeof(int));
        }
    }

    // Se o nó não for uma folha, chama recursivamente a função para salvar cada um dos seus filhos.
    if (!node->isLeaf) {
        for (int i = 0; i <= node->keyCount; ++i) {
            saveBTreeNode_recursive(file, node->children[i]);
        }
    }
}

/**
 * @brief Carrega recursivamente um nó da Árvore B e seus descendentes de um ficheiro binário.
 * @param file O stream do ficheiro de entrada (ifstream) aberto em modo binário.
 * @param minDegree O grau mínimo da árvore, necessário para recriar os nós.
 * @return BTreeNode* Um ponteiro para o nó recriado, ou nullptr se o fim do ramo foi alcançado.
 */
BTreeNode* SaveManager::loadBTreeNode_recursive(std::ifstream& file, int minDegree) {
    // Lê o marcador de nulidade. Se for 'true', significa que este nó era nulo.
    bool is_null;
    file.read(reinterpret_cast<char*>(&is_null), sizeof(is_null));
    if (is_null || file.eof()) {
        return nullptr;
    }

    // Lê as propriedades básicas do nó.
    bool isLeaf;
    int keyCount;
    file.read(reinterpret_cast<char*>(&isLeaf), sizeof(isLeaf));
    file.read(reinterpret_cast<char*>(&keyCount), sizeof(keyCount));

    // Cria um novo nó com as propriedades lidas.
    BTreeNode* node = new BTreeNode(minDegree, isLeaf);
    // O acesso aos membros privados é permitido pela relação de amizade.
    node->keyCount = keyCount;

    // Itera para ler todas as chaves que este nó continha.
    for (int i = 0; i < keyCount; ++i) {
        AchievementKey& key = node->keys[i];
        // Lê a contagem de conquistas.
        file.read(reinterpret_cast<char*>(&key.achievementCount), sizeof(key.achievementCount));

        // Lê o tamanho do vetor de IDs de jogadores.
        size_t playerIdsSize;
        file.read(reinterpret_cast<char*>(&playerIdsSize), sizeof(playerIdsSize));
        if (playerIdsSize > 0) {
            // Redimensiona o vetor e lê os dados diretamente para a sua memória.
            key.playerIds.resize(playerIdsSize);
            file.read(reinterpret_cast<char*>(key.playerIds.data()), playerIdsSize * sizeof(int));
        }
    }

    // Se não for uma folha, carrega recursivamente os nós filhos.
    if (!isLeaf) {
        for (int i = 0; i <= keyCount; ++i) {
            node->children[i] = loadBTreeNode_recursive(file, minDegree);
        }
    }
    return node;
}

// --- Implementação dos métodos públicos do SaveManager ---

/**
 * @brief Salva uma Árvore B completa num ficheiro binário.
 * @param tree Ponteiro para a árvore a ser salva.
 * @param filename O nome do ficheiro de destino.
 */
void SaveManager::saveBTree(BTree* tree, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file || !tree) {
        std::cerr << "Erro ao abrir o ficheiro para salvar a Árvore B ou árvore nula." << std::endl;
        return;
    }

    // O primeiro dado salvo é o grau mínimo da árvore, essencial para recriá-la.
    int minDegree = tree->minDegree; // Acesso permitido por 'friend class'
    file.write(reinterpret_cast<const char*>(&minDegree), sizeof(minDegree));
    
    // Inicia o processo de salvamento recursivo a partir do nó raiz.
    saveBTreeNode_recursive(file, tree->root); // Acesso permitido por 'friend class'
    file.close();
}

/**
 * @brief Carrega uma Árvore B completa de um ficheiro binário.
 * @param filename O nome do ficheiro de onde carregar os dados.
 * @return BTree* Um ponteiro para a árvore recém-criada, ou nullptr se o ficheiro não existir.
 */
BTree* SaveManager::loadBTree(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return nullptr; // Ficheiro não encontrado não é um erro, significa que é a primeira execução.
    }

    // Lê o grau mínimo, o primeiro dado do ficheiro.
    int minDegree;
    file.read(reinterpret_cast<char*>(&minDegree), sizeof(minDegree));
    if (file.eof()) {
        std::cerr << "Erro: ficheiro da Árvore B está vazio ou corrompido." << std::endl;
        return nullptr;
    }

    // Cria uma nova árvore com o grau lido.
    BTree* tree = new BTree(minDegree);
    // Inicia o processo de carregamento recursivo e define a nova raiz.
    tree->root = loadBTreeNode_recursive(file, minDegree); // Acesso permitido por 'friend class'

    file.close();
    return tree;
}

// --- Implementação dos métodos de Hash do SaveManager (A SER PREENCHIDA) ---

/**
 * @brief Salva uma Tabela Hash num ficheiro binário.
 * @param tabela Ponteiro para a tabela (do tipo base HashBase) a ser salva.
 * @param filename O nome do ficheiro de destino.
 */
void SaveManager::saveHash(HashBase* tabela, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file || !tabela) {
        std::cerr << "Erro ao criar o ficheiro para salvar a Tabela Hash ou tabela nula: " << filename << std::endl;
        return;
    }
    // TODO: Implementar a lógica de serialização da sua Tabela Hash aqui.
    // Você precisará percorrer os baldes/entradas da tabela e escrever cada
    // jogador e suas informações no ficheiro.
    file.close();
}

/**
 * @brief Carrega uma Tabela Hash de um ficheiro binário.
 * @param tabela Ponteiro para a tabela (vazia) que será preenchida.
 * @param filename O nome do ficheiro de onde carregar os dados.
 */
void SaveManager::loadHash(HashBase* tabela, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file || !tabela) {
        return; // Não é um erro se o ficheiro ainda não existe.
    }
    // TODO: Implementar a lógica de desserialização da sua Tabela Hash aqui.
    // Você precisará ler os dados do ficheiro e usar o método de inserção
    // da sua tabela para recriar o seu estado em memória.
    file.close();
}
