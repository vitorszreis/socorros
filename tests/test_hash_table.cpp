#include "../../include/structures/HashEncadeamento.h"
#include "../../include/structures/HashSondagem.h"
#include "../../include/entities/Player.h"
#include <iostream>
#include <cassert>
#include <vector>

/**
 * * @details
 * Esta função testa todas as operações fundamentais da tabela hash:
 * 1.  Inserção de múltiplos jogadores.
 * 2.  Teste de colisão: Insere um jogador cujo ID colide com um ID existente.
 * 3.  Busca por ID e Nickname: Valida se os jogadores, incluindo os de colisão, são encontrados.
 * 4.  Busca por elemento inexistente: Garante que a busca falha corretamente.
 * 5.  Remoção por ID e Nickname: Testa a remoção e verifica se a busca falha após a remoção,
 * e se outros elementos permanecem acessíveis.
 * 6.  Teste de redimensionamento: Insere um grande número de elementos para forçar o resize.
 * 7.  Teste de limpeza: Verifica se a função clear() esvazia a tabela corretamente.
 * * @param table Uma referência para um objeto que herda de HashBase.
 * @param type Uma string que descreve o tipo de tabela hash sendo testada
 */
void run_hash_tests(HashBase& table, const std::string& type) {
    std::cout << "\n--- Iniciando Teste Abrangente para Tabela Hash com " << type << " ---" << std::endl;

    // Limpa a tabela para garantir um estado inicial limpo
    table.clear();
    assert(table.getNumElementos() == 0 && "A tabela deveria estar vazia no inicio.");
    std::cout << "[PASSED] Teste de estado inicial limpo." << std::endl;

    // --- 1. Teste de Inserção e Colisão ---
    Player p1(1, "Vitor", "BR");
    Player p2(2, "Player2", "US");
    // ID 11 colide com 1 se o tamanho da tabela for 10
    // ID 1025 colide com 1 se o tamanho for 1024
    Player p3(table.getTamanhoTabela() + 1, "Collision", "CA"); 

    table.insert(p1);
    table.insert(p2);
    table.insert(p3);

    assert(table.getNumElementos() == 3 && "Deveria haver 3 elementos na tabela.");
    std::cout << "[PASSED] Teste de insercao basica e de colisao." << std::endl;
    table.printStats();

    // --- 2. Teste de Busca (ID e Nickname) ---
    Player* found_p1 = table.searchById(1);
    assert(found_p1 != nullptr && "Jogador 1 deveria ser encontrado por ID.");
    assert(found_p1->getNickname() == "Vitor" && "Nickname do jogador 1 incorreto.");
    std::cout << "[PASSED] Busca por ID (1) bem-sucedida." << std::endl;
    
    Player* found_p3 = table.searchById(table.getTamanhoTabela() + 1);
    assert(found_p3 != nullptr && "Jogador de colisao deveria ser encontrado por ID.");
    std::cout << "[PASSED] Busca por ID com colisao bem-sucedida." << std::endl;

    Player* found_p2_nick = table.searchByNick("Player2");
    assert(found_p2_nick != nullptr && "Jogador 2 deveria ser encontrado por Nickname.");
    assert(found_p2_nick->getId() == 2 && "ID do jogador 2 incorreto.");
    std::cout << "[PASSED] Busca por Nickname ('Player2') bem-sucedida." << std::endl;

    Player* not_found = table.searchById(999);
    assert(not_found == nullptr && "Jogador 999 nao deveria ser encontrado.");
    std::cout << "[PASSED] Busca por ID inexistente (999) bem-sucedida." << std::endl;

    // --- 3. Teste de Remoção ---
    // Remove por nickname
    bool removed_p2 = table.removeByNick("Player2");
    assert(removed_p2 && "Remocao do Player2 por nickname deveria retornar true.");
    assert(table.searchByNick("Player2") == nullptr && "Player2 nao deveria ser encontrado apos remocao.");
    assert(table.getNumElementos() == 2 && "Deveria haver 2 elementos apos remocao.");
    std::cout << "[PASSED] Remocao por Nickname ('Player2') bem-sucedida." << std::endl;

    // Garante que o jogador de colisão ainda pode ser encontrado após uma remoção
    found_p3 = table.searchById(table.getTamanhoTabela() + 1);
    assert(found_p3 != nullptr && "Jogador de colisao ainda deveria ser encontrado.");
    std::cout << "[PASSED] Consistencia da tabela apos remocao." << std::endl;
    
    // Remove por ID
    bool removed_p1 = table.removeById(1);
    assert(removed_p1 && "Remocao do Jogador 1 por ID deveria retornar true.");
    assert(table.searchById(1) == nullptr && "Jogador 1 nao deveria ser encontrado apos remocao.");
    assert(table.getNumElementos() == 1 && "Deveria haver 1 elemento apos remocao.");
    std::cout << "[PASSED] Remocao por ID (1) bem-sucedida." << std::endl;
    
    table.printStats();

    // --- 4. Teste de Redimensionamento (Resize) ---
    std::cout << "\n--- Iniciando teste de insercao em massa para forcar redimensionamento ---" << std::endl;
    int initial_size = table.getTamanhoTabela();
    // Insere 80% da capacidade inicial para forçar o resize (que ocorre a 70%)
    for (int i = 10; i < initial_size * 0.8 + 10; ++i) {
        table.insert(Player(i, "Player" + std::to_string(i), "XX"));
    }
    assert(table.getTamanhoTabela() > initial_size && "A tabela deveria ter sido redimensionada.");
    std::cout << "[PASSED] Teste de redimensionamento (tabela cresceu de " << initial_size << " para " << table.getTamanhoTabela() << ")." << std::endl;
    table.printStats();

    // --- 5. Teste de Limpeza (clear) ---
    table.clear();
    assert(table.getNumElementos() == 0 && "A tabela deveria estar vazia apos clear().");
    assert(table.searchById(table.getTamanhoTabela() + 1) == nullptr && "Nenhum elemento deveria ser encontrado apos clear().");
    std::cout << "[PASSED] Teste de limpeza (clear) bem-sucedido." << std::endl;
    table.printStats();

    std::cout << "\n--- Teste para " << type << " concluido com SUCESSO! ---\n" << std::endl;
}

int main() {

    int test_size = 11;

    // Testa a implementação com Encadeamento
    HashEncadeamento encadeamento(test_size);
    run_hash_tests(encadeamento, "Encadeamento");

    // Testa a implementação com Sondagem Linear
    HashSondagem sondagem(test_size);
    run_hash_tests(sondagem, "Sondagem Linear");

    std::cout << "Todos os testes para ambas as implementacoes foram concluidos." << std::endl;

    return 0;
}