#include "structures/BTree.h"
#include "data/SaveManager.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <string>

// Função auxiliar para imprimir vetores para facilitar a depuração.
void print_vector(const std::string& message, const std::vector<int>& vec) {
    std::cout << message;
    for (int id : vec) {
        std::cout << id << " ";
    }
    std::cout << std::endl;
}

// Função para comparar duas árvores B, essencial para validar a persistência.
bool areTreesEqual(BTree& t1, BTree& t2, int max_key) {
    for (int k = 0; k <= max_key; ++k) {
        // Usa o novo método search que retorna diretamente o vetor de IDs.
        std::vector<int> r1 = t1.search(k);
        std::vector<int> r2 = t2.search(k);
        std::sort(r1.begin(), r1.end());
        std::sort(r2.begin(), r2.end());
        if (r1 != r2) {
            std::cout << "Discrepancia encontrada na chave " << k << std::endl;
            print_vector("  Arvore 1:", r1);
            print_vector("  Arvore 2:", r2);
            return false;
        }
    }
    return true;
}

int main() {
    // Grau mínimo t=3. Cada nó pode ter no máximo 5 chaves e no mínimo 2.
    BTree tree(3); 

    // --- SEÇÃO 1: TESTE DE INSERÇÃO ---
    std::cout << "--- Iniciando Teste de Insercao na Arvore B ---" << std::endl;
    tree.insert(10, 101);
    tree.insert(20, 201);
    tree.insert(5, 501);
    tree.insert(6, 601);
    tree.insert(12, 1201);
    tree.insert(30, 3001);
    tree.insert(7, 701);
    tree.insert(17, 1701);
    tree.insert(10, 102); // Chave duplicada para testar a lista de IDs de jogadores.

    std::cout << "Arvore B apos insercoes:" << std::endl;
    tree.traverse();
    std::cout << "\n------------------------------------------" << std::endl;

    // --- SEÇÃO 2: TESTE DE BUSCA ---
    std::cout << "\n--- Iniciando Teste de Busca por Contagem de Conquistas ---" << std::endl;
    std::vector<int> searchResult = tree.search(10);
    std::sort(searchResult.begin(), searchResult.end());
    assert(searchResult.size() == 2 && searchResult[0] == 101 && searchResult[1] == 102);
    print_vector("[PASSOU] IDs encontrados para contagem 10: ", searchResult);

    searchResult = tree.search(7);
    assert(searchResult.size() == 1 && searchResult[0] == 701);
    print_vector("[PASSOU] ID encontrado para contagem 7: ", searchResult);

    searchResult = tree.search(99);
    assert(searchResult.empty());
    std::cout << "[PASSOU] Busca por contagem inexistente (99) retornou vazio." << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // --- SEÇÃO 3: TESTE DE BUSCA POR INTERVALO ---
    std::cout << "\n--- Iniciando Teste de Busca por Intervalo de Conquistas ---" << std::endl;
    std::vector<int> rangeResult = tree.searchRange(6, 17);
    std::sort(rangeResult.begin(), rangeResult.end());
    print_vector("Jogadores com contagem de conquistas entre 6 e 17: ", rangeResult);
    assert(rangeResult.size() == 6); 
    std::cout << "[PASSOU] Busca por intervalo (6-17) retornou o numero correto de resultados." << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // --- SEÇÃO 4: TESTE DE REMOÇÃO ---
    std::cout << "\n--- Iniciando Teste de Remocao ---" << std::endl;
    tree.remove(10, 101);
    searchResult = tree.search(10);
    assert(searchResult.size() == 1 && searchResult[0] == 102);
    std::cout << "[PASSOU] Remocao de um ID especifico (101) da chave 10." << std::endl;
    tree.traverse();

    tree.remove(10, 102);
    searchResult = tree.search(10);
    assert(searchResult.empty());
    std::cout << "\n[PASSOU] Remocao completa da chave 10." << std::endl;
    tree.traverse();
    
    tree.remove(6, 601);
    tree.remove(7, 701);
    std::cout << "\n[INFO] Removidas chaves 6 e 7 para testar casos complexos de remocao." << std::endl;
    std::cout << "Estado final da arvore antes da persistencia:" << std::endl;
    tree.traverse();
    assert(tree.search(6).empty());
    std::cout << "\n[PASSOU] Remocao complexa concluida com sucesso." << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // --- SEÇÃO 5: TESTE DE PERSISTÊNCIA EM DISCO ---
    std::cout << "\n--- Iniciando Teste de Persistencia em Disco ---" << std::endl;
    const std::string filename = "btree_test.dat";
    SaveManager::saveBTree(&tree, filename);
    std::cout << "[INFO] Arvore salva no arquivo: " << filename << std::endl;

    BTree* loadedTree = SaveManager::loadBTree(filename);
    assert(loadedTree != nullptr);
    std::cout << "[INFO] Arvore carregada do arquivo." << std::endl;
    std::cout << "Estado da arvore carregada:" << std::endl;
    loadedTree->traverse();

    assert(areTreesEqual(tree, *loadedTree, 50));
    std::cout << "\n[PASSOU] Arvore original e arvore carregada sao identicas." << std::endl;
    
    searchResult = loadedTree->search(20);
    assert(searchResult.size() == 1 && searchResult[0] == 201);
    std::cout << "[PASSOU] Busca na arvore carregada foi bem-sucedida." << std::endl;
    delete loadedTree; 
    std::cout << "------------------------------------------" << std::endl;
    
    std::cout << "\nTODOS OS TESTES DA ARVORE B FORAM CONCLUIDOS COM SUCESSO!" << std::endl;

    return 0;
}
