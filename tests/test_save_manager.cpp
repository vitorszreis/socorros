#include "data/SaveManager.h"
#include "structures/HashEncadeamento.h" // Implementação concreta para teste
#include "structures/BTree.h"
#include "entities/Player.h"
#include <iostream>
#include <cassert>
#include <filesystem> // Necessário para verificar e remover arquivos
#include <algorithm>
#include <vector>

// --- Funções Auxiliares para os Testes ---

/**
 * @brief Compara duas tabelas hash para verificar se são idênticas.
 * @details Essencial para validar se o processo de salvar e carregar funcionou.
 * A função verifica o número de elementos e busca cada jogador da tabela original
 * na tabela carregada, comparando seus dados.
 * @param original A tabela hash original.
 * @param loaded A tabela hash carregada do arquivo.
 * @return true se as tabelas forem idênticas, false caso contrário.
 */
bool areHashTablesEqual(HashBase& original, HashBase& loaded) {
    if (original.getNumElementos() != loaded.getNumElementos()) {
        return false;
    }
    std::vector<Player> original_players = original.getAllPlayers();
    for (const auto& p_orig : original_players) {
        Player* p_loaded = loaded.searchById(p_orig.getId());
        if (!p_loaded || p_loaded->getNickname() != p_orig.getNickname() || p_loaded->getCountry() != p_orig.getCountry()) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Compara duas Árvores B para verificar se são idênticas.
 * @details Valida o processo de salvar e carregar a árvore, buscando por um
 * grande intervalo de chaves e comparando os resultados.
 * @param t1 A árvore original.
 * @param t2 A árvore carregada.
 * @param max_key A chave máxima a ser verificada.
 * @return true se as árvores forem idênticas, false caso contrário.
 */
bool areBTreesEqual(BTree& t1, BTree& t2, int max_key) {
    for (int k = 0; k <= max_key; ++k) {
        std::vector<int> r1 = t1.searchByAchievementCount(k);
        std::vector<int> r2 = t2.searchByAchievementCount(k);
        std::sort(r1.begin(), r1.end());
        std::sort(r2.begin(), r2.end());
        if (r1 != r2) return false;
    }
    return true;
}


int main() {
    std::cout << "--- Iniciando Teste do SaveManager (Hash + Arvore B) ---" << std::endl;

    const std::string hash_filename = "hash_test_save.dat";
    const std::string btree_filename = "btree_test_save.dat";

    // --- 1. Teste de Salvamento e Carregamento da Tabela Hash ---
    std::cout << "\n--- Testando persistencia da Tabela Hash ---" << std::endl;
    {
        // Bloco para garantir que 'tabela_para_salvar' seja destruída após salvar.
        HashEncadeamento tabela_para_salvar(10);
        tabela_para_salvar.insert(Player(1, "Player1", "BR"));
        tabela_para_salvar.insert(Player(11, "Player11", "US")); // Causa colisão
        tabela_para_salvar.insert(Player(2, "Player2", "PT"));

        std::cout << "[INFO] Salvando tabela hash para o arquivo: " << hash_filename << std::endl;
        SaveManager::saveHash(&tabela_para_salvar, hash_filename);

        assert(std::filesystem::exists(hash_filename));
        std::cout << "[PASSOU] Arquivo da tabela hash foi criado com sucesso." << std::endl;
    }

    {
        // Bloco para testar o carregamento em uma nova tabela.
        HashEncadeamento tabela_carregada(10);
        
        std::cout << "[INFO] Carregando tabela hash do arquivo: " << hash_filename << std::endl;
        SaveManager::loadHash(&tabela_carregada, hash_filename);

        assert(tabela_carregada.getNumElementos() == 3);
        std::cout << "[PASSOU] Numero correto de elementos carregado da tabela hash." << std::endl;

        // Recria a tabela original para comparação
        HashEncadeamento tabela_original(10);
        tabela_original.insert(Player(1, "Player1", "BR"));
        tabela_original.insert(Player(11, "Player11", "US"));
        tabela_original.insert(Player(2, "Player2", "PT"));

        assert(areHashTablesEqual(tabela_original, tabela_carregada));
        std::cout << "[PASSOU] Tabela hash carregada e identica a original." << std::endl;
    }
    std::cout << "------------------------------------------------" << std::endl;

    // --- 2. Teste de Salvamento e Carregamento da Árvore B ---
    std::cout << "\n--- Testando persistencia da Arvore B ---" << std::endl;
    {
        BTree arvore_para_salvar(3);
        arvore_para_salvar.insertAchievement(150, 1);
        arvore_para_salvar.insertAchievement(300, 2);
        arvore_para_salvar.insertAchievement(150, 3);
        arvore_para_salvar.insertAchievement(450, 6);

        std::cout << "[INFO] Salvando Arvore B para o arquivo: " << btree_filename << std::endl;
        SaveManager::saveBTree(&arvore_para_salvar, btree_filename);

        assert(std::filesystem::exists(btree_filename));
        std::cout << "[PASSOU] Arquivo da Arvore B foi criado com sucesso." << std::endl;
    }

    {
        BTree* arvore_carregada = SaveManager::loadBTree(btree_filename);
        assert(arvore_carregada != nullptr);
        std::cout << "[INFO] Carregando Arvore B do arquivo: " << btree_filename << std::endl;

        // Recria a árvore original para comparação
        BTree arvore_original(3);
        arvore_original.insertAchievement(150, 1);
        arvore_original.insertAchievement(300, 2);
        arvore_original.insertAchievement(150, 3);
        arvore_original.insertAchievement(450, 6);

        assert(areBTreesEqual(arvore_original, *arvore_carregada, 500));
        std::cout << "[PASSOU] Arvore B carregada e identica a original." << std::endl;

        delete arvore_carregada;
    }
    std::cout << "------------------------------------------------" << std::endl;

    // --- 3. Limpeza dos Arquivos de Teste ---
    std::cout << "\n[INFO] Limpando arquivos de teste..." << std::endl;
    if (std::filesystem::exists(hash_filename)) {
        std::filesystem::remove(hash_filename);
    }
    if (std::filesystem::exists(btree_filename)) {
        std::filesystem::remove(btree_filename);
    }
    std::cout << "[INFO] Arquivos de teste removidos." << std::endl;

    std::cout << "\nTODOS OS TESTES DO SAVEMANAGER FORAM CONCLUIDOS COM SUCESSO!" << std::endl;

    return 0;
}
