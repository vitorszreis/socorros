# Adiciona o diretório de includes do projeto para que possamos encontrar os headers
include_directories(../include)

# Cria o executável 'main_app' a partir do ficheiro main.cpp
add_executable(main_app main.cpp)

# Faz o link do nosso executável com a biblioteca 'core'
# Isto garante que o main_app pode usar as classes BTree, SaveManager, etc.
target_link_libraries(main_app PRIVATE core)
