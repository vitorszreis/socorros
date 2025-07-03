# Este arquivo cria um programa executável separado para cada arquivo de teste.

# --- Teste da B-Tree ---
# Cria o executável 'test_btree' a partir do arquivo 'test_btree.cpp'
add_executable(test_btree test_btree.cpp)
# Linka o teste com a biblioteca core, permitindo que ele use as classes aplicação.
target_link_libraries(test_btree PRIVATE core) 
# Adiciona o executável à lista de testes do CTest pra rodá-los facilmente.
add_test(NAME BTreeTest COMMAND test_btree)


# --- Teste da Hash Table ---
add_executable(test_hash_table test_hash_table.cpp)
target_link_libraries(test_hash_table PRIVATE core)
add_test(NAME HashTableTest COMMAND test_hash_table)


# --- Teste das Queries ---
add_executable(test_queries test_queries.cpp)
target_link_libraries(test_queries PRIVATE core)
add_test(NAME QueriesTest COMMAND test_queries)


# --- Teste do Save Manager ---
add_executable(test_save_manager test_save_manager.cpp)
target_link_libraries(test_save_manager PRIVATE core)
add_test(NAME SaveManagerTest COMMAND test_save_manager)
