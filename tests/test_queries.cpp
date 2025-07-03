#include "structures/BTree.h"
#include "structures/HashEncadeamento.h" // Usando uma implementação concreta para o teste
#include "entities/Player.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

// --- Funções Auxiliares para os Testes ---

/**
 * @brief Imprime os detalhes de um jogador para facilitar a visualização.
 * @param p Ponteiro para o objeto Player.
 */
void imprimir_detalhes_jogador(Player* p) {
    if (p) {
        std::cout << "  - ID: " << p->getId() 
                  << ", Nickname: " << p->getNickname() 
                  << ", Pais: " << p->getCountry() << std::endl;
    } else {
        std::cout << "  - Jogador nao encontrado." << std::endl;
    }
}

/**
 * @brief Imprime uma lista de jogadores a partir de seus IDs.
 * @param titulo Mensagem para descrever a lista.
 * @param ids Vetor com os IDs dos jogadores.
 * @param tabela Tabela hash para buscar os detalhes dos jogadores.
 */
void imprimir_lista_jogadores(const std::string& titulo, const std::vector<int>& ids, HashBase& tabela) {
    std::cout << titulo << " (" << ids.size() << " encontrados)" << std::endl;
    for (int id : ids) {
        imprimir_detalhes_jogador(tabela.searchById(id));
    }
}

int main() {
    std::cout << "--- Iniciando Teste de Consultas Integradas (Hash + Arvore B) ---" << std::endl;

    // --- 1. Configuração do Ambiente de Teste ---
    // Usamos HashEncadeamento para a tabela de jogadores e BTree para o índice de conquistas.
    HashEncadeamento tabela_de_jogadores(50);
    BTree arvore_de_conquistas(3); // Grau mínimo 3
    
    // Criando um conjunto de dados de teste (mock data)
    Player p1(1, "Vitor", "BR");
    Player p2(2, "Ana", "PT");
    Player p3(3, "John", "US");
    Player p4(4, "Zoe", "CA");
    Player p5(5, "Chen", "CN");
    Player p6(6, "Maria", "BR");

    tabela_de_jogadores.insert(p1);
    tabela_de_jogadores.insert(p2);
    tabela_de_jogadores.insert(p3);
    tabela_de_jogadores.insert(p4);
    tabela_de_jogadores.insert(p5);
    tabela_de_jogadores.insert(p6);

    // Associando os jogadores a uma contagem de conquistas na Árvore B
    arvore_de_conquistas.insertAchievement(150, p1.getId()); // Vitor: 150
    arvore_de_conquistas.insertAchievement(300, p2.getId()); // Ana: 300
    arvore_de_conquistas.insertAchievement(150, p3.getId()); // John: 150
    arvore_de_conquistas.insertAchievement(50, p4.getId());  // Zoe: 50
    arvore_de_conquistas.insertAchievement(300, p5.getId()); // Chen: 300
    arvore_de_conquistas.insertAchievement(450, p6.getId()); // Maria: 450 (a maior)

    std::cout << "[INFO] Ambiente de teste configurado com 6 jogadores." << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    // --- 2. Teste da Consulta: Busca por username ou ID ---
    // Este teste valida o requisito de encontrar um jogador específico usando a tabela hash.
    std::cout << "\n--- Teste: Busca por Jogador (ID e Nickname) ---" << std::endl;
    Player* jogador_buscado = tabela_de_jogadores.searchById(3);
    assert(jogador_buscado != nullptr && jogador_buscado->getNickname() == "John");
    std::cout << "[PASSOU] Busca por ID (3) encontrou 'John'." << std::endl;
    imprimir_detalhes_jogador(jogador_buscado);

    jogador_buscado = tabela_de_jogadores.searchByNick("Vitor");
    assert(jogador_buscado != nullptr && jogador_buscado->getId() == 1);
    std::cout << "[PASSOU] Busca por Nickname ('Vitor') encontrou o ID 1." << std::endl;
    imprimir_detalhes_jogador(jogador_buscado);
    std::cout << "---------------------------------------------------------" << std::endl;

    // --- 3. Teste da Consulta: Jogadores em um intervalo de conquistas ---
    // Este teste valida o uso da Árvore B para encontrar jogadores que se encaixam
    // em um critério numérico (neste caso, entre 100 e 350 conquistas).
    std::cout << "\n--- Teste: Jogadores com Conquistas entre 100 e 350 ---" << std::endl;
    std::vector<int> ids_no_intervalo = arvore_de_conquistas.searchByAchievementRange(100, 350);
    // Esperamos encontrar os jogadores com 150 (Vitor, John) e 300 (Ana, Chen) conquistas. Total: 4.
    assert(ids_no_intervalo.size() == 4);
    std::cout << "[PASSOU] A busca por intervalo encontrou 4 jogadores." << std::endl;
    imprimir_lista_jogadores("Detalhes dos jogadores encontrados:", ids_no_intervalo, tabela_de_jogadores);
    std::cout << "---------------------------------------------------------" << std::endl;

    // --- 4. Teste da Consulta: Top N jogadores (simulação) ---
    // Este teste simula a busca por "Top 3 jogadores". Em uma implementação real, isso poderia
    // ser feito com uma travessia reversa na árvore. Aqui, simulamos buscando por um
    // range alto e validando os resultados.
    std::cout << "\n--- Teste: Top 3 Jogadores com Mais Conquistas (simulacao) ---" << std::endl;
    std::vector<int> top_ids = arvore_de_conquistas.searchByAchievementRange(300, 500);
    // Esperamos encontrar os jogadores com 300 (Ana, Chen) e 450 (Maria) conquistas. Total: 3.
    assert(top_ids.size() == 3);
    std::cout << "[PASSOU] A busca pelos jogadores de maior pontuacao retornou 3 resultados." << std::endl;
    imprimir_lista_jogadores("Top 3 jogadores:", top_ids, tabela_de_jogadores);
    std::cout << "---------------------------------------------------------" << std::endl;

    // --- 5. Teste da Consulta: Jogador com o maior número de conquistas ---
    // Um caso específico do "Top N", onde N=1.
    std::cout << "\n--- Teste: Jogador com o Maior Numero de Conquistas ---" << std::endl;
    std::vector<int> maior_pontuador_id = arvore_de_conquistas.searchByAchievementRange(450, 450);
    assert(maior_pontuador_id.size() == 1 && maior_pontuador_id[0] == 6);
    std::cout << "[PASSOU] A busca pelo maior pontuador encontrou 'Maria'." << std::endl;
    imprimir_lista_jogadores("Jogador com mais conquistas:", maior_pontuador_id, tabela_de_jogadores);
    std::cout << "---------------------------------------------------------" << std::endl;

    return 0;
}
